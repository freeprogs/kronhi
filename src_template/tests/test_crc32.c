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
#include "../crc32.h"

void test_can_count_crc32_of_bytes(void);
void test_can_count_crc32_of_stream(void);

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

    if (CU_add_test(suite, "can count crc32 of bytes",
                    test_can_count_crc32_of_bytes) == NULL
     || CU_add_test(suite, "can count crc32 of stream",
                    test_can_count_crc32_of_stream) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_count_crc32_of_bytes(void)
{
    char bytes[100];

    sprintf(bytes, "abc");
    CU_ASSERT_EQUAL(crc32(bytes, strlen(bytes)), 0x352441C2);

    sprintf(bytes, "def");
    CU_ASSERT_EQUAL(crc32(bytes, strlen(bytes)), 0x0CC4E161);
}

void test_can_count_crc32_of_stream(void)
{
    FILE *fp;

    fp = tmpfile();
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(fp, "abc");

    rewind(fp);
    CU_ASSERT_EQUAL(crc32stream(fp), 0x352441C2);

    fclose(fp);
}
