#!/bin/bash

if [[ $1 != '' ]];then
  BASEDIR=$1
elif [[ $POLARIS_DEPS_DIR != '' ]]; then
  BASEDIR=$POLARIS_DEPS_DIR
else
  BASEDIR=$PWD
fi

./build-boost-1.60.0.sh $BASEDIR
./build-odb-2.4.0.sh $BASEDIR
./build-gtest-1.7.0.sh $BASEDIR
