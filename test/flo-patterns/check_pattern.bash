#include "tempdir.bash"
#include "emulator.bash"

set -ex
set -o pipefail

# Extract the actual name of the test from the provided argument list
testname="$(basename "$(dirname "$0")")"
pattern="$(echo "$testname" | cut -d- -f1)"
args="$(echo "$testname" | cut -d- -f2-)"

# Now actually generate the Flo output
$PTEST_BINARY --show $pattern $args
cat Torture.flo
cat Torture.vcd

# Build that Flo output into a C++ emulator
flo-llvm Torture.flo

# Build the test harness that goes along with this code
flo-llvm-release Torture.flo --harness > harness.c++
g++ harness.c++ -c -o harness.o

# Link everything together
g++ harness.o Torture.o -o Torture

# FIXME: This should really be generated from a VCD file
cat >test.in <<EOF
step 100
quit
EOF

# Run the tests to see what happens
mv Torture.vcd Torture-gold.vcd
cat test.in | ./Torture

cat Torture.vcd
cat Torture-gold.vcd

# Check and see if the results are the same!
vcddiff Torture-gold.vcd Torture.vcd
vcddiff Torture.vcd Torture-gold.vcd
