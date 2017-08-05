#!/bin/bash

pushd doxygen
rm -fr output/*
doxygen
popd
