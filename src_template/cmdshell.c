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
#include <ctype.h>
#include <stdarg.h>
#include "cmdshell.h"
#include "input.h"

int str_isspace(const char *s);

void cmdshell_start(void)
{
    printf("cmdshell: start()\n");
}

enum cmdshell_code
cmdshell_prompt_command(const char *prompt, char in[], int maxsize)
{
    char input[CMDSHELL_MAXINPUT];
    int retval;

    *in = '\0';
    while (1) {
        retval = input_line(prompt, input, sizeof input);
        if (!retval || str_isspace(input)) {
            putchar('\n'); /* avoid prompts join when Ctrl + D is
                              pressed */
        }
        else if (strcmp(input, "help") == 0) {
            return CMD_HELP;
        }
        else if (strcmp(input, "quit") == 0) {
            return CMD_QUIT;
        }
        else {
            strcpy(in, input);
            break;
        }
    }
    return CMD_UNKNOWN;
}

void cmdshell_print_error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "cmdshell: error: ");
    vfprintf(stderr, fmt, args);
    putc('\n', stderr);
    va_end(args);
}

void cmdshell_print_help(void)
{
    printf("cmdshell: print_help()\n");
}

void cmdshell_end(void)
{
    printf("cmdshell: end()\n");
}

/* str_isspace: check string for whitespace content
                1 - all chars are whitespace
                0 - non-whitespace char found */
int str_isspace(const char *s)
{
    while (isspace(*s))
        s++;
    return *s == '\0';
}
