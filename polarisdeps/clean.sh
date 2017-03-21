#!/bin/bash

if [[ $1 != '' ]];then
  BASEDIR=$1
elif [[ $POLARIS_DEPS_DIR != '' ]]; then
  BASEDIR=$POLARIS_DEPS_DIR
else
  BASEDIR=$PWD
fi
echo BASEDIR=$BASEDIR

rm -r $BASEDIR/odb-2.4.0-x86_64-linux-gnu.tar.bz2
rm -r $BASEDIR/odb-2.4.0-x86_64-linux-gnu
rm -r $BASEDIR/libodb-sqlite-2.4.0.tar.bz2
rm -r $BASEDIR/libodb-sqlite-2.4.0
rm -r $BASEDIR/libodb-2.4.0
rm -r $BASEDIR/libodb-2.4.0.tar.bz2
rm -r $BASEDIR/sqlite-autoconf-3110100
rm -r $BASEDIR/sqlite-autoconf-3110100.tar.gz


