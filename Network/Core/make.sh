#!/bin/bash

g++ -shared *.cpp ./RakNet/*.cpp -o ../../Binary/Network.Core.so -I. -I../../Shared/ -lpthread
