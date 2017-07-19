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

#include "binfield.h"

/* binfield_start: start binfield and set internal values */
void binfield_start(
    struct binfield *self,
    struct cryptor *cryptor)
{
    self->cryptor = cryptor;
}

/* binfield_raw_create:
   allocate a raw field with allocated value of given size
   return pointer to field
   return NULL if can't allocate */
struct binfield_raw *binfield_raw_create(struct binfield *self, size_t size)
{
    struct binfield_raw *p;

    p = malloc(sizeof(struct binfield_raw));
    if (p == NULL)
        return NULL;
    p->val = malloc(size);
    if (p->val == NULL) {
        free(p);
        return NULL;
    }
    p->len = 0;
    p->maxsize = size;
    return p;
}

/* binfield_num_create:
   allocate a number field with allocated value of given size
   return pointer to field
   return NULL if can't allocate */
struct binfield_num *binfield_num_create(struct binfield *self, size_t size)
{
    struct binfield_num *p;

    p = malloc(sizeof(struct binfield_num));
    if (p == NULL)
        return NULL;
    p->val = malloc(size);
    if (p->val == NULL) {
        free(p);
        return NULL;
    }
    p->len = 0;
    p->maxsize = size;
    return p;
}

/* binfield_stream_make: allocate a stream field
   return pointer to field
   return NULL if can't allocate */
struct binfield_stream *binfield_stream_create(struct binfield *self)
{
    struct binfield_stream *p;

    p = malloc(sizeof(struct binfield_stream));
    if (p == NULL)
        return NULL;
    p->valfp = NULL;
    if (!bignumber_set_value_int(&p->len, 0)) {
        free(p);
        return NULL;
    }
    return p;
}

/* binfield_raw_set:
   set raw field value and length to given value and length
   return 1 if has set correctly
   return 0 if can't set */
int binfield_raw_set(
    struct binfield *self,
    struct binfield_raw *field,
    const void *value,
    size_t length)
{
    if (length > field->maxsize)
        return 0;
    memcpy(field->val, value, length);
    field->len = length;
    return 1;
}

/* binfield_raw_get: get raw field value
                     return 1 if has gotten correctly
                     return 0 if can't get */
int binfield_raw_get(
    struct binfield *self,
    const struct binfield_raw *field,
    void *out)
{
    memcpy(out, field->val, field->len);
    return 1;
}

/* binfield_num_set:
   set number field value and length to given value and length
   return 1 if has set correctly
   return 0 if can't set */
int binfield_num_set(
    struct binfield *self,
    struct binfield_num *field,
    const void *value,
    size_t length)
{
    if (length > field->maxsize)
        return 0;
    memcpy(field->val, value, length);
    field->len = length;
    return 1;
}

/* binfield_num_get: get number field value
                     return 1 if has gotten correctly
                     return 0 if can't get */
int binfield_num_get(
    struct binfield *self,
    const struct binfield_num *field,
    void *out)
{
    memcpy(out, field->val, field->len);
    return 1;
}

/* binfield_stream_set:
   set stream field value to given stream and zero length
   return 1 if has set correctly
   return 0 if can't set */
int binfield_stream_set(
    struct binfield *self,
    struct binfield_stream *field,
    FILE *iofp)
{
    field->valfp = iofp;
    if (!bignumber_set_value_int(&field->len, 0))
        return 0;
    return 1;
}

/* binfield_stream_get: get stream field value
                        return 1 if has gotten correctly
                        return 0 if can't get */
int binfield_stream_get(
    struct binfield *self,
    const struct binfield_stream *field,
    FILE **out)
{
    *out = field->valfp;
    return 1;
}

int _binfield_raw_read_plain(
    struct binfield_raw *field,
    FILE *ifp,
    size_t size);
int _binfield_raw_read_crypt(
    struct binfield_raw *field,
    FILE *ifp,
    size_t size,
    struct cryptor *cryptor);

/* binfield_raw_read: read raw field from input stream
                      return 1 if has read correctly
                      return 0 if an error happened */
int binfield_raw_read(
    struct binfield *self,
    struct binfield_raw *field,
    FILE *ifp,
    size_t size)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_raw_read_plain(field, ifp, size);
    }
    else {
        retval = _binfield_raw_read_crypt(field, ifp, size, self->cryptor);
    }
    return retval;
}

