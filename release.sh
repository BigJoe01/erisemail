#!/bin/bash

if [ $# = 3 ]
then
	echo $1
else
	echo "release 1.6.08 utf8 ubuntu"
	exit 1
fi

path=$(dirname $0)
oldpwd=$(pwd)
cd ${path}
path=$(pwd)

cd ${path}

#############################################################################
# Platform
m=`uname -m`
if uname -o | grep -i linux;
then
	o=linux
fi

##############################################################################
# Simplfied Chinese
rm -rf $3-erisemail-bin-cn-$2-${m}-${o}
mkdir $3-erisemail-bin-cn-$2-${m}-${o}
mkdir $3-erisemail-bin-cn-$2-${m}-${o}/html

cp cn-html-$2/*.js $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.jpg $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.gif $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.png $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.ico $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.html $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.css $3-erisemail-bin-cn-$2-${m}-${o}/html/
cp cn-html-$2/*.xml $3-erisemail-bin-cn-$2-${m}-${o}/html/

cp src/erisemaild $3-erisemail-bin-cn-$2-${m}-${o}/erisemaild
cp src/eriseutil $3-erisemail-bin-cn-$2-${m}-${o}/eriseutil
cp src/liberisestorage.so $3-erisemail-bin-cn-$2-${m}-${o}/liberisestorage.so
cp src/liberiseantijunk.so $3-erisemail-bin-cn-$2-${m}-${o}/liberiseantijunk.so
cp src/postudf.so $3-erisemail-bin-cn-$2-${m}-${o}/postudf.so

cp script/eriseaddgroup $3-erisemail-bin-cn-$2-${m}-${o}/eriseaddgroup
cp script/eriseadduser $3-erisemail-bin-cn-$2-${m}-${o}/eriseadduser
cp script/erisedelgroup $3-erisemail-bin-cn-$2-${m}-${o}/erisedelgroup
cp script/erisedeluser $3-erisemail-bin-cn-$2-${m}-${o}/erisedeluser
cp script/eriseaddusertogroup $3-erisemail-bin-cn-$2-${m}-${o}/eriseaddusertogroup
cp script/erisedeluserfromgroup $3-erisemail-bin-cn-$2-${m}-${o}/erisedeluserfromgroup

cp script/erisepasswd $3-erisemail-bin-cn-$2-${m}-${o}/erisepasswd

cp script/eriseenableuser $3-erisemail-bin-cn-$2-${m}-${o}/eriseenableuser
cp script/erisedisableuser $3-erisemail-bin-cn-$2-${m}-${o}/erisedisableuser

cp script/install.sh $3-erisemail-bin-cn-$2-${m}-${o}/
cp script/uninstall.sh $3-erisemail-bin-cn-$2-${m}-${o}/

cp script/erisemail-$2.conf $3-erisemail-bin-cn-$2-${m}-${o}/erisemail.conf
cp script/permit.list $3-erisemail-bin-cn-$2-${m}-${o}/
cp script/reject.list $3-erisemail-bin-cn-$2-${m}-${o}/
cp script/mfilter.xml $3-erisemail-bin-cn-$2-${m}-${o}/
cp script/domain.list $3-erisemail-bin-cn-$2-${m}-${o}/
cp script/webadmin.list $3-erisemail-bin-cn-$2-${m}-${o}/

cp script/erisemail.sh $3-erisemail-bin-cn-$2-${m}-${o}/

cp ca/ca.crt $3-erisemail-bin-cn-$2-${m}-${o}/ca.crt
cp ca/server.crt $3-erisemail-bin-cn-$2-${m}-${o}/server.crt
cp ca/server.p12 $3-erisemail-bin-cn-$2-${m}-${o}/server.p12
cp ca/server.key $3-erisemail-bin-cn-$2-${m}-${o}/server.key

cp ca/client.crt $3-erisemail-bin-cn-$2-${m}-${o}/client.crt
cp ca/client.p12 $3-erisemail-bin-cn-$2-${m}-${o}/client.p12
cp ca/client.key $3-erisemail-bin-cn-$2-${m}-${o}/client.key

cp script/README-EN.html $3-erisemail-bin-cn-$2-${m}-${o}/
cp script/README-ZH.html $3-erisemail-bin-cn-$2-${m}-${o}/

chmod a+x $3-erisemail-bin-cn-$2-${m}-${o}/*
#ls -al $3-erisemail-bin-cn-$2-${m}-${o}
tar zcf $3-erisemail-bin-cn-$2-${m}-${o}-$1.tar.gz $3-erisemail-bin-cn-$2-${m}-${o}
##############################################################################
# English
rm -rf $3-erisemail-bin-en-$2-${m}-${o}
mkdir $3-erisemail-bin-en-$2-${m}-${o}
mkdir $3-erisemail-bin-en-$2-${m}-${o}/html

cp en-html-$2/*.js $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.jpg $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.gif $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.ico $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.png $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.html $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.css $3-erisemail-bin-en-$2-${m}-${o}/html/
cp en-html-$2/*.xml $3-erisemail-bin-en-$2-${m}-${o}/html/

cp src/erisemaild $3-erisemail-bin-en-$2-${m}-${o}/erisemaild
cp src/eriseutil $3-erisemail-bin-en-$2-${m}-${o}/eriseutil
cp src/liberisestorage.so $3-erisemail-bin-en-$2-${m}-${o}/liberisestorage.so
cp src/liberiseantijunk.so $3-erisemail-bin-en-$2-${m}-${o}/liberiseantijunk.so
cp src/postudf.so $3-erisemail-bin-en-$2-${m}-${o}/postudf.so

cp script/eriseaddgroup $3-erisemail-bin-en-$2-${m}-${o}/eriseaddgroup
cp script/eriseadduser $3-erisemail-bin-en-$2-${m}-${o}/eriseadduser
cp script/erisedelgroup $3-erisemail-bin-en-$2-${m}-${o}/erisedelgroup
cp script/erisedeluser $3-erisemail-bin-en-$2-${m}-${o}/erisedeluser
cp script/eriseaddusertogroup $3-erisemail-bin-en-$2-${m}-${o}/eriseaddusertogroup
cp script/erisedeluserfromgroup $3-erisemail-bin-en-$2-${m}-${o}/erisedeluserfromgroup

cp script/erisepasswd $3-erisemail-bin-en-$2-${m}-${o}/erisepasswd

cp script/eriseenableuser $3-erisemail-bin-en-$2-${m}-${o}/eriseenableuser
cp script/erisedisableuser $3-erisemail-bin-en-$2-${m}-${o}/erisedisableuser

cp script/install.sh $3-erisemail-bin-en-$2-${m}-${o}/
cp script/uninstall.sh $3-erisemail-bin-en-$2-${m}-${o}/

cp script/erisemail-$2.conf $3-erisemail-bin-en-$2-${m}-${o}/erisemail.conf
cp script/permit.list $3-erisemail-bin-en-$2-${m}-${o}/
cp script/reject.list $3-erisemail-bin-en-$2-${m}-${o}/
cp script/mfilter.xml $3-erisemail-bin-en-$2-${m}-${o}/
cp script/domain.list $3-erisemail-bin-en-$2-${m}-${o}/
cp script/webadmin.list $3-erisemail-bin-en-$2-${m}-${o}/

cp script/erisemail.sh $3-erisemail-bin-en-$2-${m}-${o}/

cp ca/ca.crt $3-erisemail-bin-en-$2-${m}-${o}/ca.crt
cp ca/server.p12 $3-erisemail-bin-en-$2-${m}-${o}/server.p12
cp ca/server.crt $3-erisemail-bin-en-$2-${m}-${o}/server.crt
cp ca/server.key $3-erisemail-bin-en-$2-${m}-${o}/server.key

cp ca/client.p12 $3-erisemail-bin-en-$2-${m}-${o}/client.p12
cp ca/client.crt $3-erisemail-bin-en-$2-${m}-${o}/client.crt
cp ca/client.key $3-erisemail-bin-en-$2-${m}-${o}/client.key

cp script/README-EN.html $3-erisemail-bin-en-$2-${m}-${o}/README.html
cp script/README-ZH.html $3-erisemail-bin-en-$2-${m}-${o}/README.html

chmod a+x $3-erisemail-bin-en-$2-${m}-${o}/*
tar zcf $3-erisemail-bin-en-$2-${m}-${o}-$1.tar.gz $3-erisemail-bin-en-$2-${m}-${o}
cd ${oldpwd}
