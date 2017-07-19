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

int main(void)
{
    CU_pSuite suite1 = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    suite1 = CU_add_suite("plain", NULL, NULL);
    if (suite1 == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite1, "can create raw field",
                    test_can_create_raw_field) == NULL
     || CU_add_test(suite1, "can create set field",
                    test_can_set_raw_field) == NULL
     || CU_add_test(suite1, "raise on set raw field with value overflow",
                    test_raise_on_set_raw_field_with_value_overflow) == NULL
     || CU_add_test(suite1, "test can get raw field",
                    test_can_get_raw_field) == NULL
     || CU_add_test(suite1, "test can read raw field",
                    test_can_read_raw_field) == NULL
     || CU_add_test(suite1, "test can write raw field",
                    test_can_write_raw_field) == NULL
     || CU_add_test(suite1, "test can skip raw field",
                    test_can_skip_raw_field) == NULL
     || CU_add_test(suite1, "can create number field",
                    test_can_create_number_field) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
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
