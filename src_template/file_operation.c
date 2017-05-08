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

/* file_test_exists: test whether file exists
                     return 1 if exists
                     return 0 if doesn't exist */
int file_test_exists(const char *path)
{
    FILE *fp;

    if ((fp = fopen(path, "r")) == NULL)
        return 0;
    fclose(fp);
    return 1;
}

/* file_test_write_perm: test whether file has write permission
                         return 1 if has write permission
                         return 0 if has no write permission */
int file_test_write_perm(const char *path)
{
    FILE *fp;

    if ((fp = fopen(path, "r")) == NULL)
        return 0;
    fclose(fp);
    if ((fp = fopen(path, "r+")) == NULL)
        return 0;
    fclose(fp);
    return 1;
}

/* file_test_size:
   tests whether file has space after offset for given size
   return 1 if has enough space
   return 0 if has no enough space */
int file_test_size(const char *path, size_t offset, size_t datasize)
{
    FILE *fp;

    if ((fp = fopen(path, "r")) == NULL)
        return 0;
    for ( ; offset > 0; offset--) {
        if (getc(fp) == EOF) {
            fclose(fp);
            return 0;
        }
    }
    for ( ; datasize > 0; datasize--) {
        if (getc(fp) == EOF) {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

/* file_write: write to file after offset data of given size
               return 1 if has written right
               return 0 if some error has happen
               (open file, too big offset, stream error) */
int file_write(const char *path, size_t offset, void *data, size_t datasize)
{
    FILE *fp;
    int f_waserror;

    if ((fp = fopen(path, "r+b")) == NULL)
        return 0;
    for ( ; offset > 0; offset--) {
        if (getc(fp) == EOF) {
            fclose(fp);
            return 0;
        }
    }
    while (datasize > 0) {
        size_t blocksize;
        if (datasize > W_BLOCK_SIZE)
            blocksize = W_BLOCK_SIZE;
        else
            blocksize = datasize;
        if (fwrite(data, blocksize, 1, fp) == 0) {
            fclose(fp);
            return 0;
        }
        data = (unsigned char *) data + blocksize;
        datasize -= blocksize;
    }
    f_waserror = ferror(fp) != 0;
    fclose(fp);
    return !f_waserror;
}
