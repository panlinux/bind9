#!/bin/sh
#
# Copyright (C) 2000, 2001, 2004, 2007, 2011, 2012, 2014-2016  Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

#
# Clean up after zone transfer tests.
#

rm -f */named.memstats
rm -f */named.run
rm -f dig.out.?.ns5.test*
rm -f dig.out.ns2.test*
rm -f dig.out.ns3.test*
rm -f dig.out.ns4.test*
rm -f log.out
rm -f ns*/named.lock
rm -f ns2/example.db
rm -f ns2/x21.db*
rm -f ns3/example.bk
rm -f ns4/x21.bk*
rm -f ns5/x21.bk-b
rm -f ns5/x21.bk-b.jnl
rm -f ns5/x21.bk-c
rm -f ns5/x21.bk-c.jnl
rm -f ns5/x21.db.jnl
