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

#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <stdio.h>

enum cryptor_algorithm {
    CRYPTOR_ALGORITHM_XOR,
    CRYPTOR_ALGORITHM_NONE
};

struct cryptor {
    enum cryptor_algorithm algo;
    const unsigned char *psw;
    size_t len;
    size_t pos;
};

void cryptor_start(
    struct cryptor *self,
    enum cryptor_algorithm algorithm,
    const unsigned char *password,
    size_t password_length);
int cryptor_algo_get(
    struct cryptor *self,
    enum cryptor_algorithm *out);
int cryptor_pos_set(struct cryptor *self, size_t pos);
int cryptor_pos_get(struct cryptor *self, size_t *out);
int cryptor_pos_rshift(struct cryptor *self, size_t n);
int cryptor_encrypt(
    struct cryptor *self,
    unsigned char *ibytes,
    size_t ilen,
    unsigned char *obytes,
    size_t *olen);
int cryptor_decrypt(
    struct cryptor *self,
    unsigned char *ibytes,
    size_t ilen,
    unsigned char *obytes,
    size_t *olen);
void cryptor_end(struct cryptor *self);

#endif
