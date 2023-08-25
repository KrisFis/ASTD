@echo off

cd ..
mkdir Intermediate
cd Intermediate
cmake ..
cmake --build .
cd ..

pause