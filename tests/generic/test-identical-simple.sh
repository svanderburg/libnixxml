set -e

source setup.sh

../../src/nixxml-pp/nixxml-pp -s -f nix test-ast-simple.xml > test-ast2-simple.nix
sh nixexpr2xml --attr-style simple --enable-types test-ast2-simple.nix > test-ast2-simple.xml

echo test-ast-simple.xml >&2
cat test-ast-simple.xml >&2
echo test-ast2-simple.xml >&2
cat test-ast2-simple.xml >&2

cmp test-ast-simple.xml test-ast2-simple.xml
