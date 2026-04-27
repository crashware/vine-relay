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

#include "config.h"

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vine_relay_config_t vine_config_defaults(void)
{
    vine_relay_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.listen_addr       = "0.0.0.0:9100";
    cfg.transport_mode    = 0; /* TRELLIS_TRANSPORT_RAW */
    cfg.camouflage_type   = 5; /* TRELLIS_CAMO_RAW_OBFS */
    cfg.routing_mode      = -1; /* use trellis default */
    cfg.always_on         = true;
    cfg.metrics_port      = 9101;
    cfg.canary_enabled    = true;
    cfg.log_level         = VINE_LOG_INFO;
    cfg.no_discover       = true;

    return cfg;
}

#define CLI_LISTEN          (UINT64_C(1) <<  0)
#define CLI_IDENTITY        (UINT64_C(1) <<  1)
#define CLI_SEED            (UINT64_C(1) <<  2)
#define CLI_TRANSPORT       (UINT64_C(1) <<  3)
#define CLI_CAMOUFLAGE      (UINT64_C(1) <<  4)
#define CLI_MORPH           (UINT64_C(1) <<  5)
#define CLI_COVER_TRAFFIC   (UINT64_C(1) <<  6)
#define CLI_POW             (UINT64_C(1) <<  7)
#define CLI_METRICS_PORT    (UINT64_C(1) <<  8)
#define CLI_CANARY          (UINT64_C(1) <<  9)
#define CLI_CANARY_STMT     (UINT64_C(1) << 10)
#define CLI_INTRO_POINT     (UINT64_C(1) << 11)
#define CLI_EXIT_RELAY      (UINT64_C(1) << 12)
#define CLI_EXCLUDE_JUR     (UINT64_C(1) << 13)
#define CLI_LOG_LEVEL       (UINT64_C(1) << 14)
#define CLI_LOG_FILE        (UINT64_C(1) << 15)
#define CLI_PIDFILE         (UINT64_C(1) << 16)
#define CLI_DAEMON          (UINT64_C(1) << 17)
#define CLI_ALWAYS_ON       (UINT64_C(1) << 18)
#define CLI_DECOY_INTERVAL  (UINT64_C(1) << 19)
#define CLI_BTCPAY_URL      (UINT64_C(1) << 20)
#define CLI_BTCPAY_KEY      (UINT64_C(1) << 21)
#define CLI_DISCOVER        (UINT64_C(1) << 22)
#define CLI_GUARD           (UINT64_C(1) << 23)
#define CLI_GUARD_PATH      (UINT64_C(1) << 24)
#define CLI_CELL_MODE       (UINT64_C(1) << 25)
#define CLI_CELL_SIZE       (UINT64_C(1) << 26)
#define CLI_PT_BINARY       (UINT64_C(1) << 27)
#define CLI_PT_TRANSPORT    (UINT64_C(1) << 28)
#define CLI_PT_ARGS         (UINT64_C(1) << 29)
#define CLI_ROUTING         (UINT64_C(1) << 30)
#define CLI_VERIFY_PEER     (UINT64_C(1) << 31)
#define CLI_EXIT_POLICY     (UINT64_C(1) << 32)
#define CLI_TURN            (UINT64_C(1) << 33)
#define CLI_TURN_LISTEN     (UINT64_C(1) << 34)
#define CLI_TURN_SECRET     (UINT64_C(1) << 35)
#define CLI_TURN_REALM      (UINT64_C(1) << 36)
#define CLI_TURN_CERT       (UINT64_C(1) << 37)
#define CLI_TURN_KEY        (UINT64_C(1) << 38)
#define CLI_TURN_MAX_ALLOC  (UINT64_C(1) << 39)
#define CLI_TURN_MAX_BPS    (UINT64_C(1) << 40)
#define CLI_TURN_TRANSPORT  (UINT64_C(1) << 41)
#define CLI_TURN_PRIVACY    (UINT64_C(1) << 42)
#define CLI_TURN_FRONT      (UINT64_C(1) << 43)
#define CLI_TURN_RELAY_TGT  (UINT64_C(1) << 44)
#define CLI_TURN_ECH        (UINT64_C(1) << 45)
#define CLI_TURN_PT         (UINT64_C(1) << 46)
#define CLI_TURN_EXT_IP     (UINT64_C(1) << 47)
#define CLI_LNCLI_PATH      (UINT64_C(1) << 48)
#define CLI_SATS_PER_MB     (UINT64_C(1) << 49)
#define CLI_EXIT_SOCKS5     (UINT64_C(1) << 50)

// --- CLI parsing ---
enum {
    OPT_NO_DISCOVER  = 256,
    OPT_CONFIG,
    OPT_SEED,
    OPT_TRANSPORT,
    OPT_CAMOUFLAGE,
    OPT_MORPH,
    OPT_NO_MORPH,
    OPT_COVER_TRAFFIC,
    OPT_POW_ITERS,
    OPT_METRICS_PORT,
    OPT_CANARY,
    OPT_NO_CANARY,
    OPT_CANARY_STMT,
    OPT_INTRO_POINT,
    OPT_EXIT_RELAY,
    OPT_EXCLUDE_JUR,
    OPT_LOG_LEVEL,
    OPT_LOG_FILE,
    OPT_PIDFILE,
    OPT_DAEMON,
    OPT_ALWAYS_ON,
    OPT_DECOY_INTERVAL,
    OPT_BTCPAY_URL,
    OPT_BTCPAY_KEY,
    OPT_DISCOVER,
    OPT_GUARD,
    OPT_NO_GUARD,
    OPT_GUARD_PATH,
    OPT_CELL_MODE,
    OPT_NO_CELL_MODE,
    OPT_CELL_SIZE,
    OPT_PT_BINARY,
    OPT_PT_TRANSPORT,
    OPT_PT_ARGS,
    OPT_ROUTING,
    OPT_VERIFY_PEER,
    OPT_NO_VERIFY_PEER,
    OPT_EXIT_POLICY,
    OPT_NO_INTRO_POINT,
    OPT_NO_EXIT_RELAY,
    OPT_NO_ALWAYS_ON,
    OPT_NO_ECLIPSE,
    OPT_NO_SYBIL,
    OPT_VERSION,
    OPT_DUMP_CONFIG,
    OPT_TURN,
    OPT_TURN_LISTEN,
    OPT_TURN_SECRET,
    OPT_TURN_REALM,
    OPT_TURN_CERT,
    OPT_TURN_KEY,
    OPT_TURN_MAX_ALLOC,
    OPT_TURN_MAX_BPS,
    OPT_TURN_TRANSPORT,
    OPT_TURN_PRIVACY,
    OPT_TURN_FRONT_HOST,
    OPT_TURN_RELAY_TARGET,
    OPT_TURN_ECH,
    OPT_TURN_PT_BINARY,
    OPT_TURN_PT_TRANSPORT,
    OPT_TURN_EXTERNAL_IP,
    OPT_LNCLI_PATH,
    OPT_SATS_PER_MB,
    OPT_EXIT_SOCKS5_PORT,
};

