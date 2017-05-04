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

int binarycmd_write_dir(
    char destination[], size_t offset,
    char dirdesc[], enum write_cipher_type cipher)
{
    struct bindir *dir;
    unsigned char dirheader[BINDIR_MAXHEADER];
    size_t dirheadersize;

    dir = bindir_create();
    bindir_desc_set(dir, dirdesc);
    bindir_num_of_files_set(dir, 0);
    bindir_file_offset_set(dir, 0);
    bindir_print(dir);
    dirheadersize = bindir_make_bin_header(dir, dirheader);
    hexdump_dump(dirheader, dirheadersize);
    bindir_free(dir);

    return 1;
}
