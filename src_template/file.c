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

#include "file.h"

/* file_filename_set: set file name for file */
void file_filename_set(struct file *file, const char *s)
{
    strcpy(file->filename, s);
}

/* file_filename_get: get file name of file */
char *file_filename_get(const struct file *file, char out[])
{
    return strcpy(out, file->filename);
}

/* file_description_set: set description for file */
void file_description_set(struct file *file, const char *s)
{
    strcpy(file->description, s);
}

/* file_description_get: get description of file */
char *file_description_get(const struct file *file, char out[])
{
    return strcpy(out, file->description);
}

/* file_relative_offset_set: set relative offset for file */
void file_relative_offset_set(struct file *file, size_t offset)
{
    file->relative_offset = offset;
}

/* file_relative_offset_get: get relative offset of file */
size_t file_relative_offset_get(const struct file *file)
{
    return file->relative_offset;
}
