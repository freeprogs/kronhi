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

/* binfield_raw_create:
   allocate a raw field with allocated value of given size
   return pointer to field
   return NULL if can't allocate */
struct field_raw *binfield_raw_create(size_t size)
{
    struct field_raw *p;

    p = malloc(sizeof *p);
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
struct field_num *binfield_num_create(size_t size)
{
    struct field_num *p;

    p = malloc(sizeof *p);
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

/* binfield_raw_set:
   set raw field value and length to given value and length
   return 1 if has set correctly
   return 0 if can't set */
int binfield_raw_set(struct field_raw *field, const void *value, size_t length)
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
int binfield_raw_get(const struct field_raw *field, void *out)
{
    memcpy(out, field->val, field->len);
    return 1;
}

/* binfield_num_set:
   set number field value and length to given value and length
   return 1 if has set correctly
   return 0 if can't set */
int binfield_num_set(struct field_num *field, const void *value, size_t length)
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
int binfield_num_get(const struct field_num *field, void *out)
{
    memcpy(out, field->val, field->len);
    return 1;
}

/* binfield_raw_read: read raw field from input stream
                      return 1 if has read correctly
                      return 0 if an error happened */
int binfield_raw_read(FILE *ifp, struct field_raw *field, size_t size)
{
    if (size > field->maxsize)
        return 0;
    return fread(field->val, size, 1, ifp) == 1;
}

/* binfield_raw_write: write raw field to output stream
                       return 1 if has written correctly
                       return 0 if an error happened */
int binfield_raw_write(const struct field_raw *field, FILE *ofp)
{
    if (field->len > 0)
        return fwrite(field->val, field->len, 1, ofp) == 1;
    return 1;
}

/* binfield_num_read: read number field from input stream
                      return 1 if has read correctly
                      return 0 if an error happened */
int binfield_num_read(FILE *ifp, struct field_num *field, size_t size)
{
    unsigned char buf[NUMBUFMAX];

    if (fread(buf, size, 1, ifp) != 1)
        return 0;
    bytes_from_bigend(buf, size);
    memcpy(field->val, buf, size);
    field->len = size;
    return 1;
}

/* binfield_num_write: write number field to output stream
                       return 1 if has written correctly
                       return 0 if an error happened */
int binfield_num_write(const struct field_num *field, FILE *ofp)
{
    unsigned char buf[NUMBUFMAX];

    if (field->len > 0) {
        memcpy(buf, field->val, field->len);
        bytes_to_bigend(buf, field->len);
        return fwrite(buf, field->len, 1, ofp) == 1;
    }
    return 1;
}

/* binfield_raw_free: free raw field value and field itself */
void binfield_raw_free(struct field_raw *field)
{
    free(field->val);
    free(field);
}

/* binfield_num_free: free number field value and field itself */
void binfield_num_free(struct field_num *field)
{
    free(field->val);
    free(field);
}
