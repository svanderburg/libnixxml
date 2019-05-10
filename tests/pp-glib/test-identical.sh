#!/bin/sh -e

../../src/nixxml-pp/nixxml-pp -f nix test-ast.xml > test-ast2.nix
sh nixexpr2xml --attr-style verbose --enable-types test-ast2.nix > test-ast2.xml
cmp test-ast.xml test-ast2.xml