static struct option long_opts[] = {
    {"listen",               required_argument, NULL, 'l'},
    {"identity",             required_argument, NULL, 'i'},
    {"help",                 no_argument,       NULL, 'h'},
    {"config",               required_argument, NULL, OPT_CONFIG},
    {"seed",                 required_argument, NULL, OPT_SEED},
    {"transport",            required_argument, NULL, OPT_TRANSPORT},
    {"camouflage",           required_argument, NULL, OPT_CAMOUFLAGE},
    {"morph",                no_argument,       NULL, OPT_MORPH},
    {"no-morph",             no_argument,       NULL, OPT_NO_MORPH},
    {"cover-traffic",        required_argument, NULL, OPT_COVER_TRAFFIC},
    {"pow-iters",            required_argument, NULL, OPT_POW_ITERS},
    {"metrics-port",         required_argument, NULL, OPT_METRICS_PORT},
    {"canary",               no_argument,       NULL, OPT_CANARY},
    {"no-canary",            no_argument,       NULL, OPT_NO_CANARY},
    {"canary-statement",     required_argument, NULL, OPT_CANARY_STMT},
    {"intro-point",          no_argument,       NULL, OPT_INTRO_POINT},
    {"no-intro-point",       no_argument,       NULL, OPT_NO_INTRO_POINT},
    {"exit-relay",           no_argument,       NULL, OPT_EXIT_RELAY},
    {"no-exit-relay",        no_argument,       NULL, OPT_NO_EXIT_RELAY},
    {"exclude-jurisdiction", required_argument, NULL, OPT_EXCLUDE_JUR},
    {"log-level",            required_argument, NULL, OPT_LOG_LEVEL},
    {"log-file",             required_argument, NULL, OPT_LOG_FILE},
    {"pidfile",              required_argument, NULL, OPT_PIDFILE},
    {"daemon",               no_argument,       NULL, OPT_DAEMON},
    {"always-on",            no_argument,       NULL, OPT_ALWAYS_ON},
    {"no-always-on",         no_argument,       NULL, OPT_NO_ALWAYS_ON},
    {"decoy-interval",       required_argument, NULL, OPT_DECOY_INTERVAL},
    {"btcpay-url",           required_argument, NULL, OPT_BTCPAY_URL},
    {"btcpay-key",           required_argument, NULL, OPT_BTCPAY_KEY},
    {"no-discover",          no_argument,       NULL, OPT_NO_DISCOVER},
    {"discover",             no_argument,       NULL, OPT_DISCOVER},
    {"guard",                no_argument,       NULL, OPT_GUARD},
    {"no-guard",             no_argument,       NULL, OPT_NO_GUARD},
    {"guard-persist",        required_argument, NULL, OPT_GUARD_PATH},
    {"cell-mode",            no_argument,       NULL, OPT_CELL_MODE},
    {"no-cell-mode",         no_argument,       NULL, OPT_NO_CELL_MODE},
    {"cell-size",            required_argument, NULL, OPT_CELL_SIZE},
    {"pt-binary",            required_argument, NULL, OPT_PT_BINARY},
    {"pt-transport",         required_argument, NULL, OPT_PT_TRANSPORT},
    {"pt-args",              required_argument, NULL, OPT_PT_ARGS},
    {"routing",              required_argument, NULL, OPT_ROUTING},
    {"verify-peer",          no_argument,       NULL, OPT_VERIFY_PEER},
    {"no-verify-peer",       no_argument,       NULL, OPT_NO_VERIFY_PEER},
    {"exit-policy",          required_argument, NULL, OPT_EXIT_POLICY},
    {"no-eclipse",           no_argument,       NULL, OPT_NO_ECLIPSE},
    {"no-sybil",             no_argument,       NULL, OPT_NO_SYBIL},
    {"version",              no_argument,       NULL, OPT_VERSION},
    {"dump-config",          no_argument,       NULL, OPT_DUMP_CONFIG},
    {"turn",                 no_argument,       NULL, OPT_TURN},
    {"turn-listen",          required_argument, NULL, OPT_TURN_LISTEN},
    {"turn-secret",          required_argument, NULL, OPT_TURN_SECRET},
    {"turn-realm",           required_argument, NULL, OPT_TURN_REALM},
    {"turn-cert",            required_argument, NULL, OPT_TURN_CERT},
    {"turn-key",             required_argument, NULL, OPT_TURN_KEY},
    {"turn-max-alloc",       required_argument, NULL, OPT_TURN_MAX_ALLOC},
    {"turn-max-bps",         required_argument, NULL, OPT_TURN_MAX_BPS},
    {"turn-transport",       required_argument, NULL, OPT_TURN_TRANSPORT},
    {"turn-privacy",         required_argument, NULL, OPT_TURN_PRIVACY},
    {"turn-front-host",      required_argument, NULL, OPT_TURN_FRONT_HOST},
    {"turn-relay-target",    required_argument, NULL, OPT_TURN_RELAY_TARGET},
    {"turn-ech-public-name", required_argument, NULL, OPT_TURN_ECH},
    {"turn-pt-binary",       required_argument, NULL, OPT_TURN_PT_BINARY},
    {"turn-pt-transport",    required_argument, NULL, OPT_TURN_PT_TRANSPORT},
    {"turn-external-ip",     required_argument, NULL, OPT_TURN_EXTERNAL_IP},
    {"lncli-path",           required_argument, NULL, OPT_LNCLI_PATH},
    {"sats-per-mb",          required_argument, NULL, OPT_SATS_PER_MB},
    {"exit-socks5-port",     required_argument, NULL, OPT_EXIT_SOCKS5_PORT},
    {NULL, 0, NULL, 0}
};

