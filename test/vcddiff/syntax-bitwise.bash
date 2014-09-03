#include "harness.bash"

cat >$TMPA <<"EOF"
$timescale 1ps $end
$scope module Counter $end
$var wire 2 N0 count [1:0] $end
$upscope $end
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
$scope module Counter $end
$var wire 1 N1 count [1] $end
$var wire 1 N0 count [0] $end
$upscope $end
$enddefinitions $end
$dumpvars
$end
#0
0N0
0N1
#1
1N0
0N1
#2
0N0
1N1
#3
1N0
1N1
EOF

cat >$RETGOLD <<"EOF"
0
EOF

cat >$OUTGOLD <<"EOF"
EOF
