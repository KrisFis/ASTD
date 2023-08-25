#!/bin/bash

cd ..
mkdir -p Intermediate
cd Intermediate
cmake ..
cmake --build .
cd ..