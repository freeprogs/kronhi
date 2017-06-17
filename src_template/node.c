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
    if (!binfield_num_write(dir->descsize, ofp))
        f_error = 1;
    if (!binfield_raw_write(dir->desc, ofp))
        f_error = 1;
    if (!binfield_num_write(dir->num_of_files, ofp))
        f_error = 1;
    if (!binfield_num_write(dir->file_offset, ofp))
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
    binfield_raw_read(f, ifp, 1);
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

    if (!binfield_raw_read(dir->type_sign, ifp, 1))
        f_error = 1;
    if (!binfield_num_read(dir->descsize, ifp, 2))
        f_error = 1;
    if (!bindir_descsize_get(dir, &descsize))
        f_error = 1;
    if (!binfield_raw_read(dir->desc, ifp, descsize))
        f_error = 1;
    if (!binfield_num_read(dir->num_of_files, ifp, 4))
        f_error = 1;
    if (!binfield_num_read(dir->file_offset, ifp, 4))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ifp))
        return 0;
    return 1;
}

/* node_write_dir_header_field:
   write directory node header field(s) to output stream
   field flags may be conjugated by bitwise OR;
   return 1 if fields have written and skipped correctly
   return 0 if errors happened */
int node_write_dir_header_field(
    FILE *ofp,
    const struct bindir *dir,
    enum dir_field_flags fieldflags)
{
    int f_error;

    f_error = 0;

    if (fieldflags & DIRFLD_TYPESIGN) {
        if (!binfield_raw_write(dir->type_sign, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(dir->type_sign, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_DESCSIZE) {
        if (!binfield_num_write(dir->descsize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(dir->descsize, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_DESC) {
        if (!binfield_raw_write(dir->desc, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(dir->desc, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_NUMOFFILES) {
        if (!binfield_num_write(dir->num_of_files, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(dir->num_of_files, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_FILEOFFSET) {
        if (!binfield_num_write(dir->file_offset, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(dir->file_offset, ofp))
            f_error = 1;
    }

    if (f_error)
        return 0;
    if (ferror(ofp))
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
    binfield_raw_read(f, ifp, 1);
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

    if (!binfield_raw_read(file->type_sign, ifp, 1))
        f_error = 1;
    if (!binfield_num_read(file->namesize, ifp, 1))
        f_error = 1;
    if (!binfile_namesize_get(file, &namesize))
        f_error = 1;
    if (!binfield_raw_read(file->name, ifp, namesize))
        f_error = 1;
    if (!binfield_num_read(file->descsize, ifp, 2))
        f_error = 1;
    if (!binfile_descsize_get(file, &descsize))
        f_error = 1;
    if (!binfield_raw_read(file->desc, ifp, descsize))
        f_error = 1;
    if (!binfield_raw_read(file->datetime, ifp, 14))
        f_error = 1;
    if (!binfield_num_read(file->ctrlsum, ifp, 4))
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

    if (!binfield_num_read(file->file_offset, ifp, 4))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ifp))
        return 0;
    return 1;
}

/* node_write_file_header_field:
   write file node header field(s) to output stream
   field flags may be conjugated by bitwise OR;
   return 1 if fields have written and skipped correctly
   return 0 if errors happened */
int node_write_file_header_field(
    FILE *ofp,
    const struct binfile *file,
    enum file_field_flags fieldflags)
{
    int f_error, f_file_to_file_error, f_file_skip_error;

    f_error = f_file_to_file_error = f_file_skip_error = 0;

    if (fieldflags & FILFLD_TYPESIGN) {
        if (!binfield_raw_write(file->type_sign, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(file->type_sign, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_NAMESIZE) {
        if (!binfield_num_write(file->namesize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(file->namesize, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_NAME) {
        if (!binfield_raw_write(file->name, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(file->name, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_DESCSIZE) {
        if (!binfield_num_write(file->descsize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(file->descsize, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_DESC) {
        if (!binfield_raw_write(file->desc, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(file->desc, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_DATETIME) {
        if (!binfield_raw_write(file->datetime, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(file->datetime, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_CTRLSUM) {
        if (!binfield_num_write(file->ctrlsum, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(file->ctrlsum, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_CONTENTSIZE) {
        if (!binfield_raw_write(file->contentsize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(file->contentsize, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_CONTENTSTREAM) {
        if (!file_write_file(ofp, file->contentstream))
            f_file_to_file_error = 1;
    }
    else {
        struct bignumber contentsize;
        char buffer[100];

        if (!binfile_contentsize_get(file, buffer))
            f_file_skip_error = 1;
        if (!bignumber_set_value_string(&contentsize, buffer))
            f_file_skip_error = 1;
        if (!file_skip_bytes(ofp, &contentsize))
            f_file_skip_error = 1;
    }

    if (fieldflags & FILFLD_FILEOFFSET) {
        if (!binfield_num_write(file->file_offset, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(file->file_offset, ofp))
            f_error = 1;
    }

    if (f_error || f_file_skip_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}