static int parse_transport(const char *s)
{
    if (strcmp(s, "tcp") == 0 || strcmp(s, "raw") == 0) return 0;
    if (strcmp(s, "tls") == 0)  return 1;
    if (strcmp(s, "pt") == 0)   return 2;
    fprintf(stderr, "unknown transport: %s\n", s);
    return -1;
}

static int parse_camouflage(const char *s)
{
    if (strcmp(s, "none") == 0)     return 0;
    if (strcmp(s, "tls13") == 0)    return 1;
    if (strcmp(s, "http2") == 0)    return 2;
    if (strcmp(s, "dns") == 0)      return 3;
    if (strcmp(s, "quic") == 0)     return 4;
    if (strcmp(s, "raw-obfs") == 0) return 5;
    fprintf(stderr, "unknown camouflage: %s\n", s);
    return -1;
}

static int parse_routing(const char *s)
{
    if (strcmp(s, "direct") == 0)    return 0;
    if (strcmp(s, "relay") == 0)     return 1;
    if (strcmp(s, "multipath") == 0) return 2;
    if (strcmp(s, "onion") == 0)     return 3;
    if (strcmp(s, "auto") == 0)      return 4;
    if (strcmp(s, "adaptive") == 0)  return 5;
    fprintf(stderr, "unknown routing mode: %s\n", s);
    return -1;
}

static int parse_privacy(const char *s)
{
    if (strcmp(s, "speed") == 0)    return 0;
    if (strcmp(s, "balanced") == 0) return 1;
    if (strcmp(s, "max") == 0)      return 2;
    fprintf(stderr, "unknown privacy level: %s\n", s);
    return -1;
}

static vine_log_level_t parse_log_level(const char *s)
{
    if (strcmp(s, "debug") == 0) return VINE_LOG_DEBUG;
    if (strcmp(s, "info") == 0)  return VINE_LOG_INFO;
    if (strcmp(s, "warn") == 0)  return VINE_LOG_WARN;
    if (strcmp(s, "error") == 0) return VINE_LOG_ERROR;
    fprintf(stderr, "unknown log level: %s (using info)\n", s);
    return VINE_LOG_INFO;
}

