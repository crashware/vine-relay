/*
===============================================================================
                                Author: cRash
                            Date: April 25th, 2026
                Comments: Special thanks to Kate and everyone 
                who's ever believed in me -- you're the best!
===============================================================================
                                                               :
                                                              ::
                                                             ::
                                                             ::
                                                            ::
                                                            ::
                                              __           ::
                   _..-'/-¯¯--/_          ,.--. ''.     |`\=,..
                -:--.---''-..  /_         |\\_\..  \    `-.=._/
                .-|¯         '.  \         \_ \-`/\ |    ::`
                  /  @  \      \  -_   _..--|-\¯¯``'--.-/_\
                  |   .-'|      \  \\-'\_/     ¯/-.|-.\_\_/
                  \_./` /        \_//-''/    .-'
                       |           '-/'@====/              _.--.
                   __.'             /¯¯'-. \-'.          _/   /¯'
                .''____|   /       |'--\__\/-._        .'    |
                 \ \_. \  |       _| -/        \-.__  /     /
                  \___\ '/   _.  ('-..| /       '_  ''   _.'
                        /  .'     ¯¯¯¯ /        | ``'--''
                       (  / ¯```¯¯¯¯¯|-|        |
                        \ \_.         \ \      /
                         \___\         '.'.   /
                                         /    |
                                        /   .'
                                       /  .' |
                                     .'  / \  \
                                    /___| /___'
===============================================================================
"
                               ==Phrack Inc.==

                    Volume One, Issue 7, Phile 3 of 10

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
The following was written shortly after my arrest...

                       \/\The Conscience of a Hacker/\/

                                      by

                               +++The Mentor+++

                          Written on January 8, 1986
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

        Another one got caught today, it's all over the papers.  "Teenager
Arrested in Computer Crime Scandal", "Hacker Arrested after Bank Tampering"...
        Damn kids.  They're all alike.

        But did you, in your three-piece psychology and 1950's technobrain,
ever take a look behind the eyes of the hacker?  Did you ever wonder what
made him tick, what forces shaped him, what may have molded him?
        I am a hacker, enter my world...
        Mine is a world that begins with school... I'm smarter than most of
the other kids, this crap they teach us bores me...
        Damn underachiever.  They're all alike.

        I'm in junior high or high school.  I've listened to teachers explain
for the fifteenth time how to reduce a fraction.  I understand it.  "No, Ms.
Smith, I didn't show my work.  I did it in my head..."
        Damn kid.  Probably copied it.  They're all alike.

        I made a discovery today.  I found a computer.  Wait a second, this is
cool.  It does what I want it to.  If it makes a mistake, it's because I
screwed it up.  Not because it doesn't like me...
                Or feels threatened by me...
                Or thinks I'm a smart ass...
                Or doesn't like teaching and shouldn't be here...
        Damn kid.  All he does is play games.  They're all alike.

        And then it happened... a door opened to a world... rushing through
the phone line like heroin through an addict's veins, an electronic pulse is
sent out, a refuge from the day-to-day incompetencies is sought... a board is
found.
        "This is it... this is where I belong..."
        I know everyone here... even if I've never met them, never talked to
them, may never hear from them again... I know you all...
        Damn kid.  Tying up the phone line again.  They're all alike...

        You bet your ass we're all alike... we've been spoon-fed baby food at
school when we hungered for steak... the bits of meat that you did let slip
through were pre-chewed and tasteless.  We've been dominated by sadists, or
ignored by the apathetic.  The few that had something to teach found us will-
ing pupils, but those few are like drops of water in the desert.

        This is our world now... the world of the electron and the switch, the
beauty of the baud.  We make use of a service already existing without paying
for what could be dirt-cheap if it wasn't run by profiteering gluttons, and
you call us criminals.  We explore... and you call us criminals.  We seek
after knowledge... and you call us criminals.  We exist without skin color,
without nationality, without religious bias... and you call us criminals.
You build atomic bombs, you wage wars, you murder, cheat, and lie to us
and try to make us believe it's for our own good, yet we're the criminals.

        Yes, I am a criminal.  My crime is that of curiosity.  My crime is
that of judging people by what they say and think, not what they look like.
My crime is that of outsmarting you, something that you will never forgive me
for.

        I am a hacker, and this is my manifesto.  You may stop this individual,
but you can't stop us all... after all, we're all alike.

                               +++The Mentor+++
"
===============================================================================
                                 .,ad88888ba,.    
                             .,ad8888888888888a,  
                            d8P"""98888P"""98888b,
                            9b    d8888,    `9888B
                          ,d88aaa8888888b,,,d888P'
                         d8888888888888888888888b 
                        d888888P""98888888888888P 
                        88888P'    9888888888888  
                        `98P'       9888888888P'  
                                     `"9888P"'    
                                        `"'
===============================================================================
*/

/*
 * vine-relay — production relay server for the Bloom Protocol mesh
 *
 * Listens on a public address and relays encrypted messages between
 * peers that cannot reach each other directly (e.g. behind NAT).
 * Integrates metrics, warrant canary, morph engine, cover traffic,
 * greenhouse introduction point, and incentive receipt subsystems.
 */

#include <trellis/trellis.h>
#include <trellis/turn.h>
#include <uv.h>

#include "config.h"
#include "log.h"

#include <mbedtls/ssl.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/pk.h>
#include <mbedtls/error.h>
#include <mbedtls/timing.h>
#include <mbedtls/ssl_cookie.h>
#include <mbedtls/version.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>

static void on_started(const trellis_event_data_t *ev, void *ctx);
static void on_stopped(const trellis_event_data_t *ev, void *ctx);
static void on_peer_added(const trellis_event_data_t *ev, void *ctx);
static void on_peer_removed(const trellis_event_data_t *ev, void *ctx);
static void on_handshake_complete(const trellis_event_data_t *ev, void *ctx);
static void on_eclipse_detected(const trellis_event_data_t *ev, void *ctx);
static void on_gossip(const trellis_event_data_t *ev, void *ctx);
static void on_morph_changed(const trellis_event_data_t *ev, void *ctx);
static void on_routing_changed(const trellis_event_data_t *ev, void *ctx);
static void on_greenhouse_started(const trellis_event_data_t *ev, void *ctx);
static void on_greenhouse_conn(const trellis_event_data_t *ev, void *ctx);
static void on_metrics_tick(uv_timer_t *handle);

typedef struct turn_tls_ctx {
    mbedtls_x509_crt         cert;
    mbedtls_pk_context       key;
    mbedtls_entropy_context  entropy;
    mbedtls_ctr_drbg_context drbg;
    bool                     initialized;
} turn_tls_ctx_t;

// --- DTLS peer registry ---
#define TURN_DTLS_MAX_PEERS   256
#define TURN_DTLS_PEER_TTL_S  300   /* expire after 5 minutes of inactivity */

typedef struct relay_state relay_state_t;

typedef struct turn_dtls_peer {
    stun_addr_t                    addr;
    mbedtls_ssl_context            ssl;
    mbedtls_ssl_config             ssl_conf;
    mbedtls_timing_delay_context   timer;
    bool                           active;
    bool                           established;
    uint64_t                       last_active;  /* uv_now() timestamp */
    relay_state_t                 *state;

    /* Single-datagram BIO: recv holds current inbound packet, send
     * accumulates outbound (flushed after each mbedTLS call). */
    const uint8_t                 *dgram_in;
    size_t                         dgram_in_len;
    size_t                         dgram_in_off;
    uint8_t                       *dgram_out;
    size_t                         dgram_out_len;
    size_t                         dgram_out_cap;
} turn_dtls_peer_t;

// --- Relay state ---
struct relay_state {
    trellis_client_t        *client;
    trellis_metrics_t       *metrics;
    trellis_canary_timer_t  *canary_timer;
    vine_relay_config_t      config;

    // TURN server state.
    turn_server_t           *turn_srv;
    uv_tcp_t                 turn_tcp;
    bool                     turn_active;
    uv_udp_t                 turn_udp;
    bool                     turn_udp_active;
    uv_timer_t               turn_tick_timer;
    bool                     turn_tick_active;
    uv_async_t               turn_shutdown_async;
    volatile bool            turn_shutdown_done;

    // TURN startup synchronization (main thread → loop thread)
    uv_async_t               turn_setup_async;
    volatile int             turn_setup_result; /* 0=pending, 1=ok, -1=err */

    // TLS/DTLS for TURNS.
    turn_tls_ctx_t           turn_tls;
    bool                     turn_tls_required;  /* turn_transport == "tls" */
    turn_dtls_peer_t         dtls_peers[TURN_DTLS_MAX_PEERS];
    mbedtls_ssl_cookie_ctx   dtls_cookie_ctx;

    uv_loop_t                main_loop;
    uv_signal_t              sigint;
    uv_signal_t              sigterm;
    uv_signal_t              sighup;
    uv_timer_t               metrics_sync_timer;
    bool                     metrics_sync_active;
};

static relay_state_t g_state;

// --- Signal handling ---
static void on_signal(uv_signal_t *handle, int signum)
{
    relay_state_t *rs = handle->data;

    vine_info("received signal %d, shutting down...", signum);

    uv_signal_stop(&rs->sigint);
    uv_signal_stop(&rs->sigterm);
    uv_signal_stop(&rs->sighup);

    if (rs->metrics_sync_active)
        uv_timer_stop(&rs->metrics_sync_timer);

    uv_stop(&rs->main_loop);
}

static void on_sighup(uv_signal_t *handle, int signum)
{
    (void)handle;
    (void)signum;
    vine_info("received SIGHUP, reopening log file");
    vine_log_reopen();
}

// --- Daemonize ---
static int daemonize_process(void)
{
    pid_t pid = fork();
    if (pid < 0)
        return -1;
    if (pid > 0)
        _exit(0);

    if (setsid() < 0)
        return -1;

    pid = fork();
    if (pid < 0)
        return -1;
    if (pid > 0)
        _exit(0);

    umask(0);
    if (chdir("/") != 0)
        return -1;

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 0;
}

static int write_pidfile(const char *path)
{
    if (!path)
        return 0;

    FILE *f = fopen(path, "w");
    if (!f) {
        vine_error("cannot write pidfile %s: %s", path, strerror(errno));
        return -1;
    }
    fprintf(f, "%d\n", (int)getpid());
    fclose(f);
    return 0;
}

static void remove_pidfile(const char *path)
{
    if (path)
        unlink(path);
}

static void on_gh_connect(trellis_conn_t *tunnel,
                          const trellis_fingerprint_t *client_fp,
                          void *ctx)
{
    (void)ctx;
    char hex[9];
    if (client_fp)
        vine_fp_short(client_fp->bytes, hex);
    else
        strcpy(hex, "????????");
    const char *addr = tunnel ? trellis_conn_remote_addr(tunnel) : "unknown";
    vine_info("greenhouse connection from %s... (%s)", hex, addr);
}

