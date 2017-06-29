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
void test_can_write_dir_header_by_field(void);
void test_can_write_file_header_by_field(void);
void can_read_dir_header(void);
void can_read_file_header(void);

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
     || CU_add_test(suite, "can write dir header by field",
                    test_can_write_dir_header_by_field) == NULL
     || CU_add_test(suite, "can read dir header",
                    can_read_dir_header) == NULL
     || CU_add_test(suite, "can test for file type",
                    test_can_test_for_file_type) == NULL
     || CU_add_test(suite, "can write file",
                    test_can_write_file) == NULL
     || CU_add_test(suite, "can write file header by field",
                    test_can_write_file_header_by_field) == NULL
     || CU_add_test(suite, "can read file header",
                    can_read_file_header) == NULL) {
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
    fclose(ifp);
    fclose(iofp);
}

void test_can_write_dir_header_by_field(void)
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

    bindir_type_set(&dir, 'd');
    bindir_descsize_set(&dir, 1);
    bindir_desc_set(&dir, "a");
    bindir_num_of_files_set(&dir, 1);
    bindir_file_offset_set(&dir, 2);


    /* Check writing of type sign field */

    dirsize = 12;
    memcpy(dirbytes,
           "\x64"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00"
           "\x00\x00\x00\x00",
           dirsize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    node_write_dir_header_field(iofp, &dir, DIRFLD_TYPESIGN);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write DIRFLD_TYPESIGN correctly" ,
         memcmp(buffer, dirbytes, dirsize)), 0);


    /* Check writing of description size field */

    dirsize = 12;
    memcpy(dirbytes,
           "\x00"
           "\x00\x01"
           "\x00"
           "\x00\x00\x00\x00"
           "\x00\x00\x00\x00",
           dirsize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    node_write_dir_header_field(iofp, &dir, DIRFLD_DESCSIZE);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write DIRFLD_DESCSIZE correctly" ,
         memcmp(buffer, dirbytes, dirsize)), 0);


    /* Check writing of description field */

    dirsize = 12;
    memcpy(dirbytes,
           "\x00"
           "\x00\x00"
           "\x61"
           "\x00\x00\x00\x00"
           "\x00\x00\x00\x00",
           dirsize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    node_write_dir_header_field(iofp, &dir, DIRFLD_DESC);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write DIRFLD_DESC correctly" ,
         memcmp(buffer, dirbytes, dirsize)), 0);


    /* Check writing of number of files field */

    dirsize = 12;
    memcpy(dirbytes,
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x01"
           "\x00\x00\x00\x00",
           dirsize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    node_write_dir_header_field(iofp, &dir, DIRFLD_NUMOFFILES);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write DIRFLD_NUMOFFILES correctly" ,
         memcmp(buffer, dirbytes, dirsize)), 0);


    /* Check writing of file offset field */

    dirsize = 12;
    memcpy(dirbytes,
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00"
           "\x00\x00\x00\x02",
           dirsize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    node_write_dir_header_field(iofp, &dir, DIRFLD_FILEOFFSET);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write DIRFLD_FILEOFFSET correctly" ,
         memcmp(buffer, dirbytes, dirsize)), 0);


    /* Check writing of all fields together */

    dirsize = 12;
    memcpy(dirbytes,
           "\x64"
           "\x00\x01"
           "\x61"
           "\x00\x00\x00\x01"
           "\x00\x00\x00\x02",
           dirsize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    node_write_dir_header_field(
        iofp,
        &dir,
        DIRFLD_TYPESIGN
        | DIRFLD_DESCSIZE
        | DIRFLD_DESC
        | DIRFLD_NUMOFFILES
        | DIRFLD_FILEOFFSET
    );
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write DIRFLD_* correctly" ,
         memcmp(buffer, dirbytes, dirsize)), 0);


    bindir_end(&dir);
    fclose(iofp);
}

