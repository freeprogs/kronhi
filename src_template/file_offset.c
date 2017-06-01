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

#include "file_offset.h"

/* fileoffset_fromstring: make offset from string
                          return 1 if string is right
                          return 0 if string is incorrect */
int fileoffset_fromstring(struct file_offset *offset, const char *str)
{
    if (bignumber_set_value_string(&offset->number, str))
        return 1;
    else
        return 0;
}

/* fileoffset_clear: clear offset (set to zero) */
void fileoffset_clear(struct file_offset *offset)
{
    bignumber_set_value_int(&offset->number, 0);
}

/* fileoffset_tostr: convert offset to string
                     return output string */
char *fileoffset_tostr(const struct file_offset *offset, char out[])
{
    return bignumber_tostr(&offset->number, out);
}

/* fileoffset_lt:
   compare if left offset is less than right offset
   return 1 if left offset less than right offset
   return 0 if left offset greater or equal to right offset */
int fileoffset_lt(const struct file_offset *offset_left,
                  const struct file_offset *offset_right)
{
    return bignumber_lt_big(&offset_left->number, &offset_right->number);
}

/* fileoffset_inc1: increment offset to 1 */
void fileoffset_inc1(struct file_offset *offset)
{
    bignumber_add_int(&offset->number, 1);
}

/* fileoffset_add_number: add number to offset
                          return 1 if added with no errors
                          return 0 if an error happened */
int fileoffset_add_number(struct file_offset *offset, size_t number)
{
    return bignumber_add_ulong(&offset->number, number);
}
