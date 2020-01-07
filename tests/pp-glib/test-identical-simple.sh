set -e

source setup.sh

./pp-glib -s -f nix test-ast-simple.xml > test-ast2-simple.nix
sh nixexpr2xml --attr-style simple --enable-types test-ast2-simple.nix > test-ast2-simple.xml
cmp test-ast-simple.xml test-ast2-simple.xml
