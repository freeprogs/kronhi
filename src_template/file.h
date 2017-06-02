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

#ifndef FILE_H
#define FILE_H

#include <string.h>

/* maximum length of file name */
#define FILE_MAXFILENAME  255

/* maximum length of file description */
#define FILE_MAXDESCRIPTION  65535

struct file {
    char filename[FILE_MAXFILENAME];
    char description[FILE_MAXDESCRIPTION];
};

void file_filename_set(struct file *file, const char *s);
char *file_filename_get(const struct file *file, char out[]);
void file_description_set(struct file *file, const char *s);
char *file_description_get(const struct file *file, char out[]);

#endif
