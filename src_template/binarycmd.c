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

#include "binarycmd.h"

enum binarycmd_code binarycmd_write_dir(
    const char *destination, const struct file_offset *offset,
    const char *dirdesc, enum write_cipher_type cipher)
{
    struct chain chain;
    enum chain_code retval;

    chain_start(&chain, destination, offset);
    retval = chain_create_dir(&chain, dirdesc, 0, 0);
    chain_end(&chain);

    if (retval == CHAIN_ERROR_DIR_MEMORY)
        return BINCMD_ERROR_DIR_MEMORY;
    if (retval == CHAIN_ERROR_DIR_HEADER)
        return BINCMD_ERROR_DIR_HEADER;
    if (retval == CHAIN_ERROR_DIR_NOFILE)
        return BINCMD_ERROR_DIR_NOFILE;
    if (retval == CHAIN_ERROR_DIR_FILE_PERM_WRITE)
        return BINCMD_ERROR_DIR_FILE_PERM_WRITE;
    if (retval == CHAIN_ERROR_DIR_FILE_SIZE)
        return BINCMD_ERROR_DIR_FILE_SIZE;
    if (retval == CHAIN_ERROR_DIR_FILE_WRITE)
        return BINCMD_ERROR_DIR_FILE_WRITE;
    return BINCMD_OK;
}
