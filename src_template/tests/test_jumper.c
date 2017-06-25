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
#include "../jumper.h"

void test_can_jump_in_dir_to_file_offset(void);
void test_can_jump_in_file_to_file_offset(void);

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

    if (CU_add_test(suite, "test can jump in dir to file offset",
                    test_can_jump_in_dir_to_file_offset) == NULL
     || CU_add_test(suite, "test can jump in file to file offset",
                    test_can_jump_in_file_to_file_offset) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_jump_in_dir_to_file_offset(void)
{
    FILE *iofp;
    size_t offset;

    int i;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    offset = 10;
    retval = jumper_dir_jump_file_offset(iofp, offset);

    CU_ASSERT_TRUE(retval);
    CU_ASSERT_EQUAL(ftell(iofp), offset);

    fclose(iofp);
}

void test_can_jump_in_file_to_file_offset(void)
{
    FILE *iofp;
    size_t offset;

    int i;
    int retval;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    offset = 10;
    retval = jumper_file_jump_file_offset(iofp, offset);

    CU_ASSERT_TRUE(retval);
    CU_ASSERT_EQUAL(ftell(iofp), offset);

    fclose(iofp);
}
