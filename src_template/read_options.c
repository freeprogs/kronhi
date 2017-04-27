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

#include "read_options.h"

/* read_options_init: set read options from strings
                      return 0 when wrong values
                      return 1 when right values */
int read_options_init(
    struct read_options *popts,
    const char *src, const char *dst,
    const char *offset, const char *cipher)
{
    int retval;
    int f_bad_offset, f_bad_cipher;

    f_bad_offset = f_bad_cipher = 0;

    strcpy(popts->src, src);
    strcpy(popts->dst, dst);
    if (sscanf(offset, "%lu", (unsigned long *) &popts->offset) != 1) {
        f_bad_offset = 1;
    }
    if (strcmp(cipher, "xor") == 0) {
        popts->cipher = R_CIPHER_XOR;
    }
    else if (strcmp(cipher, "none") == 0) {
        popts->cipher = R_CIPHER_NONE;
    }
    else {
        f_bad_cipher = 1;
    }
    retval = !(f_bad_offset || f_bad_cipher);
    return retval;
}

/* read_options_clear: clear read options */
void read_options_clear(struct read_options *popts)
{
    *popts->src = '\0';
    *popts->dst = '\0';
    popts->offset = 0;
    popts->cipher = R_CIPHER_NONE;
}

/* read_options_tostr_source: convert source option to string */
char *read_options_tostr_source(struct read_options *popts)
{
    static char out[READ_OPTIONS_MAXREPR];
    strcpy(out, popts->src);
    return out;
}

/* read_options_tostr_destination: convert destination option to string */
char *read_options_tostr_destination(struct read_options *popts)
{
    static char out[READ_OPTIONS_MAXREPR];
    strcpy(out, popts->dst);
    return out;
}

/* read_options_tostr_offset: convert offset option to string */
char *read_options_tostr_offset(struct read_options *popts)
{
    static char out[READ_OPTIONS_MAXREPR];
    sprintf(out, "%lu", (unsigned long) popts->offset);
    return out;
}

/* read_options_tostr_cipher: convert cipher option to string*/
char *read_options_tostr_cipher(struct read_options *popts)
{
    static char out[READ_OPTIONS_MAXREPR];

    if (popts->cipher == R_CIPHER_XOR)
        sprintf(out, "%s", "xor");
    else if (popts->cipher == R_CIPHER_NONE)
        sprintf(out, "%s", "none");
    else
        sprintf(out, "%s", "undefined");
    return out;
}
