/*
 * Copyright (C) 2000  Internet Software Consortium.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#ifndef DIG_H
#define DIG_H

#define SDIG_BUFFER_SIZE 2048
#include <isc/lang.h>
#include <isc/socket.h>
#include <isc/buffer.h>
#include <isc/bufferlist.h>
#include <isc/sockaddr.h>
#include <isc/boolean.h>
#include <isc/mem.h>
#include <isc/list.h>
#include <isc/print.h>

#define MXSERV 4
#define MXNAME 256
#define MXRD 32
#define BUFSIZE 512
#define COMMSIZE 32767
#define RESOLVCONF "/etc/resolv.conf"
#define OUTPUTBUF 32767
#define LOOKUP_LIMIT 64
/* Lookup_limit is just a limiter, keeping too many lookups from being
 * created.  It's job is mainly to prevent the program from running away
 * in a tight loop of constant lookups.  It's value is arbitrary.
 */
#define ROOTNS 1
/* Set the number of root servers to ask for information when running in
 * trace mode.
 * XXXMWS -- trace mode is currently semi-broken, and this number *MUST*
 * be 1.
 */

ISC_LANG_BEGINDECLS

typedef struct dig_lookup dig_lookup_t;
typedef struct dig_query dig_query_t;
typedef struct dig_server dig_server_t;
typedef struct dig_searchlist dig_searchlist_t;

struct dig_lookup {
	isc_boolean_t
	        pending, /* Pending a successful answer */
		waiting_connect,
		doing_xfr,
		ns_search_only,
		use_my_server_list,
		identify,
		recurse,
		aaonly,
		trace,
		trace_root,
		defname,
		tcp_mode,
		comments,
		stats,
		section_question,
		section_answer,
		section_authority,
		section_additional;
	char textname[MXNAME]; /* Name we're going to be looking up */
	char rttext[MXRD]; /* rdata type text */
	char rctext[MXRD]; /* rdata class text */
	char namespace[BUFSIZE];
	char onamespace[BUFSIZE];
	isc_buffer_t namebuf;
	isc_buffer_t onamebuf;
	isc_buffer_t sendbuf;
	char sendspace[COMMSIZE];
	dns_name_t *name;
	isc_timer_t *timer;
	isc_interval_t interval;
	dns_message_t *sendmsg;
	dns_name_t *oname;
	ISC_LINK(dig_lookup_t) link;
	ISC_LIST(dig_query_t) q;
	ISC_LIST(dig_server_t) my_server_list;
	dig_searchlist_t *origin;
	dig_query_t *xfr_q;
	int retries;
	int nsfound;
	isc_uint16_t udpsize;
};

struct dig_query {
	dig_lookup_t *lookup;
	isc_boolean_t working,
		waiting_connect,
		first_pass,
		first_soa_rcvd;
	int retries;
	char *servname;
	isc_bufferlist_t sendlist,
		recvlist,
		lengthlist;
	isc_buffer_t recvbuf,
		lengthbuf,
		slbuf;
	char recvspace[COMMSIZE],
		lengthspace[4],
		slspace[4];
	isc_socket_t *sock;
	ISC_LINK(dig_query_t) link;
	isc_sockaddr_t sockaddr;
	isc_time_t time_sent;
};

struct dig_server {
	char servername[MXNAME];
	ISC_LINK(dig_server_t) link;
};

struct dig_searchlist {
	char origin[MXNAME];
	ISC_LINK(dig_searchlist_t) link;
};

/* Routines in dighost.c */
void
get_address(char *host, in_port_t port, isc_sockaddr_t *sockaddr);
void
fatal(char *format, ...) ;
void
debug(char *format, ...) ;
void
check_result(isc_result_t result, char *msg);
isc_boolean_t
isclass(char *text) ;
isc_boolean_t
istype(char *text) ;
void
setup_lookup(dig_lookup_t *lookup);
void
do_lookup(dig_lookup_t *lookup);
void
start_lookup (void);
void
send_udp(dig_lookup_t *lookup);
int
dhmain(int argc, char **argv);
void
setup_libs(void);
void
setup_system(void);
void
free_lists(int exitcode);
dig_lookup_t
*requeue_lookup(dig_lookup_t *lookold, isc_boolean_t servers);


/* Routines needed in dig.c and host.c */
isc_result_t
printmessage(dig_query_t *query, dns_message_t *msg, isc_boolean_t headers) ;
void
received(int bytes, int frmsize, char *frm, dig_query_t *query);
void
trying(int frmsize, char *frm, dig_lookup_t *lookup);
void
dighost_shutdown(void);

ISC_LANG_ENDDECLS

#endif
