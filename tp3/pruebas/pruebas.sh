#!/usr/bin/env bash

set -eu

PROGRAMA="python3 $1"

RET=0
OUT=`mktemp`
trap "rm -f $OUT" EXIT

for x in *.test; do  
  b=${x%.test}
  echo -n "Prueba $b... "
  cat "$x"
  echo

  ($PROGRAMA $2 <${b}_in || RET=$?) |
    diff -u --label "${b}_cátedra" --label "${b}_estudiante" ${b}_out - >$OUT || :

  if [[ -s $OUT ]]; then
    echo -e "output incorrecto:\n"
    cat $OUT
    exit 1

  else
    echo -e "OK."
  fi
  echo
done