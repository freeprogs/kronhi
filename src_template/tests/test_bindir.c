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
#include "../bindir.h"

void test_can_get_size(void);
void test_can_set_and_get_type_sign_field(void);
void test_can_set_and_get_descsize_field(void);
void test_can_set_and_get_file_offset_field(void);
void test_can_set_and_get_num_of_files_field(void);

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

    if (CU_add_test(suite, "can get size",
                    test_can_get_size) == NULL
     || CU_add_test(suite, "can set and get type sign field",
                    test_can_set_and_get_type_sign_field) == NULL
     || CU_add_test(suite, "can set and get descsize field",
                    test_can_set_and_get_descsize_field) == NULL
     || CU_add_test(suite, "can set and get num_of_files field",
                    test_can_set_and_get_num_of_files_field) == NULL
     || CU_add_test(suite, "can set and get file_offset field",
                    test_can_set_and_get_file_offset_field) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_get_size(void)
{
    struct bindir dir;

    size_t out;

    bindir_start(&dir);

    /* 1 + 2 + 1 + 4 + 4 */
    bindir_type_set(&dir, 'd');
    bindir_descsize_set(&dir, 1);
    bindir_desc_set(&dir, "a");
    bindir_num_of_files_set(&dir, 0);
    bindir_file_offset_set(&dir, 0);
    out = 0;
    bindir_get_size(&dir, &out);

    CU_ASSERT_EQUAL(out, 12);

    bindir_end(&dir);
}

void test_can_set_and_get_type_sign_field(void)
{
    struct bindir dir;
    char out;

    bindir_start(&dir);

    bindir_type_set(&dir, 'd');
    out = 0;
    bindir_type_get(&dir, &out);
    CU_ASSERT_EQUAL(out, 'd');

    bindir_end(&dir);
}

void test_can_set_and_get_descsize_field(void)
{
    struct bindir dir;
    unsigned short out;

    bindir_start(&dir);

    bindir_descsize_set(&dir, 1);
    out = 0;
    bindir_descsize_get(&dir, &out);
    CU_ASSERT_EQUAL(out, 1);

    bindir_end(&dir);
}

void test_can_set_and_get_num_of_files_field(void)
{
    struct bindir dir;
    size_t out;

    bindir_start(&dir);

    bindir_num_of_files_set(&dir, 1);
    out = 0;
    bindir_num_of_files_get(&dir, &out);
    CU_ASSERT_EQUAL(out, 1);

    bindir_end(&dir);
}

void test_can_set_and_get_file_offset_field(void)
{
    struct bindir dir;
    size_t out;

    bindir_start(&dir);

    bindir_file_offset_set(&dir, 1);
    out = 0;
    bindir_file_offset_get(&dir, &out);
    CU_ASSERT_EQUAL(out, 1);

    bindir_end(&dir);
}
