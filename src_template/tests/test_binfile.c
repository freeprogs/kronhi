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
#include "../binfile.h"

void test_can_get_size(void);
void test_can_set_and_get_namesize_field(void);
void test_can_set_and_get_descsize_field(void);
void test_can_set_and_get_contentsize_field(void);
void test_can_set_and_get_file_offset_field(void);

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
     || CU_add_test(suite, "can set and get namesize field",
                    test_can_set_and_get_namesize_field) == NULL
     || CU_add_test(suite, "can set and get descsize field",
                    test_can_set_and_get_descsize_field) == NULL
     || CU_add_test(suite, "can set and get contentsize field",
                    test_can_set_and_get_contentsize_field) == NULL
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
    struct binfile file;
    struct bignumber size;
    char out[100];

    binfile_start(&file);

    /* 1 + 1 + 1 + 2 + 1 + 14 + 4 + 2 + 1 + 4 */
    binfile_type_set(&file, 'f');
    binfile_namesize_set(&file, 1);
    binfile_name_set(&file, "a");
    binfile_descsize_set(&file, 1);
    binfile_desc_set(&file, "b");
    binfile_datetime_set(&file, "00000000000000");
    binfile_ctrlsum_set(&file, 0x00000000);
    binfile_contentsize_set(&file, "1");
    binfile_contentstream_set(&file, NULL);
    binfile_file_offset_set(&file, 0);

    bignumber_set_value_int(&size, 0);
    binfile_get_size(&file, &size);
    *out = '\0';
    bignumber_tostr(&size, out);

    CU_ASSERT_STRING_EQUAL(out, "31");

    binfile_end(&file);
}

void test_can_set_and_get_namesize_field(void)
{
    struct binfile file;
    unsigned char out;

    binfile_start(&file);

    binfile_namesize_set(&file, 1);
    out = 0;
    binfile_namesize_get(&file, &out);
    CU_ASSERT_EQUAL(out, 1);

    binfile_end(&file);
}

void test_can_set_and_get_descsize_field(void)
{
    struct binfile file;
    unsigned short out;

    binfile_start(&file);

    binfile_descsize_set(&file, 1);
    out = 0;
    binfile_descsize_get(&file, &out);
    CU_ASSERT_EQUAL(out, 1);

    binfile_end(&file);
}

void test_can_set_and_get_contentsize_field(void)
{
    struct binfile file;
    char out[100];

    binfile_start(&file);

    binfile_contentsize_set(&file, "1");
    *out = '\0';
    binfile_contentsize_get(&file, out);
    CU_ASSERT_STRING_EQUAL(out, "1");

    binfile_end(&file);
}

void test_can_set_and_get_file_offset_field(void)
{
    struct binfile file;
    size_t out;

    binfile_start(&file);

    binfile_file_offset_set(&file, 1);
    out = 0;
    binfile_file_offset_get(&file, &out);
    CU_ASSERT_EQUAL(out, 1);

    binfile_end(&file);
}
