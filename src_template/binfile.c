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

#include "binfile.h"

/* binfile_start: create file fields and fill them by zeros
                  return 1 if file created and filled
                  return 0 if an error happened */
int binfile_start(struct binfile *file)
{
    struct binfield field;
    struct binfield_raw *rp;
    struct binfield_num *np;
    struct binfield_stream *sp;

    binfield_start(&field, NULL);

    rp = binfield_raw_create(&field, _TYPE_SIGN_FIELD_SIZE);
    if (!rp)
        return 0;
    file->type_sign = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    np = binfield_num_create(&field, _NAMESIZE_FIELD_SIZE);
    if (!np)
        return 0;
    file->namesize = np;
    binfield_num_set(&field, np, "\0", 1);

    rp = binfield_raw_create(&field, _NAME_FIELD_SIZE);
    if (!rp)
        return 0;
    file->name = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    np = binfield_num_create(&field, _DESCSIZE_FIELD_SIZE);
    if (!np)
        return 0;
    file->descsize = np;
    binfield_num_set(&field, np, "\0", 1);

    rp = binfield_raw_create(&field, _DESC_FIELD_SIZE);
    if (!rp)
        return 0;
    file->desc = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    rp = binfield_raw_create(&field, _DATETIME_FIELD_SIZE);
    if (!rp)
        return 0;
    file->datetime = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    np = binfield_num_create(&field, _CTRLSUM_FIELD_SIZE);
    if (!np)
        return 0;
    file->ctrlsum = np;
    binfield_num_set(&field, np, "\0", 1);

    rp = binfield_raw_create(&field, _CONTENTSIZE_FIELD_SIZE);
    if (!rp)
        return 0;
    file->contentsize = rp;
    binfield_raw_set(&field, rp, "\0", 1);

    sp = binfield_stream_create(&field);
    if (!sp)
        return 0;
    file->contentstream = sp;
    binfield_stream_set(&field, sp, NULL);

    np = binfield_num_create(&field, _FILE_OFFSET_FIELD_SIZE);
    if (!np)
        return 0;
    file->file_offset = np;
    binfield_num_set(&field, np, "\0", 1);

    binfield_end(&field);

    return 1;
}

/* binfile_type_set: set in file the type sign field
                     return 1 if field has set
                     return 0 if an error happened */
int binfile_type_set(struct binfile *file, char type)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        file->type_sign,
        &type,
        sizeof type);
    binfield_end(&field);
    return retval;
}

/* binfile_type_get: get from file the type sign field
                     return 1 if field has gotten
                     return 0 if an error happened */
int binfile_type_get(const struct binfile *file, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, file->type_sign, out);
    binfield_end(&field);
    return retval;
}

/* binfile_namesize_set: set in file the name size field
                         return 1 if field has set
                         return 0 if an error happened */
int binfile_namesize_set(struct binfile *file, unsigned char filenamesize)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        file->namesize,
        &filenamesize,
        sizeof filenamesize);
    binfield_end(&field);
    return retval;
}

/* binfile_namesize_get: get from file the name size field
                         return 1 if field has gotten
                         return 0 if an error happened */
int binfile_namesize_get(struct binfile *file, unsigned char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_get(&field, file->namesize, out);
    binfield_end(&field);
    return retval;
}

/* binfile_name_set: set in file the name field
                     return 1 if field has set
                     return 0 if an error happened */
int binfile_name_set(struct binfile *file, const char *filename)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        file->name,
        filename,
        strlen(filename));
    binfield_end(&field);
    return retval;
}

/* binfile_name_get: get from file the name field
                     return 1 if field has gotten
                     return 0 if an error happened */
int binfile_name_get(const struct binfile *file, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, file->name, out);
    binfield_end(&field);
    return retval;
}

/* binfile_descsize_set: set in file the description size field
                         return 1 if field has set
                         return 0 if an error happened */
int binfile_descsize_set(struct binfile *file, unsigned short filedescsize)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        file->descsize,
        &filedescsize,
        sizeof filedescsize);
    binfield_end(&field);
    return retval;
}

/* binfile_descsize_get: get from file the description size field
                         return 1 if field has gotten
                         return 0 if an error happened */
int binfile_descsize_get(struct binfile *file, unsigned short *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_get(&field, file->descsize, out);
    binfield_end(&field);
    return retval;
}

/* binfile_desc_set: set in file the description field
                     return 1 if field has set
                     return 0 if an error happened */
int binfile_desc_set(struct binfile *file, const char *filedesc)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        file->desc,
        filedesc,
        strlen(filedesc));
    binfield_end(&field);
    return retval;
}

/* binfile_desc_get: get from file the description field
                     return 1 if field has gotten
                     return 0 if an error happened */
int binfile_desc_get(const struct binfile *file, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, file->desc, out);
    binfield_end(&field);
    return retval;
}

/* binfile_datetime_set: set in file the datetime field
                         return 1 if field has set
                         return 0 if an error happened */
