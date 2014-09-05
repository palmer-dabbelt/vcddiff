#include "harness.bash"

cat >$TMPA <<"EOF"
$timescale 1ps $end
$scope module AMod $end
$scope module Test $end
$var wire 2 N0 sig $end
$upscope end
$upscope end
$enddefinitions $end
$dumpvars
$end
#0
b00 N0
#1
b01 N0
#2
b10 N0
#3
b11 N0
EOF

cat >$TMPB <<"EOF"
$timescale 1ps $end
$scope module BMod $end
$scope module Test $end
$var wire 2 N0 sig $end
$upscope end
$upscope end
$enddefinitions $end
$dumpvars
$end
#0
b00 N0
#1
b01 N0
#2
b10 N0
#3
b11 N0
EOF

echo "--raise-a-signals=1 --raise-b-signals=1" > $ARGS_FILE

cat >$RETGOLD <<"EOF"
0
EOF

cat >$OUTGOLD <<"EOF"
EOF
