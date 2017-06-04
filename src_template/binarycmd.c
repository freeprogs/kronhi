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

/* binarycmd_write_dir:
   write directory to the destination file with offset
   return success code if written without errors
   return error code if was not written or was written with errors */
enum binarycmd_code binarycmd_write_dir(
    const char *destination, const struct file_offset *offset,
    const char *dirdesc, enum write_cipher_type cipher)
{
    struct chain chain;
    enum chain_code chret;

    chain_start(&chain, destination, offset);
    chret = chain_create_dir(&chain, dirdesc, 0, 0);
    chain_end(&chain);

    if (chret == CHAIN_ERROR_DIR_OPENFILE)
        return BINCMD_ERROR_DIR_OPENFILE;
    if (chret == CHAIN_ERROR_DIR_SKIPOFFSET)
        return BINCMD_ERROR_DIR_SKIPOFFSET;
    if (chret == CHAIN_ERROR_DIR_FILESIZE)
        return BINCMD_ERROR_DIR_FILESIZE;
    if (chret == CHAIN_ERROR_DIR_WRITENODE)
        return BINCMD_ERROR_DIR_WRITENODE;
    if (chret == CHAIN_ERROR_DIR_WRITEFILE)
        return BINCMD_ERROR_DIR_WRITEFILE;
    if (chret == CHAIN_ERROR_DIR_FILESYS)
        return BINCMD_ERROR_DIR_FILESYS;
    return BINCMD_OK;
}

/* binarycmd_write_file:
   write file to directory in the destination file with offset
   return success code if written without errors
   return error code if was not written or was written with errors */
enum binarycmd_code binarycmd_write_file(
    const char *source,
    const char *destination,
    const struct file_offset *offset,
    const char *filename,
    const char *filedesc,
    size_t filereloff,
    enum write_cipher_type cipher)
{
    struct chain chain;
    enum chain_code chret;

    chain_start(&chain, destination, offset);
    chret = chain_append_file(&chain, source, filename, filedesc, filereloff);
    chain_end(&chain);

    if (chret == CHAIN_ERROR_FILE_DIRENTRY)
        return BINCMD_ERROR_FILE_DIRENTRY;
    return BINCMD_OK;
}
