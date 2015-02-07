#!/bin/bash

set -eu

find . -type f | egrep "(\.cpp$)|(\.c$)|(\.h$)" | grep -v '/deps/' | xargs clang-format -i

