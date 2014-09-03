#include "harness.bash"

cat >$TMPA <<"EOF"
$timescale 1ps $end
$scope module Counter $end
$var wire 1 N0 reset $end
$var wire 32 N1 reg $end
$var wire 32 N2 io_count $end
$upscope $end
$enddefinitions $end
$dumpvars
$end
#0
b00000000000000000000000000000010 N1
b00000000000000000000000000000010 N2
#1
b00000000000000000000000000000010 N1
#2
b00000000000000000000000000000000 N2
#3
b00000000000000000000000000000001 N1
EOF

cat >$TMPB <<"EOF"
$timescale 1ps $end
$scope module Counter $end
$var wire 1 N0 reset $end
$var wire 32 N1 reg $end
$var wire 32 N2 io_count $end
$upscope $end
$enddefinitions $end
$dumpvars
$end
#0
b00000000000000000000000000000010 N1
b00000000000000000000000000000010 N2
#2
b00000000000000000000000000000000 N2
#3
b00000000000000000000000000000001 N1
EOF

cat >$RETGOLD <<"EOF"
0
EOF

cat >$OUTGOLD <<"EOF"
EOF
