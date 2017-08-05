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
#include "binfile.h"
#include "datetime.h"
#include "file_operation.h"
#include "bignumber.h"
#include "node.h"
#include "jumper.h"

enum chain_code {
    CHAIN_ERROR_DIR_OPENFILE,
    CHAIN_ERROR_DIR_SKIPOFFSET,
    CHAIN_ERROR_DIR_FILESIZE,
    CHAIN_ERROR_DIR_WRITENODE,
    CHAIN_ERROR_DIR_WRITEFILE,
    CHAIN_ERROR_DIR_FILESYS,
    CHAIN_ERROR_FILE_DIRENTRY,
    CHAIN_ERROR_FILE_OPENFILE,
    CHAIN_ERROR_FILE_SKIPOFFSET,
    CHAIN_ERROR_FILE_NODIR,
    CHAIN_ERROR_FILE_FILESIZE,
    CHAIN_ERROR_FILE_READDIRHEADER,
    CHAIN_ERROR_FILE_DIRGETNOF,
    CHAIN_ERROR_FILE_WRITENODE,
    CHAIN_ERROR_FILE_DIRGETOFFSET,
    CHAIN_ERROR_FILE_NOFILE,
    CHAIN_ERROR_FILE_READFILEHEADER,
    CHAIN_ERROR_FILE_FILEGETOFFSET,
    CHAIN_ERROR_FILE_WRITEFILE,
    CHAIN_ERROR_FILE_FILESYS,
    CHAIN_ERROR_FILE_OPENSOURCE,
    CHAIN_ERROR_FILE_READSOURCE,
    CHAIN_ERROR_FILE_SOURCESYS,
    CHAIN_OK
};

struct chain {
    const char *dst;
    const struct file_offset *start;
    struct node *node;
};

void chain_start(
    struct chain *self,
    const char *dst,
    const struct file_offset *start,
    struct node *node);
enum chain_code chain_create_dir(
    const struct chain *self,
    const char *dirdesc,
    unsigned num_of_files,
    size_t relative_offset);
enum chain_code chain_append_file(
    const struct chain *self,
    const char *source,
    const char *filename,
    const char *filedesc,
    size_t filereloff);
void chain_end(struct chain *self);

#endif
