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

#ifndef WRITE_OPTIONS_H
#define WRITE_OPTIONS_H

#include <stdio.h>
#include <string.h>

#define WRITE_OPTIONS_MAXPATH 1000  /* maximum path string option length */

enum write_cipher_type {
    W_CIPHER_XOR,
    W_CIPHER_NONE
};

struct write_options {
    char src[WRITE_OPTIONS_MAXPATH];
    char dst[WRITE_OPTIONS_MAXPATH];
    size_t offset;
    enum write_cipher_type cipher;
};

int write_options_init(
    struct write_options *opts,
    const char *src, const char *dst,
    const char *offset, const char *cipher);
void write_options_clear(struct write_options *opts);
char *write_options_tostr_source(struct write_options *opts, char out[]);
char *write_options_tostr_destination(struct write_options *opts, char out[]);
char *write_options_tostr_offset(struct write_options *opts, char out[]);
char *write_options_tostr_cipher(struct write_options *opts, char out[]);

#endif
