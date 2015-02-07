#!/bin/bash

set -eu

cppcheck --enable=all --error-exitcode=1 -v *.c *.h
./format.sh

