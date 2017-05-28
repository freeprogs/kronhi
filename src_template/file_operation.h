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

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

/* Linux definition for opening big files (greater 2Gb)
   (Must be placed before stdio.h inclusion.) */
#define _FILE_OFFSET_BITS  64

#include <stdio.h>
#include <limits.h>

/* Block size for writings */
#define W_BLOCK_SIZE  8192

int file_test_size(FILE *fp, size_t size);

#endif
