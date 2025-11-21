#!/bin/bash

cmake -S . -B Build/Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D CMAKE_BUILD_TYPE=Debug
mv -f Build/Debug/compile_commands.json Build
cmake --build Build/Debug --config Debug
ln -s Build/compile_commands.json .
cd Build/Debug/Run

./MyProject
cd ../../..

