#include "tempdir.bash"

set -ex
set -o pipefail

# Extract the actual name of the test from the provided argument list
testname="$(basename "$(dirname "$0")")"
pattern="$(echo "$testname" | cut -d- -f1)"
args="$(echo "$testname" | cut -d- -f2-)"

# Now actually generate the Flo output
$PTEST_BINARY --show $pattern $args
cat Torture.scala
cat Torture.vcd

# Have Chisel generate a VCD file
mv Torture.vcd Torture-gold.vcd
chisel-jargen chisel.jar
scalac -classpath chisel.jar:. Torture.scala
scala -classpath chisel.jar:. Torture --vcd --backend flo
scala -classpath chisel.jar:. Torture --vcd --backend c --genHarness --compile
cat Torture.cpp
cat Torture-emulator.cpp

# Show the Chisel-generated Flo file
cat Torture.flo

# Convert the VCD's input nodes into something the Chisel tester
# understands.
vcd2step Torture-gold.vcd Torture.flo test.in
cat test.in

# Run the tests to see what happens
cat test.in | ./Torture || true

cat Torture.vcd
cat Torture-gold.vcd

# Check and see if the results are the same!
vcddiff Torture-gold.vcd Torture.vcd

# FIXME: Chisel sometimes produces spurious nodes in the output file
# (currently all registers end up VCD dumped), so I can't diff the
# other way.
#vcddiff Torture.vcd Torture-gold.vcd

# At this point we can run the tests through the Flo backend, just to
# make sure that exactly the same thing comes out.
flo-llvm --torture Torture.flo
flo-llvm-torture Torture.flo --harness > harness.c++
g++ harness.c++ Torture.o -o Torture

# We can diff the VCDs in both directions now!
cat test.in | ./Torture
vcddiff Torture-gold.vcd Torture.vcd
vcddiff Torture.vcd Torture-gold.vcd
