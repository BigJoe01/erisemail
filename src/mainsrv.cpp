/*
	Copyright (c) openheap, uplusware
	uplusware@gmail.com
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <time.h>
#include "service.h"
#include "cli.h"
#include <string>
#include <iostream>
#include <syslog.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include "util/md5.h"
#include "dns.h"
#include <libgen.h>
#include <errno.h>
#include <sys/param.h> 
#include <sys/stat.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <streambuf>
#include <semaphore.h>
#include "mta.h"
#include "util/qp.h"
#include "base.h"
#include "fstring.h"
#include "util/trace.h"
#include "util/general.h"

using namespace std;

static void usage()
{
	printf("Usage:erisemaild start | stop | status | reload | version [CONFIG FILE]\n");
}

//set to daemon mode
static void daemon_init()
{
	setsid();
	chdir("/");
	umask(0);
 	close(STDIN_FILENO);
	close(STDOUT_FILENO);
    if(CMailBase::m_close_stderr)
        close(STDERR_FILENO);
	signal(SIGCHLD,SIG_IGN);
}

char PIDFILE[256] = "/tmp/erisemail/erisemaild.pid";

int Run()
{
	int retVal = 0;
	int mda_pid = -1;
    
    vector<service_param_t> server_params;
    
    if(CMailBase::m_enablesmtp)
    {
        service_param_t service_param;
        service_param.st = stSMTP;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_smtpport;
        service_param.is_ssl = FALSE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    if(CMailBase::m_enablesmtps)
    {
        service_param_t service_param;
        service_param.st = stSMTP;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_smtpsport;
        service_param.is_ssl = TRUE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
    if(CMailBase::m_enablepop3)
    {
        service_param_t service_param;
        service_param.st = stPOP3;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_pop3port;
        service_param.is_ssl = FALSE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
    if(CMailBase::m_enablepop3s)
    {
        service_param_t service_param;
        service_param.st = stPOP3;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_pop3sport;
        service_param.is_ssl = TRUE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
    if(CMailBase::m_enableimap)
    {
        service_param_t service_param;
        service_param.st = stIMAP;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_imapport;
        service_param.is_ssl = FALSE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
    if(CMailBase::m_enableimaps)
    {
        service_param_t service_param;
        service_param.st = stIMAP;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_imapsport;
        service_param.is_ssl = TRUE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
    if(CMailBase::m_enablehttp)
    {
        service_param_t service_param;
        service_param.st = stHTTP;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_httpport;
        service_param.is_ssl = FALSE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
    if(CMailBase::m_enablehttps)
    {
        service_param_t service_param;
        service_param.st = stHTTP;
        service_param.host_ip = CMailBase::m_hostip.c_str();
        service_param.host_port = CMailBase::m_httpsport;
        service_param.is_ssl = TRUE;
        service_param.sockfd = -1;
        server_params.push_back(service_param);
    }
    
	do
	{
		int pfd[2];
		pipe(pfd);
		mda_pid = fork();
		if(mda_pid == 0)
		{
			char szFlag[128];
			sprintf(szFlag, "/tmp/erisemail/%s.pid", MDA_SERVICE_NAME);
			if(check_single_on(szFlag)) 
			{
				printf("%s is aready runing.\n", MDA_SERVICE_NAME);   
				exit(-1);
			}
				
			close(pfd[0]);
			daemon_init();
			Service mda_svr;
			mda_svr.Run(pfd[1], server_params);
            close(pfd[1]);
			exit(0);
			
		}
		else if(mda_pid > 0)
		{
			unsigned int result;
			close(pfd[1]);
			read(pfd[0], &result, sizeof(unsigned int));
			if(result == 0)
				printf("Start %s Service OK \t\t\t[%u]\n", MDA_SERVICE_NAME, mda_pid);
			else
			{
				
				printf("Start %s Service Failed. \t\t\t[Error]\n", MDA_SERVICE_NAME);
			}
			close(pfd[0]);
		}
		else
		{
			close(pfd[0]);
			close(pfd[1]);
			retVal = -1;
			break;
		}
		
        if(CMailBase::m_enablemta)
        {
            //Relay service
            int mta_pids;
            pipe(pfd);
            mta_pids = fork();
            if(mta_pids == 0)
            {
                char szFlag[128];
                sprintf(szFlag, "/tmp/erisemail/%s.pid", MTA_SERVICE_NAME);
                if(check_single_on(szFlag) )   
                {   
                    printf("%s is aready runing.\n", MTA_SERVICE_NAME);   
                    exit(-1);  
                }
                
                close(pfd[0]);	
                daemon_init();
                MTA mta;
                mta.Run(pfd[1]);
                close(pfd[1]);
                exit(0);
            }
            else if(mta_pids > 0)
            {
                unsigned int result;
                close(pfd[1]);
                read(pfd[0], &result, sizeof(unsigned int));
                if(result == 0)
                    printf("Start %s Service OK \t\t\t[%u]\n", MTA_SERVICE_NAME, mta_pids);
                else
                {
                    
                    printf("Start %s Service Failed \t\t\t[Error]\n", MTA_SERVICE_NAME);
                }
                close(pfd[0]);
            }
            else
            {
                retVal = -1;
                break;
            }     
        }
		//Watcher
		int watcher_pids;
		pipe(pfd);
		watcher_pids = fork();
		if(watcher_pids == 0)
		{
			char szFlag[128];
			sprintf(szFlag, "/tmp/erisemail/%s.pid", "WATCHER");
			if(check_single_on(szFlag) )   
			{   
				printf("Service %s is aready runing.\n", "Watcher");   
				exit(-1);  
			}
			
			close(pfd[0]);	
			daemon_init();
			Watcher watcher;
			watcher.Run(pfd[1], server_params);
            close(pfd[1]);
			exit(0);
		}
		else if(watcher_pids > 0)
		{
			unsigned int result;
			close(pfd[1]);
			read(pfd[0], &result, sizeof(unsigned int));
			if(result == 0)
            {
				printf("Start Service Watcher OK \t\t[%u]\n", watcher_pids);
            }
			else
			{
				printf("Start Service Watcher Failed \t\t\t[Error]\n");
			}
			close(pfd[0]);
		}
		else
		{
			retVal = -1;
			break;
		}
	}while(0);
	
	CMailBase::UnLoadConfig();
	
	return retVal;
}

static int Stop()
{
	printf("Stop eRisemail service ...\n");
	
	Watcher watcher;
	watcher.Stop();
	
	Service mda_svr;
	mda_svr.Stop();	
	
	MTA mta_svr;
	mta_svr.Stop();
}

static void Version()
{
	printf("v%s\n", CMailBase::m_sw_version.c_str());
}

static int Reload()
{
	printf("Reload eRisemail configuration ...\n");
	
	Service mda_svr;
	mda_svr.ReloadConfig();
	
	MTA mta_svr;
	mta_svr.ReloadConfig();
}

static int processcmd(const char* cmd, const char* conf, const char* permit, const char* reject, const char* domain, const char* webadmin)
{
	CMailBase::SetConfigFile(conf, permit, reject, domain, webadmin);
	if(!CMailBase::LoadConfig())
	{
		printf("Load Configure File Failed.\n");
		return -1;
	}
	
	if(strcasecmp(cmd, "stop") == 0)
	{
		Stop();
	}
	else if(strcasecmp(cmd, "start") == 0)
	{
		Run();
	}
	else if(strcasecmp(cmd, "reload") == 0)
	{
		Reload();
	}
	else if(strcasecmp(cmd, "status") == 0)
	{
		char szFlag[128];
		sprintf(szFlag, "/tmp/erisemail/%s.pid", MDA_SERVICE_NAME);
		if(check_single_on(szFlag) )   
		{   
			printf("%s Service is runing.\n", MDA_SERVICE_NAME);   
		}
		else
		{
			printf("%s Service stopped.\n", MTA_SERVICE_NAME);   
		}

		sprintf(szFlag, "/tmp/erisemail/%s.pid", MTA_SERVICE_NAME);
		if(check_single_on(szFlag) )   
		{   
			printf("%s Service is runing.\n", MTA_SERVICE_NAME);   
		}
		else
		{
			printf("%s Service stopped.\n", MTA_SERVICE_NAME);   
		}
		
	}
	else if(strcasecmp(cmd, "version") == 0)
	{
		Version();
	}
	else
	{
		usage();
	}
	CMailBase::UnLoadConfig();
	return 0;	

}

static void handle_signal(int sid) 
{ 
	signal(SIGPIPE, handle_signal);
}

int main(int argc, char* argv[])
{	    
	if(getgid() != 0)
	{
		printf("You need root privileges to run this program\n");
		return -1;
	}
	else
	{
		mkdir("/tmp/erisemail", 0777);
		chmod("/tmp/erisemail", 0777);

		mkdir("/var/log/erisemail/", 0744);
		
		// Set up the signal handler
		signal(SIGPIPE, SIG_IGN);
		sigset_t signals;
		sigemptyset(&signals);
		sigaddset(&signals, SIGPIPE);
		sigprocmask(SIG_BLOCK, &signals, NULL);

#if OPENSSL_VERSION_NUMBER >= 0x010100000L
        OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS, NULL);
#else
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
#endif /* OPENSSL_VERSION_NUMBER */

		if(argc == 2)
		{
			processcmd(argv[1], CONFIG_FILE_PATH, PERMIT_FILE_PATH, REJECT_FILE_PATH, DOMAIN_FILE_PATH, WEBADMIN_FILE_PATH);
		}
		else
		{
			usage();
			return -1;
		}
		return 0;
	}
}

