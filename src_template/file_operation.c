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

/* file_test_size:
   tests whether stream has space for given size
   return 1 if has enough space
   return 0 if has no enough space */
int file_test_size(FILE *fp, size_t size)
{
    int retval;
    fpos_t savepos;

    fgetpos(fp, &savepos);
    while (size > LONG_MAX) {
        fseek(fp, LONG_MAX, SEEK_CUR);
        size -= LONG_MAX;
    }
    if (size > 0) {
        fseek(fp, size, SEEK_CUR);
    }
    retval = feof(fp) == 0;
    fsetpos(fp, &savepos);
    return retval;
}
