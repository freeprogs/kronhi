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
                    test_can_create_raw_field) == NULL) {
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
