#!/bin/bash

g++ PlatonServer.cpp ABI.cpp -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-client -ljsonrpccpp-server -osimulateserver -std=c++11

echo "you should copy testint.ll to the directory where simulateserver run in."