// --- Build trellis config from vine config ---
static trellis_client_config_t build_trellis_config(const vine_relay_config_t *vc)
{
    trellis_client_config_t cfg = trellis_client_config_default();

    cfg.listen_addr                = vc->listen_addr;
    cfg.identity_path              = vc->identity_path;
    cfg.seed_addr                  = vc->seed_count > 0 ? vc->seed_addrs[0] : NULL;
    cfg.transport_mode             = (trellis_transport_mode_t)vc->transport_mode;
    cfg.wire_camouflage_type       = (trellis_camouflage_type_t)vc->camouflage_type;
    cfg.morph_metamorphic          = vc->morph_metamorphic;
    cfg.cover_traffic_ms           = vc->cover_traffic_ms;
    cfg.disable_discovery          = vc->no_discover;
    cfg.always_on_daemon           = vc->always_on;
    cfg.decoy_circuit_interval_ms  = vc->decoy_interval_ms;
    cfg.conn_pow_iterations        = vc->pow_iterations;
    cfg.guard_enabled              = vc->guard_enabled;
    cfg.guard_persist_path         = vc->guard_persist_path;
    cfg.morph_cell_mode            = vc->morph_cell_mode;
    cfg.morph_cell_size            = vc->morph_cell_size;
    cfg.verify_peer                = vc->verify_peer;
    cfg.pt_binary                  = vc->pt_binary;
    cfg.pt_transport_name          = vc->pt_transport_name;
    cfg.pt_args                    = vc->pt_args;

    if (vc->routing_mode >= 0)
        cfg.default_routing = (trellis_routing_mode_t)vc->routing_mode;

    /* When TURN is enabled with domain fronting or ECH, apply to the
     * inter-relay trellis transport as well (traffic between vine-relays
     * benefits from the same censorship resistance). */
    if (vc->turn_enabled) {
        if (vc->turn_front_host)
            cfg.tls_front_host = vc->turn_front_host;
        if (vc->turn_relay_target)
            cfg.tls_relay_target = vc->turn_relay_target;
        if (vc->turn_ech_public_name)
            cfg.tls_ech_public_name = vc->turn_ech_public_name;
        if (vc->turn_pt_binary && !cfg.pt_binary) {
            cfg.pt_binary = vc->turn_pt_binary;
            cfg.pt_transport_name = vc->turn_pt_transport;
        }
    }

    return cfg;
}

// --- Jurisdiction exclusion setup ---
static void setup_jurisdiction_exclusion(trellis_client_t *client,
                                         const vine_relay_config_t *vc)
{
    if (vc->exclude_jur_count == 0)
        return;

    trellis_router_t *router = trellis_client_get_router(client);
    if (!router) {
        vine_warn("router not available for jurisdiction exclusion");
        return;
    }

    const char *codes[VINE_MAX_JURISDICTIONS + 1];
    for (size_t i = 0; i < vc->exclude_jur_count; i++)
        codes[i] = vc->exclude_jurs[i];
    codes[vc->exclude_jur_count] = NULL;

    trellis_err_t err = trellis_route_exclude_jurisdictions(router, codes);
    if (err != TRELLIS_OK)
        vine_warn("failed to set jurisdiction exclusions: %s",
                  trellis_err_str(err));
    else
        vine_info("excluding %zu jurisdiction(s) from routing",
                  vc->exclude_jur_count);
}

static void setup_incentive(trellis_client_t *client,
                            const vine_relay_config_t *vc)
{
    if (!vc->btcpay_url)
        return;

    trellis_incentive_t *inc = trellis_client_get_incentive(client);
    if (!inc) {
        vine_warn("incentive subsystem not available");
        return;
    }

    trellis_incentive_set_lightning(inc, vc->btcpay_url, vc->btcpay_key,
                                   vc->lncli_path, vc->sats_per_mb);
    vine_info("lightning incentive configured: %s", vc->btcpay_url);
}

static void on_metrics_tick(uv_timer_t *handle)
{
    relay_state_t *rs = handle->data;
    if (!rs->metrics || !rs->client)
        return;

    trellis_metrics_set_peer_connections(
        rs->metrics, (uint64_t)trellis_client_peer_count(rs->client));

    trellis_dht_t *dht = trellis_client_get_dht(rs->client);
    if (dht)
        trellis_metrics_set_dht_peers(
            rs->metrics, (uint64_t)trellis_dht_peer_count(dht));
}

// --- Event handlers ---
static void on_started(const trellis_event_data_t *ev, void *ctx)
{
    (void)ev;
    (void)ctx;
    vine_info("client event loop running");
}

static void on_stopped(const trellis_event_data_t *ev, void *ctx)
{
    (void)ev;
    (void)ctx;
    vine_debug("client event loop stopped");
}

static void on_peer_added(const trellis_event_data_t *ev, void *ctx)
{
    relay_state_t *rs = ctx;
    char hex[9];
    vine_fp_short(ev->peer.bytes, hex);
    vine_info("peer connected: %s...", hex);

    if (rs->metrics)
        trellis_metrics_set_peer_connections(
            rs->metrics, (uint64_t)trellis_client_peer_count(rs->client));
}

static void on_peer_removed(const trellis_event_data_t *ev, void *ctx)
{
    relay_state_t *rs = ctx;
    char hex[9];
    vine_fp_short(ev->peer.bytes, hex);
    vine_info("peer disconnected: %s...", hex);

    if (rs->metrics)
        trellis_metrics_set_peer_connections(
            rs->metrics, (uint64_t)trellis_client_peer_count(rs->client));
}

static void on_handshake_complete(const trellis_event_data_t *ev, void *ctx)
{
    relay_state_t *rs = ctx;
    char hex[9];
    vine_fp_short(ev->peer.bytes, hex);
    vine_debug("handshake complete with %s...", hex);

    if (rs->metrics)
        trellis_metrics_inc_handshake_ok(rs->metrics);
}

static void on_eclipse_detected(const trellis_event_data_t *ev, void *ctx)
{
    (void)ev;
    (void)ctx;
    vine_warn("ECLIPSE ATTACK DETECTED — routing table may be compromised");
}

static void on_gossip(const trellis_event_data_t *ev, void *ctx)
{
    (void)ctx;
    if (ev->garden_name)
        vine_debug("gossip activity: %s (%zu bytes)",
                   ev->garden_name, ev->data_len);
    else
        vine_debug("gossip activity (%zu bytes)", ev->data_len);
}

static void on_morph_changed(const trellis_event_data_t *ev, void *ctx)
{
    (void)ctx;
    char hex[9];
    vine_fp_short(ev->peer.bytes, hex);
    vine_debug("morph encoding changed for peer %s...", hex);
}

static void on_routing_changed(const trellis_event_data_t *ev, void *ctx)
{
    (void)ev;
    (void)ctx;
    vine_debug("routing mode changed");
}

static void on_greenhouse_started(const trellis_event_data_t *ev, void *ctx)
{
    (void)ev;
    (void)ctx;
    vine_info("greenhouse introduction point active");
}

static void on_greenhouse_conn(const trellis_event_data_t *ev, void *ctx)
{
    relay_state_t *rs = ctx;
    (void)ev;
    vine_debug("greenhouse connection established");

    if (rs->metrics)
        trellis_metrics_inc_greenhouse_conn(rs->metrics);
}

static void on_message(const trellis_event_data_t *ev, void *ctx)
{
    relay_state_t *rs = ctx;
    if (!rs || !rs->turn_srv || !ev || !ev->data || ev->data_len == 0)
        return;

    /* Data arriving from a remote vine-relay through the trellis mesh.
     * This is the multi-relay case: the sending vine-relay forwarded TURN
     * media through the mesh because the destination peer's TURN allocation
     * lives on this server.
     *
     * To deliver, we need a mapping from the source peer's fingerprint to
     * the TURN peer_addr that the local client used in CreatePermission.
     * Without this mapping, turn_server_deliver will fail the permission
     * check. Multi-relay delivery requires a fingerprint-to-peer-address
     * registry which is not yet implemented. */
    vine_debug("TURN mesh-to-local: %zu bytes from peer (multi-relay not yet implemented)",
               ev->data_len);
    (void)rs;
}

// --- TURN server integration ---
#define TURN_MAX_CLIENTS 256
#define TURN_BIO_MAX_CAP  (256 * 1024)  /* 256KB cap on TLS/DTLS BIO buffers */

static const char *turn_alpn_protos[] = { "stun.turn", NULL };

typedef struct turn_client_conn {
    uv_tcp_t         handle;
    relay_state_t   *state;
    stun_addr_t      addr;
    uint8_t          read_buf[65536];
    uint8_t          reassembly_buf[65536];
    size_t           reassembly_len;
    bool             active;

    // TLS state (TURNS over TCP)
    bool                     tls_enabled;
    bool                     tls_established;
    mbedtls_ssl_context      ssl;
    mbedtls_ssl_config       ssl_conf;
    uint8_t                 *bio_in;
    size_t                   bio_in_len;
    size_t                   bio_in_cap;
    uint8_t                 *bio_out;
    size_t                   bio_out_len;
    size_t                   bio_out_cap;
} turn_client_conn_t;

static turn_client_conn_t *g_turn_clients[TURN_MAX_CLIENTS];
static size_t g_turn_client_count = 0;

static bool turn_addr_eq(const stun_addr_t *a, const stun_addr_t *b)
{
    if (a->family != b->family || a->port != b->port) return false;
    if (a->family == 0x01) return memcmp(a->ip.v4, b->ip.v4, 4) == 0;
    if (a->family == 0x02) return memcmp(a->ip.v6, b->ip.v6, 16) == 0;
    return false;
}

static turn_client_conn_t *turn_find_client(const stun_addr_t *addr)
{
    for (size_t i = 0; i < g_turn_client_count; i++) {
        if (g_turn_clients[i] && g_turn_clients[i]->active &&
            turn_addr_eq(&g_turn_clients[i]->addr, addr))
            return g_turn_clients[i];
    }
    return NULL;
}

static void turn_register_client(turn_client_conn_t *cc)
{
    for (size_t i = 0; i < g_turn_client_count; i++) {
        if (!g_turn_clients[i] || !g_turn_clients[i]->active) {
            g_turn_clients[i] = cc;
            return;
        }
    }
    if (g_turn_client_count < TURN_MAX_CLIENTS)
        g_turn_clients[g_turn_client_count++] = cc;
}

static void turn_unregister_client(turn_client_conn_t *cc)
{
    for (size_t i = 0; i < g_turn_client_count; i++) {
        if (g_turn_clients[i] == cc) {
            g_turn_clients[i] = NULL;
            break;
        }
    }
}

static void turn_write_done(uv_write_t *req, int status)
{
    (void)status;
    free(req->data);
    free(req);
}

static void turn_udp_send_done(uv_udp_send_t *req, int status)
{
    (void)status;
    free(req->data);
    free(req);
}

