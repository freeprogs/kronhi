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

struct bindir *bindir_create(void)
{
    printf("bindir_create()\n");
    return NULL;
}

int bindir_desc_set(struct bindir *dir, char *dirdesc)
{
    printf("bindir_desc_set()\n");
}

void bindir_num_of_files_set(struct bindir *dir, unsigned long num)
{
    printf("bindir_num_of_files_set()\n");
}

void bindir_file_offset_set(struct bindir *dir, size_t offset)
{
    printf("bindir_file_offset_set()\n");
}

void bindir_print(struct bindir *dir)
{
    printf("bindir_print()\n");
}

void bindir_free(struct bindir *dir)
{
    printf("bindir_free()\n");
}
