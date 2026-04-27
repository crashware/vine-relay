#!/usr/bin/env python3

"""
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
                            d8P"" "98888P"" "98888b,
                            9b    d8888,    `9888B
                          ,d88aaa8888888b,,,d888P'
                         d8888888888888888888888b 
                        d888888P""98888888888888P 
                        88888P'    9888888888888  
                        `98P'       9888888888P'  
                                     `"9888P"'    
                                        `"'
===============================================================================
"""

"""
Build script for vine-relay — the Bloom Protocol relay node.

Builds libtrellis (with all FetchContent dependencies) and vine-relay
from the top-level trellis CMake project, then copies the binary into
this directory.

Requirements:
    - CMake 3.20+
    - C/C++ compiler (gcc/clang)
    - libsodium-dev >= 1.0.18
    - libuv1-dev
    - pkg-config
    - Python 3.6+

Usage:
    python3 build.py              # Release build
    python3 build.py --debug      # Debug build
    python3 build.py --clean      # Clean and rebuild
    python3 build.py --jobs 4     # Limit parallel jobs
"""

import argparse
import multiprocessing
import os
import shutil
import subprocess
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TRELLIS_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, '..', '..'))
DEFAULT_BUILD_DIR = os.path.join(TRELLIS_ROOT, 'build')


def check_prerequisites():
    missing = []

    if not shutil.which('cmake'):
        missing.append('cmake')
    if not shutil.which('make') and not shutil.which('ninja'):
        missing.append('make (or ninja)')
    if not shutil.which('pkg-config'):
        missing.append('pkg-config')

    cc = os.environ.get('CC', '')
    if not cc:
        if not shutil.which('cc') and not shutil.which('gcc') and not shutil.which('clang'):
            missing.append('C compiler (gcc or clang)')

    result = subprocess.run(
        ['pkg-config', '--exists', 'libsodium'],
        capture_output=True
    )
    if result.returncode != 0:
        missing.append('libsodium-dev (apt install libsodium-dev)')

    if missing:
        print('Missing prerequisites:', file=sys.stderr)
        for m in missing:
            print(f'  - {m}', file=sys.stderr)
        print('\nInstall with:', file=sys.stderr)
        print('  sudo apt install build-essential cmake pkg-config libsodium-dev libuv1-dev',
              file=sys.stderr)
        return False
    return True


def run(cmd, cwd=None, env=None):
    print(f'  $ {" ".join(cmd)}')
    result = subprocess.run(cmd, cwd=cwd, env=env)
    if result.returncode != 0:
        print(f'\nCommand failed with exit code {result.returncode}', file=sys.stderr)
        sys.exit(result.returncode)


def main():
    parser = argparse.ArgumentParser(description='Build vine-relay')
    parser.add_argument('--debug', action='store_true', help='Debug build')
    parser.add_argument('--clean', action='store_true', help='Clean build directory first')
    parser.add_argument('--jobs', '-j', type=int, default=multiprocessing.cpu_count(),
                        help='Parallel build jobs (default: all CPUs)')
    parser.add_argument('--build-dir', default=DEFAULT_BUILD_DIR,
                        help=f'Build directory (default: {DEFAULT_BUILD_DIR})')
    args = parser.parse_args()

    print('vine-relay build script')
    print('=' * 50)

    if not check_prerequisites():
        sys.exit(1)

    build_type = 'Debug' if args.debug else 'Release'
    build_dir = os.path.abspath(args.build_dir)

    if args.clean and os.path.exists(build_dir):
        print(f'\nCleaning {build_dir}...')
        shutil.rmtree(build_dir)

    os.makedirs(build_dir, exist_ok=True)

    # CMake configure
    print(f'\n[1/3] Configuring ({build_type})...')
    cmake_env = os.environ.copy()
    # Some systems have a broken CMAKE_ROOT; help cmake find its modules
    cmake_root = shutil.which('cmake')
    if cmake_root:
        cmake_root = os.path.dirname(os.path.dirname(os.path.realpath(cmake_root)))
        modules = os.path.join(cmake_root, 'share')
        for d in os.listdir(modules) if os.path.isdir(modules) else []:
            if d.startswith('cmake'):
                candidate = os.path.join(modules, d)
                if os.path.isdir(os.path.join(candidate, 'Modules')):
                    cmake_env['CMAKE_ROOT'] = candidate
                    break

    run([
        'cmake', TRELLIS_ROOT,
        f'-DCMAKE_BUILD_TYPE={build_type}',
        '-DTRELLIS_BUILD_STATIC=ON',
        '-DTRELLIS_BUILD_TESTS=OFF',
        '-DTRELLIS_WITH_LIGHTNING=ON',
    ], cwd=build_dir, env=cmake_env)

    # Build vine-relay (and its dependency: trellis_static)
    print(f'\n[2/3] Building vine-relay ({args.jobs} jobs)...')
    run([
        'cmake', '--build', build_dir,
        '--target', 'vine-relay',
        '--config', build_type,
        '-j', str(args.jobs),
    ], env=cmake_env)

    # Copy binary to this directory
    built_binary = os.path.join(build_dir, 'apps', 'vine-relay', 'vine-relay')
    if not os.path.isfile(built_binary):
        print(f'Error: expected binary at {built_binary}', file=sys.stderr)
        sys.exit(1)

    dest = os.path.join(SCRIPT_DIR, 'vine-relay')
    shutil.copy2(built_binary, dest)
    os.chmod(dest, 0o755)

    size_mb = os.path.getsize(dest) / (1024 * 1024)
    print(f'\n[3/3] Done!')
    print(f'  Binary: {dest} ({size_mb:.1f} MB)')
    print(f'  Run:    ./vine-relay --help')


if __name__ == '__main__':
    main()
