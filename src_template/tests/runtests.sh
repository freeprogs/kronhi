#!/bin/bash

# This file is a part of __PROGRAM_NAME__ __PROGRAM_VERSION__
#
# This file runs automatic tests for the program parts.
#
# __PROGRAM_COPYRIGHT__ __PROGRAM_AUTHOR__ __PROGRAM_AUTHOR_EMAIL__
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

progname=`basename $0`
progdir=`dirname $0`

# Run tests silently
# run_tests()
run_tests()
{
    for i in test_*; do
        echo "Run $i:"
        run_one_test "$i" | filter_lines "^Suite" "^  Test:"
    done
}

# Run one test by name
# run_one_test(testname)
run_one_test()
{
    local testname=$1

    ./"$testname"
}

# Filter lines from stdin containing word or words
# filter_lines(word, ...)
filter_lines()
{
    local regexp=""

    for i in "$@"; do
        if [ -z "$regexp" ]; then
            regexp="$i"
        else
            regexp="$regexp\|$i"
        fi
    done
    grep "$regexp"
}

# Run tests verbosely
# run_tests_verbose()
run_tests_verbose()
{
    for i in test_*; do
        echo "Run $i:"
        run_one_test "$i"
    done
}

# Print an error message to stderr
# error(str)
error()
{
    echo "error: $progname: $1" >&2
}

# Print short help about this script
# usage()
usage()
{
    echo "Try \`./$progname --help' for more information." >&2
}

# Print full help about this script
# print_help()
print_help()
{
    {
        echo "usage: ./$progname [ -v ]"
        echo ""
        echo "  noarg   --  Run tests silently."
        echo "     -v   --  Run tests verbosely."
        echo ""
    } >&2
}

# Run main script operations
# main([cmdarg])
main()
{
    cd "$progdir"
    if [ $# -ne 0 -a "$1" = "--help" ]; then
        print_help
        return 1
    fi
    usage
    if [ $# -eq 0 ]; then
        run_tests
        return 0
    fi
    if [ $# -ne 0 -a "$1" = "-v" ]; then
        run_tests_verbose
        return 0
    fi
}

main "$@" || exit 1
exit 0
