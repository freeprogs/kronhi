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

#include "binfield.h"

/* sizes of bindir fields for memory allocation */
#define _TYPE_SIGN_FIELD_SIZE     1
#define _DESCSIZE_FIELD_SIZE      2
#define _DESC_FIELD_SIZE          65535
#define _NUM_OF_FILES_FIELD_SIZE  4
#define _FILE_OFFSET_FIELD_SIZE   4

/*
 * struct bindir contents:
 * type_sign     --  the sign of the directory header
 * descsize      --  the directory description size
 * desc          --  the directory description itself
 * num_of_files  --  the number of files in the directory
 * file_offset   --  the offset of the first file in the directory
 *                   this offset is relative to its place in memory
*/
struct bindir {
    struct field_raw *type_sign;
    struct field_num *descsize;
    struct field_raw *desc;
    struct field_num *num_of_files;
    struct field_num *file_offset;
};

int bindir_start(struct bindir *dir);
int bindir_type_set(struct bindir *dir, char type);
int bindir_descsize_set(struct bindir *dir, unsigned short dirdescsize);
int bindir_desc_set(struct bindir *dir, const char *dirdesc);
int bindir_num_of_files_set(struct bindir *dir, size_t num_of_files);
int bindir_num_of_files_get(const struct bindir *dir, size_t *out);
int bindir_file_offset_set(struct bindir *dir, size_t file_offset);
int bindir_file_offset_get(const struct bindir *dir, size_t *out);
size_t bindir_get_size(const struct bindir *dir);
void bindir_end(struct bindir *dir);

#endif
