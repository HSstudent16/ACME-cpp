#!/bin/bash

name="runme"

g++ -o "$name" test.cpp ../src/acme.cpp ../lib/vecN.cpp
