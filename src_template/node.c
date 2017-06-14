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

#include "node.h"

/* node_write_dir: write directory node to output stream
                   return 1 if has written correctly
                   return 0 if errors happened */
int node_write_dir(FILE *ofp, const struct bindir *dir)
{
    int f_error;

    f_error = 0;

    if (!binfield_raw_write(dir->type_sign, ofp))
        f_error = 1;
    if(!binfield_num_write(dir->descsize, ofp))
        f_error = 1;
    if(!binfield_raw_write(dir->desc, ofp))
        f_error = 1;
    if(!binfield_num_write(dir->num_of_files, ofp))
        f_error = 1;
    if(!binfield_num_write(dir->file_offset, ofp))
        f_error = 1;

    if (f_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}

/* node_write_file: write file node to output stream
                    return 1 if has written correctly
                    return 0 if errors happened */
int node_write_file(FILE *ofp, const struct binfile *file)
{
    int f_error, f_file_to_file_error;

    f_error = f_file_to_file_error = 0;

    if(!binfield_raw_write(file->type_sign, ofp))
        f_error = 1;
    if(!binfield_num_write(file->namesize, ofp))
        f_error = 1;
    if(!binfield_raw_write(file->name, ofp))
        f_error = 1;
    if(!binfield_num_write(file->descsize, ofp))
        f_error = 1;
    if(!binfield_raw_write(file->desc, ofp))
        f_error = 1;
    if(!binfield_raw_write(file->datetime, ofp))
        f_error = 1;
    if(!binfield_num_write(file->ctrlsum, ofp))
        f_error = 1;
    if(!binfield_raw_write(file->contentsize, ofp))
        f_error = 1;
    if (!file_write_file(ofp, file->contentstream))
        f_file_to_file_error = 1;
    if (!binfield_num_write(file->file_offset, ofp))
        f_error = 1;

    if (f_error || f_file_to_file_error)
        return 0;
    if (ferror(ofp))
        return 0;
    return 1;
}
