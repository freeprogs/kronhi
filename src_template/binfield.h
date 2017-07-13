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
#include "bignumber.h"
#include "file_operation.h"
#include "cryptor.h"

/* maximum number buffer length for changing endianness */
#define NUMBUFMAX  10

struct binfield {
    struct cryptor *cryptor;
};

struct binfield_raw {
    unsigned char *val;
    size_t len;
    size_t maxsize;
};

struct binfield_num {
    unsigned char *val;
    size_t len;
    size_t maxsize;
};

struct binfield_stream {
    FILE *valfp;
    struct bignumber len;
};

void binfield_start(
    struct binfield *self,
    struct cryptor *cryptor);
struct binfield_raw *binfield_raw_create(
    struct binfield *self,
    size_t size);
struct binfield_num *binfield_num_create(
    struct binfield *self,
    size_t size);
struct binfield_stream *binfield_stream_create(
    struct binfield *self);
int binfield_raw_set(
    struct binfield *self,
    struct binfield_raw *field,
    const void *value,
    size_t length);
int binfield_raw_get(
    struct binfield *self,
    const struct binfield_raw *field,
    void *out);
int binfield_num_set(
    struct binfield *self,
    struct binfield_num *field,
    const void *value,
    size_t length);
int binfield_num_get(
    struct binfield *self,
    const struct binfield_num *field,
    void *out);
int binfield_stream_set(
    struct binfield *self,
    struct binfield_stream *field,
    FILE *iofp);
int binfield_stream_get(
    struct binfield *self,
    const struct binfield_stream *field,
    FILE **out);
int binfield_raw_read(
    struct binfield *self,
    struct binfield_raw *field,
    FILE *ifp,
    size_t size);
int binfield_raw_write(
    struct binfield *self,
    const struct binfield_raw *field,
    FILE *ofp);
int binfield_raw_skip(
    struct binfield *self,
    const struct binfield_raw *field,
    FILE *iofp);
int binfield_num_read(
    struct binfield *self,
    struct binfield_num *field,
    FILE *ifp,
    size_t size);
int binfield_num_write(
    struct binfield *self,
    const struct binfield_num *field,
    FILE *ofp);
int binfield_num_skip(
    struct binfield *self,
    const struct binfield_num *field,
    FILE *iofp);
int binfield_stream_write(
    struct binfield *self,
    const struct binfield_stream *field,
    FILE *ofp);
int binfield_stream_skip(
    struct binfield *self,
    const struct binfield_stream *field,
    FILE *iofp);
void binfield_raw_free(
    struct binfield *self,
    struct binfield_raw *field);
void binfield_num_free(
    struct binfield *self,
    struct binfield_num *field);
void binfield_stream_free(
    struct binfield *self,
    struct binfield_stream *field);
void binfield_end(struct binfield *self);

#endif
