/*
 * This file is a part of __PROGRAM_NAME__ __PROGRAM_VERSION__
 *
 * __PROGRAM_COPYRIGHT__ __PROGRAM_AUTHOR__ __PROGRAM_AUTHOR_EMAIL__
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include "input.h"

/* input_line: read a line from stdin with a prompt
               1 - was read a character at least
               0 - nothing was read */
int input_line(const char *prompt, char in[], int maxsize)
{
    char line[INPUT_MAXLINE];
    char fmt[INPUT_MAXFORMAT];
    int ret;

    sprintf(fmt, "%%%d[^\n]", maxsize - 1);

    printf("%s", prompt);
    fflush(stdout);

    ret = 0;
    if (fgets(line, sizeof line, stdin)) {
        *in = '\0';
        sscanf(line, fmt, in);
        ret = *in != '\0';
    }
    return ret;
}
