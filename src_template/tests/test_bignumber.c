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
#include "../bignumber.h"

void test_can_set_value_from_int(void);
void test_can_set_value_from_string(void);
void test_can_get_value_to_string(void);
void test_can_compare_two_big_numbers_for_lt(void);
void test_can_add_integer_number(void);
void test_can_add_unsigned_long_number(void);

int main(void)
{
    CU_pSuite suite = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    suite = CU_add_suite("suite", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite, "can set value from int",
                    test_can_set_value_from_int) == NULL
     || CU_add_test(suite, "can set value from string",
                    test_can_set_value_from_string) == NULL
     || CU_add_test(suite, "can get value to string",
                    test_can_get_value_to_string) == NULL
     || CU_add_test(suite, "can compare two big numbers for less than",
                    test_can_compare_two_big_numbers_for_lt) == NULL
     || CU_add_test(suite, "test can add integer number",
                    test_can_add_integer_number) == NULL
     || CU_add_test(suite, "test can add unsigned long number",
                    test_can_add_unsigned_long_number) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_set_value_from_int(void)
{
    struct bignumber number;

    char out[100];

    bignumber_set_value_int(&number, 1);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "1");
}

void test_can_set_value_from_string(void)
{
    struct bignumber number;

    char out[100];

    bignumber_set_value_string(&number, "1");
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "1");
}

void test_can_get_value_to_string(void)
{
    struct bignumber number;

    char out[100];

    bignumber_set_value_int(&number, 1);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "1");

    bignumber_set_value_int(&number, 2);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "2");
}

void test_can_compare_two_big_numbers_for_lt(void)
{
    struct bignumber number1, number2;

    bignumber_set_value_int(&number1, 1);
    bignumber_set_value_int(&number2, 2);
    CU_ASSERT_TRUE(bignumber_lt_big(&number1, &number2));

    bignumber_set_value_int(&number1, 1);
    bignumber_set_value_int(&number2, 1);
    CU_ASSERT_FALSE(bignumber_lt_big(&number1, &number2));

    bignumber_set_value_int(&number1, 2);
    bignumber_set_value_int(&number2, 1);
    CU_ASSERT_FALSE(bignumber_lt_big(&number1, &number2));
}

void test_can_add_integer_number(void)
{
    struct bignumber number;

    char out[100];

    bignumber_set_value_int(&number, 0);
    bignumber_add_int(&number, 1);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "1");

    bignumber_add_int(&number, 1);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "2");
}

void test_can_add_unsigned_long_number(void)
{
    struct bignumber number;

    char out[100];

    bignumber_set_value_int(&number, 0);
    bignumber_add_ulong(&number, 1);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "1");

    bignumber_add_ulong(&number, 1);
    bignumber_tostr(&number, out);
    CU_ASSERT_STRING_EQUAL(out, "2");
}
