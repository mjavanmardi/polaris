#!/bin/bash

BASEDIR=$PWD
mkdir $BASEDIR/build
mkdir $BASEDIR/build/debug
mkdir $BASEDIR/build/release
cd $BASEDIR/build/debug
cmake -D Unittests=1 -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" $BASEDIR
cd $BASEDIR/build/release
cmake -D Unittests=1 -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" $BASEDIR