int binfile_datetime_set(struct binfile *file, const char *datetime)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        file->datetime,
        datetime,
        strlen(datetime));
    binfield_end(&field);
    return retval;
}

/* binfile_datetime_get: get from file the datetime field
                         return 1 if field has gotten
                         return 0 if an error happened */
int binfile_datetime_get(const struct binfile *file, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, file->datetime, out);
    binfield_end(&field);
    return retval;
}

/* binfile_ctrlsum_set: set in file the control sum field
                        return 1 if field has set
                        return 0 if an error happened */
int binfile_ctrlsum_set(struct binfile *file, unsigned long ctrlsum)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        file->ctrlsum,
        &ctrlsum,
        sizeof ctrlsum);
    binfield_end(&field);
    return retval;
}

/* binfile_ctrlsum_get: get from file the control sum field
                        return 1 if field has gotten
                        return 0 if an error happened */
int binfile_ctrlsum_get(const struct binfile *file, unsigned long *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_get(&field, file->ctrlsum, out);
    binfield_end(&field);
    return retval;
}

/* binfile_contentsize_set: set in file the content size field
                            return 1 if field has set
                            return 0 if an error happened */
int binfile_contentsize_set(struct binfile *file, const char *contentsize)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_set(
        &field,
        file->contentsize,
        contentsize,
        strlen(contentsize) + 1);
    binfield_end(&field);
    return retval;
}

/* binfile_contentsize_get: get from file the content size field
                            return 1 if field has gotten
                            return 0 if an error happened */
int binfile_contentsize_get(const struct binfile *file, char *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_raw_get(&field, file->contentsize, out);
    binfield_end(&field);
    return retval;
}

/* binfile_contentstream_set: set in file the content stream field
                              return 1 if field has set
                              return 0 if an error happened */
int binfile_contentstream_set(struct binfile *file, FILE *contentstream)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_stream_set(
        &field,
        file->contentstream,
        contentstream);
    binfield_end(&field);
    return retval;
}

/* binfile_contentstream_get: get from file the content stream field
                              return 1 if field has gotten
                              return 0 if an error happened */
int binfile_contentstream_get(const struct binfile *file, FILE **out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_stream_get(&field, file->contentstream, out);
    binfield_end(&field);
    return retval;
}

/* binfile_file_offset_set: set in file the file offset field
                            return 1 if field has set
                            return 0 if an error happened */
int binfile_file_offset_set(struct binfile *file, size_t file_offset)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_set(
        &field,
        file->file_offset,
        &file_offset,
        sizeof file_offset);
    binfield_end(&field);
    return retval;
}

/* binfile_file_offset_get: get from file the file offset field
                            return 1 if field has gotten
                            return 0 if an error happened */
int binfile_file_offset_get(const struct binfile *file, size_t *out)
{
    struct binfield field;
    int retval;

    binfield_start(&field, NULL);
    retval = binfield_num_get(&field, file->file_offset, out);
    binfield_end(&field);
    return retval;
}

/* binfile_get_size: get file fields total size */
int binfile_get_size(const struct binfile *file, struct bignumber *out)
{
    struct bignumber tmp, contentsize;
    int f_error;

    f_error = 0;

    if (!bignumber_set_value_int(&tmp, 0))
        f_error = 1;
    if (!bignumber_add_ulong(&tmp, file->type_sign->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->namesize->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->name->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->descsize->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->desc->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->datetime->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->ctrlsum->len))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->contentsize->len))
        f_error = 1;
    if (!bignumber_set_value_int(&contentsize, 0))
        f_error = 1;
    if(!bignumber_set_value_string(&contentsize,
                                   (char *) file->contentsize->val))
        f_error = 1;
    if(!bignumber_add_big(&tmp, &contentsize))
        f_error = 1;
    if(!bignumber_add_ulong(&tmp, file->file_offset->len))
        f_error = 1;
    if (!f_error) {
        *out = tmp;
        return 1;
    }
    return 0;
}

/* binfile_end: delete file fields and fill it by zeros */
void binfile_end(struct binfile *file)
{
    struct binfield field;

    binfield_start(&field, NULL);

    binfield_raw_free(&field, file->type_sign);
    binfield_num_free(&field, file->namesize);
    binfield_raw_free(&field, file->name);
    binfield_num_free(&field, file->descsize);
    binfield_raw_free(&field, file->desc);
    binfield_raw_free(&field, file->datetime);
    binfield_num_free(&field, file->ctrlsum);
    binfield_raw_free(&field, file->contentsize);
    binfield_stream_free(&field, file->contentstream);
    binfield_num_free(&field, file->file_offset);
    file->type_sign = NULL;
    file->namesize = NULL;
    file->name = NULL;
    file->descsize = NULL;
    file->desc = NULL;
    file->datetime = NULL;
    file->ctrlsum = NULL;
    file->contentsize = NULL;
    file->contentstream = NULL;
    file->file_offset = NULL;

    binfield_end(&field);
}
