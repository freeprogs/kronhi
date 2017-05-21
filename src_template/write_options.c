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

#include "write_options.h"

/* write_options_init: set write options from strings
                       return 0 when wrong values
                       return 1 when right values */
int write_options_init(
    struct write_options *opts,
    const char *src, const char *dst,
    const char *offset, const char *cipher)
{
    int retval;
    int f_bad_offset, f_bad_cipher;

    f_bad_offset = f_bad_cipher = 0;

    strcpy(opts->src, src);
    strcpy(opts->dst, dst);
    if (!fileoffset_fromstring(&opts->offset, offset)) {
        f_bad_offset = 1;
    }
    if (strcmp(cipher, "xor") == 0) {
        opts->cipher = W_CIPHER_XOR;
    }
    else if (strcmp(cipher, "none") == 0) {
        opts->cipher = W_CIPHER_NONE;
    }
    else {
        f_bad_cipher = 1;
    }
    retval = !(f_bad_offset || f_bad_cipher);
    return retval;
}

/* write_options_clear: clear write options */
void write_options_clear(struct write_options *opts)
{
    *opts->src = '\0';
    *opts->dst = '\0';
    fileoffset_clear(&opts->offset);
    opts->cipher = W_CIPHER_NONE;
}

/* write_options_tostr_source: convert source option to string
                               return output string */
char *write_options_tostr_source(struct write_options *opts, char out[])
{
    return strcpy(out, opts->src);
}

/* write_options_tostr_destination: convert destination option to string
                                    return output string */
char *write_options_tostr_destination(struct write_options *opts, char out[])
{
    return strcpy(out, opts->dst);
}

/* write_options_tostr_offset: convert offset option to string
                               return output string */
char *write_options_tostr_offset(struct write_options *opts, char out[])
{
    return fileoffset_tostr(&opts->offset, out);
}

/* write_options_tostr_cipher: convert cipher option to string
                               return output string */
char *write_options_tostr_cipher(struct write_options *opts, char out[])
{
    if (opts->cipher == W_CIPHER_XOR)
        sprintf(out, "%s", "xor");
    else if (opts->cipher == W_CIPHER_NONE)
        sprintf(out, "%s", "none");
    else
        sprintf(out, "%s", "undefined");
    return out;
}


/* write_options_source_get: get source option
                             return source option */
char *write_options_source_get(struct write_options *opts, char out[])
{
    return strcpy(out, opts->src);
}

/* write_options_destination_get: get destination option
                                  return destination option */
char *write_options_destination_get(struct write_options *opts, char out[])
{
    return strcpy(out, opts->dst);
}

/* write_options_offset_get: get offset option
                             return offset option */
struct file_offset write_options_offset_get(struct write_options *opts)
{
    return opts->offset;
}

/* write_options_cipher_get: get cipher option
                             return cipher option */
enum write_cipher_type write_options_cipher_get(struct write_options *opts)
{
    return opts->cipher;
}
