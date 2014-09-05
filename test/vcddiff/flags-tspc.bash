#include "harness.bash"

cat >$TMPA <<"EOF"
$timescale 1ps $end
$scope module Test $end
$var wire 2 N0 sig $end
$upscope end
$enddefinitions $end
$dumpvars
$end
#0
b00 N0
#3
b01 N0
#6
b10 N0
#9
b11 N0
EOF

cat >$TMPB <<"EOF"
$timescale 1ps $end
$scope module Test $end
$var wire 2 N0 sig $end
$upscope end
$enddefinitions $end
$dumpvars
$end
#2
b00 N0
#4
b01 N0
#6
b10 N0
#8
b11 N0
EOF

echo "--a-tspc=3 --b-tspc=2" > $ARGS_FILE

cat >$RETGOLD <<"EOF"
0
EOF

cat >$OUTGOLD <<"EOF"
EOF
