#!/bin/sh
#
# Copyright (C) 2015-2017  Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# touch dnsrps-off to not test with DNSRPS

set -e

SYSTEMTESTTOP=..
. $SYSTEMTESTTOP/conf.sh

USAGE="$0: [-xD]"
DEBUG=
while getopts "xD" c; do
    case $c in
	x) set -x; DEBUG=-x;;
        D) TEST_DNSRPS="-D";;
	*) echo "$USAGE" 1>&2; exit 1;;
    esac
done
shift `expr $OPTIND - 1 || true`
if test "$#" -ne 0; then
    echo "$USAGE" 1>&2
    exit 1
fi

$SHELL clean.sh $DEBUG

$PERL testgen.pl
cp -f ns2/named.default.conf ns2/named.conf
cp -f ns3/named1.conf ns3/named.conf

# decide whether to test DNSRPS
$SHELL ../rpz/ckdnsrps.sh $TEST_DNSRPS $DEBUG
test -z "`grep 'dnsrps-enable yes' dnsrps.conf`" && TEST_DNSRPS=

CWD=`pwd`
cat <<EOF >dnsrpzd.conf
PID-FILE $CWD/dnsrpzd.pid;

include $CWD/dnsrpzd-license-cur.conf

zone "policy" { type master; file "`pwd`/ns3/policy.db"; };
EOF
sed -n -e 's/^ *//' -e "/zone.*.*master/s@file \"@&$CWD/ns2/@p" ns2/*.conf \
    >>dnsrpzd.conf

# Run dnsrpzd to get the license and prime the static policy zones
if test -n "$TEST_DNSRPS"; then
    DNSRPZD="`../rpz/dnsrps -p`"
    "$DNSRPZD" -D./dnsrpzd.rpzf -S./dnsrpzd.sock -C./dnsrpzd.conf \
		-w 0 -dddd -L stdout >./dnsrpzd.run 2>&1
fi
