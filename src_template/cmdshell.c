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

#include "cmdshell.h"

int str_isspace(const char *s);
int str_isdigit(const char *s);
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
        else if (strcmp(input, "init write file") == 0) {
            return CMD_INIT_WRITE_FILE;
        }
        else if (strcmp(input, "init read") == 0) {
            return CMD_INIT_READ;
        }
        else if (strcmp(input, "init password") == 0) {
            return CMD_INIT_PASSWORD;
        }
        else if (strcmp(input, "status write") == 0) {
            return CMD_STATUS_WRITE;
        }
        else if (strcmp(input, "status write dir") == 0) {
            return CMD_STATUS_WRITE_DIR;
        }
        else if (strcmp(input, "status write file") == 0) {
            return CMD_STATUS_WRITE_FILE;
        }
        else if (strcmp(input, "status read") == 0) {
            return CMD_STATUS_READ;
        }
        else if (strcmp(input, "status password") == 0) {
            return CMD_STATUS_PASSWORD;
        }
        else if (strcmp(input, "write dir") == 0) {
            return CMD_WRITE_DIR;
        }
        else if (strcmp(input, "write file") == 0) {
            return CMD_WRITE_FILE;
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
        "init write file   --  initialize write file data\n",
        "                      (archive filename, description, relative offset)\n",
        "init read         --  initialize options for reading\n",
        "                      (source, destination, offset, cipher)\n"
        "init password     --  initialize password for read and write operations\n",
        "status write      --  show set write options\n",
        "status write dir  --  show write directory contents\n",
        "status write file --  show write file data\n",
        "status read       --  show set read options\n",
        "status password   --  show set password for read and write operations\n",
        "write dir         --  write directory data to destination\n",
        "write file        --  write file data and contents to destination\n",
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
        double tmp;
        if (strlen(input) > BIG_MAXSTRING) {
            f_bad_offset = 1;
            printf("Fail \"%s\", should be between 1 and %d digits\n",
                   input, BIG_MAXSTRING);
        }
        else if (!str_isdigit(input)) {
            f_bad_offset = 1;
            printf("Fail \"%s\", should be an unsigned number\n", input);
        }
        else if (sscanf(input, "%lf", &tmp) == 1) {
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
        double tmp;
        if (strlen(input) > BIG_MAXSTRING) {
            f_bad_offset = 1;
            printf("Fail \"%s\", should be between 1 and %d digits\n",
                   input, BIG_MAXSTRING);
        }
        else if (!str_isdigit(input)) {
            f_bad_offset = 1;
            printf("Fail \"%s\", should be an unsigned number\n", input);
        }
        else if (sscanf(input, "%lf", &tmp) == 1) {
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

/* cmdshell_init_password: input password for read and write operations
                           return 0 when password was not input
                           return 1 when password was input */
int cmdshell_init_password(char password[])
{
    char input[CMDSHELL_MAXINPUT];
    int retval, retin;

    *input = '\0';

    retin = input_line("Input password: ", input, sizeof input);
    if (!retin) {
        *password = '\0';
        printf("Ok empty\n");
        retval = 1;
    }
    else {
        strcpy(password, input);
        printf("Ok %u characters\n", (unsigned) strlen(password));
        retval = 1;
    }
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


/* cmdshell_print_status_write_file:
   print status of write file to standard output */
void cmdshell_print_status_write_file(
    const char *filename, const char *filedesc, const char *filereloff)
{
    printf(
        "\n"
        "Write file filename:\n"
        "%s\n"
        "Write file description:\n"
        "%s"
        "\n"
        "Write file relative offset:\n"
        "%s\n"
        ,
        (*filename != '\0' ? filename : "empty\n"),
        (*filedesc != '\0' ? filedesc : "empty\n"),
        (*filereloff != '\0' ? filereloff : "empty\n"));
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

/* cmdshell_print_status_password:
   print status of set password for read and write operations to
   standard output */
void cmdshell_print_status_password(const char *password)
{
    printf(
        "\n"
        "Password (in angle brackets):\n"
        "<%s>\n"
        "%u characters\n",
        password,
        (unsigned) strlen(password)
    );
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

/* cmdshell_init_write_file:
   input settings for write file
   return 1 when right settings were input
   return 0 when wrong settings were input */
int cmdshell_init_write_file(
    char filename[], char filedesc[], char filereloff[])
{
    int retval, retin;
    char input[CMDSHELL_MAXINPUT];
    char text[CMDSHELL_MAXTEXTINPUT];
    int f_filename_is_set, f_offset_is_set;

    f_filename_is_set = f_offset_is_set = 0;

    *input = '\0';
    retin = input_line("Input filename: ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        printf("Fail empty\n");
    }
    else {
        strcpy(filename, input);
        f_filename_is_set = 1;
        printf("Ok \"%s\"\n", input);
    }

    *text = '\0';
    retin = input_text_end(
        "Input description (end with single . on line):\n",
        text, sizeof text, ".");
    if (!retin) {
        *filedesc = '\0';
        printf("Ok text empty\n");
    }
    else {
        strcpy(filedesc, text);
        printf("Ok text\n%s\n", text);
    }

    *input = '\0';
    retin = input_line(
        "Input relative offset (number >= 0): ", input, sizeof input);
    if (!retin || str_isspace(input)) {
        f_offset_is_set = 1;
        printf("Ok unchanged \"%s\"\n", filereloff);
    }
    else {
        double tmp;
        if (strlen(input) > BIG_MAXSTRING) {
            f_offset_is_set = 0;
            printf("Fail \"%s\", should be between 1 and %d digits\n",
                   input, BIG_MAXSTRING);
        }
        else if (!str_isdigit(input)) {
            f_offset_is_set = 0;
            printf("Fail \"%s\", should be an unsigned number\n", input);
        }
        else if (sscanf(input, "%lf", &tmp) == 1) {
            if (tmp >= 0 && tmp <= ULONG_MAX) {
                f_offset_is_set = 1;
                strcpy(filereloff, input);
                printf("Ok \"%s\"\n", input);
            }
            else {
                f_offset_is_set = 0;
                printf("Fail \"%s\", should be between %d and %lu\n",
                       input, 0, ULONG_MAX);
            }
        }
        else {
            f_offset_is_set = 0;
            printf("Fail \"%s\", should be a number\n", input);
        }
    }

    retval = f_filename_is_set && f_offset_is_set;
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

/* str_isdigit: check string for digit content
                1 - all chars are digits
                0 - non-digit char found */
int str_isdigit(const char *s)
{
    while (isdigit(*s))
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
