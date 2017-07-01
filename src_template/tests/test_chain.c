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
void test_can_append_file_to_empty_directory(void);
void test_can_append_file_to_non_empty_directory(void);
void test_raise_on_append_file_to_broken_directory(void);
void test_raise_on_append_file_to_broken_file(void);
void test_can_append_file_with_offset_to_directory(void);
void test_can_append_file_with_offset_to_file(void);

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
                    test_raise_on_absent_directory) == NULL
     || CU_add_test(suite2, "can append file to empty directory",
                    test_can_append_file_to_empty_directory) == NULL
     || CU_add_test(suite2, "can append file to non-empty directory",
                    test_can_append_file_to_non_empty_directory) == NULL
     || CU_add_test(suite2, "raise on append file to broken directory",
                    test_raise_on_append_file_to_broken_directory) == NULL
     || CU_add_test(suite2, "raise_on_append_file_to_broken_file",
                    test_raise_on_append_file_to_broken_file) == NULL
     || CU_add_test(suite2, "can append file with offset to directory",
                    test_can_append_file_with_offset_to_directory) == NULL
     || CU_add_test(suite2, "can append file with offset to file",
                    test_can_append_file_with_offset_to_file) == NULL) {
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

void test_can_append_file_to_empty_directory(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code chretval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *ofp_src, *ofp_dst, *ifp_dst;
    int i;
    char dirbytes[100];
    size_t dirsize;
    char filebytes[100];
    int retval;

    char dirbytes_match[100];
    size_t dirbytes_match_size;
    char filebytes_match[100];
    size_t filebytes_match_size;
    char datetime[100];

    ofp_dst = fopen(destination, "wb");
    if (ofp_dst == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', ofp_dst);
    rewind(ofp_dst);

    /* assert: destination has filled by x characters */

    dirsize = 14;
    memcpy(
        dirbytes,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x01",
        dirsize);
    if (fwrite(dirbytes, 1, dirsize, ofp_dst) != dirsize)
        CU_FAIL("can't prepare directory in temporary file");

    fclose(ofp_dst);

    /* assert: directory has been prepared and written on
       destination */

    ofp_src = fopen(source, "wb");
    if (ofp_src == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(ofp_src, "abc");
    fclose(ofp_src);

    /* assert: source file has been prepared for write */

    dirbytes_match_size = 14;
    memcpy(
        dirbytes_match,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x00",
        dirbytes_match_size);

    /* assert: directory binary data pattern for match has been
       prepared */

    filebytes_match_size = 33;
    memcpy(
        filebytes_match,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62",
        6
    );
    datetime_get_now(datetime);
    memcpy(filebytes_match + 6, datetime, 14);
    memcpy(
        filebytes_match + 6 + 14,
        "\x35\x24\x41\xC2"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x00",
        13
    );

    /* assert: source file binary data pattern for match has been
       prepared */

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 0;
    chretval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(chretval, CHAIN_OK);

    /* assert: source file has been written to directory on
       destination */

    ifp_dst = fopen(destination, "rb");
    if (ifp_dst == NULL)
        CU_FAIL("can't open destination file");

    rewind(ifp_dst);
    if (fread(dirbytes, 1, dirbytes_match_size, ifp_dst)
     != dirbytes_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        dirbytes,
        dirbytes_match,
        dirbytes_match_size
    );

    CU_ASSERT_EQUAL(((void) "directory correct" , retval), 0);

    /* assert: directory has been read from destination and matched
       with pattern */

    if (fread(filebytes, 1, filebytes_match_size, ifp_dst)
     != filebytes_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        filebytes,
        filebytes_match,
        filebytes_match_size
    );

    CU_ASSERT_EQUAL(((void) "file correct" , retval), 0);

    /* assert: source file has been read from destination and matched
       with pattern */

    fclose(ifp_dst);


    remove(source);
    remove(destination);
}

void test_can_append_file_to_non_empty_directory(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code chretval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *ofp_src, *ofp_dst, *ifp_dst;
    int i;
    char dirbytes[100];
    size_t dirsize;
    char filebytes[100];
    size_t filesize;
    int retval;

    char dirbytes_match[100];
    size_t dirbytes_match_size;
    char filebytes1_match[100];
    size_t filebytes1_match_size;
    char filebytes2_match[100];
    size_t filebytes2_match_size;
    char datetime[100];

    ofp_dst = fopen(destination, "wb");
    if (ofp_dst == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', ofp_dst);
    rewind(ofp_dst);

    /* assert: destination has filled by x characters */

    dirsize = 14;
    memcpy(
        dirbytes,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x00",
        dirsize);
    if (fwrite(dirbytes, 1, dirsize, ofp_dst) != dirsize)
        CU_FAIL("can't prepare directory in temporary file");

    /* assert: directory has been prepared and written on
       destination */

    filesize = 33;
    memcpy(
        filebytes,
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
    if (fwrite(filebytes, 1, filesize, ofp_dst) != filesize)
        CU_FAIL("can't prepare file in directory in temporary file");

    fclose(ofp_dst);

    /* assert: first file has been prepared and written to directory
       on destination */

    ofp_src = fopen(source, "wb");
    if (ofp_src == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(ofp_src, "abc");
    fclose(ofp_src);

    /* assert: source file has been prepared for write */

    dirbytes_match_size = 14;
    memcpy(
        dirbytes_match,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x02"
        "\x00\x00\x00\x00",
        dirbytes_match_size);

    /* assert: directory binary data pattern for match has been
       prepared */

    filebytes1_match_size = 33;
    memcpy(
        filebytes1_match,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62"
        "\x32\x30\x31\x37\x30\x31\x30\x32\x30\x33\x30\x34\x30\x35"
        "\x01\x02\x03\x04"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x00",
        filebytes1_match_size
    );

    /* assert: first file binary data pattern for match has been
       prepared */

    filebytes2_match_size = 33;
    memcpy(
        filebytes2_match,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62",
        6
    );
    datetime_get_now(datetime);
    memcpy(filebytes2_match + 6, datetime, 14);
    memcpy(
        filebytes2_match + 6 + 14,
        "\x35\x24\x41\xC2"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x00",
        13
    );

    /* assert: source file binary data pattern for match has been
       prepared */

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 0;
    chretval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(chretval, CHAIN_OK);

    /* assert: source file has been written to directory with file on
       destination */

    ifp_dst = fopen(destination, "rb");
    if (ifp_dst == NULL)
        CU_FAIL("can't open destination file");

    rewind(ifp_dst);
    if (fread(dirbytes, 1, dirbytes_match_size, ifp_dst)
     != dirbytes_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        dirbytes,
        dirbytes_match,
        dirbytes_match_size
    );

    CU_ASSERT_EQUAL(((void) "directory correct" , retval), 0);

    /* assert: directory has been read from destination and matched
       with pattern */

    if (fread(filebytes, 1, filebytes1_match_size, ifp_dst)
     != filebytes1_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        filebytes,
        filebytes1_match,
        filebytes1_match_size
    );

    CU_ASSERT_EQUAL(((void) "file 1 correct" , retval), 0);

    /* assert: first file has been read from destination and matched
       with pattern */

    if (fread(filebytes, 1, filebytes2_match_size, ifp_dst)
     != filebytes2_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        filebytes,
        filebytes2_match,
        filebytes2_match_size
    );

    CU_ASSERT_EQUAL(((void) "file 2 correct" , retval), 0);

    /* assert: source file has been read from destination and matched
       with pattern */

    fclose(ifp_dst);


    remove(source);
    remove(destination);
}

void test_raise_on_append_file_to_broken_directory(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code chretval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *ofp_src, *ofp_dst;
    int i;
    char dirbytes[100];
    size_t dirsize;

    ofp_dst = fopen(destination, "wb");
    if (ofp_dst == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', ofp_dst);
    rewind(ofp_dst);

    /* assert: destination has filled by x characters */

    dirsize = 14;
    memcpy(
        dirbytes,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x00",
        dirsize);
    if (fwrite(dirbytes, 1, dirsize, ofp_dst) != dirsize)
        CU_FAIL("can't prepare directory in temporary file");

    fclose(ofp_dst);

    /* assert: directory has been prepared and written on
       destination */

    ofp_src = fopen(source, "wb");
    if (ofp_src == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(ofp_src, "abc");
    fclose(ofp_src);

    /* assert: source file has been prepared for write */

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 0;
    chretval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(chretval, CHAIN_ERROR_FILE_NOFILE);

    /* assert: source file has been written to directory on
       destination */

    remove(source);
    remove(destination);
}

void test_raise_on_append_file_to_broken_file(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code chretval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *ofp_src, *ofp_dst;
    int i;
    char dirbytes[100];
    size_t dirsize;
    char filebytes[100];
    size_t filesize;

    ofp_dst = fopen(destination, "wb");
    if (ofp_dst == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', ofp_dst);
    rewind(ofp_dst);

    dirsize = 14;
    memcpy(
        dirbytes,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x02"
        "\x00\x00\x00\x00",
        dirsize);
    if (fwrite(dirbytes, 1, dirsize, ofp_dst) != dirsize)
        CU_FAIL("can't prepare directory in temporary file");

    filesize = 33;
    memcpy(
        filebytes,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62"
        "\x32\x30\x31\x37\x30\x31\x30\x32\x30\x33\x30\x34\x30\x35"
        "\x01\x02\x03\x04"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x00",
        filesize);
    if (fwrite(filebytes, 1, filesize, ofp_dst) != filesize)
        CU_FAIL("can't prepare file in directory in temporary file");

    fclose(ofp_dst);

    ofp_src = fopen(source, "wb");
    if (ofp_src == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(ofp_src, "abc");
    fclose(ofp_src);

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 0;
    chretval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(chretval, CHAIN_ERROR_FILE_NOFILE);

    remove(source);
    remove(destination);
}

void test_can_append_file_with_offset_to_directory(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code chretval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *ofp_src, *ofp_dst, *ifp_dst;
    int i;
    char dirbytes[100];
    size_t dirsize;
    char filebytes[100];
    int retval;

    char dirbytes_match[100];
    size_t dirbytes_match_size;
    char filebytes_match[100];
    size_t filebytes_match_size;
    char datetime[100];

    ofp_dst = fopen(destination, "wb");
    if (ofp_dst == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', ofp_dst);
    rewind(ofp_dst);

    dirsize = 14;
    memcpy(
        dirbytes,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x01",
        dirsize);
    if (fwrite(dirbytes, 1, dirsize, ofp_dst) != dirsize)
        CU_FAIL("can't prepare directory in temporary file");

    fclose(ofp_dst);

    ofp_src = fopen(source, "wb");
    if (ofp_src == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(ofp_src, "abc");
    fclose(ofp_src);


    dirbytes_match_size = 14;
    memcpy(
        dirbytes_match,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x0A",
        dirbytes_match_size);

    filebytes_match_size = 33;
    memcpy(
        filebytes_match,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62",
        6
    );
    datetime_get_now(datetime);
    memcpy(filebytes_match + 6, datetime, 14);
    memcpy(
        filebytes_match + 6 + 14,
        "\x35\x24\x41\xC2"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x00",
        13
    );

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 10;
    chretval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(chretval, CHAIN_OK);


    ifp_dst = fopen(destination, "rb");
    if (ifp_dst == NULL)
        CU_FAIL("can't open destination file");

    rewind(ifp_dst);
    if (fread(dirbytes, 1, dirbytes_match_size, ifp_dst)
     != dirbytes_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        dirbytes,
        dirbytes_match,
        dirbytes_match_size
    );

    CU_ASSERT_EQUAL(((void) "directory correct" , retval), 0);

    for (i = 0; i < filereloff; i++)
        getc(ifp_dst);

    if (fread(filebytes, 1, filebytes_match_size, ifp_dst)
     != filebytes_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        filebytes,
        filebytes_match,
        filebytes_match_size
    );

    CU_ASSERT_EQUAL(((void) "file correct" , retval), 0);

    fclose(ifp_dst);


    remove(source);
    remove(destination);
}

void test_can_append_file_with_offset_to_file(void)
{
    struct chain chain;
    const char *destination = "file.txt";
    struct file_offset offset;
    enum chain_code chretval;

    const char *source = "source.txt";
    const char *filename;
    const char *filedesc;
    size_t filereloff;

    FILE *ofp_src, *ofp_dst, *ifp_dst;
    int i;
    char dirbytes[100];
    size_t dirsize;
    char filebytes[100];
    size_t filesize;
    int retval;

    char dirbytes_match[100];
    size_t dirbytes_match_size;
    char filebytes1_match[100];
    size_t filebytes1_match_size;
    char filebytes2_match[100];
    size_t filebytes2_match_size;
    char datetime[100];

    ofp_dst = fopen(destination, "wb");
    if (ofp_dst == NULL)
        CU_FAIL("can't create temporary file");
    for (i = 0; i < 100; i++)
        putc('x', ofp_dst);
    rewind(ofp_dst);

    dirsize = 14;
    memcpy(
        dirbytes,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x00",
        dirsize);
    if (fwrite(dirbytes, 1, dirsize, ofp_dst) != dirsize)
        CU_FAIL("can't prepare directory in temporary file");

    filesize = 33;
    memcpy(
        filebytes,
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
    if (fwrite(filebytes, 1, filesize, ofp_dst) != filesize)
        CU_FAIL("can't prepare file in directory in temporary file");

    fclose(ofp_dst);

    ofp_src = fopen(source, "wb");
    if (ofp_src == NULL)
        CU_FAIL("can't create temporary file");
    fprintf(ofp_src, "abc");
    fclose(ofp_src);


    dirbytes_match_size = 14;
    memcpy(
        dirbytes_match,
        "\x64"
        "\x00\x03"
        "\x61\x62\x63"
        "\x00\x00\x00\x02"
        "\x00\x00\x00\x00",
        dirbytes_match_size);

    filebytes1_match_size = 33;
    memcpy(
        filebytes1_match,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62"
        "\x32\x30\x31\x37\x30\x31\x30\x32\x30\x33\x30\x34\x30\x35"
        "\x01\x02\x03\x04"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x0A",
        filebytes1_match_size
    );

    filebytes2_match_size = 33;
    memcpy(
        filebytes2_match,
        "\x66"
        "\x01"
        "\x61"
        "\x00\x01"
        "\x62",
        6
    );
    datetime_get_now(datetime);
    memcpy(filebytes2_match + 6, datetime, 14);
    memcpy(
        filebytes2_match + 6 + 14,
        "\x35\x24\x41\xC2"
        "\x33\x00"
        "\x61\x62\x63"
        "\x00\x00\x00\x00",
        13
    );

    fileoffset_clear(&offset);
    chain_start(&chain, destination, &offset);

    filename = "a";
    filedesc = "b";
    filereloff = 10;
    chretval = chain_append_file(
        &chain, source, filename, filedesc, filereloff);

    chain_end(&chain);

    CU_ASSERT_EQUAL(chretval, CHAIN_OK);


    ifp_dst = fopen(destination, "rb");
    if (ifp_dst == NULL)
        CU_FAIL("can't open destination file");

    rewind(ifp_dst);
    if (fread(dirbytes, 1, dirbytes_match_size, ifp_dst)
     != dirbytes_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        dirbytes,
        dirbytes_match,
        dirbytes_match_size
    );

    CU_ASSERT_EQUAL(((void) "directory correct" , retval), 0);

    if (fread(filebytes, 1, filebytes1_match_size, ifp_dst)
     != filebytes1_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        filebytes,
        filebytes1_match,
        filebytes1_match_size
    );

    CU_ASSERT_EQUAL(((void) "file 1 correct" , retval), 0);

    for (i = 0; i < filereloff; i++)
        getc(ifp_dst);

    if (fread(filebytes, 1, filebytes2_match_size, ifp_dst)
     != filebytes2_match_size)
        CU_FAIL("can't read destination file");
    retval = memcmp(
        filebytes,
        filebytes2_match,
        filebytes2_match_size
    );

    CU_ASSERT_EQUAL(((void) "file 2 correct" , retval), 0);

    fclose(ifp_dst);


    remove(source);
    remove(destination);
}