static void parse_jurisdictions(vine_relay_config_t *cfg, const char *s)
{
    char buf[256];
    strncpy(buf, s, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *tok = strtok(buf, ",");
    while (tok && cfg->exclude_jur_count < VINE_MAX_JURISDICTIONS) {
        while (*tok == ' ') tok++;
        size_t len = strlen(tok);
        while (len > 0 && tok[len - 1] == ' ') len--;
        if (len >= 2 && len < VINE_JURISDICTION_LEN) {
            memcpy(cfg->exclude_jurs[cfg->exclude_jur_count], tok, len);
            cfg->exclude_jurs[cfg->exclude_jur_count][len] = '\0';
            cfg->exclude_jur_count++;
        }
        tok = strtok(NULL, ",");
    }
}

void vine_config_print_help(const char *prog)
{
    fprintf(stderr,
        "vine-relay — Bloom Protocol relay server\n\n"
        "Usage: %s [OPTIONS]\n\n"
        "Network:\n"
        "  -l, --listen <addr>              Listen address (default: 0.0.0.0:9100)\n"
        "  -i, --identity <path>            Identity key file\n"
        "      --seed <addr>                Bootstrap seed node (repeatable)\n"
        "      --no-discover                Disable LAN discovery (default)\n"
        "      --discover                   Enable LAN discovery\n"
        "\nTransport & Privacy:\n"
        "      --transport <tcp|tls|pt>     Transport mode (default: tcp)\n"
        "      --pt-binary <path>           Pluggable transport binary (required for --transport pt)\n"
        "      --pt-transport <name>        PT protocol: obfs4|snowflake|webtunnel (default: obfs4)\n"
        "      --pt-args <args>             Extra pluggable transport arguments\n"
        "      --camouflage <type>          Wire camouflage: none|tls13|http2|dns|quic|raw-obfs\n"
        "      --verify-peer                Enable TLS peer certificate verification\n"
        "      --no-verify-peer             Disable TLS peer verification (default)\n"
        "      --routing <mode>             Routing: direct|relay|multipath|onion|auto|adaptive\n"
        "      --morph                      Enable metamorphic encoding\n"
        "      --no-morph                   Disable metamorphic encoding (default)\n"
        "      --cover-traffic <ms>         Cover traffic interval in ms (0 = disabled)\n"
        "      --decoy-interval <ms>        Decoy circuit interval in ms (0 = disabled)\n"
        "      --always-on                  Keep running even when idle (default)\n"
        "      --no-always-on               Allow idle shutdown\n"
        "      --pow-iters <n>              Per-connection PoW iterations (0 = default)\n"
        "\nMonitoring:\n"
        "      --metrics-port <port>        Prometheus metrics port (default: 9101, 0 = off)\n"
        "      --canary                     Publish warrant canary (default)\n"
        "      --no-canary                  Disable warrant canary\n"
        "      --canary-statement <text>    Canary attestation text\n"
        "\nServices:\n"
        "      --intro-point                Serve as greenhouse introduction point\n"
        "      --no-intro-point             Disable introduction point\n"
        "      --exit-relay                 Enable clearnet exit relay\n"
        "      --no-exit-relay              Disable exit relay\n"
        "      --exit-socks5-port <port>    Local SOCKS5 proxy port for exit relay (0 = disabled)\n"
        "      --exit-policy <rule>         Exit policy rule (repeatable, e.g. allow:*:443)\n"
        "      --exclude-jurisdiction <CC>  Exclude jurisdictions (comma-separated ISO codes)\n"
        "\nIncentive:\n"
        "      --btcpay-url <url>           BTCPay Server URL for receipt redemption\n"
        "      --btcpay-key <key>           BTCPay API key\n"
        "      --lncli-path <path>          Path to lncli binary (default: lncli)\n"
        "      --sats-per-mb <n>            Payment rate in sats per MB relayed (default: 1)\n"
        "\nAnonymity Hardening:\n"
        "      --guard                      Pin entry guard nodes (Sybil resistant)\n"
        "      --no-guard                   Disable guard pinning (default)\n"
        "      --guard-persist <path>       Guard state file (default: auto)\n"
        "      --cell-mode                  Fixed-size cell fragmentation\n"
        "      --no-cell-mode               Disable cell mode (default)\n"
        "      --cell-size <bytes>          Cell size in bytes (default: 512)\n"
        "      --no-eclipse                 Disable eclipse attack detection (local testing)\n"
        "      --no-sybil                   Disable all Sybil defenses (local testing)\n"
        "\nTURN Relay:\n"
        "      --turn                       Enable TURN relay server\n"
        "      --turn-listen <addr>         TURN listen address (default: 0.0.0.0:443)\n"
        "      --turn-secret <secret>       HMAC shared secret for TURN auth\n"
        "      --turn-realm <realm>         TURN realm (e.g. neverroute.com)\n"
        "      --turn-cert <path>           TLS certificate for TURNS\n"
        "      --turn-key <path>            TLS private key for TURNS\n"
        "      --turn-max-alloc <n>         Max TURN allocations (default: 200)\n"
        "      --turn-max-bps <n>           Per-allocation bandwidth cap in bytes/s\n"
        "      --turn-transport <tls,quic>  TURN transport modes (default: tls)\n"
        "      --turn-privacy <mode>        speed|balanced|max (default: speed)\n"
        "      --turn-front-host <host>     Domain fronting CDN host\n"
        "      --turn-relay-target <addr>   Domain fronting relay target\n"
        "      --turn-ech-public-name <sni> ECH outer public name\n"
        "      --turn-external-ip <ip>      Public IP for relay address (auto-detected if omitted)\n"
        "      --turn-pt-binary <path>      Pluggable transport binary for TURN\n"
        "      --turn-pt-transport <name>   PT protocol for TURN clients\n"
        "\nLogging:\n"
        "      --log-level <level>          debug|info|warn|error (default: info)\n"
        "      --log-file <path>            Log to file (default: stderr)\n"
        "\nDaemon:\n"
        "      --config <path>              Config file (key=value format)\n"
        "      --pidfile <path>             Write PID file\n"
        "      --daemon                     Fork to background\n"
        "      --dump-config                Print resolved config and exit\n"
        "  -V, --version                    Show version and exit\n"
        "  -h, --help                       Show this help\n",
        prog);
}

int vine_config_parse_cli(vine_relay_config_t *cfg, int argc, char **argv)
{
    int opt;
    int val;

    optind = 1;
    while ((opt = getopt_long(argc, argv, "l:i:hV", long_opts, NULL)) != -1) {
        switch (opt) {
        case 'l': cfg->listen_addr = optarg;   cfg->_cli_set |= CLI_LISTEN; break;
        case 'i': cfg->identity_path = optarg;  cfg->_cli_set |= CLI_IDENTITY; break;
        case 'h': vine_config_print_help(argv[0]); return 1;
        case 'V':
        case OPT_VERSION: return 3;
        case OPT_DUMP_CONFIG: return 2;

        case OPT_CONFIG:         cfg->config_path = optarg; break;
        case OPT_SEED:
            if (cfg->seed_count < VINE_MAX_SEEDS) {
                cfg->seed_addrs[cfg->seed_count++] = optarg;
                cfg->_cli_set |= CLI_SEED;
            }
            break;
        case OPT_TRANSPORT:
            val = parse_transport(optarg);
            if (val < 0) return -1;
            cfg->transport_mode = val;
            cfg->_cli_set |= CLI_TRANSPORT;
            break;
        case OPT_CAMOUFLAGE:
            val = parse_camouflage(optarg);
            if (val < 0) return -1;
            cfg->camouflage_type = val;
            cfg->_cli_set |= CLI_CAMOUFLAGE;
            break;
        case OPT_MORPH:
            cfg->morph_metamorphic = true;   cfg->_cli_set |= CLI_MORPH; break;
        case OPT_NO_MORPH:
            cfg->morph_metamorphic = false;  cfg->_cli_set |= CLI_MORPH; break;
        case OPT_COVER_TRAFFIC:
            cfg->cover_traffic_ms = (uint32_t)atoi(optarg); cfg->_cli_set |= CLI_COVER_TRAFFIC; break;
        case OPT_POW_ITERS:
            cfg->pow_iterations = (uint32_t)atoi(optarg);   cfg->_cli_set |= CLI_POW; break;
        case OPT_METRICS_PORT:
            cfg->metrics_port = atoi(optarg);                cfg->_cli_set |= CLI_METRICS_PORT; break;
        case OPT_CANARY:
            cfg->canary_enabled = true;  cfg->_cli_set |= CLI_CANARY; break;
        case OPT_NO_CANARY:
            cfg->canary_enabled = false; cfg->_cli_set |= CLI_CANARY; break;
        case OPT_CANARY_STMT:
            cfg->canary_statement = optarg; cfg->_cli_set |= CLI_CANARY_STMT; break;
        case OPT_INTRO_POINT:
            cfg->intro_point = true;  cfg->_cli_set |= CLI_INTRO_POINT; break;
        case OPT_NO_INTRO_POINT:
            cfg->intro_point = false; cfg->_cli_set |= CLI_INTRO_POINT; break;
        case OPT_EXIT_RELAY:
            cfg->exit_relay = true;   cfg->_cli_set |= CLI_EXIT_RELAY; break;
        case OPT_NO_EXIT_RELAY:
            cfg->exit_relay = false;  cfg->_cli_set |= CLI_EXIT_RELAY; break;
        case OPT_EXCLUDE_JUR:
            parse_jurisdictions(cfg, optarg); cfg->_cli_set |= CLI_EXCLUDE_JUR; break;
        case OPT_LOG_LEVEL:
            cfg->log_level = parse_log_level(optarg); cfg->_cli_set |= CLI_LOG_LEVEL; break;
        case OPT_LOG_FILE:
            cfg->log_file = optarg;   cfg->_cli_set |= CLI_LOG_FILE; break;
        case OPT_PIDFILE:
            cfg->pidfile = optarg;    cfg->_cli_set |= CLI_PIDFILE; break;
        case OPT_DAEMON:
            cfg->daemonize = true;    cfg->_cli_set |= CLI_DAEMON; break;
        case OPT_ALWAYS_ON:
            cfg->always_on = true;    cfg->_cli_set |= CLI_ALWAYS_ON; break;
        case OPT_NO_ALWAYS_ON:
            cfg->always_on = false;   cfg->_cli_set |= CLI_ALWAYS_ON; break;
        case OPT_DECOY_INTERVAL:
            cfg->decoy_interval_ms = (uint32_t)atoi(optarg); cfg->_cli_set |= CLI_DECOY_INTERVAL; break;
        case OPT_BTCPAY_URL:
            cfg->btcpay_url = optarg; cfg->_cli_set |= CLI_BTCPAY_URL; break;
        case OPT_BTCPAY_KEY:
            cfg->btcpay_key = optarg; cfg->_cli_set |= CLI_BTCPAY_KEY; break;
        case OPT_LNCLI_PATH:
            cfg->lncli_path = optarg; cfg->_cli_set |= CLI_LNCLI_PATH; break;
        case OPT_SATS_PER_MB:
            cfg->sats_per_mb = (uint64_t)strtoull(optarg, NULL, 10); cfg->_cli_set |= CLI_SATS_PER_MB; break;
        case OPT_NO_DISCOVER:
            cfg->no_discover = true;  cfg->_cli_set |= CLI_DISCOVER; break;
        case OPT_DISCOVER:
            cfg->no_discover = false; cfg->_cli_set |= CLI_DISCOVER; break;
        case OPT_GUARD:
            cfg->guard_enabled = true;  cfg->_cli_set |= CLI_GUARD; break;
        case OPT_NO_GUARD:
            cfg->guard_enabled = false; cfg->_cli_set |= CLI_GUARD; break;
        case OPT_GUARD_PATH:
            cfg->guard_persist_path = optarg; cfg->_cli_set |= CLI_GUARD_PATH; break;
        case OPT_CELL_MODE:
            cfg->morph_cell_mode = true;  cfg->_cli_set |= CLI_CELL_MODE; break;
        case OPT_NO_CELL_MODE:
            cfg->morph_cell_mode = false; cfg->_cli_set |= CLI_CELL_MODE; break;
        case OPT_CELL_SIZE:
            cfg->morph_cell_size = (uint32_t)atoi(optarg); cfg->_cli_set |= CLI_CELL_SIZE; break;
        case OPT_PT_BINARY:
            cfg->pt_binary = optarg;         cfg->_cli_set |= CLI_PT_BINARY; break;
        case OPT_PT_TRANSPORT:
            cfg->pt_transport_name = optarg; cfg->_cli_set |= CLI_PT_TRANSPORT; break;
        case OPT_PT_ARGS:
            cfg->pt_args = optarg;           cfg->_cli_set |= CLI_PT_ARGS; break;
        case OPT_ROUTING:
            val = parse_routing(optarg);
            if (val < 0) return -1;
            cfg->routing_mode = val;
            cfg->_cli_set |= CLI_ROUTING;
            break;
        case OPT_VERIFY_PEER:
            cfg->verify_peer = true;  cfg->_cli_set |= CLI_VERIFY_PEER; break;
        case OPT_NO_VERIFY_PEER:
            cfg->verify_peer = false; cfg->_cli_set |= CLI_VERIFY_PEER; break;
        case OPT_EXIT_POLICY:
            if (cfg->exit_policy_count < VINE_MAX_EXIT_POLICIES) {
                cfg->exit_policies[cfg->exit_policy_count++] = optarg;
                cfg->_cli_set |= CLI_EXIT_POLICY;
            }
            break;
        case OPT_EXIT_SOCKS5_PORT:
            cfg->exit_socks5_port = atoi(optarg); cfg->_cli_set |= CLI_EXIT_SOCKS5; break;
        case OPT_NO_ECLIPSE:
            cfg->no_eclipse = true; break;
        case OPT_NO_SYBIL:
            cfg->no_sybil = true; break;
        case OPT_TURN:
            cfg->turn_enabled = true;       cfg->_cli_set |= CLI_TURN; break;
        case OPT_TURN_LISTEN:
            cfg->turn_listen_addr = optarg; cfg->_cli_set |= CLI_TURN_LISTEN; break;
        case OPT_TURN_SECRET:
            cfg->turn_secret = optarg;      cfg->_cli_set |= CLI_TURN_SECRET; break;
        case OPT_TURN_REALM:
            cfg->turn_realm = optarg;       cfg->_cli_set |= CLI_TURN_REALM; break;
        case OPT_TURN_CERT:
            cfg->turn_cert_pem = optarg;    cfg->_cli_set |= CLI_TURN_CERT; break;
        case OPT_TURN_KEY:
            cfg->turn_key_pem = optarg;     cfg->_cli_set |= CLI_TURN_KEY; break;
        case OPT_TURN_MAX_ALLOC:
            cfg->turn_max_allocations = (uint32_t)atoi(optarg); cfg->_cli_set |= CLI_TURN_MAX_ALLOC; break;
        case OPT_TURN_MAX_BPS:
            cfg->turn_max_bps = (uint32_t)atoi(optarg); cfg->_cli_set |= CLI_TURN_MAX_BPS; break;
        case OPT_TURN_TRANSPORT:
            cfg->turn_transport = optarg;   cfg->_cli_set |= CLI_TURN_TRANSPORT; break;
        case OPT_TURN_PRIVACY:
            val = parse_privacy(optarg);
            if (val < 0) return -1;
            cfg->turn_privacy = val;        cfg->_cli_set |= CLI_TURN_PRIVACY; break;
        case OPT_TURN_FRONT_HOST:
            cfg->turn_front_host = optarg;    cfg->_cli_set |= CLI_TURN_FRONT; break;
        case OPT_TURN_RELAY_TARGET:
            cfg->turn_relay_target = optarg;  cfg->_cli_set |= CLI_TURN_RELAY_TGT; break;
        case OPT_TURN_ECH:
            cfg->turn_ech_public_name = optarg; cfg->_cli_set |= CLI_TURN_ECH; break;
        case OPT_TURN_PT_BINARY:
            cfg->turn_pt_binary = optarg;   cfg->_cli_set |= CLI_TURN_PT; break;
        case OPT_TURN_PT_TRANSPORT:
            cfg->turn_pt_transport = optarg; cfg->_cli_set |= CLI_TURN_PT; break;
        case OPT_TURN_EXTERNAL_IP:
            cfg->turn_external_ip = optarg;  cfg->_cli_set |= CLI_TURN_EXT_IP; break;
        default:
            vine_config_print_help(argv[0]);
            return -1;
        }
    }
    return 0;
}

// --- Config file parser (simple key = value) ---
static char *strip(char *s)
{
    while (isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) end--;
    *end = '\0';
    return s;
}

static char *unquote(char *s)
{
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        s[len - 1] = '\0';
        return s + 1;
    }
    return s;
}

