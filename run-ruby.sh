#!/bin/sh

GDB=${GDB:-}
RUBY=${RUBY:-ruby}

base_dir=$(cd $(dirname $0) && pwd)

${base_dir}/run.sh ${GDB} ${RUBY} "$@"
