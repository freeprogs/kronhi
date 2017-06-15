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

#include "node.h"

/* node_write_dir: write directory node to output stream
                   return 1 if has written correctly
                   return 0 if errors happened */
int node_write_dir(FILE *ofp, const struct bindir *dir)
{
    int f_error;

    f_error = 0;

    if (!binfield_raw_write(dir->type_sign, ofp))
        f_error = 1;
    if(!binfield_num_write(dir->descsize, ofp))
        f_error = 1;
    if(!binfield_raw_write(dir->desc, ofp))
        f_error = 1;
    if(!binfield_num_write(dir->num_of_files, ofp))
        f_error = 1;
    if(!binfield_num_write(dir->file_offset, ofp))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}

/* node_test_isdir: test if there is a directory node in input stream
                    return 1 if there is a directory node
                    return 0 if an error happened */
int node_test_isdir(FILE *ifp)
{
    fpos_t pos;
    struct field_raw *f;
    int retval;

    fgetpos(ifp, &pos);
    f = binfield_raw_create(1);
    binfield_raw_read(ifp, f, 1);
    retval = f->val[0] == 'd';
    binfield_raw_free(f);
    fsetpos(ifp, &pos);
    return retval;
}

/* node_read_dir_header: read directory node header from input stream
                         return 1 if has read correctly
                         return 0 if errors happened */
int node_read_dir_header(FILE *ifp, struct bindir *dir)
{
    unsigned short descsize;
    int f_error;

    f_error = 0;

    if (!binfield_raw_read(ifp, dir->type_sign, 1))
        f_error = 1;
    if (!binfield_num_read(ifp, dir->descsize, 2))
        f_error = 1;
    if (!bindir_descsize_get(dir, &descsize))
        f_error = 1;
    if (!binfield_raw_read(ifp, dir->desc, descsize))
        f_error = 1;
    if (!binfield_num_read(ifp, dir->num_of_files, 4))
        f_error = 1;
    if (!binfield_num_read(ifp, dir->file_offset, 4))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ifp))
        return 0;
    return 1;
}

/* node_write_file: write file node to output stream
                    return 1 if has written correctly
                    return 0 if errors happened */
int node_write_file(FILE *ofp, const struct binfile *file)
{
    int f_error, f_file_to_file_error;

    f_error = f_file_to_file_error = 0;

    if (!binfield_raw_write(file->type_sign, ofp))
        f_error = 1;
    if (!binfield_num_write(file->namesize, ofp))
        f_error = 1;
    if (!binfield_raw_write(file->name, ofp))
        f_error = 1;
    if (!binfield_num_write(file->descsize, ofp))
        f_error = 1;
    if (!binfield_raw_write(file->desc, ofp))
        f_error = 1;
    if (!binfield_raw_write(file->datetime, ofp))
        f_error = 1;
    if (!binfield_num_write(file->ctrlsum, ofp))
        f_error = 1;
    if (!binfield_raw_write(file->contentsize, ofp))
        f_error = 1;
    if (!file_write_file(ofp, file->contentstream))
        f_file_to_file_error = 1;
    if (!binfield_num_write(file->file_offset, ofp))
        f_error = 1;

    if (f_error || f_file_to_file_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}

/* node_test_isfile: test if there is a file node in input stream
                     return 1 if there is a file node
                     return 0 if an error happened */
int node_test_isfile(FILE *ifp)
{
    fpos_t pos;
    struct field_raw *f;
    int retval;

    fgetpos(ifp, &pos);
    f = binfield_raw_create(1);
    binfield_raw_read(ifp, f, 1);
    retval = f->val[0] == 'f';
    binfield_raw_free(f);
    fsetpos(ifp, &pos);
    return retval;
}

/* node_read_file_header: read file node header from input stream
                          return 1 if has read correctly
                          return 0 if errors happened */
int node_read_file_header(FILE *ifp, struct binfile *file)
{
    unsigned char namesize;
    unsigned short descsize;
    char buffer[100], *p = buffer;
    struct bignumber contentsize, i;
    int c;
    int f_error;

    f_error = 0;

    if (!binfield_raw_read(ifp, file->type_sign, 1))
        f_error = 1;
    if (!binfield_num_read(ifp, file->namesize, 1))
        f_error = 1;
    if (!binfile_namesize_get(file, &namesize))
        f_error = 1;
    if (!binfield_raw_read(ifp, file->name, namesize))
        f_error = 1;
    if (!binfield_num_read(ifp, file->descsize, 2))
        f_error = 1;
    if (!binfile_descsize_get(file, &descsize))
        f_error = 1;
    if (!binfield_raw_read(ifp, file->desc, descsize))
        f_error = 1;
    if (!binfield_raw_read(ifp, file->datetime, 14))
        f_error = 1;
    if (!binfield_num_read(ifp, file->ctrlsum, 4))
        f_error = 1;

    for (p = buffer; (c = getc(ifp)) != EOF; p++) {
        *p = c;
        if (c == '\0')
            break;
    }
    if (c == EOF)
        return 0;

    if (!binfile_contentsize_set(file, buffer))
        f_error = 1;

    bignumber_set_value_string(&contentsize, buffer);
    bignumber_set_value_int(&i, 0);
    while (bignumber_lt_big(&i, &contentsize)) {
        c = getc(ifp);
        bignumber_add_int(&i, 1);
    }
    if (c == EOF)
        return 0;

    if (!binfield_num_read(ifp, file->file_offset, 4))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ifp))
        return 0;
    return 1;
}
