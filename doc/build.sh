#!/bin/bash

if [ ! -d env ]; then
    virtualenv env
else
    echo "using existing virtualenv"
fi

. env/bin/activate
pip install --upgrade sphinx
pip install --upgrade breathe

pushd doxygen
rm -fr output/*
doxygen
popd

python -m breathe.apidoc doxygen/output/xml/ -o sphinx -f
