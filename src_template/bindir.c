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

#include "bindir.h"

/* bindir_start: create directory fields and fill them by zeros
                 return 1 if directory created and filled
                 return 0 if an error happened */
int bindir_start(struct bindir *dir)
{
    struct field_raw *rp;
    struct field_num *np;

    rp = binfield_raw_create(1);
    if (!rp)
        return 0;
    dir->type_sign = rp;
    binfield_raw_set(rp, "\0", 1);

    np = binfield_num_create(2);
    if (!np)
        return 0;
    dir->descsize = np;
    binfield_num_set(np, "\0", 1);

    rp = binfield_raw_create(65535);
    if (!rp)
        return 0;
    dir->desc = rp;
    binfield_raw_set(rp, "\0", 1);

    np = binfield_num_create(4);
    if (!np)
        return 0;
    dir->num_of_files = np;
    binfield_num_set(np, "\0", 1);

    np = binfield_num_create(4);
    if (!np)
        return 0;
    dir->file_offset = np;
    binfield_num_set(np, "\0", 1);

    return 1;
}

/* bindir_type_set: set in directory the type sign field
                    return 1 if field has set
                    return 0 if an error happened */
int bindir_type_set(struct bindir *dir, char type)
{
    return binfield_raw_set(dir->type_sign,
                            &type,
                            sizeof type);
}

/* bindir_descsize_set: set in directory the description size field
                        return 1 if field has set
                        return 0 if an error happened */
int bindir_descsize_set(struct bindir *dir, unsigned short dirdescsize)
{
    return binfield_num_set(dir->descsize,
                            &dirdescsize,
                            sizeof dirdescsize);
}

/* bindir_desc_set: set in directory the description field
                    return 1 if field has set
                    return 0 if an error happened */
int bindir_desc_set(struct bindir *dir, const char *dirdesc)
{
    return binfield_raw_set(dir->desc,
                            dirdesc,
                            strlen(dirdesc));
}

/* bindir_num_of_files_set: set in directory the number of files field
                            return 1 if field has set
                            return 0 if an error happened */
int bindir_num_of_files_set(struct bindir *dir, size_t num_of_files)
{
    return binfield_num_set(dir->num_of_files,
                            &num_of_files,
                            sizeof num_of_files);
}

/* bindir_file_offset_set: set in directory the relative file offset field
                           return 1 if field has set
                           return 0 if an error happened */
int bindir_file_offset_set(struct bindir *dir, size_t file_offset)
{
    return binfield_num_set(dir->file_offset,
                            &file_offset,
                            sizeof file_offset);
}

/* bindir_end: delete directory fields and fill it by zeros */
void bindir_end(struct bindir *dir)
{
    binfield_raw_free(dir->type_sign);
    binfield_num_free(dir->descsize);
    binfield_raw_free(dir->desc);
    binfield_num_free(dir->num_of_files);
    binfield_num_free(dir->file_offset);
    dir->type_sign =
        dir->descsize =
        dir->desc =
        dir->num_of_files =
        dir->file_offset = NULL;
}
