#!/bin/sh
# Re-verify every staged compiler test with the compiler+flags named in its header.
# Run from the repo root:  tools/dr sh tools/compiler_tests/check.sh
# Add MATRIX=1 to also try the other compiler/flag combinations.
cd /work/tools/compiler_tests || exit 1
fail=0
for f in sub_0*.c; do
  n=${f%.c}; a=0x${n#sub_}
  cc=$(sed -n 's/^ \* Flags:  \([a-z_]*\) .*/\1/p' $f); fl=$(sed -n 's/^ \* Flags:  [a-z_]* \(.*\)/\1/p' $f)
  python3 cmp.py -q --cc "$cc" --flags="$fl" $f $a || fail=1
  if [ -n "$MATRIX" ]; then
    for cfg in "old_agbcc|-mthumb-interwork -O2" "agbcc|-mthumb-interwork -O2 -fprologue-bugfix" "agbcc|-mthumb-interwork -O2" "old_agbcc|-mthumb-interwork -O1" "agbcc|-mthumb-interwork -O1"; do
      python3 cmp.py -q --cc "${cfg%%|*}" --flags="${cfg#*|}" $f $a | sed 's/^/    /'
    done
  fi
done
exit $fail
