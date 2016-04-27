#!/bin/bash

BASEDIR=$PWD
if [ ! -d $BASEDIR/build ]
  then
    mkdir $BASEDIR/build
fi 
if [ ! -d $BASEDIR/build/debug ]
  then
    mkdir $BASEDIR/build/debug
fi
if [ ! -d $BASEDIR/build/release ]
  then
    mkdir $BASEDIR/build/release
fi

# configure debug build
cd $BASEDIR/build/debug
cmake -DCOMMANDLINE_ARGS=1 -DUnittests=1 -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" $BASEDIR

#configure release build
cd $BASEDIR/build/release
cmake -DCOMMANDLINE_ARGS=1 -DUnittests=1 -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" $BASEDIR
