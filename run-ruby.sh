#!/bin/sh

GDB=${GDB:-}
VALGRIND=${VALGRIND:-}
RUBY=${RUBY:-ruby}

base_dir=$(cd $(dirname $0) && pwd)

${base_dir}/run.sh ${GDB} ${VALGRIND} ${RUBY} "$@"
