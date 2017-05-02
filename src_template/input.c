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
#include <string.h>
#include "input.h"

/* input_line: read a line from stdin with a prompt
               return 1 when was read a character at least
               return 0 when nothing was read */
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

/* input_text_end:
   read a text from stdin with a prompt and ending on a line;
   skip input lines until ending line if there is no room;
   return 1 when was read a character at least
   return 0 when nothing was read */
int input_text_end(
    const char *prompt, char in[], size_t maxsize, const char *end)
{
    char line[INPUT_MAXLINE];
    size_t linelen;
    char endline[INPUT_MAXLINE];
    int ret;

    printf("%s", prompt);
    fflush(stdout);

    sprintf(endline, "%s\n", end);

    ret = 0;
    *in = '\0';
    while (fgets(line, sizeof line, stdin)) {
        if (strcmp(line, endline) == 0)
            break;
        if ((linelen = strlen(line)) < maxsize) {
            strcat(in, line);
            maxsize -= linelen;
        }
    }
    ret = *in != '\0';
    return ret;
}

/* input_from_file: read a string from file
                    return 1 when was read a character at least
                    return 0 when nothing was read */
int input_from_file(char in[], size_t maxsize, const char *ifname)
{
    int retval;
    FILE *ifp;

    retval = 1;
    ifp = fopen(ifname, "rb");
    if (ifp == NULL) {
        retval = 0;
    }
    else {
        size_t ret = fread(in, 1, maxsize - 1, ifp);
        if (ret > 0)
            in[ret] = '\0';
        else
            retval = 0;
    }
    if (ifp != NULL)
        fclose(ifp);
    return retval;
}