// --- TLS BIO callbacks (TCP stream) ---
static int turn_tls_bio_send(void *ctx, const unsigned char *buf, size_t len)
{
    turn_client_conn_t *cc = ctx;
    size_t needed = cc->bio_out_len + len;
    if (needed < cc->bio_out_len)
        return MBEDTLS_ERR_SSL_ALLOC_FAILED;
    if (needed > TURN_BIO_MAX_CAP)
        return MBEDTLS_ERR_SSL_ALLOC_FAILED;
    if (needed > cc->bio_out_cap) {
        size_t newcap = cc->bio_out_cap ? cc->bio_out_cap * 2 : 4096;
        while (newcap < needed)
            newcap *= 2;
        if (newcap > TURN_BIO_MAX_CAP) newcap = TURN_BIO_MAX_CAP;
        uint8_t *tmp = realloc(cc->bio_out, newcap);
        if (!tmp) return MBEDTLS_ERR_SSL_ALLOC_FAILED;
        cc->bio_out = tmp;
        cc->bio_out_cap = newcap;
    }
    memcpy(cc->bio_out + cc->bio_out_len, buf, len);
    cc->bio_out_len += len;
    return (int)len;
}

static int turn_tls_bio_recv(void *ctx, unsigned char *buf, size_t len)
{
    turn_client_conn_t *cc = ctx;
    if (cc->bio_in_len == 0)
        return MBEDTLS_ERR_SSL_WANT_READ;
    size_t to_copy = len < cc->bio_in_len ? len : cc->bio_in_len;
    memcpy(buf, cc->bio_in, to_copy);
    cc->bio_in_len -= to_copy;
    if (cc->bio_in_len > 0)
        memmove(cc->bio_in, cc->bio_in + to_copy, cc->bio_in_len);
    return (int)to_copy;
}

typedef struct turn_tls_flush_req {
    uv_write_t req;
    uint8_t   *data;
} turn_tls_flush_req_t;

static void turn_tls_on_flush(uv_write_t *req, int status)
{
    (void)status;
    turn_tls_flush_req_t *fr = req->data;
    free(fr->data);
    free(fr);
}

static void turn_tls_close_cb(uv_handle_t *handle);

static void turn_tls_flush_bio_out(turn_client_conn_t *cc)
{
    if (cc->bio_out_len == 0) return;
    if (uv_is_closing((uv_handle_t *)&cc->handle)) {
        cc->bio_out_len = 0;
        return;
    }

    turn_tls_flush_req_t *fr = malloc(sizeof(*fr));
    if (!fr) return;
    fr->data = malloc(cc->bio_out_len);
    if (!fr->data) { free(fr); return; }

    memcpy(fr->data, cc->bio_out, cc->bio_out_len);
    size_t len = cc->bio_out_len;
    cc->bio_out_len = 0;

    fr->req.data = fr;
    uv_buf_t uvbuf = uv_buf_init((char *)fr->data, (unsigned int)len);
    if (uv_write(&fr->req, (uv_stream_t *)&cc->handle, &uvbuf, 1,
                  turn_tls_on_flush) != 0) {
        free(fr->data);
        free(fr);
        if (cc->active && !uv_is_closing((uv_handle_t *)&cc->handle)) {
            vine_error("TURNS TCP write failed, closing connection");
            cc->active = false;
            turn_unregister_client(cc);
            uv_close((uv_handle_t *)&cc->handle, turn_tls_close_cb);
        }
    }
}

static void turn_tls_client_free(turn_client_conn_t *cc)
{
    if (cc->tls_enabled) {
        mbedtls_ssl_free(&cc->ssl);
        mbedtls_ssl_config_free(&cc->ssl_conf);
        free(cc->bio_in);
        free(cc->bio_out);
        cc->bio_in = NULL;
        cc->bio_out = NULL;
    }
    free(cc);
}

// --- DTLS BIO callbacks (single-datagram) ---
static int turn_dtls_bio_send(void *ctx, const unsigned char *buf, size_t len)
{
    turn_dtls_peer_t *dp = ctx;
    size_t needed = dp->dgram_out_len + len;
    if (needed < dp->dgram_out_len)
        return MBEDTLS_ERR_SSL_ALLOC_FAILED;
    if (needed > TURN_BIO_MAX_CAP)
        return MBEDTLS_ERR_SSL_ALLOC_FAILED;
    if (needed > dp->dgram_out_cap) {
        size_t newcap = dp->dgram_out_cap ? dp->dgram_out_cap * 2 : 4096;
        while (newcap < needed) newcap *= 2;
        if (newcap > TURN_BIO_MAX_CAP) newcap = TURN_BIO_MAX_CAP;
        uint8_t *tmp = realloc(dp->dgram_out, newcap);
        if (!tmp) return MBEDTLS_ERR_SSL_ALLOC_FAILED;
        dp->dgram_out = tmp;
        dp->dgram_out_cap = newcap;
    }
    memcpy(dp->dgram_out + dp->dgram_out_len, buf, len);
    dp->dgram_out_len += len;
    return (int)len;
}

static int turn_dtls_bio_recv(void *ctx, unsigned char *buf, size_t len)
{
    turn_dtls_peer_t *dp = ctx;
    size_t avail = dp->dgram_in_len - dp->dgram_in_off;
    if (avail == 0)
        return MBEDTLS_ERR_SSL_WANT_READ;
    size_t to_copy = len < avail ? len : avail;
    memcpy(buf, dp->dgram_in + dp->dgram_in_off, to_copy);
    dp->dgram_in_off += to_copy;
    return (int)to_copy;
}

static void turn_dtls_flush(turn_dtls_peer_t *dp)
{
    if (dp->dgram_out_len == 0 || !dp->state) return;
    relay_state_t *rs = dp->state;
    if (!rs->turn_udp_active) return;

    uint8_t *copy = malloc(dp->dgram_out_len);
    if (!copy) { dp->dgram_out_len = 0; return; }
    memcpy(copy, dp->dgram_out, dp->dgram_out_len);

    uv_udp_send_t *sreq = malloc(sizeof(uv_udp_send_t));
    if (!sreq) { free(copy); dp->dgram_out_len = 0; return; }
    sreq->data = copy;

    struct sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    if (dp->addr.family == 0x01) {
        struct sockaddr_in *sin = (struct sockaddr_in *)&ss;
        sin->sin_family = AF_INET;
        sin->sin_port = htons(dp->addr.port);
        memcpy(&sin->sin_addr, dp->addr.ip.v4, 4);
    } else {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&ss;
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(dp->addr.port);
        memcpy(&sin6->sin6_addr, dp->addr.ip.v6, 16);
    }

    uv_buf_t buf = uv_buf_init((char *)copy, (unsigned int)dp->dgram_out_len);
    dp->dgram_out_len = 0;

    if (uv_udp_send(sreq, &rs->turn_udp, &buf, 1,
                     (const struct sockaddr *)&ss, turn_udp_send_done) != 0) {
        free(copy);
        free(sreq);
    }
}

// --- DTLS peer lookup / creation ---
static turn_dtls_peer_t *turn_dtls_find_peer(relay_state_t *rs,
                                              const stun_addr_t *addr)
{
    for (int i = 0; i < TURN_DTLS_MAX_PEERS; i++) {
        turn_dtls_peer_t *dp = &rs->dtls_peers[i];
        if (dp->active && turn_addr_eq(&dp->addr, addr))
            return dp;
    }
    return NULL;
}

static turn_dtls_peer_t *turn_dtls_create_peer(relay_state_t *rs,
                                                const stun_addr_t *addr)
{
    turn_dtls_peer_t *dp = NULL;
    for (int i = 0; i < TURN_DTLS_MAX_PEERS; i++) {
        if (!rs->dtls_peers[i].active) { dp = &rs->dtls_peers[i]; break; }
    }
    if (!dp) return NULL;

    memset(dp, 0, sizeof(*dp));
    dp->addr = *addr;
    dp->state = rs;
    dp->active = true;
    dp->established = false;

    uv_loop_t *loop = trellis_client_get_loop(rs->client);
    dp->last_active = loop ? uv_now(loop) : 0;

    mbedtls_ssl_init(&dp->ssl);
    mbedtls_ssl_config_init(&dp->ssl_conf);

    int ret = mbedtls_ssl_config_defaults(&dp->ssl_conf,
                                          MBEDTLS_SSL_IS_SERVER,
                                          MBEDTLS_SSL_TRANSPORT_DATAGRAM,
                                          MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0) goto fail;

    mbedtls_ssl_conf_min_tls_version(&dp->ssl_conf,
                                     MBEDTLS_SSL_VERSION_TLS1_2);
    mbedtls_ssl_conf_rng(&dp->ssl_conf, mbedtls_ctr_drbg_random,
                         &rs->turn_tls.drbg);
    mbedtls_ssl_conf_own_cert(&dp->ssl_conf, &rs->turn_tls.cert,
                              &rs->turn_tls.key);
    mbedtls_ssl_conf_dtls_cookies(&dp->ssl_conf,
                                  mbedtls_ssl_cookie_write,
                                  mbedtls_ssl_cookie_check,
                                  &rs->dtls_cookie_ctx);
    mbedtls_ssl_conf_alpn_protocols(&dp->ssl_conf, turn_alpn_protos);

    ret = mbedtls_ssl_setup(&dp->ssl, &dp->ssl_conf);
    if (ret != 0) goto fail;

    mbedtls_ssl_set_bio(&dp->ssl, dp, turn_dtls_bio_send, turn_dtls_bio_recv,
                        NULL);
    mbedtls_ssl_set_timer_cb(&dp->ssl, &dp->timer,
                             mbedtls_timing_set_delay,
                             mbedtls_timing_get_delay);
    mbedtls_ssl_set_mtu(&dp->ssl, 1200);

    // Set client transport ID for cookie verification.
    {
        unsigned char cid[18];
        size_t cid_len = 0;
        if (addr->family == 0x01) {
            memcpy(cid, addr->ip.v4, 4);
            cid[4] = (uint8_t)(addr->port >> 8);
            cid[5] = (uint8_t)(addr->port & 0xFF);
            cid_len = 6;
        } else {
            memcpy(cid, addr->ip.v6, 16);
            cid[16] = (uint8_t)(addr->port >> 8);
            cid[17] = (uint8_t)(addr->port & 0xFF);
            cid_len = 18;
        }
        mbedtls_ssl_set_client_transport_id(&dp->ssl, cid, cid_len);
    }

    return dp;

fail:
    mbedtls_ssl_free(&dp->ssl);
    mbedtls_ssl_config_free(&dp->ssl_conf);
    dp->active = false;
    return NULL;
}

static void turn_dtls_destroy_peer(turn_dtls_peer_t *dp)
{
    if (!dp->active) return;
    mbedtls_ssl_close_notify(&dp->ssl);
    turn_dtls_flush(dp);
    mbedtls_ssl_free(&dp->ssl);
    mbedtls_ssl_config_free(&dp->ssl_conf);
    free(dp->dgram_out);
    dp->dgram_out = NULL;
    dp->dgram_out_len = 0;
    dp->dgram_out_cap = 0;
    dp->active = false;
    dp->established = false;
}

