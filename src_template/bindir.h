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

#ifndef BINDIR_H
#define BINDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "endian.h"

/*
 * struct bindir contents:
 * type_sign     --  the sign of the directory header
 * descsize      --  the directory description size
 * descp         --  the directory description itself
 * num_of_files  --  the number of files in the directory
 * file_offset   --  the offset of the first file in the directory
 *                   this offset is relative to its place in memory
*/
struct bindir {
    unsigned char type_sign;
    unsigned short descsize;
    unsigned char *descp;
    unsigned long num_of_files;
    size_t file_offset;
};

struct bindir *bindir_create(void);
int bindir_desc_set(struct bindir *dir, const char *dirdesc);
void bindir_num_of_files_set(struct bindir *dir, unsigned long num);
void bindir_file_offset_set(struct bindir *dir, size_t offset);
void bindir_print(struct bindir *dir);
void bindir_free(struct bindir *dir);
size_t bindir_make_bin_header(struct bindir *dir, unsigned char dirheader[]);

#endif
