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
    return sscanf(str, "%lu", (unsigned long *) &offset->offset);
}

/* fileoffset_clear: clear offset (set to zero) */
void fileoffset_clear(struct file_offset *offset)
{
    offset->offset = 0UL;
}

/* fileoffset_tostr: convert offset to string
                     return output string */
char *fileoffset_tostr(const struct file_offset *offset, char out[])
{
    sprintf(out, "%lu", (unsigned long) offset->offset);
    return out;
}

/* fileoffset_lt:
   compare if left offset is less than right offset
   return 1 if left offset less than right offset
   return 0 if left offset greater or equal to right offset */
int fileoffset_lt(const struct file_offset *offset_left,
                  const struct file_offset *offset_right)
{
    return offset_left->offset < offset_right->offset;
}

/* fileoffset_inc1: increment offset to 1 */
void fileoffset_inc1(struct file_offset *offset)
{
    if (offset->offset < 4294967295UL)
        offset->offset++;
}