// --- TLS context initialization ---
static int turn_tls_ctx_init(turn_tls_ctx_t *ctx, const char *cert_pem,
                             const char *key_pem)
{
    mbedtls_x509_crt_init(&ctx->cert);
    mbedtls_pk_init(&ctx->key);
    mbedtls_entropy_init(&ctx->entropy);
    mbedtls_ctr_drbg_init(&ctx->drbg);

    int ret = mbedtls_ctr_drbg_seed(&ctx->drbg, mbedtls_entropy_func,
                                    &ctx->entropy,
                                    (const unsigned char *)"vine-turn-tls",
                                    13);
    if (ret != 0) {
        char errbuf[128];
        mbedtls_strerror(ret, errbuf, sizeof(errbuf));
        vine_error("TURNS DRBG seed failed: %s", errbuf);
        mbedtls_ctr_drbg_free(&ctx->drbg);
        mbedtls_entropy_free(&ctx->entropy);
        mbedtls_pk_free(&ctx->key);
        mbedtls_x509_crt_free(&ctx->cert);
        return -1;
    }

    // Load cert — try file path first, then PEM string.
    ret = mbedtls_x509_crt_parse_file(&ctx->cert, cert_pem);
    if (ret != 0) {
        ret = mbedtls_x509_crt_parse(&ctx->cert,
                                     (const unsigned char *)cert_pem,
                                     strlen(cert_pem) + 1);
    }
    if (ret != 0) {
        char errbuf[128];
        mbedtls_strerror(ret, errbuf, sizeof(errbuf));
        vine_error("TURNS cert load failed: %s", errbuf);
        mbedtls_x509_crt_free(&ctx->cert);
        mbedtls_pk_free(&ctx->key);
        mbedtls_ctr_drbg_free(&ctx->drbg);
        mbedtls_entropy_free(&ctx->entropy);
        return -1;
    }

    // Load key — try file path first, then PEM string.
    ret = mbedtls_pk_parse_keyfile(&ctx->key, key_pem, NULL,
                                  mbedtls_ctr_drbg_random, &ctx->drbg);
    if (ret != 0) {
        ret = mbedtls_pk_parse_key(&ctx->key,
                                   (const unsigned char *)key_pem,
                                   strlen(key_pem) + 1,
                                   NULL, 0,
                                   mbedtls_ctr_drbg_random, &ctx->drbg);
    }
    if (ret != 0) {
        char errbuf[128];
        mbedtls_strerror(ret, errbuf, sizeof(errbuf));
        vine_error("TURNS key load failed: %s", errbuf);
        mbedtls_x509_crt_free(&ctx->cert);
        mbedtls_pk_free(&ctx->key);
        mbedtls_ctr_drbg_free(&ctx->drbg);
        mbedtls_entropy_free(&ctx->entropy);
        return -1;
    }

    ctx->initialized = true;
    vine_info("TURNS TLS context initialized (cert + key loaded)");
    return 0;
}

static void turn_tls_ctx_free(turn_tls_ctx_t *ctx)
{
    if (!ctx->initialized) return;
    mbedtls_x509_crt_free(&ctx->cert);
    mbedtls_pk_free(&ctx->key);
    mbedtls_ctr_drbg_free(&ctx->drbg);
    mbedtls_entropy_free(&ctx->entropy);
    ctx->initialized = false;
}

static void turn_send_udp(relay_state_t *rs, const uint8_t *data, size_t len,
                          const stun_addr_t *dst)
{
    uint8_t *copy = malloc(len);
    if (!copy) return;
    memcpy(copy, data, len);

    uv_udp_send_t *sreq = malloc(sizeof(uv_udp_send_t));
    if (!sreq) { free(copy); return; }
    sreq->data = copy;

    struct sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    if (dst->family == 0x01) {
        struct sockaddr_in *sin = (struct sockaddr_in *)&ss;
        sin->sin_family = AF_INET;
        sin->sin_port = htons(dst->port);
        memcpy(&sin->sin_addr, dst->ip.v4, 4);
    } else {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&ss;
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(dst->port);
        memcpy(&sin6->sin6_addr, dst->ip.v6, 16);
    }

    uv_buf_t buf = uv_buf_init((char *)copy, (unsigned int)len);
    if (uv_udp_send(sreq, &rs->turn_udp, &buf, 1,
                     (const struct sockaddr *)&ss,
                     turn_udp_send_done) != 0) {
        free(copy);
        free(sreq);
    }
}

static void turn_send_dtls(relay_state_t *rs, const uint8_t *data, size_t len,
                           const stun_addr_t *dst);

static void turn_send_to_client(const uint8_t *data, size_t len,
                                const stun_addr_t *dst, void *ctx)
{
    relay_state_t *rs = (relay_state_t *)ctx;
    if (!data || len == 0 || !dst) return;

    // Try TCP first: if the client has an active TCP connection, use it.
    turn_client_conn_t *cc = turn_find_client(dst);
    if (cc && !uv_is_closing((uv_handle_t *)&cc->handle)) {
        if (cc->tls_enabled && cc->tls_established) {
            size_t written = 0;
            int retries = 0;
            while (written < len) {
                if (uv_is_closing((uv_handle_t *)&cc->handle))
                    return;
                int ret = mbedtls_ssl_write(&cc->ssl, data + written,
                                            len - written);
                if (ret < 0) {
                    if ((ret == MBEDTLS_ERR_SSL_WANT_WRITE ||
                         ret == MBEDTLS_ERR_SSL_WANT_READ) &&
                        ++retries < 16) {
                        turn_tls_flush_bio_out(cc);
                        continue;
                    }
                    vine_error("TURNS write failed: -0x%04x", (unsigned)-ret);
                    return;
                }
                written += (size_t)ret;
                retries = 0;
            }
            turn_tls_flush_bio_out(cc);
        } else {
            // Plain TCP.
            uint8_t *frame = malloc(len);
            if (!frame) return;
            memcpy(frame, data, len);

            uv_write_t *wreq = malloc(sizeof(uv_write_t));
            if (!wreq) { free(frame); return; }
            wreq->data = frame;

            uv_buf_t buf = uv_buf_init((char *)frame, (unsigned int)len);
            if (uv_write(wreq, (uv_stream_t *)&cc->handle, &buf, 1,
                          turn_write_done) != 0) {
                free(frame);
                free(wreq);
            }
        }
        return;
    }

    // No TCP connection found — send via UDP.
    if (rs && rs->turn_udp_active) {
        if (rs->turn_tls.initialized) {
            turn_send_dtls(rs, data, len, dst);
        } else {
            turn_send_udp(rs, data, len, dst);
        }
    }
}

static void turn_send_dtls(relay_state_t *rs, const uint8_t *data, size_t len,
                           const stun_addr_t *dst)
{
    turn_dtls_peer_t *dp = turn_dtls_find_peer(rs, dst);
    if (!dp || !dp->established)
        return;

    int ret = mbedtls_ssl_write(&dp->ssl, data, len);
    if (ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
        turn_dtls_flush(dp);
        ret = mbedtls_ssl_write(&dp->ssl, data, len);
    }
    if (ret < 0)
        vine_debug("DTLS write to peer failed: -0x%04x", (unsigned)-ret);
    turn_dtls_flush(dp);
}

// --- UDP TURN listener ---
static void turn_udp_alloc_buf(uv_handle_t *handle, size_t suggested,
                               uv_buf_t *buf)
{
    (void)handle;
    static uint8_t udp_read_buf[65536];
    (void)suggested;
    buf->base = (char *)udp_read_buf;
    buf->len = sizeof(udp_read_buf);
}

static void turn_udp_on_recv(uv_udp_t *handle, ssize_t nread,
                             const uv_buf_t *buf,
                             const struct sockaddr *addr,
                             unsigned flags)
{
    (void)flags;
    relay_state_t *rs = (relay_state_t *)handle->data;
    if (!rs || !rs->turn_srv) return;
    if (nread <= 0 || !addr) return;

    stun_addr_t client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *sin = (const struct sockaddr_in *)addr;
        client_addr.family = 0x01;
        client_addr.port = ntohs(sin->sin_port);
        memcpy(client_addr.ip.v4, &sin->sin_addr, 4);
    } else if (addr->sa_family == AF_INET6) {
        const struct sockaddr_in6 *sin6 = (const struct sockaddr_in6 *)addr;
        client_addr.family = 0x02;
        client_addr.port = ntohs(sin6->sin6_port);
        memcpy(client_addr.ip.v6, &sin6->sin6_addr, 16);
    } else {
        return;
    }

    if (!rs->turn_tls.initialized) {
        // Plain UDP path.
        turn_server_process(rs->turn_srv,
                            (const uint8_t *)buf->base, (size_t)nread,
                            &client_addr, 17 /* UDP */);
        return;
    }

    // DTLS path.
    turn_dtls_peer_t *dp = turn_dtls_find_peer(rs, &client_addr);
    if (!dp) {
        dp = turn_dtls_create_peer(rs, &client_addr);
        if (!dp) {
            vine_debug("DTLS peer limit reached, dropping datagram");
            return;
        }
    }

    uv_loop_t *loop = trellis_client_get_loop(rs->client);
    if (loop) dp->last_active = uv_now(loop);

    // Feed the received datagram to mbedTLS.
    dp->dgram_in = (const uint8_t *)buf->base;
    dp->dgram_in_len = (size_t)nread;
    dp->dgram_in_off = 0;

    if (!dp->established) {
        int ret = mbedtls_ssl_handshake(&dp->ssl);
        turn_dtls_flush(dp);

        if (ret == MBEDTLS_ERR_SSL_WANT_READ ||
            ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
            dp->dgram_in = NULL;
            return;
        }
        if (ret == MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED) {
            // Cookie exchange: reset and wait for retransmit.
            mbedtls_ssl_session_reset(&dp->ssl);
            // Re-set client transport ID after reset.
            unsigned char cid[18];
            size_t cid_len = 0;
            if (client_addr.family == 0x01) {
                memcpy(cid, client_addr.ip.v4, 4);
                cid[4] = (uint8_t)(client_addr.port >> 8);
                cid[5] = (uint8_t)(client_addr.port & 0xFF);
                cid_len = 6;
            } else {
                memcpy(cid, client_addr.ip.v6, 16);
                cid[16] = (uint8_t)(client_addr.port >> 8);
                cid[17] = (uint8_t)(client_addr.port & 0xFF);
                cid_len = 18;
            }
            mbedtls_ssl_set_client_transport_id(&dp->ssl, cid, cid_len);
            dp->dgram_in = NULL;
            return;
        }
        if (ret != 0) {
            vine_debug("DTLS handshake failed: -0x%04x", (unsigned)-ret);
            turn_dtls_destroy_peer(dp);
            dp->dgram_in = NULL;
            return;
        }

        dp->established = true;
        vine_debug("DTLS handshake complete for peer port %u",
                   client_addr.port);
    }

    // Read decrypted application data.
    uint8_t plaintext[65536];
    int ret = mbedtls_ssl_read(&dp->ssl, plaintext, sizeof(plaintext));
    turn_dtls_flush(dp);
    dp->dgram_in = NULL;

    if (ret > 0) {
        turn_server_process(rs->turn_srv, plaintext, (size_t)ret,
                            &client_addr, 17 /* UDP */);
    } else if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
        turn_dtls_destroy_peer(dp);
    } else if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
               ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
        vine_debug("DTLS read error: -0x%04x", (unsigned)-ret);
    }
}

