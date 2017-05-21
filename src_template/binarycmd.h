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

#ifndef BINARYCMD_H
#define BINARYCMD_H

#include <stdio.h>
#include "write_options.h"
#include "bindir.h"
#include "file_operation.h"
#include "file_offset.h"

/* maximum length of writable directory header */
#define BINDIR_MAXHEADER  1 + 2 + 65535 + 4 + 4

enum binarycmd_code {
    BINCMD_ERROR_DIR_MEMORY,
    BINCMD_ERROR_DIR_HEADER,
    BINCMD_ERROR_FILE_NOFILE,
    BINCMD_ERROR_FILE_PERM_WRITE,
    BINCMD_ERROR_FILE_SIZE,
    BINCMD_ERROR_FILE_WRITE,
    BINCMD_OK
};

int binarycmd_write_dir(
    const char *destination, const struct file_offset *offset,
    const char *dirdesc, enum write_cipher_type cipher);

#endif
