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

#include "endian.h"

/* bytes_to_bigend: convert bytes to big-endian order
                    return converted bytes */
void *bytes_to_bigend(void *bytes, size_t size)
{
    if (is_little_endian())
        bytes_reverse(bytes, size);
    return bytes;
}

/* bytes_to_litend: convert bytes to little-endian order
                    return converted bytes */
void *bytes_to_litend(void *bytes, size_t size)
{
    if (is_big_endian())
        bytes_reverse(bytes, size);
    return bytes;
}

/* is_little_endian: determines architecture endianness
                     return 1 if little-endian
                     return 0 if not little-endian */
int is_little_endian(void)
{
    unsigned short n = 1;

    return *((unsigned char *) &n) != 0;
}

/* is_big_endian: determines architecture endianness
                  return 1 if big-endian
                  return 0 if not big-endian */
int is_big_endian(void)
{
    unsigned short n = 1;

    return *((unsigned char *) &n) != 256;
}

/* bytes_reverse: reverse bytes sequence
                  return reversed bytes */
void *bytes_reverse(void *bytes, size_t size)
{
    unsigned char *p, *q;
    unsigned char c;

    for (p = bytes, q = (unsigned char *) bytes + size - 1; p < q; p++, q--)
        c = *p, *p = *q, *q = c;
    return bytes;
}
