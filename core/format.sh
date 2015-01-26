#!/bin/sh

find . -type f | egrep "(\.cpp$)|(\.c$)|(\.h$)" | grep -v '/deps/' | xargs clang-format -i

