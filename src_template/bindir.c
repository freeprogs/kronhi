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
    struct binfield field;
    struct binfield_raw *rp;
    struct binfield_num *np;

    binfield_start(&field, NULL);

    rp = binfield_raw_create(&field, _TYPE_SIGN_FIELD_SIZE);
    if (!rp)
        return 0;
    dir->type_sign = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    np = binfield_num_create(&field, _DESCSIZE_FIELD_SIZE);
    if (!np)
        return 0;
    dir->descsize = np;
    binfield_num_set(&field, np, "\0", 1);

    rp = binfield_raw_create(&field, _DESC_FIELD_SIZE);
    if (!rp)
        return 0;
    dir->desc = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    np = binfield_num_create(&field, _NUM_OF_FILES_FIELD_SIZE);
    if (!np)
        return 0;
    dir->num_of_files = np;
    binfield_num_set(&field, np, "\0", 1);

    np = binfield_num_create(&field, _FILE_OFFSET_FIELD_SIZE);
    if (!np)
        return 0;
    dir->file_offset = np;
    binfield_num_set(&field, np, "\0", 1);

    binfield_end(&field);

    return 1;
}

/* bindir_type_set: set in directory the type sign field
                    return 1 if field has set
                    return 0 if an error happened */
int bindir_type_set(struct bindir *dir, char type)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        dir->type_sign,
        &type,
        _TYPE_SIGN_FIELD_SIZE);
    binfield_end(&field);
    return retval;
}

/* bindir_type_get: get from directory the type sign field
                    return 1 if field has gotten
                    return 0 if an error happened */
int bindir_type_get(const struct bindir *dir, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, dir->type_sign, out);
    binfield_end(&field);
    return retval;
}

/* bindir_descsize_set: set in directory the description size field
                        return 1 if field has set
                        return 0 if an error happened */
int bindir_descsize_set(struct bindir *dir, unsigned short dirdescsize)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        dir->descsize,
        &dirdescsize,
        _DESCSIZE_FIELD_SIZE);
    binfield_end(&field);
    return retval;
}

/* bindir_descsize_get: get from directory the description size field
                            return 1 if field has gotten
                            return 0 if an error happened */
int bindir_descsize_get(const struct bindir *dir, unsigned short *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_get(&field, dir->descsize, out);
    binfield_end(&field);
    return retval;
}

/* bindir_desc_set: set in directory the description field
                    return 1 if field has set
                    return 0 if an error happened */
int bindir_desc_set(struct bindir *dir, const char *dirdesc)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        dir->desc,
        dirdesc,
        strlen(dirdesc));
    binfield_end(&field);
    return retval;
}

/* bindir_desc_get: get from directory the description field
                    return 1 if field has gotten
                    return 0 if an error happened */
int bindir_desc_get(const struct bindir *dir, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, dir->desc, out);
    binfield_end(&field);
    return retval;
}

/* bindir_num_of_files_set: set in directory the number of files field
                            return 1 if field has set
                            return 0 if an error happened */
int bindir_num_of_files_set(struct bindir *dir, size_t num_of_files)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        dir->num_of_files,
        &num_of_files,
        _NUM_OF_FILES_FIELD_SIZE);
    binfield_end(&field);
    return retval;
}

/* bindir_num_of_files_get: get from directory the number of files field
                            return 1 if field has gotten
                            return 0 if an error happened */
int bindir_num_of_files_get(const struct bindir *dir, size_t *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    *out = 0;
    retval = binfield_num_get(&field, dir->num_of_files, out);
    binfield_end(&field);
    return retval;
}

/* bindir_file_offset_set: set in directory the relative file offset field
                           return 1 if field has set
                           return 0 if an error happened */
int bindir_file_offset_set(struct bindir *dir, size_t file_offset)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        dir->file_offset,
        &file_offset,
        _FILE_OFFSET_FIELD_SIZE);
    binfield_end(&field);
    return retval;
}

/* bindir_file_offset_get: get from directory the relative file offset field
                           return 1 if field has gotten
                           return 0 if an error happened */
int bindir_file_offset_get(const struct bindir *dir, size_t *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    *out = 0;
    retval = binfield_num_get(&field, dir->file_offset, out);
    binfield_end(&field);
    return retval;
}

/* bindir_get_size: get directory fields total size
                    return 1 if size has gotten
                    return 0 if an error happened */
int bindir_get_size(const struct bindir *dir, size_t *out)
{
    size_t size;

    size = dir->type_sign->len
        + dir->descsize->len
        + dir->desc->len
        + dir->num_of_files->len
        + dir->file_offset->len;
    *out = size;
    return 1;
}

/* bindir_end: delete directory fields and fill it by zeros */
void bindir_end(struct bindir *dir)
{
    struct binfield field;

    binfield_start(&field, NULL);

    binfield_raw_free(&field, dir->type_sign);
    binfield_num_free(&field, dir->descsize);
    binfield_raw_free(&field, dir->desc);
    binfield_num_free(&field, dir->num_of_files);
    binfield_num_free(&field, dir->file_offset);
    dir->type_sign = NULL;
    dir->descsize = NULL;
    dir->desc = NULL;
    dir->num_of_files = NULL;
    dir->file_offset = NULL;

    binfield_end(&field);
}
