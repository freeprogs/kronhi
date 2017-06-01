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

#ifndef FILE_OFFSET_H
#define FILE_OFFSET_H

#include <stdio.h>
#include "bignumber.h"

struct file_offset {
    struct bignumber number;
};

int fileoffset_fromstring(struct file_offset *offset, const char *str);
void fileoffset_clear(struct file_offset *offset);
char *fileoffset_tostr(const struct file_offset *offset, char out[]);
int fileoffset_lt(const struct file_offset *offset_left,
                  const struct file_offset *offset_right);
void fileoffset_inc1(struct file_offset *offset);
int fileoffset_add_number(struct file_offset *file_offset, size_t number);

#endif
