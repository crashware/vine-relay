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
 * test_vine_config.c — Unit tests for vine-relay configuration parsing
 *
 * Tests default values, CLI argument parsing, and edge cases without
 * starting the relay.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "config.h"

#define RUN_TEST(fn) do { printf("  %-50s", #fn "..."); fn(); printf("PASS\n"); } while(0)
#define ASSERT(cond) do { if (!(cond)) { fprintf(stderr, "FAIL at %s:%d: %s\n", __FILE__, __LINE__, #cond); abort(); } } while(0)

static void test_config_defaults(void)
{
    vine_relay_config_t cfg = vine_config_defaults();

    ASSERT(cfg.listen_addr != NULL);
    ASSERT(strcmp(cfg.listen_addr, "0.0.0.0:9100") == 0);
    ASSERT(cfg.metrics_port == 9101);
    ASSERT(cfg.canary_enabled == true);
    ASSERT(cfg.always_on == true);
    ASSERT(cfg.routing_mode == -1);
    ASSERT(cfg.exit_relay == false);
    ASSERT(cfg.exit_socks5_port == 0);
    ASSERT(cfg.exit_policy_count == 0);
    ASSERT(cfg.seed_count == 0);
    ASSERT(cfg.no_discover == true);
}

static void test_config_parse_listen(void)
{
    vine_relay_config_t cfg = vine_config_defaults();
    char *argv[] = {"vine-relay", "--listen", "127.0.0.1:8000", NULL};
    int argc = 3;

    optind = 1;
    int rc = vine_config_parse_cli(&cfg, argc, argv);
    ASSERT(rc == 0);
    ASSERT(strcmp(cfg.listen_addr, "127.0.0.1:8000") == 0);
}

static void test_config_parse_exit_socks5(void)
{
    vine_relay_config_t cfg = vine_config_defaults();
    char *argv[] = {"vine-relay", "--exit-relay", "--exit-socks5-port", "1080", NULL};
    int argc = 4;

    optind = 1;
    int rc = vine_config_parse_cli(&cfg, argc, argv);
    ASSERT(rc == 0);
    ASSERT(cfg.exit_relay == true);
    ASSERT(cfg.exit_socks5_port == 1080);
}

static void test_config_parse_exit_policy(void)
{
    vine_relay_config_t cfg = vine_config_defaults();
    char *argv[] = {
        "vine-relay",
        "--exit-policy", "allow:*:443",
        "--exit-policy", "reject:*:*",
        NULL
    };
    int argc = 5;

    optind = 1;
    int rc = vine_config_parse_cli(&cfg, argc, argv);
    ASSERT(rc == 0);
    ASSERT(cfg.exit_policy_count == 2);
    ASSERT(strcmp(cfg.exit_policies[0], "allow:*:443") == 0);
    ASSERT(strcmp(cfg.exit_policies[1], "reject:*:*") == 0);
}

static void test_config_parse_routing_onion(void)
{
    vine_relay_config_t cfg = vine_config_defaults();
    char *argv[] = {"vine-relay", "--routing", "onion", NULL};
    int argc = 3;

    optind = 1;
    int rc = vine_config_parse_cli(&cfg, argc, argv);
    ASSERT(rc == 0);
    // TRELLIS_ROUTE_ONION == 3
    ASSERT(cfg.routing_mode == 3);
}

static void test_config_parse_help_returns_1(void)
{
    vine_relay_config_t cfg = vine_config_defaults();
    char *argv[] = {"vine-relay", "--help", NULL};
    int argc = 2;

    optind = 1;
    int rc = vine_config_parse_cli(&cfg, argc, argv);
    ASSERT(rc == 1);
}

int main(void)
{
    printf("=== Test Suite: Vine Relay Config ===\n");

    RUN_TEST(test_config_defaults);
    RUN_TEST(test_config_parse_listen);
    RUN_TEST(test_config_parse_exit_socks5);
    RUN_TEST(test_config_parse_exit_policy);
    RUN_TEST(test_config_parse_routing_onion);
    RUN_TEST(test_config_parse_help_returns_1);

    printf("=== All vine relay config tests passed ===\n");
    return 0;
}
