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

#include "bignumber.h"

/* bignumber_set_value_string:
   set value of big number from a string
   return 1 if has set
   return 0 if string is too long or it's not a number */
int bignumber_set_value_string(struct bignumber *number, const char *str)
{
    int retval;

    if (strlen(str) > BIG_MAXSTRING)
        return 0;
    retval = sscanf(str, "%lf", &number->number);
    return retval == 1;
}

/* bignumber_set_value_int:
   set value of big number from an signed integer
   return 1 if has set
   return 0 if has set incorrectly */
int bignumber_set_value_int(struct bignumber *number, int value)
{
    number->number = value;
    return number->number == value;
}

/* bignumber_tostr: convert big number to a string
                    return output string */
char *bignumber_tostr(const struct bignumber *number, char out[])
{
    sprintf(out, "%.0f", number->number);
    return out;
}

/* bignumber_lt_big:
   test if left big number is less than right big number
   return 1 if left big number is less than right big number
   return 0 if left big number is greater or equal to right big number */
int bignumber_lt_big(const struct bignumber *number_left,
                     const struct bignumber *number_right)
{
    return number_left->number < number_right->number;
}

/* bignumber_add_int: add a signed integer to the big number
                      return 1 if has added
                      return 0 if has not added */
int bignumber_add_int(struct bignumber *number, int value)
{
    double prev;

    prev = number->number;
    number->number += value;
    return number->number - value == prev;
}
