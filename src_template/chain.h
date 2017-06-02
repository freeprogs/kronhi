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

#ifndef CHAIN_H
#define CHAIN_H

#include <stdio.h>
#include <string.h>
#include "file_offset.h"
#include "bindir.h"
#include "file_operation.h"
#include "bignumber.h"
#include "node.h"

enum chain_code {
    CHAIN_ERROR_DIR_OPENFILE,
    CHAIN_ERROR_DIR_SKIPOFFSET,
    CHAIN_ERROR_DIR_FILESIZE,
    CHAIN_ERROR_DIR_WRITENODE,
    CHAIN_ERROR_DIR_WRITEFILE,
    CHAIN_ERROR_DIR_FILESYS,
    CHAIN_ERROR_FILE_DIRENTRY,
    CHAIN_OK
};

struct chain {
    const char *dst;
    const struct file_offset *start;
};

void chain_start(
    struct chain *self,
    const char *dst,
    const struct file_offset *start);
enum chain_code chain_create_dir(
    const struct chain *self,
    const char *dirdesc,
    unsigned num_of_files,
    size_t relative_offset);
enum chain_code chain_append_file(
    const struct chain *self,
    const char *source,
    const char *filename,
    const char *filedesc);
void chain_end(struct chain *self);

#endif
