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
    const char *dirdesc, enum write_cipher_type cipher, const char *password)
{
    struct chain chain;
    enum chain_code chret;
    enum binarycmd_code retval;

    chain_start(&chain, destination, offset);
    chret = chain_create_dir(&chain, dirdesc, 0, 0);
    chain_end(&chain);

    switch (chret) {
    case CHAIN_ERROR_DIR_OPENFILE:
        retval = BINCMD_ERROR_DIR_OPENFILE;
        break;
    case CHAIN_ERROR_DIR_SKIPOFFSET:
        retval = BINCMD_ERROR_DIR_SKIPOFFSET;
        break;
    case CHAIN_ERROR_DIR_FILESIZE:
        retval = BINCMD_ERROR_DIR_FILESIZE;
        break;
    case CHAIN_ERROR_DIR_WRITENODE:
        retval = BINCMD_ERROR_DIR_WRITENODE;
        break;
    case CHAIN_ERROR_DIR_WRITEFILE:
        retval = BINCMD_ERROR_DIR_WRITEFILE;
        break;
    case CHAIN_ERROR_DIR_FILESYS:
        retval = BINCMD_ERROR_DIR_FILESYS;
        break;
    default:
        retval = BINCMD_OK;
        break;
    }
    return retval;
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
    enum binarycmd_code retval;

    chain_start(&chain, destination, offset);
    chret = chain_append_file(&chain, source, filename, filedesc, filereloff);
    chain_end(&chain);

    switch (chret) {
    case CHAIN_ERROR_FILE_DIRENTRY:
        retval = BINCMD_ERROR_FILE_DIRENTRY;
        break;
    case CHAIN_ERROR_FILE_OPENFILE:
        retval = BINCMD_ERROR_FILE_OPENFILE;
        break;
    case CHAIN_ERROR_FILE_SKIPOFFSET:
        retval = BINCMD_ERROR_FILE_SKIPOFFSET;
        break;
    case CHAIN_ERROR_FILE_NODIR:
        retval = BINCMD_ERROR_FILE_NODIR;
        break;
    case CHAIN_ERROR_FILE_FILESIZE:
        retval = BINCMD_ERROR_FILE_FILESIZE;
        break;
    case CHAIN_ERROR_FILE_READDIRHEADER:
        retval = BINCMD_ERROR_FILE_READDIRHEADER;
        break;
    case CHAIN_ERROR_FILE_DIRGETNOF:
        retval = BINCMD_ERROR_FILE_DIRGETNOF;
        break;
    case CHAIN_ERROR_FILE_WRITENODE:
        retval = BINCMD_ERROR_FILE_WRITENODE;
        break;
    case CHAIN_ERROR_FILE_DIRGETOFFSET:
        retval = BINCMD_ERROR_FILE_DIRGETOFFSET;
        break;
    case CHAIN_ERROR_FILE_NOFILE:
        retval = BINCMD_ERROR_FILE_NOFILE;
        break;
    case CHAIN_ERROR_FILE_READFILEHEADER:
        retval = BINCMD_ERROR_FILE_READFILEHEADER;
        break;
    case CHAIN_ERROR_FILE_FILEGETOFFSET:
        retval = BINCMD_ERROR_FILE_FILEGETOFFSET;
        break;
    case CHAIN_ERROR_FILE_WRITEFILE:
        retval = BINCMD_ERROR_FILE_WRITEFILE;
        break;
    case CHAIN_ERROR_FILE_FILESYS:
        retval = BINCMD_ERROR_FILE_FILESYS;
        break;
    case CHAIN_ERROR_FILE_OPENSOURCE:
        retval = BINCMD_ERROR_FILE_OPENSOURCE;
        break;
    case CHAIN_ERROR_FILE_READSOURCE:
        retval = BINCMD_ERROR_FILE_READSOURCE;
        break;
    case CHAIN_ERROR_FILE_SOURCESYS:
        retval = BINCMD_ERROR_FILE_SOURCESYS;
        break;
    default:
        retval = BINCMD_OK;
        break;
    }
    return retval;
}