int _binfield_raw_read_plain(
    struct binfield_raw *field,
    FILE *ifp,
    size_t size)
{
    int retval;

    if (size > field->maxsize)
        return 0;
    retval = fread(field->val, 1, size, ifp) == size;
    field->len = size;
    return retval;
}

int _binfield_raw_read_crypt(
    struct binfield_raw *field,
    FILE *ifp,
    size_t size,
    struct cryptor *cryptor)
{
    unsigned char ibuffer[CRYPTBUFMAX];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMAX * 2];
    size_t osize;

    if (fread(ibuffer, 1, size, ifp) != size)
        return 0;
    isize = size;
    if (!cryptor_decrypt(cryptor, ibuffer, isize, obuffer, &osize))
        return 0;
    memcpy(field->val, obuffer, osize);
    field->len = osize;
    return 1;
}

int _binfield_raw_write_plain(
    const struct binfield_raw *field,
    FILE *ofp);
int _binfield_raw_write_crypt(
    const struct binfield_raw *field,
    FILE *ofp,
    struct cryptor *cryptor);

/* binfield_raw_write: write raw field to output stream
                       return 1 if has written correctly
                       return 0 if an error happened */
int binfield_raw_write(
    struct binfield *self,
    const struct binfield_raw *field,
    FILE *ofp)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_raw_write_plain(field, ofp);
    }
    else {
        retval = _binfield_raw_write_crypt(field, ofp, self->cryptor);
    }
    return retval;
}

int _binfield_raw_write_plain(
    const struct binfield_raw *field,
    FILE *ofp)
{
    if (field->len > 0)
        return fwrite(field->val, field->len, 1, ofp) == 1;
    return 1;
}

int _binfield_raw_write_crypt(
    const struct binfield_raw *field,
    FILE *ofp,
    struct cryptor *cryptor)
{
    unsigned char ibuffer[CRYPTBUFMAX];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMAX * 2];
    size_t osize;

    if (field->len > 0) {
        memcpy(ibuffer, field->val, field->len);
        isize = field->len;
        if (!cryptor_encrypt(cryptor, ibuffer, isize, obuffer, &osize))
            return 0;
        return fwrite(obuffer, osize, 1, ofp) == 1;
    }
    return 1;
}

int _binfield_raw_skip_plain(
    const struct binfield_raw *field,
    FILE *iofp);
int _binfield_raw_skip_crypt(
    const struct binfield_raw *field,
    FILE *iofp,
    struct cryptor *cryptor);

/* binfield_raw_skip: skip raw field in stream
                      return 1 if has skipped correctly
                      return 0 if an error happened */
int binfield_raw_skip(
    struct binfield *self,
    const struct binfield_raw *field,
    FILE *iofp)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_raw_skip_plain(field, iofp);
    }
    else {
        retval = _binfield_raw_skip_crypt(field, iofp, self->cryptor);
    }
    return retval;
}

int _binfield_raw_skip_plain(
    const struct binfield_raw *field,
    FILE *iofp)
{
    int retval;
    size_t i;

    for (i = 0; i < field->len; i++)
        getc(iofp);
    retval = ferror(iofp) == 0;
    return retval;
}

int _binfield_raw_skip_crypt(
    const struct binfield_raw *field,
    FILE *iofp,
    struct cryptor *cryptor)
{
    unsigned char ibuffer[CRYPTBUFMAX];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMAX * 2];
    size_t osize;
    size_t i;

    memcpy(ibuffer, field->val, field->len);
    isize = field->len;
    if (!cryptor_encrypt(cryptor, ibuffer, isize, obuffer, &osize))
        return 0;
    for (i = 0; i < osize; i++)
        getc(iofp);
    return ferror(iofp) == 0;
}

int _binfield_num_read_plain(
    struct binfield_num *field,
    FILE *ifp,
    size_t size);
int _binfield_num_read_crypt(
    struct binfield_num *field,
    FILE *ifp,
    size_t size,
    struct cryptor *cryptor);

/* binfield_num_read: read number field from input stream
                      return 1 if has read correctly
                      return 0 if an error happened */
