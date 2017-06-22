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
#include "../node.h"

void test_can_test_for_dir_type(void);
void test_can_test_for_file_type(void);
void test_can_write_dir(void);
void test_can_write_file(void);

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

    if (CU_add_test(suite, "can test for dir type",
                    test_can_test_for_dir_type) == NULL
     || CU_add_test(suite, "can write dir",
                    test_can_write_dir) == NULL
     || CU_add_test(suite, "can test for file type",
                    test_can_test_for_file_type) == NULL
     || CU_add_test(suite, "can write file",
                    test_can_write_file) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_test_for_dir_type(void)
{
    FILE *iofp;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    putc('d', iofp);
    rewind(iofp);
    CU_ASSERT_TRUE(node_test_isdir(iofp));

    rewind(iofp);
    putc('x', iofp);
    rewind(iofp);
    CU_ASSERT_FALSE(node_test_isdir(iofp));

    fclose(iofp);
}

void test_can_test_for_file_type(void)
{
    FILE *iofp;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    putc('f', iofp);
    rewind(iofp);
    CU_ASSERT_TRUE(node_test_isfile(iofp));

    rewind(iofp);
    putc('x', iofp);
    rewind(iofp);
    CU_ASSERT_FALSE(node_test_isfile(iofp));

    fclose(iofp);
}

void test_can_write_dir(void)
{
    FILE *iofp;

    int i;
    unsigned char dirbytes[100];
    size_t dirsize;
    struct bindir dir;
    unsigned char buffer[100];

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    bindir_start(&dir);

    dirsize = 12;
    memcpy(dirbytes,
           "\x64"
           "\x00\x01"
           "\x61"
           "\x00\x00\x00\x01"
           "\x00\x00\x00\x02",
           dirsize);

    bindir_type_set(&dir, 'd');
    bindir_descsize_set(&dir, 1);
    bindir_desc_set(&dir, "a");
    bindir_num_of_files_set(&dir, 1);
    bindir_file_offset_set(&dir, 2);

    node_write_dir(iofp, &dir);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(memcmp(buffer, dirbytes, dirsize), 0);

    bindir_end(&dir);
    fclose(iofp);
}

void test_can_write_file(void)
{
    FILE *iofp;
    FILE *ifp;

    int i;
    unsigned char filebytes[100];
    size_t filesize;
    struct binfile file;
    unsigned char buffer[100];

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    ifp = tmpfile();
    if (ifp == NULL)
        CU_FAIL("can't create temporary file");

    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    fprintf(ifp, "abc");
    rewind(ifp);

    binfile_start(&file);

    filesize = 33;
    memcpy(filebytes,
           "\x66"
           "\x01"
           "\x61"
           "\x00\x01"
           "\x62"
           "\x32\x30\x31\x37\x30\x31\x30\x32\x30\x33\x30\x34\x30\x35"
           "\x01\x02\x03\x04"
           "\x33\x00"
           "\x61\x62\x63"
           "\x00\x00\x00\x01",
           filesize);

    binfile_type_set(&file, 'f');
    binfile_namesize_set(&file, 1);
    binfile_name_set(&file, "a");
    binfile_descsize_set(&file, 1);
    binfile_desc_set(&file, "b");
    binfile_datetime_set(&file, "20170102030405");
    binfile_ctrlsum_set(&file, 0x01020304);
    binfile_contentsize_set(&file, "3");
    binfile_contentstream_set(&file, ifp);
    binfile_file_offset_set(&file, 1);

    node_write_file(iofp, &file);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(memcmp(buffer, filebytes, filesize), 0);

    binfile_end(&file);
    fclose(iofp);
}
