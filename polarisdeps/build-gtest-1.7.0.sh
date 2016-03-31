#!/bin/bash

if [[ $1 != '' ]];then
  BASEDIR=$1
elif [[ $POLARIS_DEPS_DIR != '' ]]; then
  BASEDIR=$POLARIS_DEPS_DIR
else
  BASEDIR=$PWD
fi

#Download and expand source files
GTESTTARFILE=$BASEDIR/release-1.7.0.zip
GTESTDIR=$BASEDIR/googletest-release-1.7.0
if [ ! -f $GTESTTARFILE ];then
  wget -c 'https://github.com/google/googletest/archive/release-1.7.0.zip' --directory-prefix=$BASEDIR
fi
if [ ! -d $GTESTDIR ];then
  unzip -q $GTESTTARFILE -d $BASEDIR
fi

BUILDDIR=$GTESTDIR/build
if [ ! -d $BUILDDIR ];then
  mkdir $BUILDDIR
fi
if [ ! -d $BUILDDIR/release ];then
  mkdir $BUILDDIR/release
fi
if [ ! -d $BUILDDIR/debug ];then
  mkdir $BUILDDIR/debug
fi

cd $BUILDDIR/release
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../..
make
cd $BUILDDIR/debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ../..
make
