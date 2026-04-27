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

#ifndef VINE_RELAY_CONFIG_H
#define VINE_RELAY_CONFIG_H

#include "log.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VINE_MAX_JURISDICTIONS  16
#define VINE_JURISDICTION_LEN   4
#define VINE_MAX_SEEDS          8
#define VINE_MAX_EXIT_POLICIES  32

typedef struct vine_relay_config {
    // Network
    const char *listen_addr;          /* default "0.0.0.0:9100" */
    const char *seed_addrs[VINE_MAX_SEEDS];
    size_t      seed_count;
    const char *identity_path;        /* key file path */

    // Transport & camouflage.
    int         transport_mode;       /* trellis_transport_mode_t */
    int         camouflage_type;      /* trellis_camouflage_type_t */
    bool        morph_metamorphic;    /* enable encoding mutation */
    bool        verify_peer;          /* TLS certificate verification */

    // Pluggable transport (used when transport_mode == PT)
    const char *pt_binary;            /* path to PT binary (e.g. obfs4proxy) */
    const char *pt_transport_name;    /* protocol name (e.g. "obfs4", "snowflake") */
    const char *pt_args;              /* extra PT arguments */

    // Routing
    int         routing_mode;         /* trellis_routing_mode_t (-1 = default) */

    // Privacy / anti-analysis.
    uint32_t    cover_traffic_ms;     /* cover traffic interval (0 = disabled) */
    uint32_t    decoy_interval_ms;    /* decoy circuit interval (0 = disabled) */
    bool        always_on;            /* keep running even when idle */

    // DDoS protection.
    uint32_t    pow_iterations;       /* per-connection PoW (0 = default) */

    // Monitoring
    int         metrics_port;         /* Prometheus port (0 = disabled, default 9101) */

    // Warrant canary.
    bool        canary_enabled;       /* publish warrant canary to DHT */
    const char *canary_statement;     /* canary attestation text */

    // Greenhouse
    bool        intro_point;          /* serve as introduction point */
    bool        exit_relay;           /* enable clearnet exit relay */

    int         exit_socks5_port;    /* local SOCKS5 proxy port (0 = disabled) */

    // Exit policy rules (format: "allow:*.example.com:443" or "reject:*:*")
    const char *exit_policies[VINE_MAX_EXIT_POLICIES];
    size_t      exit_policy_count;

    // Jurisdiction exclusion.
    char        exclude_jurs[VINE_MAX_JURISDICTIONS][VINE_JURISDICTION_LEN];
    size_t      exclude_jur_count;

    // Logging
    vine_log_level_t log_level;
    const char *log_file;             /* NULL = stderr */

    // Daemon
    bool        daemonize;
    const char *pidfile;

    // Incentive / Lightning.
    const char *btcpay_url;
    const char *btcpay_key;
    const char *lncli_path;           /* path to lncli binary (default: "lncli") */
    uint64_t    sats_per_mb;          /* micropayment rate (default: 1) */

    // Discovery
    bool        no_discover;          /* disable LAN discovery */

    // Guard & cell-level anonymity hardening.
    bool        guard_enabled;        /* pin entry guards (Sybil resistant) */
    const char *guard_persist_path;   /* guard state file (NULL = auto) */
    bool        morph_cell_mode;      /* fixed-size cell fragmentation */
    uint32_t    morph_cell_size;      /* cell size in bytes (0 = default 512) */
    bool        no_eclipse;           /* disable eclipse attack detection (local testing) */
    bool        no_sybil;             /* disable all Sybil defenses (local testing) */

    // TURN relay.
    bool        turn_enabled;
    const char *turn_listen_addr;     /* default "0.0.0.0:443" */
    const char *turn_secret;          /* shared secret for HMAC auth */
    const char *turn_realm;           /* e.g. "neverroute.com" */
    const char *turn_cert_pem;        /* TLS cert for TURNS */
    const char *turn_key_pem;         /* TLS private key */
    uint32_t    turn_max_allocations; /* default 200 */
    uint32_t    turn_max_bps;         /* per-allocation bandwidth cap */
    const char *turn_transport;       /* "tls", "tls,quic" */
    int         turn_privacy;         /* trellis_privacy_level_t (0=speed,1=balanced,2=max) */

    // TURN external IP (if not set, derived from listen_addr)
    const char *turn_external_ip;

    // Domain fronting (TURN port)
    const char *turn_front_host;
    const char *turn_relay_target;

    // ECH (TURN port)
    const char *turn_ech_public_name;

    // Pluggable transport fallback (TURN port)
    const char *turn_pt_binary;
    const char *turn_pt_transport;

    // Config file path (internal)
    const char *config_path;

    /* Bitmask tracking which fields were explicitly set by CLI.
     * Used by vine_config_load_file to avoid overwriting CLI values. */
    uint64_t    _cli_set;
} vine_relay_config_t;

vine_relay_config_t vine_config_defaults(void);

/*
 * Parse command-line arguments into cfg. Returns:
 *   0 = success, 1 = --help, 2 = --dump-config, 3 = --version, -1 = error.
 */
int vine_config_parse_cli(vine_relay_config_t *cfg, int argc, char **argv);

/*
 * Load a key=value config file and apply to cfg. CLI values already
 * set take precedence (non-default values are not overwritten).
 * Returns 0 on success, -1 on error. Missing file is not an error.
 */
int vine_config_load_file(vine_relay_config_t *cfg, const char *path);

void vine_config_print_help(const char *prog);
void vine_config_dump(const vine_relay_config_t *cfg, FILE *out);

#ifdef __cplusplus
}
#endif

#endif /* VINE_RELAY_CONFIG_H */
