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
#include "../chain.h"

void test_raise_on_very_big_file_offset(void);
void test_raise_on_very_big_directory_size(void);
void test_can_write_directory_to_the_end_of_file(void);
void test_raise_on_absent_directory(void);

int main(void)
{
    CU_pSuite suite1 = NULL;
    CU_pSuite suite2 = NULL;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    suite1 = CU_add_suite("chain create dir", NULL, NULL);
    if (suite1 == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    suite2 = CU_add_suite("chain append file", NULL, NULL);
    if (suite2 == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite1, "raise on very big file offset",
                    test_raise_on_very_big_file_offset) == NULL ||
        CU_add_test(suite1, "raise on very big directory size",
                    test_raise_on_very_big_directory_size) == NULL ||
        CU_add_test(suite1, "can write directory to the end of file",
                    test_can_write_directory_to_the_end_of_file) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite2, "raise on absent directory",
                    test_raise_on_absent_directory) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_raise_on_very_big_file_offset(void)
{
    const char *destination = "file.txt";
    const char *dirdesc = "";

    struct chain chain;
    enum chain_code retval;
    struct file_offset offset;

    FILE *fp;

    fp = fopen(destination, "wb");
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    putc('x', fp);
    fclose(fp);

    fileoffset_clear(&offset);
    fileoffset_add_number(&offset, 2);

    chain_start(&chain, destination, &offset);
    retval = chain_create_dir(&chain, dirdesc, 0, 0);
    chain_end(&chain);

    CU_ASSERT_EQUAL(retval, CHAIN_ERROR_DIR_SKIPOFFSET);

    remove(destination);
}

void test_raise_on_very_big_directory_size(void)
{
    const char *destination = "file.txt";
    const char *dirdesc = "";

    struct chain chain;
    enum chain_code retval;
    struct file_offset offset;

    FILE *fp;

    fp = fopen(destination, "wb");
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    putc('x', fp);
    fclose(fp);

    fileoffset_clear(&offset);

    chain_start(&chain, destination, &offset);
    retval = chain_create_dir(&chain, dirdesc, 0, 0);
    chain_end(&chain);

    CU_ASSERT_EQUAL(retval, CHAIN_ERROR_DIR_FILESIZE);

    remove(destination);
}

void test_can_write_directory_to_the_end_of_file(void)
{
    const char *destination = "file.txt";
    const char *dirdesc = "";
    const int DIRSIZE = 11;

    struct chain chain;
    enum chain_code retval;
    struct file_offset offset;

    FILE *fp;
    int i;

    fp = fopen(destination, "wb");
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < DIRSIZE; i++)
        putc('x', fp);
    fclose(fp);

    fileoffset_clear(&offset);

    chain_start(&chain, destination, &offset);
    retval = chain_create_dir(&chain, dirdesc, 0, 0);
    chain_end(&chain);

    CU_ASSERT_EQUAL(retval, CHAIN_OK);

    remove(destination);
}

void test_raise_on_absent_directory(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code retval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *fp;
    int i;

    fp = fopen(destination, "wb");
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', fp);
    fclose(fp);

    fp = fopen(source, "wb");
    if (fp == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(fp, "abc");
    fclose(fp);

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 0;
    retval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(retval, CHAIN_ERROR_FILE_NODIR);

    remove(source);
    remove(destination);
}
