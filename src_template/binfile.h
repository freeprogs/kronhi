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

#ifndef BINFILE_H
#define BINFILE_H

#include "binfield.h"
#include "bignumber.h"

/*
 * struct binfile contents:
 * type_sign         --  the sign of the file header
 * namesize          --  the file name size
 * name              --  the file name itself
 * descsize          --  the file description size
 * desc              --  the file description itself
 * datetime          --  the file creation datetime string
 * ctrlsum           --  the control sum of the file contents
 * contentsize       --  the file contents size string
 * contentpath       --  the file contents file path
 * file_offset       --  the offset of the next file in the file list
 *                       this offset is relative to its place in memory
*/
struct binfile {
    struct field_raw *type_sign;
    struct field_num *namesize;
    struct field_raw *name;
    struct field_num *descsize;
    struct field_raw *desc;
    struct field_raw *datetime;
    struct field_num *ctrlsum;
    struct field_raw *contentsize;
    struct field_raw *contentpath;
    struct field_num *file_offset;
};

int binfile_start(struct binfile *file);
int binfile_type_set(struct binfile *file, char type);
int binfile_namesize_set(struct binfile *file, unsigned short filenamesize);
int binfile_name_set(struct binfile *file, const char *filename);
int binfile_descsize_set(struct binfile *file, unsigned short filedescsize);
int binfile_desc_set(struct binfile *file, const char *filedesc);
int binfile_datetime_set(struct binfile *file, const char *datetime);
int binfile_ctrlsum_set(struct binfile *file, unsigned long ctrlsum);
int binfile_contentsize_set(struct binfile *file, const char *contentsize);
int binfile_contentpath_set(struct binfile *file, const char *contentpath);
int binfile_file_offset_set(struct binfile *file, size_t file_offset);
int binfile_get_size(const struct binfile *file, struct bignumber *out);
void binfile_end(struct binfile *file);

#endif
