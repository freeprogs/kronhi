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

/* node_start: start node and set internal values */
void node_start(struct node *self, struct binfield *field)
{
    self->field = field;
}

/* node_state_get: get internal state of the node
                   return 1 if has gotten correctly
                   return 0 if errors happened */
int node_state_get(struct node *self, struct node_state *out)
{
    if (self->field->cryptor != NULL) {
        out->has_cryptor = 1;
        cryptor_pos_get(
            self->field->cryptor,
            &out->cryptor_password_position);
    }
    else {
        out->has_cryptor = 0;
        out->cryptor_password_position = 0;
    }
    return 1;
}

/* node_state_set: set internal state of the node
                   return 1 if has set correctly
                   return 0 if errors happened */
int node_state_set(struct node *self, const struct node_state *state)
{
    if (self->field->cryptor != NULL) {
        if (state->has_cryptor) {
            return cryptor_pos_set(
                self->field->cryptor,
                state->cryptor_password_position);
        }
        else {
            return 0;
        }
    }
    return 1;
}

/* node_write_dir: write directory node to output stream
                   return 1 if has written correctly
                   return 0 if errors happened */
int node_write_dir(struct node *self, FILE *ofp, const struct bindir *dir)
{
    struct binfield *field;
    int f_error;

    field = self->field;
    f_error = 0;

    if (!binfield_raw_write(field, dir->type_sign, ofp))
        f_error = 1;
    if (!binfield_num_write(field, dir->descsize, ofp))
        f_error = 1;
    if (!binfield_raw_write(field, dir->desc, ofp))
        f_error = 1;
    if (!binfield_num_write(field, dir->num_of_files, ofp))
        f_error = 1;
    if (!binfield_num_write(field, dir->file_offset, ofp))
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
int node_test_isdir(struct node *self, FILE *ifp)
{
    struct binfield *field;
    fpos_t pos;
    struct binfield_raw *f;
    int retval;

    field = self->field;

    fgetpos(ifp, &pos);
    f = binfield_raw_create(field, 1);
    if (field->cryptor == NULL) {
        binfield_raw_read(field, f, ifp, 1);
    }
    else {
        size_t oldpos;
        cryptor_pos_get(field->cryptor, &oldpos);
        binfield_raw_read(field, f, ifp, 1);
        cryptor_pos_set(field->cryptor, oldpos);
    }
    retval = f->val[0] == 'd';
    binfield_raw_free(field, f);
    fsetpos(ifp, &pos);
    return retval;
}

/* node_read_dir_header: read directory node header from input stream
                         return 1 if has read correctly
                         return 0 if errors happened */
int node_read_dir_header(struct node *self, FILE *ifp, struct bindir *dir)
{
    struct binfield *field;
    unsigned short descsize;
    int f_error;

    field = self->field;
    f_error = 0;

    if (!binfield_raw_read(field, dir->type_sign, ifp, 1))
        f_error = 1;
    if (!binfield_num_read(field, dir->descsize, ifp, 2))
        f_error = 1;
    if (!bindir_descsize_get(dir, &descsize))
        f_error = 1;
    if (!binfield_raw_read(field, dir->desc, ifp, descsize))
        f_error = 1;
    if (!binfield_num_read(field, dir->num_of_files, ifp, 4))
        f_error = 1;
    if (!binfield_num_read(field, dir->file_offset, ifp, 4))
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
    struct node *self,
    FILE *ofp,
    const struct bindir *dir,
    enum node_dir_field_flags fieldflags)
{
    struct binfield *field;
    int f_error;

    field = self->field;
    f_error = 0;

    if (fieldflags & DIRFLD_TYPESIGN) {
        if (!binfield_raw_write(field, dir->type_sign, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, dir->type_sign, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_DESCSIZE) {
        if (!binfield_num_write(field, dir->descsize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, dir->descsize, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_DESC) {
        if (!binfield_raw_write(field, dir->desc, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, dir->desc, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_NUMOFFILES) {
        if (!binfield_num_write(field, dir->num_of_files, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, dir->num_of_files, ofp))
            f_error = 1;
    }

    if (fieldflags & DIRFLD_FILEOFFSET) {
        if (!binfield_num_write(field, dir->file_offset, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, dir->file_offset, ofp))
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
int node_write_file(struct node *self, FILE *ofp, const struct binfile *file)
{
    struct binfield *field;
    int f_error;

    field = self->field;
    f_error = 0;

    if (!binfield_raw_write(field, file->type_sign, ofp))
        f_error = 1;
    if (!binfield_num_write(field, file->namesize, ofp))
        f_error = 1;
    if (!binfield_raw_write(field, file->name, ofp))
        f_error = 1;
    if (!binfield_num_write(field, file->descsize, ofp))
        f_error = 1;
    if (!binfield_raw_write(field, file->desc, ofp))
        f_error = 1;
    if (!binfield_raw_write(field, file->datetime, ofp))
        f_error = 1;
    if (!binfield_num_write(field, file->ctrlsum, ofp))
        f_error = 1;
    if (!binfield_raw_write(field, file->contentsize, ofp))
        f_error = 1;
    if (!binfield_stream_write(field, file->contentstream, ofp))
        f_error = 1;
    if (!binfield_num_write(field, file->file_offset, ofp))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}

/* node_test_isfile: test if there is a file node in input stream
                     return 1 if there is a file node
                     return 0 if an error happened */
int node_test_isfile(struct node *self, FILE *ifp)
{
    struct binfield *field;
    fpos_t pos;
    struct binfield_raw *f;
    int retval;

    field = self->field;

    fgetpos(ifp, &pos);
    f = binfield_raw_create(field, 1);
    binfield_raw_read(field, f, ifp, 1);
    retval = f->val[0] == 'f';
    binfield_raw_free(field, f);
    fsetpos(ifp, &pos);
    return retval;
}

/* node_read_file_header: read file node header from input stream
                          return 1 if has read correctly
                          return 0 if errors happened */
int node_read_file_header(struct node *self, FILE *ifp, struct binfile *file)
{
    struct binfield *field;
    unsigned char namesize;
    unsigned short descsize;
    char buffer[100], *p = buffer;
    struct bignumber contentsize, i;
    int c;
    int f_error;

    field = self->field;
    f_error = 0;

    if (!binfield_raw_read(field, file->type_sign, ifp, 1))
        f_error = 1;
    if (!binfield_num_read(field, file->namesize, ifp, 1))
        f_error = 1;
    if (!binfile_namesize_get(file, &namesize))
        f_error = 1;
    if (!binfield_raw_read(field, file->name, ifp, namesize))
        f_error = 1;
    if (!binfield_num_read(field, file->descsize, ifp, 2))
        f_error = 1;
    if (!binfile_descsize_get(file, &descsize))
        f_error = 1;
    if (!binfield_raw_read(field, file->desc, ifp, descsize))
        f_error = 1;
    if (!binfield_raw_read(field, file->datetime, ifp, 14))
        f_error = 1;
    if (!binfield_num_read(field, file->ctrlsum, ifp, 4))
        f_error = 1;

    for (p = buffer; (c = getc(ifp)) != EOF; p++) {
        if (field->cryptor != NULL) {
            unsigned char ch = c;
            size_t ps;
            cryptor_decrypt(field->cryptor, &ch, 1, &ch, &ps);
            c = ch;
        }
        *p = c;
        if (c == '\0')
            break;
    }
    if (c == EOF)
        return 0;

    if (!binfile_contentsize_set(file, buffer))
        f_error = 1;

    if (!binfile_contentstream_set(file, NULL))
        f_error = 1;

    bignumber_set_value_string(&contentsize, buffer);
    bignumber_set_value_int(&i, 0);
    while (bignumber_lt_big(&i, &contentsize)) {
        c = getc(ifp);
        if (field->cryptor != NULL) {
            cryptor_pos_rshift(field->cryptor, 1);
        }
        bignumber_add_int(&i, 1);
    }
    if (c == EOF)
        return 0;

    if (!binfield_num_read(field, file->file_offset, ifp, 4))
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
    struct node *self,
    FILE *ofp,
    const struct binfile *file,
    enum node_file_field_flags fieldflags)
{
    struct binfield *field;
    int f_error;

    field = self->field;
    f_error = 0;

    if (fieldflags & FILFLD_TYPESIGN) {
        if (!binfield_raw_write(field, file->type_sign, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, file->type_sign, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_NAMESIZE) {
        if (!binfield_num_write(field, file->namesize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, file->namesize, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_NAME) {
        if (!binfield_raw_write(field, file->name, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, file->name, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_DESCSIZE) {
        if (!binfield_num_write(field, file->descsize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, file->descsize, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_DESC) {
        if (!binfield_raw_write(field, file->desc, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, file->desc, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_DATETIME) {
        if (!binfield_raw_write(field, file->datetime, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, file->datetime, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_CTRLSUM) {
        if (!binfield_num_write(field, file->ctrlsum, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, file->ctrlsum, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_CONTENTSIZE) {
        if (!binfield_raw_write(field, file->contentsize, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_raw_skip(field, file->contentsize, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_CONTENTSTREAM) {
        if (!binfield_stream_write(field, file->contentstream, ofp))
            f_error = 1;
    }
    else {
        struct bignumber contentsize;
        char buffer[100];

        if (!binfile_contentsize_get(file, buffer))
            f_error = 1;
        if (!bignumber_set_value_string(&contentsize, buffer))
            f_error = 1;
        file->contentstream->len = contentsize;
        if (!binfield_stream_skip(field, file->contentstream, ofp))
            f_error = 1;
    }

    if (fieldflags & FILFLD_FILEOFFSET) {
        if (!binfield_num_write(field, file->file_offset, ofp))
            f_error = 1;
    }
    else {
        if (!binfield_num_skip(field, file->file_offset, ofp))
            f_error = 1;
    }

    if (f_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}

/* node_end: stop node and clear internal values */
void node_end(struct node *self)
{
    self->field = NULL;
}
