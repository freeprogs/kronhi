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
#include "../file_operation.h"

void test_can_skip_a_byte(void);
void test_raise_on_skip_beyond_end_of_file(void);

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

    if (CU_add_test(suite, "can skip a byte",
                    test_can_skip_a_byte) == NULL ||
        CU_add_test(suite, "raise on skip beyond end of file",
                    test_raise_on_skip_beyond_end_of_file) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_skip_a_byte(void)
{
    struct file_offset offset;

    FILE *fp;

    fp = tmpfile();
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    putc('x', fp);

    rewind(fp);
    fileoffset_clear(&offset);
    fileoffset_add_number(&offset, 1);
    CU_ASSERT_TRUE(file_skip_to_offset(fp, &offset));

    rewind(fp);
    fileoffset_clear(&offset);
    fileoffset_add_number(&offset, 2);
    CU_ASSERT_FALSE(file_skip_to_offset(fp, &offset));

    fclose(fp);
}

void test_raise_on_skip_beyond_end_of_file(void)
{
    struct file_offset offset;

    FILE *fp;

    fp = tmpfile();
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    putc('x', fp);

    rewind(fp);
    fileoffset_clear(&offset);
    fileoffset_add_number(&offset, 2);
    CU_ASSERT_FALSE(file_skip_to_offset(fp, &offset));

    fclose(fp);
}
