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

#include "hexdump.h"

/* hexdump_dump: prints bytes from buffer to the standard output */
void hexdump_dump(unsigned char *buf, size_t bufsize)
{
    size_t i, j;

    for (i = 0, j = 0; bufsize > 0; bufsize--, i++, buf++) {
        if (i % 16 == 0) {
            printf("%08X ", i / 16 * 16);
        }
        if (j == 8) {
            printf(" ");
        }
        printf(" %02x", *buf);
        j++;
        if (j % 16 == 0) {
            int c_i;
            unsigned char *p = buf - 16 + 1;
            printf("  |");
            for (c_i = 0; c_i < j; c_i++) {
                if (c_i == 8) {
                    printf(" ");
                }
                if (isprint(*p)) {
                    printf("%c", *p);
                }
                else {
                    printf(".");
                }
                p++;
            }
            printf("|");
            printf("\n");
            j = 0;
        }
    }
    if (j > 0) {
        int skip_i;
        int c_i;
        unsigned char *p = buf - j;

        if (j <= 8) {
            printf(" ");
        }
        for (skip_i = j; skip_i < 16; skip_i++) {
            printf("   ");
        }
        printf("  |");
        for (c_i = 0; c_i < j; c_i++) {
            if (c_i == 8) {
                printf(" ");
            }
            if (isprint(*p)) {
                printf("%c", *p);
            }
            else {
                printf(".");
            }
            p++;
        }
        printf("|");
    }
    printf("\n");
}
