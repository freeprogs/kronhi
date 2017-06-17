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
int file_test_write_size(FILE *iofp, const struct bignumber *size)
{
    int retval;
    fpos_t savepos;
    struct bignumber cursize;

    fgetpos(iofp, &savepos);

    bignumber_set_value_int(&cursize, 0);
    while (bignumber_lt_big(&cursize, size)) {
        if (getc(iofp) == EOF)
            break;
        bignumber_add_int(&cursize, 1);
    }
    retval = feof(iofp) == 0;
    fsetpos(iofp, &savepos);
    return retval;
}

/* file_skip_to_offset: set stream pointer to given offset
                        return 1 if has set correctly
                        return 0 if an error happen */
int file_skip_to_offset(FILE *iofp, const struct file_offset *offset)
{
    struct file_offset off;

    fseek(iofp, 0L, SEEK_SET);
    fileoffset_clear(&off);
    while (fileoffset_lt(&off, offset)) {
        if (getc(iofp) == EOF)
            break;
        fileoffset_add_number(&off, 1);
    }
    if (ferror(iofp) || feof(iofp))
        return 0;
    return 1;
}

/* file_get_size: get stream size in bytes
                  return 1 if has got correctly
                  return 0 if an error happen */
int file_get_size(FILE *iofp, struct bignumber *out)
{
    int retval;
    fpos_t savepos;
    struct bignumber tmp;

    fgetpos(iofp, &savepos);
    bignumber_set_value_int(&tmp, 0);
    while (getc(iofp) != EOF)
        bignumber_add_int(&tmp, 1);
    retval = feof(iofp) && !ferror(iofp);
    fsetpos(iofp, &savepos);
    if (retval)
        *out = tmp;
    return retval;
}

/* file_get_ctrlsum: get stream control sum
                     return 1 if has got correctly
                     return 0 if an error happen */
int file_get_ctrlsum(FILE *ifp, unsigned long *out)
{
    int retval;
    fpos_t savepos;
    unsigned long sum;

    fgetpos(ifp, &savepos);
    sum = crc32stream(ifp);
    retval = !ferror(ifp);
    fsetpos(ifp, &savepos);
    if (retval)
        *out = sum;
    return retval;
}

/* file_write_file: write to stream another stream
                    return 1 if has written correctly
                    return 0 if an error happen */
int file_write_file(FILE *ofp, FILE *ifp)
{
    int retval;
    char buffer[W_BLOCK_SIZE];
    size_t n;

    while ((n = fread(buffer, 1, W_BLOCK_SIZE, ifp)) > 0) {
        if (fwrite(buffer, 1, n, ofp) != n)
            break;
    }
    retval = !ferror(ifp) && feof(ifp) && !ferror(ofp);
    return retval;
}

/* file_skip_bytes: skip given number of bytes in stream
                    return 1 if has skipped correctly
                    return 0 if an error happen */
int file_skip_bytes(FILE *iofp, const struct bignumber *count)
{
    int retval;
    struct bignumber i;

    bignumber_set_value_int(&i, 0);
    while (bignumber_lt_big(&i, count)) {
        if (getc(iofp) == EOF)
            break;
        bignumber_add_int(&i, 1);
    }
    retval = ferror(iofp) == 0;
    return retval;
}
