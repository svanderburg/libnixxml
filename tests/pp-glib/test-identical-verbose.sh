set -e

source setup.sh

./pp-glib -f nix test-ast-verbose.xml > test-ast2-verbose.nix
sh nixexpr2xml --attr-style verbose --enable-types test-ast2-verbose.nix > test-ast2-verbose.xml
cmp test-ast-verbose.xml test-ast2-verbose.xml
