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

#include <stdio.h>
#include <CUnit/Basic.h>
#include "../binfield.h"

void test_can_create_raw_field(void);
void test_can_set_raw_field(void);
void test_raise_on_set_raw_field_with_value_overflow(void);
void test_can_get_raw_field(void);
void test_can_read_raw_field(void);
void test_can_write_raw_field(void);
void test_can_skip_raw_field(void);

void test_can_create_number_field(void);
void test_can_set_number_field(void);
void test_raise_on_set_number_field_with_value_overflow(void);
void test_can_get_number_field(void);
void test_can_read_number_field(void);
void test_can_write_number_field(void);
void test_can_skip_number_field(void);

void test_can_create_stream_field(void);
void test_can_set_stream_field(void);
void test_can_get_stream_field(void);
void test_can_write_stream_field(void);
void test_can_skip_stream_field(void);

void test_can_read_raw_field_with_xor(void);
void test_can_write_raw_field_with_xor(void);
void test_can_skip_raw_field_with_xor(void);
void test_can_read_number_field_with_xor(void);
void test_can_write_number_field_with_xor(void);
void test_can_skip_number_field_with_xor(void);
void test_can_write_stream_field_with_xor(void);
void test_can_skip_stream_field_with_xor(void);

int _is_big_endian(void);
int _is_little_endian(void);
void *_bytes_reverse(void *bytes, size_t size);