// --- Mesh relay callback ---
static void turn_relay_to_mesh(uint64_t alloc_id,
                               const stun_addr_t *peer_addr,
                               const uint8_t *data, size_t len,
                               void *ctx)
{
    relay_state_t *rs = (relay_state_t *)ctx;
    if (!rs || !rs->turn_srv || !data || len == 0) return;

    vine_debug("TURN relay: alloc %lu, %zu bytes via peer %u.%u.%u.%u:%u",
               (unsigned long)alloc_id, len,
               peer_addr->ip.v4[0], peer_addr->ip.v4[1],
               peer_addr->ip.v4[2], peer_addr->ip.v4[3], peer_addr->port);

    /* Local delivery: peer_addr is the destination relay address. Find the
     * allocation that owns that relay address (the receiver), and the
     * allocation that sent the data (the sender). Deliver to the receiver
     * with the sender's relay address as the source so it passes the
     * receiver's permission check. */
    turn_allocation_t *recv_alloc = turn_server_find_alloc_by_relay(rs->turn_srv, peer_addr);
    if (recv_alloc) {
        turn_allocation_t *send_alloc = turn_server_find_alloc_by_id(rs->turn_srv, alloc_id);
        if (send_alloc) {
            turn_server_deliver(rs->turn_srv, recv_alloc->id,
                                &send_alloc->relay_addr, data, len);
            return;
        }
    }

    /* No local allocation matched. In a multi-relay deployment, data would
     * be forwarded through the trellis mesh to the remote vine-relay that
     * owns the allocation. This requires a fingerprint-to-allocation mapping
     * which is not yet implemented. */
    vine_debug("TURN relay: no local allocation for peer, mesh routing not yet implemented");
}

static void turn_tick_cb(uv_timer_t *handle)
{
    relay_state_t *rs = (relay_state_t *)handle->data;
    if (!rs) return;

    if (rs->turn_srv)
        turn_server_tick(rs->turn_srv);

    // Sweep stale DTLS peers.
    if (rs->turn_tls.initialized) {
        uv_loop_t *loop = handle->loop;
        uint64_t now = uv_now(loop);
        for (int i = 0; i < TURN_DTLS_MAX_PEERS; i++) {
            turn_dtls_peer_t *dp = &rs->dtls_peers[i];
            if (dp->active &&
                now - dp->last_active > TURN_DTLS_PEER_TTL_S * 1000) {
                vine_debug("DTLS peer expired (port %u)", dp->addr.port);
                turn_dtls_destroy_peer(dp);
            }
        }
    }
}

static void turn_on_alloc_buf(uv_handle_t *handle, size_t suggested,
                              uv_buf_t *buf)
{
    turn_client_conn_t *cc = (turn_client_conn_t *)handle->data;
    (void)suggested;
    buf->base = (char *)cc->read_buf;
    buf->len = sizeof(cc->read_buf);
}

static void turn_process_framed(turn_client_conn_t *cc)
{
    /* Process complete STUN/ChannelData messages from the reassembly buffer.
     * TURN-over-TCP uses self-framing: STUN messages have a 20-byte header
     * with a length field; ChannelData has a 4-byte header with a length
     * field. The first byte discriminates: 0x00/0x01 = STUN, 0x40-0x4F =
     * ChannelData (RFC 5766 §11.6). */
    while (cc->reassembly_len >= 4) {
        const uint8_t *p = cc->reassembly_buf;
        size_t total;

        uint8_t first_byte = p[0];
        if (first_byte >= 0x40 && first_byte <= 0x7F) {
            /* ChannelData: [channel(2)][length(2)][data][padding]
             * Channel numbers 0x4000-0x7FFF (RFC 5766 §11.4).
             * Over TCP, pad to 4-byte boundary (RFC 5766 §11.5). */
            uint16_t data_len = ((uint16_t)p[2] << 8) | p[3];
            total = (4 + (size_t)data_len + 3) & ~(size_t)3;
        } else if ((first_byte & 0xC0) == 0x00) {
            /* STUN message: first two bits are 00 (RFC 5389 §6).
             * 20-byte header, bytes 2-3 = message length. */
            uint16_t stun_len = ((uint16_t)p[2] << 8) | p[3];
            total = 20 + (size_t)stun_len;
        } else {
            /* Unknown first byte — protocol error, discard one byte to
             * attempt resync. */
            cc->reassembly_len -= 1;
            if (cc->reassembly_len > 0)
                memmove(cc->reassembly_buf, cc->reassembly_buf + 1, cc->reassembly_len);
            continue;
        }

        if (total == 0 || cc->reassembly_len < total)
            return; /* incomplete */

        turn_server_process(cc->state->turn_srv,
                            p, total,
                            &cc->addr, 6 /* TCP */);

        cc->reassembly_len -= total;
        if (cc->reassembly_len > 0)
            memmove(cc->reassembly_buf, cc->reassembly_buf + total, cc->reassembly_len);
    }
}

static void turn_tls_close_cb(uv_handle_t *handle)
{
    turn_client_conn_t *cc = (turn_client_conn_t *)handle->data;
    if (cc) turn_tls_client_free(cc);
}

static void turn_on_read(uv_stream_t *stream, ssize_t nread,
                         const uv_buf_t *buf)
{
    turn_client_conn_t *cc = (turn_client_conn_t *)stream->data;
    if (!cc || !cc->state || !cc->state->turn_srv) return;

    if (nread <= 0) {
        if (nread < 0) {
            vine_debug("TURN client disconnected");
            cc->active = false;
            turn_unregister_client(cc);
            if (cc->tls_enabled)
                uv_close((uv_handle_t *)stream, turn_tls_close_cb);
            else
                uv_close((uv_handle_t *)stream, (uv_close_cb)free);
        }
        return;
    }

    (void)buf;

    if (cc->tls_enabled) {
        // Append raw TCP bytes to TLS BIO input buffer.
        size_t needed = cc->bio_in_len + (size_t)nread;
        if (needed < cc->bio_in_len || needed > TURN_BIO_MAX_CAP) {
            vine_error("TURNS BIO input exceeds cap, closing");
            cc->active = false;
            turn_unregister_client(cc);
            uv_close((uv_handle_t *)stream, turn_tls_close_cb);
            return;
        }
        if (needed > cc->bio_in_cap) {
            size_t newcap = cc->bio_in_cap ? cc->bio_in_cap * 2 : 4096;
            while (newcap < needed) newcap *= 2;
            if (newcap > TURN_BIO_MAX_CAP) newcap = TURN_BIO_MAX_CAP;
            uint8_t *tmp = realloc(cc->bio_in, newcap);
            if (!tmp) {
                vine_error("TURNS BIO alloc failed, closing");
                cc->active = false;
                turn_unregister_client(cc);
                uv_close((uv_handle_t *)stream, turn_tls_close_cb);
                return;
            }
            cc->bio_in = tmp;
            cc->bio_in_cap = newcap;
        }
        memcpy(cc->bio_in + cc->bio_in_len, cc->read_buf, (size_t)nread);
        cc->bio_in_len += (size_t)nread;

        if (!cc->tls_established) {
            // Drive TLS handshake.
            int ret = mbedtls_ssl_handshake(&cc->ssl);
            turn_tls_flush_bio_out(cc);

            if (uv_is_closing((uv_handle_t *)stream))
                return;

            if (ret == MBEDTLS_ERR_SSL_WANT_READ ||
                ret == MBEDTLS_ERR_SSL_WANT_WRITE)
                return;

            if (ret != 0) {
                vine_error("TURNS handshake failed: -0x%04x", (unsigned)-ret);
                cc->active = false;
                turn_unregister_client(cc);
                uv_close((uv_handle_t *)stream, turn_tls_close_cb);
                return;
            }

            cc->tls_established = true;
            vine_debug("TURNS handshake complete for client port %u",
                       cc->addr.port);
        }

        // Decrypt application data into reassembly buffer.
        for (;;) {
            size_t avail = sizeof(cc->reassembly_buf) - cc->reassembly_len;
            if (avail == 0) {
                vine_error("TURNS reassembly overflow, closing");
                cc->active = false;
                turn_unregister_client(cc);
                uv_close((uv_handle_t *)stream, turn_tls_close_cb);
                return;
            }
            int ret = mbedtls_ssl_read(&cc->ssl,
                                       cc->reassembly_buf + cc->reassembly_len,
                                       avail);
            if (ret == MBEDTLS_ERR_SSL_WANT_READ ||
                ret == MBEDTLS_ERR_SSL_WANT_WRITE)
                break;
            if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY || ret <= 0) {
                if (ret != MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY && ret < 0)
                    vine_debug("TURNS read error: -0x%04x", (unsigned)-ret);
                cc->active = false;
                turn_unregister_client(cc);
                uv_close((uv_handle_t *)stream, turn_tls_close_cb);
                return;
            }
            cc->reassembly_len += (size_t)ret;
        }
        turn_tls_flush_bio_out(cc);
        if (!uv_is_closing((uv_handle_t *)stream))
            turn_process_framed(cc);
        return;
    }

    // Plain TCP path (unchanged)
    size_t avail = sizeof(cc->reassembly_buf) - cc->reassembly_len;
    if ((size_t)nread > avail) {
        vine_error("TURN client reassembly buffer overflow, closing connection");
        cc->active = false;
        turn_unregister_client(cc);
        if (!uv_is_closing((uv_handle_t *)stream))
            uv_close((uv_handle_t *)stream, (uv_close_cb)free);
        return;
    }
    memcpy(cc->reassembly_buf + cc->reassembly_len, cc->read_buf, (size_t)nread);
    cc->reassembly_len += (size_t)nread;

    turn_process_framed(cc);
}

static bool turn_has_client_slot(void)
{
    for (size_t i = 0; i < g_turn_client_count; i++) {
        if (!g_turn_clients[i] || !g_turn_clients[i]->active)
            return true;
    }
    return g_turn_client_count < TURN_MAX_CLIENTS;
}

