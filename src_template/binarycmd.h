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
#include "file_offset.h"
#include "chain.h"

enum binarycmd_code {
    BINCMD_ERROR_DIR_OPENFILE,
    BINCMD_ERROR_DIR_SKIPOFFSET,
    BINCMD_ERROR_DIR_FILESIZE,
    BINCMD_ERROR_DIR_WRITENODE,
    BINCMD_ERROR_DIR_WRITEFILE,
    BINCMD_ERROR_DIR_FILESYS,
    BINCMD_ERROR_FILE_DIRENTRY,
    BINCMD_OK
};

enum binarycmd_code binarycmd_write_dir(
    const char *destination, const struct file_offset *offset,
    const char *dirdesc, enum write_cipher_type cipher);
enum binarycmd_code binarycmd_write_file(
    const char *source,
    const char *destination,
    const struct file_offset *offset,
    const char *filename,
    const char *filedesc,
    size_t filereloff,
    enum write_cipher_type cipher);

#endif