int main(void)
{
    CU_pSuite suite1 = NULL;
    CU_pSuite suite2 = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    suite1 = CU_add_suite("plain", NULL, NULL);
    if (suite1 == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    suite2 = CU_add_suite("xor", NULL, NULL);
    if (suite2 == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite1, "can create raw field",
                    test_can_create_raw_field) == NULL
     || CU_add_test(suite1, "can create set field",
                    test_can_set_raw_field) == NULL
     || CU_add_test(suite1, "raise on set raw field with value overflow",
                    test_raise_on_set_raw_field_with_value_overflow) == NULL
     || CU_add_test(suite1, "can get raw field",
                    test_can_get_raw_field) == NULL
     || CU_add_test(suite1, "can read raw field",
                    test_can_read_raw_field) == NULL
     || CU_add_test(suite1, "can write raw field",
                    test_can_write_raw_field) == NULL
     || CU_add_test(suite1, "can skip raw field",
                    test_can_skip_raw_field) == NULL
     || CU_add_test(suite1, "can create number field",
                    test_can_create_number_field) == NULL
     || CU_add_test(suite1, "can set number field",
                    test_can_set_number_field) == NULL
     || CU_add_test(suite1, "raise on set number field with value overflow",
                    test_raise_on_set_number_field_with_value_overflow) == NULL
     || CU_add_test(suite1, "can get number field",
                    test_can_get_number_field) == NULL
     || CU_add_test(suite1, "can read number field",
                    test_can_read_number_field) == NULL
     || CU_add_test(suite1, "can write number field",
                    test_can_write_number_field) == NULL
     || CU_add_test(suite1, "can skip number field",
                    test_can_skip_number_field) == NULL
     || CU_add_test(suite1, "can create stream field",
                    test_can_create_stream_field) == NULL
     || CU_add_test(suite1, "can set stream field",
                    test_can_set_stream_field) == NULL
     || CU_add_test(suite1, "can get stream field",
                    test_can_get_stream_field) == NULL
     || CU_add_test(suite1, "can write stream field",
                    test_can_write_stream_field) == NULL
     || CU_add_test(suite1, "can skip stream field",
                    test_can_skip_stream_field) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite2, "can read raw field with xor",
                    test_can_read_raw_field_with_xor) == NULL
     || CU_add_test(suite2, "can write raw field with xor",
                    test_can_write_raw_field_with_xor) == NULL
     || CU_add_test(suite2, "can skip raw field with xor",
                    test_can_skip_raw_field_with_xor) == NULL
     || CU_add_test(suite2, "can read number field with xor",
                    test_can_read_number_field_with_xor) == NULL
     || CU_add_test(suite2, "can write number field with xor",
                    test_can_write_number_field_with_xor) == NULL
     || CU_add_test(suite2, "can skip number field with xor",
                    test_can_skip_number_field_with_xor) == NULL
     || CU_add_test(suite2, "can write stream field with xor",
                    test_can_write_stream_field_with_xor) == NULL
     || CU_add_test(suite2, "can skip stream field with xor",
                    test_can_skip_stream_field_with_xor) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

int _is_little_endian(void)
{
    unsigned short n = 1;

    return *((unsigned char *) &n) != 0;
}

int _is_big_endian(void)
{
    unsigned short n = 1;

    return *((unsigned char *) &n) != 256;
}

void *_bytes_reverse(void *bytes, size_t size)
{
    unsigned char *p, *q;
    unsigned char c;

    for (p = bytes, q = (unsigned char *) bytes + size - 1; p < q; p++, q--)
        c = *p, *p = *q, *q = c;
    return bytes;
}

void test_can_create_raw_field(void)
{
    struct binfield field;

    struct binfield_raw *out;

    binfield_start(&field, NULL);

    out = NULL;
    out = binfield_raw_create(&field, 3);

    CU_ASSERT_PTR_NOT_NULL(out);
    CU_ASSERT_EQUAL(out->maxsize, 3);
    CU_ASSERT_EQUAL(out->len, 0);

    binfield_end(&field);
}

void test_can_set_raw_field(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;

    unsigned char value[100];
    size_t vlen;
    int retval;

    binfield_start(&field, NULL);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(value, "abc", vlen);
    retval = binfield_raw_set(&field, data, value, vlen);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(value, data->val, vlen);
    CU_ASSERT_EQUAL(data->len, vlen);

    binfield_end(&field);
}

void test_raise_on_set_raw_field_with_value_overflow(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;

    unsigned char value[100];
    size_t vlen;
    int retval;

    binfield_start(&field, NULL);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 4;
    memcpy(value, "abcd", vlen);
    retval = binfield_raw_set(&field, data, value, vlen);

    CU_ASSERT_EQUAL(retval, 0);

    binfield_end(&field);
}

void test_can_get_raw_field(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;

    unsigned char value[100];
    size_t vlen;
    int retval;

    unsigned char out[100];

    binfield_start(&field, NULL);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(value, "abc", vlen);
    retval = binfield_raw_set(&field, data, value, vlen);

    CU_ASSERT_EQUAL(retval, 1);

    memset(out, 0, vlen);
    retval = binfield_raw_get(&field, data, out);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(out, value, vlen);

    binfield_end(&field);
}

void test_can_read_raw_field(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;

    FILE *iofp;
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abc");
    rewind(iofp);

    binfield_start(&field, NULL);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memset(data->val, 0, vlen);
    data->len = 0;
    retval = binfield_raw_read(&field, data, iofp, vlen);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(data->val, "abc", vlen);
    CU_ASSERT_EQUAL(data->len, vlen);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_write_raw_field(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;

    FILE *iofp;
    unsigned char value[100];
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    binfield_start(&field, NULL);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "abc", vlen);
    data->len = vlen;

    retval = binfield_raw_write(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    rewind(iofp);
    memset(value, 0, vlen);
    CU_ASSERT_EQUAL(fread(value, 1, vlen, iofp), vlen);

    CU_ASSERT_NSTRING_EQUAL(value, data->val, vlen);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_skip_raw_field(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;

    FILE *iofp;
    size_t vlen;
    int c;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abcd");
    rewind(iofp);

    binfield_start(&field, NULL);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "123", vlen);
    data->len = vlen;

    retval = binfield_raw_skip(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    c = getc(iofp);
    CU_ASSERT_EQUAL(c, 'd');

    binfield_end(&field);

    fclose(iofp);
}

void test_can_create_number_field(void)
{
    struct binfield field;

    struct binfield_num *out;

    binfield_start(&field, NULL);

    out = NULL;
    out = binfield_num_create(&field, 3);

    CU_ASSERT_PTR_NOT_NULL(out);
    CU_ASSERT_EQUAL(out->maxsize, 3);
    CU_ASSERT_EQUAL(out->len, 0);

    binfield_end(&field);
}

void test_can_set_number_field(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;

    unsigned char value[100];
    size_t vlen;
    int retval;

    binfield_start(&field, NULL);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(value, "abc", vlen);
    retval = binfield_num_set(&field, data, value, vlen);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(value, data->val, vlen);
    CU_ASSERT_EQUAL(data->len, vlen);

    binfield_end(&field);
}

void test_raise_on_set_number_field_with_value_overflow(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;

    unsigned char value[100];
    size_t vlen;
    int retval;

    binfield_start(&field, NULL);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 4;
    memcpy(value, "abcd", vlen);
    retval = binfield_num_set(&field, data, value, vlen);

    CU_ASSERT_EQUAL(retval, 0);

    binfield_end(&field);
}

void test_can_get_number_field(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;

    unsigned char value[100];
    size_t vlen;
    int retval;

    unsigned char out[100];

    binfield_start(&field, NULL);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(value, "abc", vlen);
    retval = binfield_num_set(&field, data, value, vlen);

    CU_ASSERT_EQUAL(retval, 1);

    memset(out, 0, vlen);
    retval = binfield_num_get(&field, data, out);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(out, value, vlen);

    binfield_end(&field);
}

void test_can_read_number_field(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;

    FILE *iofp;
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    if (_is_little_endian()) {
        fprintf(iofp, "cba");
        rewind(iofp);
    }
    else if (_is_big_endian()) {
        fprintf(iofp, "abc");
        rewind(iofp);
    }

    binfield_start(&field, NULL);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memset(data->val, 0, vlen);
    data->len = 0;
    retval = binfield_num_read(&field, data, iofp, vlen);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(data->val, "abc", vlen);
    CU_ASSERT_EQUAL(data->len, vlen);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_write_number_field(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;

    FILE *iofp;
    unsigned char value[100];
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    binfield_start(&field, NULL);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "abc", vlen);
    data->len = vlen;

    retval = binfield_num_write(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    rewind(iofp);
    memset(value, 0, vlen);
    CU_ASSERT_EQUAL(fread(value, 1, vlen, iofp), vlen);

    if (_is_little_endian()) {
        _bytes_reverse(value, vlen);
    }
    else if (_is_big_endian()) {
        ;
    }

    CU_ASSERT_NSTRING_EQUAL(value, data->val, vlen);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_skip_number_field(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;

    FILE *iofp;
    size_t vlen;
    int c;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abcd");
    rewind(iofp);

    binfield_start(&field, NULL);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "123", vlen);
    data->len = vlen;

    retval = binfield_num_skip(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    c = getc(iofp);
    CU_ASSERT_EQUAL(c, 'd');

    binfield_end(&field);

    fclose(iofp);
}

void test_can_create_stream_field(void)
{
    struct binfield field;

    struct bignumber len;
    struct binfield_stream *out;

    binfield_start(&field, NULL);

    out = NULL;
    out = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(out);
    CU_ASSERT_PTR_NULL(out->valfp);

    bignumber_set_value_int(&len, 0);
    CU_ASSERT_EQUAL(memcmp(&out->len, &len, sizeof len), 0);

    binfield_end(&field);
}

void test_can_set_stream_field(void)
{
    struct binfield field;
    struct binfield_stream *data;

    FILE *fp;
    struct bignumber len;
    int retval;

    binfield_start(&field, NULL);

    data = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(data);

    fp = (FILE *) 1;
    retval = binfield_stream_set(&field, data, fp);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_EQUAL(data->valfp, 1);
    bignumber_set_value_int(&len, 0);
    CU_ASSERT_EQUAL(memcmp(&data->len, &len, sizeof len), 0);

    binfield_end(&field);
}

void test_can_get_stream_field(void)
{
    struct binfield field;
    struct binfield_stream *data;

    FILE *fp;
    int retval;

    FILE *out;

    binfield_start(&field, NULL);

    data = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(data);

    fp = (FILE *) 1;
    retval = binfield_stream_set(&field, data, fp);

    CU_ASSERT_EQUAL(retval, 1);

    out = NULL;
    retval = binfield_stream_get(&field, data, &out);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_EQUAL(out, fp);

    binfield_end(&field);
}

void test_can_write_stream_field(void)
{
    struct binfield field;
    struct binfield_stream *data;

    FILE *iofp;
    FILE *srcifp;
    unsigned char buffer[100];
    size_t buflen;
    int retval;

    srcifp = tmpfile();
    if (srcifp == NULL)
        CU_FAIL("can't create temporary file");

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(srcifp, "abc");
    rewind(srcifp);

    binfield_start(&field, NULL);

    data = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(data);

    retval = binfield_stream_set(&field, data, srcifp);

    CU_ASSERT_EQUAL(retval, 1);

    retval = binfield_stream_write(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    rewind(iofp);

    buflen = 3;
    memset(buffer, 0, buflen);
    CU_ASSERT_EQUAL(fread(buffer, 1, buflen, iofp), buflen);

    CU_ASSERT_NSTRING_EQUAL(buffer, "abc", buflen);

    binfield_end(&field);

    fclose(srcifp);
    fclose(iofp);
}

void test_can_skip_stream_field(void)
{
    struct binfield field;
    struct binfield_stream *data;

    FILE *iofp;
    FILE *srcifp;
    struct bignumber len;
    int c;
    int retval;

    srcifp = tmpfile();
    if (srcifp == NULL)
        CU_FAIL("can't create temporary file");

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abcd");
    rewind(iofp);

    fprintf(srcifp, "abc");
    rewind(srcifp);

    binfield_start(&field, NULL);

    data = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(data);

    retval = binfield_stream_set(&field, data, srcifp);

    CU_ASSERT_EQUAL(retval, 1);

    bignumber_set_value_int(&len, 0);
    CU_ASSERT_EQUAL(memcmp(&data->len, &len, sizeof len), 0);

    retval = binfield_stream_skip(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    c = getc(iofp);
    CU_ASSERT_EQUAL(c, 'd');

    bignumber_set_value_int(&len, 3);
    CU_ASSERT_EQUAL(memcmp(&data->len, &len, sizeof len), 0);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_read_raw_field_with_xor(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "PPP");
    rewind(iofp);

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memset(data->val, 0, vlen);
    data->len = 0;
    retval = binfield_raw_read(&field, data, iofp, vlen);

    CU_ASSERT_EQUAL(retval, 1);
    CU_ASSERT_NSTRING_EQUAL(data->val, "123", vlen);
    CU_ASSERT_EQUAL(data->len, vlen);

    binfield_end(&field);

    cryptor_end(&cryptor);

    fclose(iofp);
}

void test_can_write_raw_field_with_xor(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    unsigned char value[100];
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "123", vlen);
    data->len = vlen;

    retval = binfield_raw_write(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    rewind(iofp);
    memset(value, 0, vlen);
    CU_ASSERT_EQUAL(fread(value, 1, vlen, iofp), vlen);

    CU_ASSERT_NSTRING_EQUAL(value, "PPP", vlen);

    binfield_end(&field);

    cryptor_end(&cryptor);

    fclose(iofp);
}

void test_can_skip_raw_field_with_xor(void)
{
    struct binfield field;
    struct binfield_raw *data;
    size_t maxsize = 3;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    size_t vlen;
    int c;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abcd");
    rewind(iofp);

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_raw_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "123", vlen);
    data->len = vlen;

    retval = binfield_raw_skip(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    c = getc(iofp);
    CU_ASSERT_EQUAL(c, 'd');

    binfield_end(&field);

    cryptor_end(&cryptor);

    fclose(iofp);
}

void test_can_read_number_field_with_xor(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    if (_is_little_endian()) {
        fprintf(iofp, "PPP");
        rewind(iofp);
    }
    else if (_is_big_endian()) {
        fprintf(iofp, "PPP");
        rewind(iofp);
    }

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memset(data->val, 0, vlen);
    data->len = 0;
    retval = binfield_num_read(&field, data, iofp, vlen);

    CU_ASSERT_EQUAL(retval, 1);
    if (_is_little_endian()) {
        CU_ASSERT_NSTRING_EQUAL(data->val, "321", vlen);
    }
    else if (_is_big_endian()) {
        CU_ASSERT_NSTRING_EQUAL(data->val, "123", vlen);
    }
    CU_ASSERT_EQUAL(data->len, vlen);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_write_number_field_with_xor(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    unsigned char value[100];
    size_t vlen;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "123", vlen);
    data->len = vlen;

    retval = binfield_num_write(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    rewind(iofp);
    memset(value, 0, vlen);
    CU_ASSERT_EQUAL(fread(value, 1, vlen, iofp), vlen);

    if (_is_little_endian()) {
        _bytes_reverse(value, vlen);
    }
    else if (_is_big_endian()) {
        ;
    }

    CU_ASSERT_NSTRING_EQUAL(value, "RPR", vlen);

    binfield_end(&field);

    fclose(iofp);
}

void test_can_skip_number_field_with_xor(void)
{
    struct binfield field;
    struct binfield_num *data;
    size_t maxsize = 3;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    size_t vlen;
    int c;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abcd");
    rewind(iofp);

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_num_create(&field, maxsize);

    CU_ASSERT_PTR_NOT_NULL(data);

    vlen = 3;
    memcpy(data->val, "123", vlen);
    data->len = vlen;

    retval = binfield_num_skip(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    c = getc(iofp);
    CU_ASSERT_EQUAL(c, 'd');

    binfield_end(&field);

    fclose(iofp);
}

void test_can_write_stream_field_with_xor(void)
{
    struct binfield field;
    struct binfield_stream *data;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    FILE *srcifp;
    unsigned char buffer[100];
    size_t buflen;
    int retval;

    srcifp = tmpfile();
    if (srcifp == NULL)
        CU_FAIL("can't create temporary file");

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(srcifp, "123");
    rewind(srcifp);

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(data);

    retval = binfield_stream_set(&field, data, srcifp);

    CU_ASSERT_EQUAL(retval, 1);

    retval = binfield_stream_write(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    rewind(iofp);

    buflen = 3;
    memset(buffer, 0, buflen);
    CU_ASSERT_EQUAL(fread(buffer, 1, buflen, iofp), buflen);

    CU_ASSERT_NSTRING_EQUAL(buffer, "PPP", buflen);

    binfield_end(&field);

    fclose(srcifp);
    fclose(iofp);
}

void test_can_skip_stream_field_with_xor(void)
{
    struct binfield field;
    struct binfield_stream *data;
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    FILE *iofp;
    FILE *srcifp;
    struct bignumber len;
    int c;
    int retval;

    srcifp = tmpfile();
    if (srcifp == NULL)
        CU_FAIL("can't create temporary file");

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    fprintf(iofp, "abcd");
    rewind(iofp);

    fprintf(srcifp, "abc");
    rewind(srcifp);

    cryptor_start(&cryptor, CRYPTOR_ALGORITHM_XOR, psw, pswlen);

    binfield_start(&field, &cryptor);

    data = binfield_stream_create(&field);

    CU_ASSERT_PTR_NOT_NULL(data);

    retval = binfield_stream_set(&field, data, srcifp);

    CU_ASSERT_EQUAL(retval, 1);

    bignumber_set_value_int(&len, 0);
    CU_ASSERT_EQUAL(memcmp(&data->len, &len, sizeof len), 0);

    retval = binfield_stream_skip(&field, data, iofp);

    CU_ASSERT_EQUAL(retval, 1);

    c = getc(iofp);
    CU_ASSERT_EQUAL(c, 'd');

    bignumber_set_value_int(&len, 3);
    CU_ASSERT_EQUAL(memcmp(&data->len, &len, sizeof len), 0);

    binfield_end(&field);

    fclose(iofp);
}