int binfield_num_read(
    struct binfield *self,
    struct binfield_num *field,
    FILE *ifp,
    size_t size)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_num_read_plain(field, ifp, size);
    }
    else {
        retval = _binfield_num_read_crypt(field, ifp, size, self->cryptor);
    }
    return retval;
}

int _binfield_num_read_plain(
    struct binfield_num *field,
    FILE *ifp,
    size_t size)
{
    unsigned char buf[NUMBUFMAX];

    if (fread(buf, size, 1, ifp) != 1)
        return 0;
    bytes_from_bigend(buf, size);
    memcpy(field->val, buf, size);
    field->len = size;
    return 1;
}

int _binfield_num_read_crypt(
    struct binfield_num *field,
    FILE *ifp,
    size_t size,
    struct cryptor *cryptor)
{
    unsigned char ibuffer[CRYPTBUFMAX];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMAX * 2];
    size_t osize;

    if (fread(ibuffer, 1, size, ifp) != size)
        return 0;
    isize = size;
    if (!cryptor_decrypt(cryptor, ibuffer, isize, obuffer, &osize))
        return 0;
    bytes_from_bigend(obuffer, osize);
    memcpy(field->val, obuffer, osize);
    field->len = osize;
    return 1;
}

int _binfield_num_write_plain(
    const struct binfield_num *field,
    FILE *ofp);
int _binfield_num_write_crypt(
    const struct binfield_num *field,
    FILE *ofp,
    struct cryptor *cryptor);

/* binfield_num_write: write number field to output stream
                       return 1 if has written correctly
                       return 0 if an error happened */
int binfield_num_write(
    struct binfield *self,
    const struct binfield_num *field,
    FILE *ofp)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_num_write_plain(field, ofp);
    }
    else {
        retval = _binfield_num_write_crypt(field, ofp, self->cryptor);
    }
    return retval;
}

int _binfield_num_write_plain(
    const struct binfield_num *field,
    FILE *ofp)
{
    unsigned char buf[NUMBUFMAX];

    if (field->len > 0) {
        memcpy(buf, field->val, field->len);
        bytes_to_bigend(buf, field->len);
        return fwrite(buf, field->len, 1, ofp) == 1;
    }
    return 1;
}

int _binfield_num_write_crypt(
    const struct binfield_num *field,
    FILE *ofp,
    struct cryptor *cryptor)
{
    unsigned char ibuffer[CRYPTBUFMAX];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMAX * 2];
    size_t osize;

    if (field->len > 0) {
        memcpy(ibuffer, field->val, field->len);
        isize = field->len;
        bytes_to_bigend(ibuffer, isize);
        if (!cryptor_encrypt(cryptor, ibuffer, isize, obuffer, &osize))
            return 0;
        return fwrite(obuffer, osize, 1, ofp) == 1;
    }
    return 1;
}

int _binfield_num_skip_plain(
    const struct binfield_num *field,
    FILE *iofp);
int _binfield_num_skip_crypt(
    const struct binfield_num *field,
    FILE *iofp,
    struct cryptor *cryptor);

/* binfield_num_skip: skip number field in stream
                      return 1 if has skipped correctly
                      return 0 if an error happened */
int binfield_num_skip(
    struct binfield *self,
    const struct binfield_num *field,
    FILE *iofp)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_num_skip_plain(field, iofp);
    }
    else {
        retval = _binfield_num_skip_crypt(field, iofp, self->cryptor);
    }
    return retval;
}

int _binfield_num_skip_plain(
    const struct binfield_num *field,
    FILE *iofp)
{
    int retval;
    size_t i;

    for (i = 0; i < field->len; i++)
        getc(iofp);
    retval = ferror(iofp) == 0;
    return retval;
}

int _binfield_num_skip_crypt(
    const struct binfield_num *field,
    FILE *iofp,
    struct cryptor *cryptor)
{
    unsigned char ibuffer[CRYPTBUFMAX];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMAX * 2];
    size_t osize;
    size_t i;

    memcpy(ibuffer, field->val, field->len);
    isize = field->len;
    if (!cryptor_encrypt(cryptor, ibuffer, isize, obuffer, &osize))
        return 0;
    for (i = 0; i < osize; i++)
        getc(iofp);
    return ferror(iofp) == 0;
}

