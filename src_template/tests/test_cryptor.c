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
#include "../cryptor.h"

void test_can_set_password_position(void);
void test_can_get_password_position(void);
void test_can_get_encryption_algorithm(void);
void test_can_shift_password_position_to_right(void);
void test_can_encrypt_bytes(void);
void test_can_encrypt_bytes_by_several_calls(void);

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

    if (CU_add_test(suite, "can set password position",
                    test_can_set_password_position) == NULL
     || CU_add_test(suite, "can get password position",
                    test_can_get_password_position) == NULL
     || CU_add_test(suite, "can get encryption algorithm",
                    test_can_get_encryption_algorithm) == NULL
     || CU_add_test(suite, "can shift password position to right",
                    test_can_shift_password_position_to_right) == NULL
     || CU_add_test(suite, "can encrypt bytes",
                    test_can_encrypt_bytes) == NULL
     || CU_add_test(suite, "can encrypt bytes by several calls",
                    test_can_encrypt_bytes_by_several_calls) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}

void test_can_set_password_position(void)
{
    struct cryptor cryptor;
    unsigned char psw[] = {'a', 'b', 'c'};

    size_t out;

    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        sizeof psw);
    cryptor_pos_set(&cryptor, 1);
    out = cryptor.pos;
    cryptor_end(&cryptor);

    CU_ASSERT_EQUAL(out, 1);
}

void test_can_get_password_position(void)
{
    struct cryptor cryptor;
    unsigned char psw[] = {'a', 'b', 'c'};

    size_t out;

    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        sizeof psw);
    cryptor_pos_set(&cryptor, 1);
    out = 0;
    cryptor_pos_get(&cryptor, &out);
    cryptor_end(&cryptor);

    CU_ASSERT_EQUAL(out, 1);
}

void test_can_get_encryption_algorithm(void)
{
    struct cryptor cryptor;
    unsigned char psw[] = {'a', 'b', 'c'};

    enum cryptor_algorithm out;

    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        sizeof psw);
    out = CRYPTOR_ALGORITHM_NONE;
    cryptor_algo_get(&cryptor, &out);
    cryptor_end(&cryptor);

    CU_ASSERT_EQUAL(out, CRYPTOR_ALGORITHM_XOR);
}

void test_can_shift_password_position_to_right(void)
{
    struct cryptor cryptor;
    unsigned char psw[] = {'a', 'b', 'c'};

    size_t out;

    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        sizeof psw);

    cryptor_pos_set(&cryptor, 0);
    cryptor_pos_rshift(&cryptor, 0);
    out = 0;
    cryptor_pos_get(&cryptor, &out);
    CU_ASSERT_EQUAL(out, 0);

    cryptor_pos_set(&cryptor, 0);
    cryptor_pos_rshift(&cryptor, 1);
    out = 0;
    cryptor_pos_get(&cryptor, &out);
    CU_ASSERT_EQUAL(out, 1);

    cryptor_pos_set(&cryptor, 0);
    cryptor_pos_rshift(&cryptor, 2);
    out = 0;
    cryptor_pos_get(&cryptor, &out);
    CU_ASSERT_EQUAL(out, 2);

    cryptor_pos_set(&cryptor, 0);
    cryptor_pos_rshift(&cryptor, 3);
    out = 0;
    cryptor_pos_get(&cryptor, &out);
    CU_ASSERT_EQUAL(out, 0);

    cryptor_pos_set(&cryptor, 0);
    cryptor_pos_rshift(&cryptor, 4);
    out = 0;
    cryptor_pos_get(&cryptor, &out);
    CU_ASSERT_EQUAL(out, 1);

    cryptor_end(&cryptor);
}

void test_can_encrypt_bytes(void)
{
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    unsigned char ibuffer[100];
    size_t isize;
    unsigned char obuffer[200];
    size_t osize;
    unsigned char obuffer_match[200];
    size_t osize_match;

    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        pswlen);

    isize = 3;
    memcpy(ibuffer, "123", isize);
    osize_match = 3;
    memcpy(obuffer_match, "PPP", osize_match);

    osize = 0;
    memset(obuffer, 0, isize);
    cryptor_encrypt(&cryptor, ibuffer, isize, obuffer, &osize);
    CU_ASSERT_EQUAL(osize, osize_match);
    CU_ASSERT_NSTRING_EQUAL(
        ((void) "string: <123> password: <abc>" , obuffer),
        obuffer_match, osize);

    cryptor_end(&cryptor);


    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        pswlen);

    isize = 6;
    memcpy(ibuffer, "123456", isize);
    osize_match = 6;
    memcpy(obuffer_match, "PPPUWU", osize_match);

    osize = 0;
    memset(obuffer, 0, isize);
    cryptor_encrypt(&cryptor, ibuffer, isize, obuffer, &osize);
    CU_ASSERT_EQUAL(osize, osize_match);
    CU_ASSERT_NSTRING_EQUAL(
        ((void) "string: <123456> password: <abc>" , obuffer),
        obuffer_match, osize);

    cryptor_end(&cryptor);
}

void test_can_encrypt_bytes_by_several_calls(void)
{
    struct cryptor cryptor;
    unsigned char psw[100] = {'a', 'b', 'c'};
    size_t pswlen = 3;

    unsigned char ibuffer[100];
    size_t isize;
    unsigned char obuffer[200];
    size_t osize;
    unsigned char obuffer_match[200];
    size_t osize_match;

    cryptor_start(
        &cryptor,
        CRYPTOR_ALGORITHM_XOR,
        psw,
        pswlen);

    isize = 2;
    memcpy(ibuffer, "12", isize);
    osize_match = 2;
    memcpy(obuffer_match, "PP", osize_match);

    osize = 0;
    memset(obuffer, 0, isize);
    cryptor_encrypt(&cryptor, ibuffer, isize, obuffer, &osize);
    CU_ASSERT_EQUAL(osize, osize_match);
    CU_ASSERT_NSTRING_EQUAL(
        ((void) "string: <12> password: <abc> from a" , obuffer),
        obuffer_match, osize);

    isize = 2;
    memcpy(ibuffer, "12", isize);
    osize_match = 2;
    memcpy(obuffer_match, "RS", osize_match);

    osize = 0;
    memset(obuffer, 0, isize);
    cryptor_encrypt(&cryptor, ibuffer, isize, obuffer, &osize);
    CU_ASSERT_EQUAL(osize, osize_match);
    CU_ASSERT_NSTRING_EQUAL(
        ((void) "string: <12> password: <abc> from c" , obuffer),
        obuffer_match, osize);

    cryptor_end(&cryptor);
}
