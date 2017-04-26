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
void info_printer(const char *lines[], int n);

/* cmdshell_start: run starting operations */
void cmdshell_start(void)
{
    ;
}

/* cmdshell_print_message: print formatted message to standard output */
void cmdshell_print_message(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    putc('\n', stdout);
    va_end(args);
}

/* cmdshell_prompt_command: print prompt and ask for a command
                            return the input command code */
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
        else if (strcmp(input, "init write") == 0) {
            return CMD_INIT_WRITE;
        }
        else if (strcmp(input, "init read") == 0) {
            return CMD_INIT_READ;
        }
        else if (strcmp(input, "status") == 0) {
            return CMD_STATUS;
        }
        else {
            strcpy(in, input);
            break;
        }
    }
    return CMD_UNKNOWN;
}

/* cmdshell_print_error: print formatted error message to standard output */
void cmdshell_print_error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "cmdshell: error: ");
    vfprintf(stderr, fmt, args);
    putc('\n', stderr);
    va_end(args);
}

/* cmdshell_print_help: print help info to standard output */
void cmdshell_print_help(void)
{
    const char *lines[] = {
        "\n",
        "Help info:\n",
        "\n",
        "init write  --  initialize options for writing\n",
        "                (source, destination, offset, cipher)\n"
        "init read   --  initialize options for reading\n",
        "                (source, destination, offset, cipher)\n"
        "status      --  show set program options\n",
        "\n",
        "help        --  print this info\n",
        "quit        --  exit the command shell\n",
        "\n"
    };
    info_printer(lines, ARRAY_SIZE(lines));
}

/* cmdshell_init_write: input options for write command
                        return 0 when wrong commands were input
                        return 1 when right commands were input */
int cmdshell_init_write(
    char src[], char dst[], char offset[], char cipher[])
{
    char input[CMDSHELL_MAXINPUT];
    int retval, retin;
    int f_bad_offset, f_bad_cipher;

    *input = '\0';
    f_bad_offset = f_bad_cipher = 0;

    retin = input_line("Input source (path): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", src);
    }
    else {
        strcpy(src, input);
        printf("Ok \"%s\"\n", input);
    }

    *input = '\0';
    retin = input_line("Input destination (path): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", dst);
    }
    else {
        strcpy(dst, input);
        printf("Ok \"%s\"\n", input);
    }

    *input = '\0';
    retin = input_line("Input offset (number >= 0): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", offset);
    }
    else {
        size_t tmp;
        if (sscanf(input, "%lu", (long unsigned *) &tmp) == 1) {
            strcpy(offset, input);
            printf("Ok \"%s\"\n", input);
        }
        else {
            f_bad_offset = 1;
            printf("Fail \"%s\", should be a number\n", input);
        }
    }

    *input = '\0';
    retin = input_line("Input cipher (xor, none): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", cipher);
    }
    else {
        if (strcmp(input, "xor") == 0) {
            strcpy(cipher, input);
            printf("Ok \"%s\"\n", input);
        }
        else if (strcmp(input, "none") == 0) {
            strcpy(cipher, input);
            printf("Ok \"%s\"\n", input);
        }
        else {
            f_bad_cipher = 1;
            printf("Fail \"%s\", unknown type\n", input);
        }
    }

    retval = !(f_bad_offset || f_bad_cipher);
    return retval;
}

/* cmdshell_init_read: input options for read command
                       return 0 when wrong commands were input
                       return 1 when right commands were input */
int cmdshell_init_read(
    char src[], char dst[], char offset[], char cipher[])
{
    char input[CMDSHELL_MAXINPUT];
    int retval, retin;
    int f_bad_offset, f_bad_cipher;

    *input = '\0';
    f_bad_offset = f_bad_cipher = 0;

    retin = input_line("Input source (path): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", src);
    }
    else {
        strcpy(src, input);
        printf("Ok \"%s\"\n", input);
    }

    *input = '\0';
    retin = input_line("Input destination (path): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", dst);
    }
    else {
        strcpy(dst, input);
        printf("Ok \"%s\"\n", input);
    }

    *input = '\0';
    retin = input_line("Input offset (number >= 0): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", offset);
    }
    else {
        size_t tmp;
        if (sscanf(input, "%lu", (long unsigned *) &tmp) == 1) {
            strcpy(offset, input);
            printf("Ok \"%s\"\n", input);
        }
        else {
            f_bad_offset = 1;
            printf("Fail \"%s\", should be a number\n", input);
        }
    }

    *input = '\0';
    retin = input_line("Input cipher (xor, none): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Ok unchanged \"%s\"\n", cipher);
    }
    else {
        if (strcmp(input, "xor") == 0) {
            strcpy(cipher, input);
            printf("Ok \"%s\"\n", input);
        }
        else if (strcmp(input, "none") == 0) {
            strcpy(cipher, input);
            printf("Ok \"%s\"\n", input);
        }
        else {
            f_bad_cipher = 1;
            printf("Fail \"%s\", unknown type\n", input);
        }
    }

    retval = !(f_bad_offset || f_bad_cipher);
    return retval;
}

/* cmdshell_print_status: print status of set options to standard output */
void cmdshell_print_status(
    const char *wsrc, const char *wdst,
    const char *woffset, const char *wcipher,
    const char *rsrc, const char *rdst,
    const char *roffset, const char *rcipher)
{
    printf(
        "\n"
        "Write options:\n"
        "  Source:             \"%s\"\n"
        "  Destination:        \"%s\"\n"
        "  Destination Offset: %s\n"
        "  Destination Cipher: %s\n"
        "\n"
        "Read options:\n"
        "  Source:             \"%s\"\n"
        "  Destination:        \"%s\"\n"
        "  Destination Offset: %s\n"
        "  Destination Cipher: %s\n"
        "\n"
        ,
        wsrc, wdst, woffset, wcipher,
        rsrc, rdst, roffset, rcipher);
}

/* cmdshell_end: run ending operations */
void cmdshell_end(void)
{
    ;
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

/* info_printer: print lines to standard output */
void info_printer(const char *lines[], int n)
{
    int i;

    for (i = 0; i < n; i++)
        fprintf(stdout, "%s", lines[i]);
}
