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

#ifndef BINFIELD_H
#define BINFIELD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "endian.h"

/* maximum number buffer length for changing endianness */
#define NUMBUFMAX  10

struct field_raw {
    unsigned char *val;
    size_t len;
    size_t maxsize;
};

struct field_num {
    unsigned char *val;
    size_t len;
    size_t maxsize;
};

struct field_raw *binfield_raw_create(size_t size);
struct field_num *binfield_num_create(size_t size);
int binfield_raw_set(struct field_raw *field, const void *value, size_t length);
int binfield_num_set(struct field_num *field, const void *value, size_t length);
int binfield_num_get(const struct field_num *field, void *out);
int binfield_raw_read(FILE *ifp, struct field_raw *field, size_t size);
int binfield_raw_write(const struct field_raw *field, FILE *ofp);
int binfield_num_read(FILE *ifp, struct field_num *field, size_t size);
int binfield_num_write(const struct field_num *field, FILE *ofp);
void binfield_raw_free(struct field_raw *field);
void binfield_num_free(struct field_num *field);

#endif
