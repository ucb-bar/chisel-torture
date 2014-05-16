set -ex

tempdir=`mktemp -d -t ptest-chisel-torture.XXXXXXXXXX`
trap "rm -rf $tempdir" EXIT
cd $tempdir
