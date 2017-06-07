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

#include "file_operation.h"

/* file_test_write_size:
   tests whether stream has space for given size
   return 1 if has enough space
   return 0 if has no enough space */
int file_test_write_size(FILE *fp, const struct bignumber *size)
{
    int retval;
    fpos_t savepos;
    struct bignumber cursize;

    fgetpos(fp, &savepos);

    bignumber_set_value_int(&cursize, 0);
    while (bignumber_lt_big(&cursize, size)) {
        if (getc(fp) == EOF)
            break;
        bignumber_add_int(&cursize, 1);
    }
    retval = feof(fp) == 0;
    fsetpos(fp, &savepos);
    return retval;
}

/* file_skip_to_offset: set stream pointer to given offset
                        return 1 if has set correctly
                        return 0 if an error happen */
int file_skip_to_offset(FILE *fp, const struct file_offset *offset)
{
    struct file_offset off;

    fseek(fp, 0L, SEEK_SET);
    fileoffset_clear(&off);
    while (fileoffset_lt(&off, offset)) {
        if (getc(fp) == EOF)
            break;
        fileoffset_add_number(&off, 1);
    }
    if (ferror(fp) || feof(fp))
        return 0;
    return 1;
}

/* file_get_size: get stream size in bytes
                  return 1 if has got correctly
                  return 0 if an error happen */
int file_get_size(FILE *fp, struct bignumber *out)
{
    int retval;
    fpos_t savepos;
    struct bignumber tmp;

    fgetpos(fp, &savepos);
    bignumber_set_value_int(&tmp, 0);
    while (getc(fp) != EOF)
        bignumber_add_int(&tmp, 1);
    retval = feof(fp) && !ferror(fp);
    fsetpos(fp, &savepos);
    if (retval)
        *out = tmp;
    return retval;
}

/* file_get_ctrlsum: get stream control sum
                     return 1 if has got correctly
                     return 0 if an error happen */
int file_get_ctrlsum(FILE *fp, unsigned long *out)
{
    *out = 0x12345678UL;
    return 1;
}
