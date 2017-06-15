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
    struct field_raw *rp;
    struct field_num *np;

    rp = binfield_raw_create(_TYPE_SIGN_FIELD_SIZE);
    if (!rp)
        return 0;
    file->type_sign = rp;
    binfield_raw_set(rp, "\0", 1);

    np = binfield_num_create(_NAMESIZE_FIELD_SIZE);
    if (!np)
        return 0;
    file->namesize = np;
    binfield_num_set(np, "\0", 1);

    rp = binfield_raw_create(_NAME_FIELD_SIZE);
    if (!rp)
        return 0;
    file->name = rp;
    binfield_raw_set(rp, "\0", 1);

    np = binfield_num_create(_DESCSIZE_FIELD_SIZE);
    if (!np)
        return 0;
    file->descsize = np;
    binfield_num_set(np, "\0", 1);

    rp = binfield_raw_create(_DESC_FIELD_SIZE);
    if (!rp)
        return 0;
    file->desc = rp;
    binfield_raw_set(rp, "\0", 1);

    rp = binfield_raw_create(_DATETIME_FIELD_SIZE);
    if (!rp)
        return 0;
    file->datetime = rp;
    binfield_raw_set(rp, "\0", 1);

    np = binfield_num_create(_CTRLSUM_FIELD_SIZE);
    if (!np)
        return 0;
    file->ctrlsum = np;
    binfield_num_set(np, "\0", 1);

    rp = binfield_raw_create(_CONTENTSIZE_FIELD_SIZE);
    if (!rp)
        return 0;
    file->contentsize = rp;
    binfield_raw_set(rp, "\0", 1);

    file->contentstream = NULL;

    np = binfield_num_create(_FILE_OFFSET_FIELD_SIZE);
    if (!np)
        return 0;
    file->file_offset = np;
    binfield_num_set(np, "\0", 1);

    return 1;
}

/* binfile_type_set: set in file the type sign field
                     return 1 if field has set
                     return 0 if an error happened */
int binfile_type_set(struct binfile *file, char type)
{
    return binfield_raw_set(file->type_sign,
                            &type,
                            sizeof type);
}

/* binfile_namesize_set: set in file the name size field
                         return 1 if field has set
                         return 0 if an error happened */
int binfile_namesize_set(struct binfile *file, unsigned short filenamesize)
{
    return binfield_num_set(file->namesize,
                            &filenamesize,
                            sizeof filenamesize);
}

/* binfile_namesize_get: get from file the name size field
                         return 1 if field has gotten
                         return 0 if an error happened */
int binfile_namesize_get(struct binfile *file, unsigned char *out)
{
    return binfield_num_get(file->namesize, out);
}

/* binfile_name_set: set in file the name field
                     return 1 if field has set
                     return 0 if an error happened */
int binfile_name_set(struct binfile *file, const char *filename)
{
    return binfield_raw_set(file->name,
                            filename,
                            strlen(filename));
}

/* binfile_descsize_set: set in file the description size field
                         return 1 if field has set
                         return 0 if an error happened */
int binfile_descsize_set(struct binfile *file, unsigned short filedescsize)
{
    return binfield_num_set(file->descsize,
                            &filedescsize,
                            sizeof filedescsize);
}

/* binfile_desc_set: set in file the description field
                     return 1 if field has set
                     return 0 if an error happened */
int binfile_desc_set(struct binfile *file, const char *filedesc)
{
    return binfield_raw_set(file->desc,
                            filedesc,
                            strlen(filedesc));
}

/* binfile_datetime_set: set in file the datetime field
                         return 1 if field has set
                         return 0 if an error happened */
int binfile_datetime_set(struct binfile *file, const char *datetime)
{
    return binfield_raw_set(file->datetime,
                            datetime,
                            strlen(datetime));
}

/* binfile_ctrlsum_set: set in file the control sum field
                        return 1 if field has set
                        return 0 if an error happened */
int binfile_ctrlsum_set(struct binfile *file, unsigned long ctrlsum)
{
    return binfield_num_set(file->ctrlsum,
                            &ctrlsum,
                            sizeof ctrlsum);
}

/* binfile_contentsize_set: set in file the content size field
                            return 1 if field has set
                            return 0 if an error happened */
int binfile_contentsize_set(struct binfile *file, const char *contentsize)
{
    return binfield_raw_set(file->contentsize,
                            contentsize,
                            strlen(contentsize) + 1);
}

/* binfile_contentstream_set: set in file the content stream
                              return 1 if field has set
                              return 0 if an error happened */
int binfile_contentstream_set(struct binfile *file, FILE *contentstream)
{
    file->contentstream = contentstream;
    return 1;
}

/* binfile_file_offset_set: set in file the file offset field
                            return 1 if field has set
                            return 0 if an error happened */
int binfile_file_offset_set(struct binfile *file, size_t file_offset)
{
    return binfield_num_set(file->file_offset,
                            &file_offset,
                            sizeof file_offset);
}

/* binfile_file_offset_get: get from file the file offset field
                            return 1 if field has gotten
                            return 0 if an error happened */
int binfile_file_offset_get(const struct binfile *file, size_t *out)
{
    return binfield_num_get(file->file_offset, out);
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
    if(!bignumber_set_value_string(&contentsize, (char *) file->contentsize->val))
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
    binfield_raw_free(file->type_sign);
    binfield_num_free(file->namesize);
    binfield_raw_free(file->name);
    binfield_num_free(file->descsize);
    binfield_raw_free(file->desc);
    binfield_raw_free(file->datetime);
    binfield_num_free(file->ctrlsum);
    binfield_raw_free(file->contentsize);
    binfield_num_free(file->file_offset);
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
}
