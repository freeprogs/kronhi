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

#ifndef CMDSHELL_H
#define CMDSHELL_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include "input.h"
#include "write_options.h"
#include "read_options.h"
#include "bignumber.h"

/* maximum command shell input line length */
#define CMDSHELL_MAXINPUT      1000

/* maximum command shell input text length */
#define CMDSHELL_MAXTEXTINPUT  65536

/* count number of elements of an array */
#define ARRAY_SIZE(array) (sizeof (array) / sizeof (array)[0])

enum cmdshell_code {
    CMD_INIT_WRITE,
    CMD_INIT_WRITE_DIR,
    CMD_INIT_WRITE_FILE,
    CMD_INIT_READ,
    CMD_STATUS_WRITE,
    CMD_STATUS_WRITE_DIR,
    CMD_STATUS_WRITE_FILE,
    CMD_STATUS_READ,
    CMD_WRITE_DIR,
    CMD_WRITE_FILE,
    CMD_HELP,
    CMD_QUIT,
    CMD_UNKNOWN
};

enum cmdshell_dir_code {
    CMD_DIR_INTER,
    CMD_DIR_FILE,
    CMD_DIR_NOOP,
    CMD_DIR_UNKNOWN
};

void cmdshell_start(void);
void cmdshell_print_message(const char *fmt, ...);
enum cmdshell_code
cmdshell_prompt_command(const char *prompt, char in[], int maxsize);
void cmdshell_print_error(const char *fmt, ...);
void cmdshell_print_help(void);
int cmdshell_init_write(
    char src[], char dst[], char offset[], char cipher[]);
int cmdshell_init_read(
    char src[], char dst[], char offset[], char cipher[]);
void cmdshell_print_status_write(
    const char *wsrc, const char *wdst,
    const char *woffset, const char *wcipher);
void cmdshell_print_status_write_dir(const char *desc);
void cmdshell_print_status_write_file(const char *filename, const char *filedesc);
void cmdshell_print_status_read(
    const char *rsrc, const char *rdst,
    const char *roffset, const char *rcipher);
enum cmdshell_dir_code
cmdshell_init_write_dir(char descinter[], char descfile[]);
int cmdshell_init_write_file(
    char filename[], char filedesc[], char filereloff[]);
void cmdshell_end(void);

#endif