static void turn_on_connection(uv_stream_t *server, int status)
{
    if (status < 0) return;
    relay_state_t *rs = (relay_state_t *)server->data;

    turn_client_conn_t *cc = calloc(1, sizeof(turn_client_conn_t));
    if (!cc) return;

    if (!turn_has_client_slot()) {
        vine_error("TURN client limit (%d) reached, rejecting connection",
                   TURN_MAX_CLIENTS);
        uv_tcp_init(server->loop, &cc->handle);
        uv_accept(server, (uv_stream_t *)&cc->handle);
        uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
        return;
    }

    cc->state = rs;
    uv_tcp_init(server->loop, &cc->handle);
    cc->handle.data = cc;

    if (uv_accept(server, (uv_stream_t *)&cc->handle) != 0) {
        uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
        return;
    }

    // Extract client address.
    struct sockaddr_storage ss;
    int sslen = sizeof(ss);
    int peer_rc = uv_tcp_getpeername(&cc->handle, (struct sockaddr *)&ss, &sslen);
    if (peer_rc != 0) {
        vine_error("TURN client getpeername failed: %s", uv_strerror(peer_rc));
        uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
        return;
    }
    if (ss.ss_family == AF_INET) {
        struct sockaddr_in *sin = (struct sockaddr_in *)&ss;
        cc->addr.family = 0x01;
        cc->addr.port = ntohs(sin->sin_port);
        memcpy(cc->addr.ip.v4, &sin->sin_addr, 4);
    } else if (ss.ss_family == AF_INET6) {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&ss;
        cc->addr.family = 0x02;
        cc->addr.port = ntohs(sin6->sin6_port);
        memcpy(cc->addr.ip.v6, &sin6->sin6_addr, 16);
    } else {
        uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
        return;
    }

    // Initialize TLS if cert+key are configured.
    if (rs->turn_tls.initialized) {
        cc->tls_enabled = true;
        cc->tls_established = false;

        mbedtls_ssl_init(&cc->ssl);
        mbedtls_ssl_config_init(&cc->ssl_conf);

        int ret = mbedtls_ssl_config_defaults(&cc->ssl_conf,
                                              MBEDTLS_SSL_IS_SERVER,
                                              MBEDTLS_SSL_TRANSPORT_STREAM,
                                              MBEDTLS_SSL_PRESET_DEFAULT);
        if (ret != 0) {
            vine_error("TURNS ssl_config_defaults failed: -0x%04x",
                       (unsigned)-ret);
            mbedtls_ssl_free(&cc->ssl);
            mbedtls_ssl_config_free(&cc->ssl_conf);
            cc->tls_enabled = false;
            uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
            return;
        }

        mbedtls_ssl_conf_min_tls_version(&cc->ssl_conf,
                                         MBEDTLS_SSL_VERSION_TLS1_2);
        mbedtls_ssl_conf_rng(&cc->ssl_conf, mbedtls_ctr_drbg_random,
                             &rs->turn_tls.drbg);
        mbedtls_ssl_conf_own_cert(&cc->ssl_conf, &rs->turn_tls.cert,
                                  &rs->turn_tls.key);
        mbedtls_ssl_conf_alpn_protocols(&cc->ssl_conf, turn_alpn_protos);

        ret = mbedtls_ssl_setup(&cc->ssl, &cc->ssl_conf);
        if (ret != 0) {
            vine_error("TURNS ssl_setup failed: -0x%04x", (unsigned)-ret);
            mbedtls_ssl_free(&cc->ssl);
            mbedtls_ssl_config_free(&cc->ssl_conf);
            cc->tls_enabled = false;
            uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
            return;
        }

        mbedtls_ssl_set_bio(&cc->ssl, cc, turn_tls_bio_send,
                            turn_tls_bio_recv, NULL);
    }

    cc->active = true;
    turn_register_client(cc);
    vine_debug("TURN%s client connected from port %u",
               cc->tls_enabled ? "S" : "", cc->addr.port);

    uv_read_start((uv_stream_t *)&cc->handle, turn_on_alloc_buf, turn_on_read);
}

/* Runs on the client's event loop thread via uv_async_send. Sets up
 * TURN TCP/UDP listeners, tick timer, etc. from within the loop thread
 * so all libuv handle operations are thread-safe. */
static void turn_setup_on_loop(uv_async_t *async)
{
    relay_state_t *rs = (relay_state_t *)async->data;
    const vine_relay_config_t *vc = &rs->config;
    uv_loop_t *loop = async->loop;

    const char *listen_addr = vc->turn_listen_addr ? vc->turn_listen_addr : "0.0.0.0:443";
    char host[256];
    uint16_t port = 443;
    strncpy(host, listen_addr, sizeof(host) - 1);
    host[sizeof(host) - 1] = '\0';
    char *colon = strrchr(host, ':');
    if (colon) {
        *colon = '\0';
        port = (uint16_t)atoi(colon + 1);
    }

    // Determine if TLS is mandatory.
    rs->turn_tls_required = vc->turn_transport &&
                            (strcmp(vc->turn_transport, "tls") == 0);

    // Initialize TLS/DTLS context if cert+key are provided.
    if (vc->turn_cert_pem && vc->turn_key_pem) {
        if (turn_tls_ctx_init(&rs->turn_tls, vc->turn_cert_pem,
                              vc->turn_key_pem) != 0) {
            if (rs->turn_tls_required) {
                vine_error("TURNS TLS context init failed and "
                           "turn_transport=tls — refusing to start");
                rs->turn_setup_result = -1;
                uv_close((uv_handle_t *)async, NULL);
                return;
            }
            vine_error("TURNS TLS context init failed, falling back to plain");
        } else {
            mbedtls_ssl_cookie_init(&rs->dtls_cookie_ctx);
            int ret = mbedtls_ssl_cookie_setup(&rs->dtls_cookie_ctx,
                                               mbedtls_ctr_drbg_random,
                                               &rs->turn_tls.drbg);
            if (ret != 0) {
                vine_error("DTLS cookie setup failed: -0x%04x", (unsigned)-ret);
                mbedtls_ssl_cookie_free(&rs->dtls_cookie_ctx);
                turn_tls_ctx_free(&rs->turn_tls);
                if (rs->turn_tls_required) {
                    vine_error("turn_transport=tls but DTLS init failed "
                               "— refusing to start");
                    rs->turn_setup_result = -1;
                    uv_close((uv_handle_t *)async, NULL);
                    return;
                }
                vine_warn("DTLS cookie setup failed, falling back to plain");
            }
        }
    } else if (rs->turn_tls_required) {
        vine_error("turn_transport=tls but no cert/key configured "
                   "— refusing to start");
        rs->turn_setup_result = -1;
        uv_close((uv_handle_t *)async, NULL);
        return;
    }

    // TCP listener.
    uv_tcp_init(loop, &rs->turn_tcp);
    rs->turn_tcp.data = rs;

    struct sockaddr_storage bind_addr;
    memset(&bind_addr, 0, sizeof(bind_addr));
    int rc;
    if (strchr(host, ':')) {
        rc = uv_ip6_addr(host, port, (struct sockaddr_in6 *)&bind_addr);
    } else {
        rc = uv_ip4_addr(host, port, (struct sockaddr_in *)&bind_addr);
    }
    if (rc != 0) {
        vine_error("TURN invalid address %s:%u: %s",
                   host, port, uv_strerror(rc));
        uv_close((uv_handle_t *)&rs->turn_tcp, NULL);
        rs->turn_setup_result = -1;
        uv_close((uv_handle_t *)async, NULL);
        return;
    }

    rc = uv_tcp_bind(&rs->turn_tcp, (const struct sockaddr *)&bind_addr, 0);
    if (rc != 0) {
        vine_error("TURN bind failed on %s:%u: %s",
                   host, port, uv_strerror(rc));
        uv_close((uv_handle_t *)&rs->turn_tcp, NULL);
        rs->turn_setup_result = -1;
        uv_close((uv_handle_t *)async, NULL);
        return;
    }

    rc = uv_listen((uv_stream_t *)&rs->turn_tcp, 128, turn_on_connection);
    if (rc != 0) {
        vine_error("TURN listen failed on %s:%u: %s",
                   host, port, uv_strerror(rc));
        uv_close((uv_handle_t *)&rs->turn_tcp, NULL);
        rs->turn_setup_result = -1;
        uv_close((uv_handle_t *)async, NULL);
        return;
    }

    rs->turn_active = true;

    // UDP listener on the same port.
    uv_udp_init(loop, &rs->turn_udp);
    rs->turn_udp.data = rs;

    rc = uv_udp_bind(&rs->turn_udp, (const struct sockaddr *)&bind_addr, UV_UDP_REUSEADDR);
    if (rc != 0) {
        vine_warn("TURN UDP bind failed on %s:%u: %s (TCP-only mode)",
                  host, port, uv_strerror(rc));
    } else {
        rc = uv_udp_recv_start(&rs->turn_udp, turn_udp_alloc_buf, turn_udp_on_recv);
        if (rc != 0) {
            vine_warn("TURN UDP recv_start failed: %s (TCP-only mode)",
                      uv_strerror(rc));
            uv_close((uv_handle_t *)&rs->turn_udp, NULL);
        } else {
            rs->turn_udp_active = true;
        }
    }

    // Periodic tick timer for allocation + DTLS peer expiry.
    uv_timer_init(loop, &rs->turn_tick_timer);
    rs->turn_tick_timer.data = rs;
    uv_timer_start(&rs->turn_tick_timer, turn_tick_cb, 5000, 5000);
    rs->turn_tick_active = true;

    // Build transport description for log.
    const char *tcp_mode = rs->turn_tls.initialized ? "TURNS-TCP" : "TCP";
    const char *udp_mode = rs->turn_udp_active
        ? (rs->turn_tls.initialized ? "+DTLS-UDP" : "+UDP")
        : "";
    vine_info("TURN server listening on %s:%u (realm: %s, transport: %s%s)",
              host, port, vc->turn_realm, tcp_mode, udp_mode);

    if (vc->turn_front_host)
        vine_info("TURN domain fronting: %s", vc->turn_front_host);
    if (vc->turn_ech_public_name)
        vine_info("TURN ECH public name: %s", vc->turn_ech_public_name);
    if (vc->turn_pt_binary)
        vine_info("TURN pluggable transport: %s (%s)",
                  vc->turn_pt_transport ? vc->turn_pt_transport : "obfs4",
                  vc->turn_pt_binary);

    rs->turn_setup_result = 1;
    uv_close((uv_handle_t *)async, NULL);
}

static void turn_log_bridge(int level, const char *msg, void *ctx)
{
    (void)ctx;
    switch (level) {
    case 0: vine_debug("TURN: %s", msg); break;
    case 1: vine_info("TURN: %s", msg);  break;
    case 2: vine_warn("TURN: %s", msg);  break;
    default: vine_error("TURN: %s", msg); break;
    }
}