int vine_config_load_file(vine_relay_config_t *cfg, const char *path)
{
    if (!path) return 0;

    FILE *f = fopen(path, "r");
    if (!f) {
        if (errno == ENOENT)
            return 0;
        fprintf(stderr, "cannot open config %s: %s\n", path, strerror(errno));
        return -1;
    }

    char line[1024];
    int lineno = 0;

    while (fgets(line, sizeof(line), f)) {
        lineno++;
        char *s = strip(line);
        if (*s == '\0' || *s == '#' || *s == '[')
            continue;

        char *eq = strchr(s, '=');
        if (!eq) {
            fprintf(stderr, "%s:%d: missing '='\n", path, lineno);
            continue;
        }

        *eq = '\0';
        char *key = strip(s);
        char *val = strip(unquote(strip(eq + 1)));

        // Only apply file values for fields not already set by CLI.
#define FILE_SET(bit, stmt) do { if (!(cfg->_cli_set & (bit))) { stmt; } } while(0)

        if (strcmp(key, "listen") == 0)
            FILE_SET(CLI_LISTEN, cfg->listen_addr = strdup(val));
        else if (strcmp(key, "identity") == 0)
            FILE_SET(CLI_IDENTITY, cfg->identity_path = strdup(val));
        else if (strcmp(key, "seed") == 0) {
            if (!(cfg->_cli_set & CLI_SEED) && cfg->seed_count < VINE_MAX_SEEDS)
                cfg->seed_addrs[cfg->seed_count++] = strdup(val);
        }
        else if (strcmp(key, "transport") == 0)
            FILE_SET(CLI_TRANSPORT, cfg->transport_mode = parse_transport(val));
        else if (strcmp(key, "camouflage") == 0)
            FILE_SET(CLI_CAMOUFLAGE, cfg->camouflage_type = parse_camouflage(val));
        else if (strcmp(key, "morph") == 0)
            FILE_SET(CLI_MORPH, cfg->morph_metamorphic = (strcmp(val, "true") == 0));
        else if (strcmp(key, "cover_traffic") == 0)
            FILE_SET(CLI_COVER_TRAFFIC, cfg->cover_traffic_ms = (uint32_t)atoi(val));
        else if (strcmp(key, "decoy_interval") == 0)
            FILE_SET(CLI_DECOY_INTERVAL, cfg->decoy_interval_ms = (uint32_t)atoi(val));
        else if (strcmp(key, "always_on") == 0)
            FILE_SET(CLI_ALWAYS_ON, cfg->always_on = (strcmp(val, "true") == 0));
        else if (strcmp(key, "pow_iterations") == 0)
            FILE_SET(CLI_POW, cfg->pow_iterations = (uint32_t)atoi(val));
        else if (strcmp(key, "metrics_port") == 0)
            FILE_SET(CLI_METRICS_PORT, cfg->metrics_port = atoi(val));
        else if (strcmp(key, "canary") == 0)
            FILE_SET(CLI_CANARY, cfg->canary_enabled = (strcmp(val, "true") == 0));
        else if (strcmp(key, "canary_statement") == 0)
            FILE_SET(CLI_CANARY_STMT, cfg->canary_statement = strdup(val));
        else if (strcmp(key, "intro_point") == 0)
            FILE_SET(CLI_INTRO_POINT, cfg->intro_point = (strcmp(val, "true") == 0));
        else if (strcmp(key, "exit_relay") == 0)
            FILE_SET(CLI_EXIT_RELAY, cfg->exit_relay = (strcmp(val, "true") == 0));
        else if (strcmp(key, "exclude_jurisdiction") == 0)
            FILE_SET(CLI_EXCLUDE_JUR, parse_jurisdictions(cfg, val));
        else if (strcmp(key, "log_level") == 0)
            FILE_SET(CLI_LOG_LEVEL, cfg->log_level = parse_log_level(val));
        else if (strcmp(key, "log_file") == 0)
            FILE_SET(CLI_LOG_FILE, cfg->log_file = strdup(val));
        else if (strcmp(key, "pidfile") == 0)
            FILE_SET(CLI_PIDFILE, cfg->pidfile = strdup(val));
        else if (strcmp(key, "daemon") == 0)
            FILE_SET(CLI_DAEMON, cfg->daemonize = (strcmp(val, "true") == 0));
        else if (strcmp(key, "discover") == 0)
            FILE_SET(CLI_DISCOVER, cfg->no_discover = (strcmp(val, "true") != 0));
        else if (strcmp(key, "btcpay_url") == 0)
            FILE_SET(CLI_BTCPAY_URL, cfg->btcpay_url = strdup(val));
        else if (strcmp(key, "btcpay_key") == 0)
            FILE_SET(CLI_BTCPAY_KEY, cfg->btcpay_key = strdup(val));
        else if (strcmp(key, "lncli_path") == 0)
            FILE_SET(CLI_LNCLI_PATH, cfg->lncli_path = strdup(val));
        else if (strcmp(key, "sats_per_mb") == 0)
            FILE_SET(CLI_SATS_PER_MB, cfg->sats_per_mb = (uint64_t)strtoull(val, NULL, 10));
        else if (strcmp(key, "guard") == 0)
            FILE_SET(CLI_GUARD, cfg->guard_enabled = (strcmp(val, "true") == 0));
        else if (strcmp(key, "guard_persist") == 0)
            FILE_SET(CLI_GUARD_PATH, cfg->guard_persist_path = strdup(val));
        else if (strcmp(key, "cell_mode") == 0)
            FILE_SET(CLI_CELL_MODE, cfg->morph_cell_mode = (strcmp(val, "true") == 0));
        else if (strcmp(key, "cell_size") == 0)
            FILE_SET(CLI_CELL_SIZE, cfg->morph_cell_size = (uint32_t)atoi(val));
        else if (strcmp(key, "pt_binary") == 0)
            FILE_SET(CLI_PT_BINARY, cfg->pt_binary = strdup(val));
        else if (strcmp(key, "pt_transport") == 0)
            FILE_SET(CLI_PT_TRANSPORT, cfg->pt_transport_name = strdup(val));
        else if (strcmp(key, "pt_args") == 0)
            FILE_SET(CLI_PT_ARGS, cfg->pt_args = strdup(val));
        else if (strcmp(key, "routing") == 0)
            FILE_SET(CLI_ROUTING, cfg->routing_mode = parse_routing(val));
        else if (strcmp(key, "verify_peer") == 0)
            FILE_SET(CLI_VERIFY_PEER, cfg->verify_peer = (strcmp(val, "true") == 0));
        else if (strcmp(key, "exit_socks5_port") == 0)
            FILE_SET(CLI_EXIT_SOCKS5, cfg->exit_socks5_port = atoi(val));
        else if (strcmp(key, "exit_policy") == 0) {
            if (!(cfg->_cli_set & CLI_EXIT_POLICY) &&
                cfg->exit_policy_count < VINE_MAX_EXIT_POLICIES)
                cfg->exit_policies[cfg->exit_policy_count++] = strdup(val);
        }
        else if (strcmp(key, "no_eclipse") == 0)
            cfg->no_eclipse = (strcmp(val, "true") == 0);
        else if (strcmp(key, "no_sybil") == 0)
            cfg->no_sybil = (strcmp(val, "true") == 0);
        else if (strcmp(key, "turn") == 0)
            FILE_SET(CLI_TURN, cfg->turn_enabled = (strcmp(val, "true") == 0));
        else if (strcmp(key, "turn_listen") == 0)
            FILE_SET(CLI_TURN_LISTEN, cfg->turn_listen_addr = strdup(val));
        else if (strcmp(key, "turn_secret") == 0)
            FILE_SET(CLI_TURN_SECRET, cfg->turn_secret = strdup(val));
        else if (strcmp(key, "turn_realm") == 0)
            FILE_SET(CLI_TURN_REALM, cfg->turn_realm = strdup(val));
        else if (strcmp(key, "turn_cert") == 0)
            FILE_SET(CLI_TURN_CERT, cfg->turn_cert_pem = strdup(val));
        else if (strcmp(key, "turn_key") == 0)
            FILE_SET(CLI_TURN_KEY, cfg->turn_key_pem = strdup(val));
        else if (strcmp(key, "turn_max_alloc") == 0)
            FILE_SET(CLI_TURN_MAX_ALLOC, cfg->turn_max_allocations = (uint32_t)atoi(val));
        else if (strcmp(key, "turn_max_bps") == 0)
            FILE_SET(CLI_TURN_MAX_BPS, cfg->turn_max_bps = (uint32_t)atoi(val));
        else if (strcmp(key, "turn_transport") == 0)
            FILE_SET(CLI_TURN_TRANSPORT, cfg->turn_transport = strdup(val));
        else if (strcmp(key, "turn_privacy") == 0)
            FILE_SET(CLI_TURN_PRIVACY, cfg->turn_privacy = parse_privacy(val));
        else if (strcmp(key, "turn_front_host") == 0)
            FILE_SET(CLI_TURN_FRONT, cfg->turn_front_host = strdup(val));
        else if (strcmp(key, "turn_relay_target") == 0)
            FILE_SET(CLI_TURN_RELAY_TGT, cfg->turn_relay_target = strdup(val));
        else if (strcmp(key, "turn_ech") == 0)
            FILE_SET(CLI_TURN_ECH, cfg->turn_ech_public_name = strdup(val));
        else if (strcmp(key, "turn_pt_binary") == 0)
            FILE_SET(CLI_TURN_PT, cfg->turn_pt_binary = strdup(val));
        else if (strcmp(key, "turn_pt_transport") == 0)
            FILE_SET(CLI_TURN_PT, cfg->turn_pt_transport = strdup(val));
        else if (strcmp(key, "turn_external_ip") == 0)
            FILE_SET(CLI_TURN_EXT_IP, cfg->turn_external_ip = strdup(val));
        else fprintf(stderr, "%s:%d: unknown key '%s'\n", path, lineno, key);

#undef FILE_SET
    }

    fclose(f);
    return 0;
}

