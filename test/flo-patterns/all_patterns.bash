#include "tempdir.bash"

set -ex
set -o pipefail

$PTEST_BINARY --list | while read pattern
do
    $PTEST_BINARY --list $pattern | while read example
    do
        echo "$pattern-$example" >> list
    done
done

cat >gold <<EOF
#include "pattern_list.bash"
EOF

sort list > list.sort
sort gold > gold.sort

diff list.sort gold.sort
