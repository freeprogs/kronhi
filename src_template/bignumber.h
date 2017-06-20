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

#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <stdio.h>
#include <string.h>

/* maximum length of the big number as a string */
#define BIG_MAXSTRING  15

struct bignumber {
    double number;
};

int bignumber_set_value_string(struct bignumber *number, const char *str);
int bignumber_set_value_int(struct bignumber *number, int value);
char *bignumber_tostr(const struct bignumber *number, char out[]);
int bignumber_lt_big(const struct bignumber *number_left,
                     const struct bignumber *number_right);
int bignumber_add_int(struct bignumber *number, int value);
int bignumber_add_ulong(struct bignumber *number, size_t value);
int bignumber_add_big(struct bignumber *number, const struct bignumber *value);

#endif