// --- Config dump ---
static const char *bool_str(bool v) { return v ? "true" : "false"; }

static const char *transport_str(int m)
{
    switch (m) {
    case 0: return "tcp";
    case 1: return "tls";
    case 2: return "pt";
    default: return "unknown";
    }
}

static const char *camouflage_str(int c)
{
    switch (c) {
    case 0: return "none";
    case 1: return "tls13";
    case 2: return "http2";
    case 3: return "dns";
    case 4: return "quic";
    case 5: return "raw-obfs";
    default: return "unknown";
    }
}

static const char *routing_str(int r)
{
    switch (r) {
    case 0: return "direct";
    case 1: return "relay";
    case 2: return "multipath";
    case 3: return "onion";
    case 4: return "auto";
    case 5: return "adaptive";
    default: return "default";
    }
}

static const char *privacy_str(int p)
{
    switch (p) {
    case 0: return "speed";
    case 1: return "balanced";
    case 2: return "max";
    default: return "speed";
    }
}

static const char *log_level_str(vine_log_level_t l)
{
    switch (l) {
    case VINE_LOG_DEBUG: return "debug";
    case VINE_LOG_INFO:  return "info";
    case VINE_LOG_WARN:  return "warn";
    case VINE_LOG_ERROR: return "error";
    default:             return "info";
    }
}

