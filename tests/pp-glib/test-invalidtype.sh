set -e

source setup.sh

./pp-glib -s -f nix test-invalidtype.xml > test-invalidtype.nix

# Despite having an invalid type, it should still print the known attributes
grep '"Hello"' test-invalidtype.nix
