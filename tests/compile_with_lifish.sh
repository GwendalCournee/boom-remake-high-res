#!/bin/bash
# COMPILE LIFISH FIRST!
${CXX:-g++} -std=c++11 -I../src/{core{,/collisions,/components,/entities,/debug},ui,lifish{,/entities,/components,/debug,/level},third_party} $1  ../CMakeFiles/lifish.dir/src/{core{,/collisions,/components,/entities,/debug},lifish{,/entities,/components,/debug,/level},third_party,ui}/*o  ../lib/libnfd.a -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio `pkg-config --libs gtk+-3.0` -o ${1%.cpp}