void vine_config_dump(const vine_relay_config_t *cfg, FILE *out)
{
    fprintf(out, "# vine-relay resolved configuration\n\n");
    fprintf(out, "[network]\n");
    fprintf(out, "listen          = %s\n", cfg->listen_addr);
    fprintf(out, "identity        = %s\n", cfg->identity_path ? cfg->identity_path : "(auto)");
    for (size_t i = 0; i < cfg->seed_count; i++)
        fprintf(out, "seed            = %s\n", cfg->seed_addrs[i]);
    fprintf(out, "discover        = %s\n", bool_str(!cfg->no_discover));

    fprintf(out, "\n[transport]\n");
    fprintf(out, "transport       = %s\n", transport_str(cfg->transport_mode));
    fprintf(out, "camouflage      = %s\n", camouflage_str(cfg->camouflage_type));
    fprintf(out, "verify_peer     = %s\n", bool_str(cfg->verify_peer));
    if (cfg->pt_binary)
        fprintf(out, "pt_binary       = %s\n", cfg->pt_binary);
    if (cfg->pt_transport_name)
        fprintf(out, "pt_transport    = %s\n", cfg->pt_transport_name);
    if (cfg->pt_args)
        fprintf(out, "pt_args         = %s\n", cfg->pt_args);

    fprintf(out, "\n[routing]\n");
    fprintf(out, "routing         = %s\n", routing_str(cfg->routing_mode));

    fprintf(out, "\n[privacy]\n");
    fprintf(out, "morph           = %s\n", bool_str(cfg->morph_metamorphic));
    fprintf(out, "cell_mode       = %s\n", bool_str(cfg->morph_cell_mode));
    if (cfg->morph_cell_mode)
        fprintf(out, "cell_size       = %u\n", cfg->morph_cell_size ? cfg->morph_cell_size : 512);
    fprintf(out, "guard           = %s\n", bool_str(cfg->guard_enabled));
    if (cfg->guard_persist_path)
        fprintf(out, "guard_persist   = %s\n", cfg->guard_persist_path);
    fprintf(out, "cover_traffic   = %u\n", cfg->cover_traffic_ms);
    fprintf(out, "decoy_interval  = %u\n", cfg->decoy_interval_ms);
    fprintf(out, "always_on       = %s\n", bool_str(cfg->always_on));
    fprintf(out, "pow_iterations  = %u\n", cfg->pow_iterations);
    fprintf(out, "no_eclipse      = %s\n", bool_str(cfg->no_eclipse));
    fprintf(out, "no_sybil        = %s\n", bool_str(cfg->no_sybil));

    fprintf(out, "\n[monitoring]\n");
    fprintf(out, "metrics_port    = %d\n", cfg->metrics_port);
    fprintf(out, "canary          = %s\n", bool_str(cfg->canary_enabled));
    if (cfg->canary_statement)
        fprintf(out, "canary_statement = \"%s\"\n", cfg->canary_statement);

    fprintf(out, "\n[services]\n");
    fprintf(out, "intro_point     = %s\n", bool_str(cfg->intro_point));
    fprintf(out, "exit_relay      = %s\n", bool_str(cfg->exit_relay));
    if (cfg->exit_socks5_port)
        fprintf(out, "exit_socks5_port = %d\n", cfg->exit_socks5_port);
    for (size_t i = 0; i < cfg->exit_policy_count; i++)
        fprintf(out, "exit_policy     = %s\n", cfg->exit_policies[i]);
    if (cfg->exclude_jur_count > 0) {
        fprintf(out, "exclude_jurisdiction = ");
        for (size_t i = 0; i < cfg->exclude_jur_count; i++)
            fprintf(out, "%s%s", i > 0 ? "," : "", cfg->exclude_jurs[i]);
        fprintf(out, "\n");
    }

    fprintf(out, "\n[incentive]\n");
    if (cfg->btcpay_url)
        fprintf(out, "btcpay_url      = %s\n", cfg->btcpay_url);
    if (cfg->btcpay_key)
        fprintf(out, "btcpay_key      = %s\n", cfg->btcpay_key);
    if (cfg->lncli_path)
        fprintf(out, "lncli_path      = %s\n", cfg->lncli_path);
    fprintf(out, "sats_per_mb     = %" PRIu64 "\n",
            cfg->sats_per_mb ? cfg->sats_per_mb : 1);

    fprintf(out, "\n[logging]\n");
    fprintf(out, "log_level       = %s\n", log_level_str(cfg->log_level));
    if (cfg->log_file)
        fprintf(out, "log_file        = %s\n", cfg->log_file);

    fprintf(out, "\n[turn]\n");
    fprintf(out, "turn            = %s\n", bool_str(cfg->turn_enabled));
    if (cfg->turn_listen_addr)
        fprintf(out, "turn_listen     = %s\n", cfg->turn_listen_addr);
    if (cfg->turn_realm)
        fprintf(out, "turn_realm      = %s\n", cfg->turn_realm);
    fprintf(out, "turn_max_alloc  = %u\n", cfg->turn_max_allocations);
    fprintf(out, "turn_max_bps    = %u\n", cfg->turn_max_bps);
    if (cfg->turn_transport)
        fprintf(out, "turn_transport  = %s\n", cfg->turn_transport);
    fprintf(out, "turn_privacy    = %s\n", privacy_str(cfg->turn_privacy));
    if (cfg->turn_external_ip)
        fprintf(out, "turn_external_ip = %s\n", cfg->turn_external_ip);
    if (cfg->turn_cert_pem)
        fprintf(out, "turn_cert       = %s\n", cfg->turn_cert_pem);
    if (cfg->turn_front_host)
        fprintf(out, "turn_front_host = %s\n", cfg->turn_front_host);
    if (cfg->turn_relay_target)
        fprintf(out, "turn_relay_target = %s\n", cfg->turn_relay_target);
    if (cfg->turn_ech_public_name)
        fprintf(out, "turn_ech        = %s\n", cfg->turn_ech_public_name);
    if (cfg->turn_pt_binary)
        fprintf(out, "turn_pt_binary  = %s\n", cfg->turn_pt_binary);
    if (cfg->turn_pt_transport)
        fprintf(out, "turn_pt_transport = %s\n", cfg->turn_pt_transport);

    fprintf(out, "\n[daemon]\n");
    fprintf(out, "daemon          = %s\n", bool_str(cfg->daemonize));
    if (cfg->pidfile)
        fprintf(out, "pidfile         = %s\n", cfg->pidfile);
}
