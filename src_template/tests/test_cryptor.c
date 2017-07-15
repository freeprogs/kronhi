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
                    test_can_get_password_position) == NULL) {
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
