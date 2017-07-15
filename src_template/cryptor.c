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

#include "cryptor.h"

/* cryptor_start: start cryptor and set internal values */
void cryptor_start(
    struct cryptor *self,
    enum cryptor_algorithm algorithm,
    const unsigned char *password,
    size_t password_length)
{
    self->algo = algorithm;
    self->psw = password;
    self->len = password_length;
    self->pos = 0;
}

/* cryptor_algo_get: get encryption algorithm
                     return 1 if algorithm has gotten correctly
                     return 0 if an error happened */
int cryptor_algo_get(
    struct cryptor *self,
    enum cryptor_algorithm *out)
{
    *out = self->algo;
    return 1;
}

/* cryptor_pos_set: set password position (from 0) cyclically
                    return 1 if position set correctly
                    return 0 if an error happened */
int cryptor_pos_set(struct cryptor *self, size_t pos)
{
    self->pos = pos % self->len;
    return 1;
}

/* cryptor_pos_get: get password position cyclically
                    return 1 if position has gotten correctly
                    return 0 if an error happened */
int cryptor_pos_get(struct cryptor *self, size_t *out)
{
    *out = self->pos;
    return 1;
}

/* cryptor_pos_rshift: shift password position cyclically to the right
                       return 1 if position has shifted correctly
                       return 0 if an error happened */
int cryptor_pos_rshift(struct cryptor *self, size_t n)
{
    return 1;
}

/* cryptor_encrypt:
   encrypt input bytes of input size to output bytes of output size
   return 1 if bytes have encrypted correctly
   return 0 if an error happened */
int cryptor_encrypt(
    struct cryptor *self,
    unsigned char *ibytes,
    size_t ilen,
    unsigned char *obytes,
    size_t *olen)
{
    return 1;
}

/* cryptor_decrypt:
   decrypt input bytes of input size to output bytes of output size
   return 1 if bytes have decrypted correctly
   return 0 if an error happened */
int cryptor_decrypt(
    struct cryptor *self,
    unsigned char *ibytes,
    size_t ilen,
    unsigned char *obytes,
    size_t *olen)
{
    return 1;
}

/* cryptor_end: stop cryptor and clear internal values */
void cryptor_end(struct cryptor *self)
{
    self->algo = CRYPTOR_ALGORITHM_NONE;
    self->psw = NULL;
    self->len = 0;
    self->pos = 0;
}
