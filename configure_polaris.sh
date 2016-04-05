#!/bin/bash

BASEDIR=$PWD
if [ ! -d $BASEDIR/build ]
	mkdir $BASEDIR/build
fi 
if [ ! -d $BASEDIR/build/debug ]
	mkdir $BASEDIR/build/debug
fi
if [ ! -d $BASEDIR/build/release ]
	mkdir $BASEDIR/build/release
fi

# configure debug build
cd $BASEDIR/build/debug
cmake -D Unittests=1 -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" $BASEDIR

#configure release build
cd $BASEDIR/build/release
cmake -D Unittests=1 -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" $BASEDIR