void test_can_write_file_header_by_field(void)
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


    /* Check writing of type sign field */

    filesize = 33;
    memcpy(filebytes,
           "\x66"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_TYPESIGN);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_TYPESIGN correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of filename size field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x01"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_NAMESIZE);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_NAMESIZE correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of filename field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x61"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_NAME);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_NAME correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of description size field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x01"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_DESCSIZE);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_DESCSIZE correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of description field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x62"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_DESC);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_DESC correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of datetime field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x32\x30\x31\x37\x30\x31\x30\x32\x30\x33\x30\x34\x30\x35"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_DATETIME);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_DATETIME correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of control sum field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x01\x02\x03\x04"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_CTRLSUM);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_CTRLSUM correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of content size field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x33\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_CONTENTSIZE);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_CONTENTSIZE correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x61\x62\x63"
           "\x00\x00\x00\x00",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_CONTENTSTREAM);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_CONTENTSTREAM correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of file offset field */

    filesize = 33;
    memcpy(filebytes,
           "\x00"
           "\x00"
           "\x00"
           "\x00\x00"
           "\x00"
           "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
           "\x00\x00\x00\x00"
           "\x00\x00"
           "\x00\x00\x00"
           "\x00\x00\x00\x01",
           filesize);

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(iofp, &file, FILFLD_FILEOFFSET);
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_FILEOFFSET correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    /* Check writing of all fields together */

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

    rewind(iofp);
    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    rewind(ifp);

    node_write_file_header_field(
        iofp,
        &file,
        FILFLD_TYPESIGN
        | FILFLD_TYPESIGN
        | FILFLD_NAMESIZE
        | FILFLD_NAME
        | FILFLD_DESCSIZE
        | FILFLD_DESC
        | FILFLD_DATETIME
        | FILFLD_CTRLSUM
        | FILFLD_CONTENTSIZE
        | FILFLD_CONTENTSTREAM
        | FILFLD_FILEOFFSET
    );
    rewind(iofp);

    memset(buffer, 0, sizeof buffer);
    fread(buffer, 1, sizeof buffer, iofp);

    CU_ASSERT_EQUAL(
        ((void) "can't write FILFLD_* correctly" ,
         memcmp(buffer, filebytes, filesize)), 0);


    binfile_end(&file);
    fclose(ifp);
    fclose(iofp);
}

void can_read_dir_header(void)
{
    FILE *iofp;

    int i;
    unsigned char bytes[100];
    size_t size;
    struct bindir dir;
    char dir_type_sign;
    unsigned short dir_descsize;
    char dir_desc[100];
    size_t dir_num_of_files;
    size_t dir_file_offset;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    size = 12;
    memcpy(bytes,
           "\x64"
           "\x00\x01"
           "\x61"
           "\x00\x00\x00\x01"
           "\x00\x00\x00\x02",
           size);

    if (fwrite(bytes, 1, size, iofp) != size)
        CU_FAIL("can't prepare data in temporary file");
    rewind(iofp);

    bindir_start(&dir);

    node_read_dir_header(iofp, &dir);

    bindir_type_get(&dir, &dir_type_sign);
    CU_ASSERT_EQUAL(dir_type_sign, 'd');

    bindir_descsize_get(&dir, &dir_descsize);
    CU_ASSERT_EQUAL(dir_descsize, 1);

    bindir_desc_get(&dir, dir_desc);
    CU_ASSERT_NSTRING_EQUAL(dir_desc, "a", dir_descsize);

    bindir_num_of_files_get(&dir, &dir_num_of_files);
    CU_ASSERT_EQUAL(dir_num_of_files, 1);

    bindir_file_offset_get(&dir, &dir_file_offset);
    CU_ASSERT_EQUAL(dir_file_offset, 2);

    bindir_end(&dir);
    fclose(iofp);
}

void can_read_file_header(void)
{
    FILE *iofp;

    int i;
    unsigned char bytes[100];
    size_t size;
    struct binfile file;
    char file_type_sign;
    unsigned char file_namesize;
    char file_name[100];
    unsigned short file_descsize;
    char file_desc[100];
    char file_datetime[100];
    const unsigned file_datetime_size = 14;
    unsigned long file_ctrlsum;
    char file_contentsize[100];
    size_t file_file_offset;

    iofp = tmpfile();
    if (iofp == NULL)
        CU_FAIL("can't create temporary file");

    for (i = 0; i < 100; i++)
        putc('\0', iofp);
    rewind(iofp);

    size = 33;
    memcpy(bytes,
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
           size);

    if (fwrite(bytes, 1, size, iofp) != size)
        CU_FAIL("can't prepare data in temporary file");
    rewind(iofp);

    binfile_start(&file);

    node_read_file_header(iofp, &file);

    binfile_type_get(&file, &file_type_sign);
    CU_ASSERT_EQUAL(file_type_sign, 'f');

    binfile_namesize_get(&file, &file_namesize);
    CU_ASSERT_EQUAL(file_namesize, 1);

    binfile_name_get(&file, file_name);
    CU_ASSERT_NSTRING_EQUAL(file_name, "a", file_namesize);

    binfile_descsize_get(&file, &file_descsize);
    CU_ASSERT_EQUAL(file_descsize, 1);

    binfile_desc_get(&file, file_desc);
    CU_ASSERT_NSTRING_EQUAL(file_desc, "b", file_descsize);

    binfile_datetime_get(&file, file_datetime);
    CU_ASSERT_NSTRING_EQUAL(
        file_datetime, "20170102030405", file_datetime_size);

    binfile_ctrlsum_get(&file, &file_ctrlsum);
    CU_ASSERT_EQUAL(file_ctrlsum, 0x01020304);

    binfile_contentsize_get(&file, file_contentsize);
    CU_ASSERT_STRING_EQUAL(file_contentsize, "3");

    binfile_file_offset_get(&file, &file_file_offset);
    CU_ASSERT_EQUAL(file_file_offset, 1);

    binfile_end(&file);
    fclose(iofp);
}