int _binfield_stream_write_plain(
    const struct binfield_stream *field,
    FILE *ofp);
int _binfield_stream_write_crypt(
    const struct binfield_stream *field,
    FILE *ofp,
    struct cryptor *cryptor);

/* binfield_stream_write: write stream field to output stream
                          return 1 if has written correctly
                          return 0 if an error happened */
int binfield_stream_write(
    struct binfield *self,
    const struct binfield_stream *field,
    FILE *ofp)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_stream_write_plain(field, ofp);
    }
    else {
        retval = _binfield_stream_write_crypt(field, ofp, self->cryptor);
    }
    return retval;
}

int _binfield_stream_write_plain(
    const struct binfield_stream *field,
    FILE *ofp)
{
    int retval;

    retval = 1;
    if (field->valfp != NULL) {
        retval = file_write_file(ofp, field->valfp);
        rewind(field->valfp);
    }
    return retval;
}

int _binfield_stream_write_crypt(
    const struct binfield_stream *field,
    FILE *ofp,
    struct cryptor *cryptor)
{
    int retval;
    unsigned char ibuffer[CRYPTBUFMOD];
    size_t isize;
    unsigned char obuffer[CRYPTBUFMOD * 2];
    size_t osize;

    retval = 1;
    if (field->valfp != NULL) {
        while ((isize = fread(ibuffer, 1, CRYPTBUFMOD, field->valfp)) > 0) {
            if (!cryptor_encrypt(cryptor, ibuffer, isize, obuffer, &osize))
                return 0;
            if (fwrite(obuffer, 1, osize, ofp) != osize)
                break;
        }
        retval = !ferror(field->valfp) && feof(field->valfp) && !ferror(ofp);
        rewind(field->valfp);
    }
    return retval;
}

int _binfield_stream_skip_plain(
    const struct binfield_stream *field,
    FILE *iofp);
int _binfield_stream_skip_crypt(
    const struct binfield_stream *field,
    FILE *ofp,
    struct cryptor *cryptor);

/* binfield_stream_skip: skip stream field in stream
                         return 1 if has skipped correctly
                         return 0 if an error happened */
int binfield_stream_skip(
    struct binfield *self,
    const struct binfield_stream *field,
    FILE *iofp)
{
    int retval;

    if (self->cryptor == NULL) {
        retval = _binfield_stream_skip_plain(field, iofp);
    }
    else {
        retval = _binfield_stream_skip_crypt(field, iofp, self->cryptor);
    }
    return retval;
}

int _binfield_stream_skip_plain(
    const struct binfield_stream *field,
    FILE *iofp)
{
    return file_skip_bytes(iofp, &field->len);
}

int _stream_size_to_encrypted_size(
    struct cryptor *cryptor,
    const struct bignumber *size,
    struct bignumber *out);

int _binfield_stream_skip_crypt(
    const struct binfield_stream *field,
    FILE *iofp,
    struct cryptor *cryptor)
{
    struct bignumber len;

    if (!_stream_size_to_encrypted_size(cryptor, &field->len, &len))
        return 0;
    return file_skip_bytes(iofp, &len);
}

int _stream_size_to_encrypted_size(
    struct cryptor *cryptor,
    const struct bignumber *size,
    struct bignumber *out)
{
    enum cryptor_algorithm algo;

    if (!cryptor_algo_get(cryptor, &algo))
        return 0;
    if (algo != CRYPTOR_ALGORITHM_XOR)
        return 0;
    *out = *size;
    return 1;
}

/* binfield_raw_free: free raw field value and field itself */
void binfield_raw_free(
    struct binfield *self,
    struct binfield_raw *field)
{
    free(field->val);
    free(field);
}

/* binfield_num_free: free number field value and field itself */
void binfield_num_free(
    struct binfield *self,
    struct binfield_num *field)
{
    free(field->val);
    free(field);
}

/* binfield_stream_free: free stream field itself */
void binfield_stream_free(
    struct binfield *self,
    struct binfield_stream *field)
{
    free(field);
}

/* binfield_end: stop binfield and clear internal values */
void binfield_end(struct binfield *self)
{
    self->cryptor = NULL;
}
