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

/* bindir_create: create directory in memory with zeroed fields */
struct bindir *bindir_create(void)
{
    struct bindir *p;

    p = malloc(sizeof(struct bindir));
    if (p != NULL) {
        p->type_sign = 'd';
        p->descsize = 0;
        p->descp = NULL;
        p->num_of_files = 0;
        p->file_offset = 0;
    }
    return p;
}

/* bindir_desc_set:
   set in directory the description field and description size field
   return 1 if description successfully allocated in memory
   return 0 if not enough memory */
int bindir_desc_set(struct bindir *dir, char *dirdesc)
{
    size_t len;
    char *p;

    if (dir->descp != NULL) {
        free(dir->descp);
        dir->descsize = 0;
        dir->descp = NULL;
    }
    len = strlen(dirdesc);
    p = malloc(len);
    if (p != NULL) {
        strncpy(p, dirdesc, len);
        dir->descp = (unsigned char *) p;
        dir->descsize = len;
    }
    return dir->descp != NULL;
}

/* bindir_num_of_files_set: set in directory the number of files field */
void bindir_num_of_files_set(struct bindir *dir, unsigned long num)
{
    dir->num_of_files = num;
}

/* bindir_file_offset_set: set in directory the file offset field */
void bindir_file_offset_set(struct bindir *dir, size_t offset)
{
    dir->file_offset = offset;
}

/* bindir_print: print directory fields to the standard output */
void bindir_print(struct bindir *dir)
{
    printf(
        "Directory:\n"
        "type:         %c\n"
        "descsize:     %hu\n"
        "descp:\n%.*s\n"
        "num_of_files: %lu\n"
        "file_offset:  %lu\n",
        dir->type_sign, dir->descsize,
        (int) dir->descsize, dir->descp,
        dir->num_of_files,
        (unsigned long) dir->file_offset);
}

/* bindir_free: free memory of directory itself and directory fields */
void bindir_free(struct bindir *dir)
{
    free(dir->descp);
    free(dir);
}

/* bindir_make_bin_header: make continuous binary header from directory fields
                           return the size of written header */
size_t bindir_make_bin_header(struct bindir *dir, unsigned char dirheader[])
{
    size_t i;

    i = 0;
    dirheader[0] = dir->type_sign;
    i += 1;
    memcpy(dirheader + i, &dir->descsize, sizeof dir->descsize);
    i += sizeof dir->descsize;
    memcpy(dirheader + i, dir->descp, dir->descsize);
    i += dir->descsize;
    memcpy(dirheader + i, &dir->num_of_files, sizeof dir->num_of_files);
    i += sizeof dir->num_of_files;
    memcpy(dirheader + i, &dir->file_offset, sizeof dir->file_offset);
    i += sizeof dir->file_offset;
    return i;
}
