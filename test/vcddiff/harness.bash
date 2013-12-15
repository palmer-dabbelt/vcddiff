set -ex

export TMP="$(mktemp -d -t vcddiff-test.XXXXXXXXXX)"
trap "rm -rf $TMP" EXIT

export TMPA="$TMP/a.vcd"
export TMPB="$TMP/b.vcd"
export RETGOLD="$TMP/ret.gold"
export OUTGOLD="$TMP/out.gold"

ARCHIVE=`awk '/^__ARCHIVE_BELOW__/ {print NR + 1; exit 0; }' $0`
tail -n+$ARCHIVE $0 | bash -ex

set +e
$PTEST_BINARY $TMPA $TMPB > $TMP/out
echo "$?" > $TMP/ret
set -e

if [[ "$(diff $RETGOLD $TMP/ret)" != "" ]]
then
    cat $TMP/ret
    exit 1
fi

if [[ "$(diff $OUTGOLD $TMP/out)" != "" ]]
then
    cat $TMP/out
    exit 1
fi

exit 0
__ARCHIVE_BELOW__
