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

#ifndef READ_OPTIONS_H
#define READ_OPTIONS_H

#include <stdio.h>
#include <string.h>
#include "file_offset.h"

#define READ_OPTIONS_MAXPATH 1000  /* maximum path string option length */

enum read_cipher_type {
    R_CIPHER_XOR,
    R_CIPHER_NONE
};

struct read_options {
    char src[READ_OPTIONS_MAXPATH];
    char dst[READ_OPTIONS_MAXPATH];
    struct file_offset offset;
    enum read_cipher_type cipher;
};

int read_options_init(
    struct read_options *opts,
    const char *src, const char *dst,
    const char *offset, const char *cipher);
void read_options_clear(struct read_options *opts);
char *read_options_tostr_source(struct read_options *opts, char out[]);
char *read_options_tostr_destination(struct read_options *opts, char out[]);
char *read_options_tostr_offset(struct read_options *opts, char out[]);
char *read_options_tostr_cipher(struct read_options *opts, char out[]);

#endif
