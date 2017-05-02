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
        else if (strcmp(input, "init write dir") == 0) {
            return CMD_INIT_WRITE_DIR;
        }
        else if (strcmp(input, "init read") == 0) {
            return CMD_INIT_READ;
        }
        else if (strcmp(input, "status write") == 0) {
            return CMD_STATUS_WRITE;
        }
        else if (strcmp(input, "status write dir") == 0) {
            return CMD_STATUS_WRITE_DIR;
        }
        else if (strcmp(input, "status read") == 0) {
            return CMD_STATUS_READ;
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
        "init write        --  initialize options for writing\n",
        "                      (source, destination, offset, cipher)\n"
        "init write dir    --  initialize write directory data\n",
        "                      (description)\n",
        "init read         --  initialize options for reading\n",
        "                      (source, destination, offset, cipher)\n"
        "status write      --  show set write options\n",
        "status write dir  --  show write directory contents\n",
        "status read       --  show set read options\n",
        "\n",
        "help              --  print this info\n",
        "quit              --  exit the command shell\n",
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

/* cmdshell_print_status_write:
   print status of set write options to standard output */
void cmdshell_print_status_write(
    const char *wsrc, const char *wdst,
    const char *woffset, const char *wcipher)
{
    printf(
        "\n"
        "Write options:\n"
        "  Source:             \"%s\"\n"
        "  Destination:        \"%s\"\n"
        "  Destination Offset: %s\n"
        "  Destination Cipher: %s\n"
        "\n"
        ,
        wsrc, wdst, woffset, wcipher);
}

/* cmdshell_print_status_write_dir:
   print status of write directory to standard output */
void cmdshell_print_status_write_dir(const char *desc)
{
    printf(
        "\n"
        "Write directory description:\n"
        "%s"
        "\n"
        ,
        (*desc != '\0' ? desc : "empty\n"));
}

/* cmdshell_print_status_read:
   print status of set read options to standard output */
void cmdshell_print_status_read(
    const char *rsrc, const char *rdst,
    const char *roffset, const char *rcipher)
{
    printf(
        "\n"
        "Read options:\n"
        "  Source:             \"%s\"\n"
        "  Destination:        \"%s\"\n"
        "  Source Offset:      %s\n"
        "  Source Cipher:      %s\n"
        "\n"
        ,
        rsrc, rdst, roffset, rcipher);
}

/* cmdshell_init_write_dir: input settings for write directory
                            return the input setting code */
enum cmdshell_dir_code
cmdshell_init_write_dir(char descinter[], char descfile[])
{
    enum cmdshell_dir_code retval;
    int retin;
    char input[CMDSHELL_MAXINPUT];
    char text[CMDSHELL_MAXTEXTINPUT];

    *input = '\0';

    retin = input_line(
        "Input description method (i (interactive), f (file)): ",
        input, sizeof input);
    if (!retin || str_isspace(input)) {
        retval = CMD_DIR_NOOP;
        printf("Ok unchanged text and filename\n");
    }
    else if (strcmp(input, "i") == 0) {
        retin = input_text_end(
            "Input description (end with single . on line):\n",
            text, sizeof text, ".");
        if (!retin) {
            *descinter = '\0';
            retval = CMD_DIR_INTER;
            printf("Ok text empty\n");
        }
        else {
            strcpy(descinter, text);
            retval = CMD_DIR_INTER;
            printf("Ok text\n%s\n", text);
        }
    }
    else if (strcmp(input, "f") == 0) {
        retin = input_line("Input file name (path): ", input, sizeof input);
        if (!retin || str_isspace(input)) {
            retval = CMD_DIR_NOOP;
            printf("Ok unchanged filename\n");
        }
        else {
            strcpy(descfile, input);
            retval = CMD_DIR_FILE;
            printf("Ok filename \"%s\"\n", input);
        }
    }
    else {
        retval = CMD_DIR_UNKNOWN;
        printf("Fail unknown command \"%s\"\n", input);
    }
    return retval;
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
