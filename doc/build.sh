#!/bin/bash

if [ ! -d env ]; then
    virtualenv env
else
    echo "using existing virtualenv"
fi

. env/bin/activate
pip install --upgrade sphinx
#pip install --upgrade breathe
# use latest from git to enable breathe_show_define_initializer
pip install --upgrade -e git+git@github.com:michaeljones/breathe.git#egg=breathe-4.7.0

pushd doxygen
rm -fr output/*
doxygen
popd

pushd doxygen
mkdir output/rst
python -m breathe.apidoc output/xml/ -o output/rst -f
popd

pushd sphinx
python -m sphinx -a -b html source build/html
popd