static bool parse_ipv4(const char *s, uint8_t out[4])
{
    unsigned a, b, c, d;
    if (sscanf(s, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
        return false;
    if (a > 255 || b > 255 || c > 255 || d > 255)
        return false;
    out[0] = (uint8_t)a;
    out[1] = (uint8_t)b;
    out[2] = (uint8_t)c;
    out[3] = (uint8_t)d;
    return true;
}

static bool is_unspecified_ip(const uint8_t ip[4])
{
    return ip[0] == 0 && ip[1] == 0 && ip[2] == 0 && ip[3] == 0;
}

static int start_turn_server(relay_state_t *rs)
{
    const vine_relay_config_t *vc = &rs->config;

    if (!vc->turn_enabled)
        return 0;

    if (!vc->turn_secret || !vc->turn_realm) {
        vine_error("TURN requires --turn-secret and --turn-realm");
        return -1;
    }

    rs->turn_srv = calloc(1, sizeof(turn_server_t));
    if (!rs->turn_srv) return -1;

    // Determine TURN listen port from config.
    const char *listen_str = vc->turn_listen_addr ? vc->turn_listen_addr : "0.0.0.0:443";
    uint16_t turn_port = 443;
    const char *port_sep = strrchr(listen_str, ':');
    if (port_sep)
        turn_port = (uint16_t)atoi(port_sep + 1);

    // Determine external IP for relay addresses.
    stun_addr_t external = {0};
    external.family = 0x01;
    external.port = turn_port;

    if (vc->turn_external_ip) {
        if (!parse_ipv4(vc->turn_external_ip, external.ip.v4)) {
            vine_error("invalid --turn-external-ip: %s", vc->turn_external_ip);
            free(rs->turn_srv);
            rs->turn_srv = NULL;
            return -1;
        }
        vine_info("TURN external IP (configured): %s", vc->turn_external_ip);
    } else {
        const char *addr = trellis_client_listen_addr(rs->client);
        if (addr) {
            char host[256];
            strncpy(host, addr, sizeof(host) - 1);
            host[sizeof(host) - 1] = '\0';
            char *colon = strrchr(host, ':');
            if (colon) *colon = '\0';
            parse_ipv4(host, external.ip.v4);
        }
        if (is_unspecified_ip(external.ip.v4))
            vine_warn("TURN external IP is 0.0.0.0 — relay candidates will be "
                      "unusable. Set --turn-external-ip to your public IP.");
    }

    trellis_err_t err = turn_server_init(rs->turn_srv,
                                         vc->turn_realm,
                                         vc->turn_secret,
                                         &external);
    if (err != TRELLIS_OK) {
        vine_error("TURN server init failed: %s", trellis_err_str(err));
        free(rs->turn_srv);
        rs->turn_srv = NULL;
        return -1;
    }

    if (vc->turn_max_allocations > 0) {
        uint32_t cap = vc->turn_max_allocations;
        if (cap > TURN_MAX_ALLOCATIONS) {
            vine_warn("turn_max_allocations %u exceeds compiled limit %d, clamping",
                      cap, TURN_MAX_ALLOCATIONS);
            cap = TURN_MAX_ALLOCATIONS;
        }
        rs->turn_srv->max_allocations = cap;
    }
    if (vc->turn_max_bps > 0)
        rs->turn_srv->max_bps = vc->turn_max_bps;

    rs->turn_srv->send_cb = turn_send_to_client;
    rs->turn_srv->send_ctx = rs;
    rs->turn_srv->relay_cb = turn_relay_to_mesh;
    rs->turn_srv->relay_ctx = rs;
    rs->turn_srv->log_cb = turn_log_bridge;
    rs->turn_srv->log_ctx = NULL;

    uv_loop_t *loop = trellis_client_get_loop(rs->client);
    if (!loop) {
        vine_error("no event loop for TURN listener");
        turn_server_destroy(rs->turn_srv);
        free(rs->turn_srv);
        rs->turn_srv = NULL;
        return -1;
    }

    /* Schedule handle creation on the event loop thread.
     * libuv handles must be created from the thread that runs their loop. */
    rs->turn_setup_result = 0;
    uv_async_init(loop, &rs->turn_setup_async, turn_setup_on_loop);
    rs->turn_setup_async.data = rs;
    uv_async_send(&rs->turn_setup_async);

    // Busy-wait for the loop thread to complete setup (typically <1ms).
    for (int i = 0; i < 5000 && rs->turn_setup_result == 0; i++) {
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000 }; /* 1ms */
        nanosleep(&ts, NULL);
    }

    if (rs->turn_setup_result != 1) {
        vine_error("TURN setup timed out or failed");
        turn_server_destroy(rs->turn_srv);
        free(rs->turn_srv);
        rs->turn_srv = NULL;
        return -1;
    }

    return 0;
}

static void turn_tcp_close_cb(uv_handle_t *handle)
{
    relay_state_t *rs = (relay_state_t *)handle->data;
    if (rs && rs->turn_srv) {
        turn_server_destroy(rs->turn_srv);
        free(rs->turn_srv);
        rs->turn_srv = NULL;
    }
}

static void turn_tick_timer_close_cb(uv_handle_t *handle) { (void)handle; }

static void turn_shutdown_async_close_cb(uv_handle_t *handle) { (void)handle; }

static void turn_shutdown_on_client_loop(uv_async_t *async)
{
    relay_state_t *rs = (relay_state_t *)async->data;

    // Close all active client connections.
    for (size_t i = 0; i < g_turn_client_count; i++) {
        turn_client_conn_t *cc = g_turn_clients[i];
        if (cc && cc->active && !uv_is_closing((uv_handle_t *)&cc->handle)) {
            cc->active = false;
            if (cc->tls_enabled && cc->tls_established) {
                mbedtls_ssl_close_notify(&cc->ssl);
                turn_tls_flush_bio_out(cc);
            }
            if (!uv_is_closing((uv_handle_t *)&cc->handle)) {
                if (cc->tls_enabled)
                    uv_close((uv_handle_t *)&cc->handle, turn_tls_close_cb);
                else
                    uv_close((uv_handle_t *)&cc->handle, (uv_close_cb)free);
            }
        }
        g_turn_clients[i] = NULL;
    }
    g_turn_client_count = 0;

    // Destroy all DTLS peers.
    for (int i = 0; i < TURN_DTLS_MAX_PEERS; i++) {
        if (rs->dtls_peers[i].active)
            turn_dtls_destroy_peer(&rs->dtls_peers[i]);
    }

    // Free TLS/DTLS context.
    if (rs->turn_tls.initialized) {
        mbedtls_ssl_cookie_free(&rs->dtls_cookie_ctx);
        turn_tls_ctx_free(&rs->turn_tls);
    }

    if (rs->turn_tick_active) {
        uv_timer_stop(&rs->turn_tick_timer);
        uv_close((uv_handle_t *)&rs->turn_tick_timer, turn_tick_timer_close_cb);
        rs->turn_tick_active = false;
    }
    if (rs->turn_udp_active) {
        uv_udp_recv_stop(&rs->turn_udp);
        uv_close((uv_handle_t *)&rs->turn_udp, NULL);
        rs->turn_udp_active = false;
    }
    if (rs->turn_active) {
        rs->turn_tcp.data = rs;
        uv_close((uv_handle_t *)&rs->turn_tcp, turn_tcp_close_cb);
        rs->turn_active = false;
    } else if (rs->turn_srv) {
        turn_server_destroy(rs->turn_srv);
        free(rs->turn_srv);
        rs->turn_srv = NULL;
    }

    uv_close((uv_handle_t *)async, turn_shutdown_async_close_cb);
    rs->turn_shutdown_done = true;
}

static void stop_turn_server(relay_state_t *rs)
{
    if (!rs->turn_srv || rs->turn_shutdown_done) return;

    uv_loop_t *client_loop = rs->client ? trellis_client_get_loop(rs->client) : NULL;
    if (client_loop) {
        rs->turn_shutdown_done = false;
        rs->turn_shutdown_async.data = rs;
        uv_async_init(client_loop, &rs->turn_shutdown_async, turn_shutdown_on_client_loop);
        uv_async_send(&rs->turn_shutdown_async);

        /* Wait for the async callback to complete on the client loop.
         * This ensures TURN handles are closed before trellis_client_stop
         * tears down the loop. Bounded to 5s to avoid hanging forever. */
        for (int i = 0; i < 500 && !rs->turn_shutdown_done; i++) {
            struct timespec ts = {0, 10000000}; /* 10ms */
            nanosleep(&ts, NULL);
        }
        if (!rs->turn_shutdown_done)
            vine_error("TURN shutdown timed out after 5s");
    } else {
        turn_server_destroy(rs->turn_srv);
        free(rs->turn_srv);
        rs->turn_srv = NULL;
        rs->turn_shutdown_done = true;
    }
}

int main(int argc, char **argv)
{
    memset(&g_state, 0, sizeof(g_state));
    g_state.config = vine_config_defaults();

    // Parse CLI first to get --config path.
    int rc = vine_config_parse_cli(&g_state.config, argc, argv);
    if (rc == 3) {
        fprintf(stdout, "vine-relay (trellis %s)\n", trellis_version());
        return 0;
    }
    if (rc == 1) return 0;
    if (rc < 0)  return 1;

    // Load config file (CLI values take precedence — already set)
    if (g_state.config.config_path) {
        if (vine_config_load_file(&g_state.config, g_state.config.config_path) < 0)
            return 1;
    }

    // Dump resolved config and exit if requested.
    if (rc == 2) {
        vine_config_dump(&g_state.config, stdout);
        return 0;
    }

    // Initialize logging early.
    vine_log_init(g_state.config.log_level, g_state.config.log_file);

    // Daemonize before anything else if requested.
    if (g_state.config.daemonize) {
        if (!g_state.config.log_file) {
            fprintf(stderr,
                "error: --daemon requires --log-file (stderr is closed after fork)\n");
            return 1;
        }
        if (daemonize_process() < 0) {
            vine_error("daemonization failed");
            return 1;
        }
    }

    // Write PID file.
    if (write_pidfile(g_state.config.pidfile) < 0)
        return 1;

    // Initialize crypto.
    trellis_err_t err = trellis_crypto_init();
    if (err != TRELLIS_OK) {
        vine_error("crypto init failed: %s", trellis_err_str(err));
        remove_pidfile(g_state.config.pidfile);
        return 1;
    }

    // Validate PT config before building the client.
    if (g_state.config.transport_mode == 2 && !g_state.config.pt_binary) {
        vine_error("--transport pt requires --pt-binary");
        trellis_crypto_cleanup();
        remove_pidfile(g_state.config.pidfile);
        return 1;
    }

    // Build trellis client config from vine config.
    trellis_client_config_t tcfg = build_trellis_config(&g_state.config);

    // Create client.
    g_state.client = trellis_client_new(&tcfg);
    if (!g_state.client) {
        vine_error("failed to create client");
        trellis_crypto_cleanup();
        remove_pidfile(g_state.config.pidfile);
        return 1;
    }

    // Register event handlers.
    trellis_client_on(g_state.client, TRELLIS_EV_STARTED,
                      on_started, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_STOPPED,
                      on_stopped, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_PEER_ADDED,
                      on_peer_added, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_PEER_REMOVED,
                      on_peer_removed, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_HANDSHAKE_COMPLETE,
                      on_handshake_complete, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_ECLIPSE_DETECTED,
                      on_eclipse_detected, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_GOSSIP,
                      on_gossip, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_MORPH_CHANGED,
                      on_morph_changed, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_ROUTING_CHANGED,
                      on_routing_changed, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_GREENHOUSE_STARTED,
                      on_greenhouse_started, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_GREENHOUSE_CONNECTION,
                      on_greenhouse_conn, &g_state);
    trellis_client_on(g_state.client, TRELLIS_EV_MESSAGE,
                      on_message, &g_state);

    // Start the client (spawns the event loop thread)
    err = trellis_client_start(g_state.client);
    if (err != TRELLIS_OK) {
        vine_error("failed to start relay: %s", trellis_err_str(err));
        trellis_client_free(g_state.client);
        trellis_crypto_cleanup();
        remove_pidfile(g_state.config.pidfile);
        return 1;
    }

    // Disable Sybil defenses for local/test environments.
    if (g_state.config.no_eclipse || g_state.config.no_sybil) {
        trellis_dht_t *dht = trellis_client_get_dht(g_state.client);
        if (dht) {
            if (g_state.config.no_eclipse || g_state.config.no_sybil) {
                trellis_dht_set_eclipse_enabled(dht, false);
                vine_info("eclipse attack detection disabled (local testing)");
            }
            if (g_state.config.no_sybil) {
                trellis_dht_disable_sybil(dht);
                vine_info("all Sybil defenses disabled (local testing)");
            }
        }
    }

    // Print identity.
    const trellis_identity_t *id = trellis_client_identity(g_state.client);
    char fp_hex[65];
    for (int i = 0; i < 32; i++)
        snprintf(fp_hex + i * 2, 3, "%02x", id->fingerprint.bytes[i]);

    char mlkem_hex[TRELLIS_ML_KEM_1024_PK_LEN * 2 + 1];
    for (int i = 0; i < TRELLIS_ML_KEM_1024_PK_LEN; i++)
        snprintf(mlkem_hex + i * 2, 3, "%02x", id->ml_kem_pk[i]);

    const char *addr = trellis_client_listen_addr(g_state.client);
    vine_info("vine-relay started (trellis %s)", trellis_version());
    vine_info("listening on %s", addr ? addr : g_state.config.listen_addr);
    vine_info("fingerprint: %s", fp_hex);
    vine_info("mlkem-pk: %s", mlkem_hex);
    vine_info("BLOOM_RELAY_SEEDS entry: %s@%s|%s", fp_hex,
              addr ? addr : g_state.config.listen_addr, mlkem_hex);

    // Log active features and transport configuration.
    {
        static const char *transport_names[] = {"raw/tcp", "tls", "pluggable-transport"};
        static const char *camo_names[] = {
            "none", "tls1.3", "http/2", "dns", "quic", "raw-obfs"};
        static const char *routing_names[] = {
            "direct", "relay", "multipath", "onion", "auto", "adaptive"};
        int tm = g_state.config.transport_mode;
        int ct = g_state.config.camouflage_type;
        int rm = g_state.config.routing_mode;
        vine_info("transport: %s, camouflage: %s",
                  (tm >= 0 && tm <= 2) ? transport_names[tm] : "unknown",
                  (ct >= 0 && ct <= 5) ? camo_names[ct] : "unknown");
        if (rm >= 0 && rm <= 5)
            vine_info("routing mode: %s", routing_names[rm]);
        if (tm == 2 && g_state.config.pt_binary)
            vine_info("pluggable transport: %s (%s)",
                      g_state.config.pt_transport_name
                          ? g_state.config.pt_transport_name : "obfs4",
                      g_state.config.pt_binary);
    }
    if (g_state.config.pow_iterations > 0)
        vine_info("connection PoW: %u iterations", g_state.config.pow_iterations);
    if (g_state.config.morph_metamorphic)
        vine_info("metamorphic encoding enabled");
    if (g_state.config.morph_cell_mode)
        vine_info("cell-mode fragmentation enabled (%u bytes)",
                  g_state.config.morph_cell_size ? g_state.config.morph_cell_size : 512);
    if (g_state.config.guard_enabled)
        vine_info("entry guard pinning enabled");
    if (g_state.config.cover_traffic_ms > 0)
        vine_info("cover traffic interval: %u ms",
                  g_state.config.cover_traffic_ms);
    if (g_state.config.decoy_interval_ms > 0)
        vine_info("decoy circuit interval: %u ms",
                  g_state.config.decoy_interval_ms);

    // Setup jurisdiction exclusion.
    setup_jurisdiction_exclusion(g_state.client, &g_state.config);

    // Setup incentive / Lightning.
    setup_incentive(g_state.client, &g_state.config);

    // Start Prometheus metrics.
    if (g_state.config.metrics_port > 0) {
        uv_loop_t *client_loop = trellis_client_get_loop(g_state.client);
        if (client_loop) {
            g_state.metrics = trellis_metrics_new(client_loop);
            if (g_state.metrics) {
                err = trellis_metrics_start_http(g_state.metrics,
                                                 g_state.config.metrics_port);
                if (err == TRELLIS_OK)
                    vine_info("prometheus metrics on 127.0.0.1:%d",
                              g_state.config.metrics_port);
                else
                    vine_warn("failed to start metrics endpoint: %s",
                              trellis_err_str(err));
            }
        }
    }

    // Start warrant canary publishing.
    if (g_state.config.canary_enabled) {
        uv_loop_t *client_loop = trellis_client_get_loop(g_state.client);
        trellis_dht_t *dht = trellis_client_get_dht(g_state.client);
        if (client_loop && dht && id) {
            g_state.canary_timer = trellis_canary_timer_start(
                client_loop, dht, id, g_state.config.canary_statement);
            if (g_state.canary_timer)
                vine_info("warrant canary publishing enabled (24h interval)");
            else
                vine_warn("failed to start canary timer");
        }
    }

    // Start greenhouse introduction point.
    if (g_state.config.intro_point) {
        err = trellis_client_start_greenhouse(g_state.client,
                                               TRELLIS_GH_PERSISTENT,
                                               on_gh_connect, &g_state);
        if (err == TRELLIS_OK)
            vine_info("greenhouse introduction point enabled");
        else
            vine_warn("failed to start greenhouse: %s", trellis_err_str(err));
    }

    // Enable exit relay.
    if (g_state.config.exit_relay) {
        err = trellis_client_enable_exit_relay(g_state.client);
        if (err == TRELLIS_OK) {
            vine_info("clearnet exit relay enabled");

            // Apply exit policies.
            if (g_state.config.exit_policy_count > 0) {
                trellis_exit_relay_t *er =
                    trellis_client_get_exit_relay(g_state.client);
                if (er) {
                    for (size_t i = 0; i < g_state.config.exit_policy_count; i++) {
                        trellis_err_t perr = trellis_exit_relay_add_policy(
                            er, g_state.config.exit_policies[i]);
                        if (perr == TRELLIS_OK)
                            vine_info("exit policy: %s",
                                      g_state.config.exit_policies[i]);
                        else
                            vine_warn("invalid exit policy '%s': %s",
                                      g_state.config.exit_policies[i],
                                      trellis_err_str(perr));
                    }
                }
            }

            // Start local SOCKS5 proxy for applications.
            if (g_state.config.exit_socks5_port > 0) {
                trellis_exit_relay_t *er =
                    trellis_client_get_exit_relay(g_state.client);
                if (er) {
                    trellis_err_t serr = trellis_exit_relay_start_socks5(
                        er, g_state.config.exit_socks5_port);
                    if (serr == TRELLIS_OK)
                        vine_info("SOCKS5 proxy on 127.0.0.1:%d",
                                  g_state.config.exit_socks5_port);
                    else
                        vine_warn("SOCKS5 start failed: %s",
                                  trellis_err_str(serr));
                }
            }
        } else {
            vine_warn("failed to enable exit relay: %s", trellis_err_str(err));
        }
    }

    // Start TURN server if enabled.
    if (g_state.config.turn_enabled) {
        if (start_turn_server(&g_state) < 0)
            vine_warn("TURN server failed to start (continuing without TURN)");
    }

    // Connect to seed nodes.
    for (size_t i = 0; i < g_state.config.seed_count; i++) {
        vine_info("connecting to seed: %s", g_state.config.seed_addrs[i]);
        trellis_client_connect_addr(g_state.client, g_state.config.seed_addrs[i]);
    }

    /*
     * Main thread event loop — handles signals only.
     * The client runs its own libuv loop on a background thread.
     */
    uv_loop_init(&g_state.main_loop);

    uv_signal_init(&g_state.main_loop, &g_state.sigint);
    g_state.sigint.data = &g_state;
    uv_signal_start(&g_state.sigint, on_signal, SIGINT);

    uv_signal_init(&g_state.main_loop, &g_state.sigterm);
    g_state.sigterm.data = &g_state;
    uv_signal_start(&g_state.sigterm, on_signal, SIGTERM);

    uv_signal_init(&g_state.main_loop, &g_state.sighup);
    g_state.sighup.data = &g_state;
    uv_signal_start(&g_state.sighup, on_sighup, SIGHUP);

    /*
     * Periodic metrics sync: update gauge values from client state.
     * Runs on the main loop at 5-second intervals.
     */
    if (g_state.metrics) {
        uv_timer_init(&g_state.main_loop, &g_state.metrics_sync_timer);
        g_state.metrics_sync_timer.data = &g_state;
        uv_timer_start(&g_state.metrics_sync_timer, on_metrics_tick, 5000, 5000);
        g_state.metrics_sync_active = true;
    }

    vine_info("relay running, waiting for connections...");

    uv_run(&g_state.main_loop, UV_RUN_DEFAULT);

    // --- Graceful shutdown ---
    vine_info("shutting down...");

    // Stop TURN server.
    stop_turn_server(&g_state);
    vine_debug("TURN server stopped");

    // Stop canary publishing.
    if (g_state.canary_timer) {
        trellis_canary_timer_stop(g_state.canary_timer);
        g_state.canary_timer = NULL;
        vine_debug("canary timer stopped");
    }

    // Stop metrics.
    if (g_state.metrics) {
        trellis_metrics_stop_http(g_state.metrics);
        trellis_metrics_free(g_state.metrics);
        g_state.metrics = NULL;
        vine_debug("metrics stopped");
    }

    // Stop client (closes connections, drains circuits)
    trellis_client_stop(g_state.client);
    trellis_client_free(g_state.client);
    g_state.client = NULL;
    vine_debug("client stopped");

    trellis_crypto_cleanup();

    // Close main loop handles before uv_loop_close (signals + timer)
    uv_close((uv_handle_t *)&g_state.sigint, NULL);
    uv_close((uv_handle_t *)&g_state.sigterm, NULL);
    uv_close((uv_handle_t *)&g_state.sighup, NULL);
    if (g_state.metrics_sync_active) {
        uv_timer_stop(&g_state.metrics_sync_timer);
        uv_close((uv_handle_t *)&g_state.metrics_sync_timer, NULL);
        g_state.metrics_sync_active = false;
    }
    uv_run(&g_state.main_loop, UV_RUN_DEFAULT);
    uv_loop_close(&g_state.main_loop);

    // Remove PID file.
    remove_pidfile(g_state.config.pidfile);

    vine_info("vine-relay stopped");
    vine_log_close();

    return 0;
}
