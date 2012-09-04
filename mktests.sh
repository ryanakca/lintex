#!/bin/sh
mkdir -p test/dir1
for ext in foo bar dir1/baz; do \
    touch test/${ext}.tex; \
done
sleep 1
for test in foo bar dir1/baz; do \
    for ext in pdf ps dvi log aux; do \
        touch test/${test}.${ext}; \
    done; \
done
sleep 1
touch test/foo.tex
