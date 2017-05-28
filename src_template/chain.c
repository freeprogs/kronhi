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

#include "chain.h"

/* chain_start: start chain and set internal values */
void chain_start(
    struct chain *self,
    const char *dst,
    const struct file_offset *start)
{
    self->dst = dst;
    self->start = start;
}

/* chain_create_dir:
   make new directory on destination file with start offset
   return chain success code if no errors happened
   return chain error code if an error happened */
enum chain_code chain_create_dir(
    const struct chain *self,
    const char *dirdesc,
    unsigned num_of_files,
    size_t relative_offset)
{
    struct bindir dir;
    FILE *ofp;
    struct bignumber dirsize;

    bindir_start(&dir);
    bindir_type_set(&dir, 'd');
    bindir_descsize_set(&dir, strlen(dirdesc));
    bindir_desc_set(&dir, dirdesc);
    bindir_num_of_files_set(&dir, 0);
    bindir_file_offset_set(&dir, 0);
    ofp = fopen(self->dst, "r+b");
    if (ofp == NULL)
        return CHAIN_ERROR_DIR_OPENFILE;
    if (!file_skip_to_offset(ofp, self->start))
        return CHAIN_ERROR_DIR_SKIPOFFSET;
    if (!bignumber_set_value_int(&dirsize, bindir_get_size(&dir)))
        return CHAIN_ERROR_DIR_FILESIZE;
    if (!file_test_write_size(ofp, &dirsize))
        return CHAIN_ERROR_DIR_FILESIZE;
    if (!node_write_dir(ofp, &dir))
        return CHAIN_ERROR_DIR_WRITENODE;
    if (ferror(ofp))
        return CHAIN_ERROR_DIR_WRITEFILE;
    if (fclose(ofp) != 0)
        return CHAIN_ERROR_DIR_FILESYS;
    bindir_end(&dir);
    return CHAIN_OK;
}

/* chain_end: stop chain and clear internal values */
void chain_end(struct chain *self)
{
    self->dst = NULL;
    self->start = NULL;
}
