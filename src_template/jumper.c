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

#include "jumper.h"

/* jumper_dir_jump_file_offset:
   jump to relative file offset of directory in the stream
   return 1 if has jumped correctly
   return 0 if an error happened */
int jumper_dir_jump_file_offset(FILE *iofp, size_t offset)
{
    int retval;

    while (offset-- > 0)
        getc(iofp);
    retval = ferror(iofp) == 0;
    return retval;
}

/* jumper_file_jump_file_offset:
   jump to relative file offset of file in the stream
   return 1 if has jumped correctly
   return 0 if an error happened */
int jumper_file_jump_file_offset(FILE *iofp, size_t offset)
{
    int retval;

    while (offset-- > 0)
        getc(iofp);
    retval = ferror(iofp) == 0;
    return retval;
}
