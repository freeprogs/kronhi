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

#include "chain.h"

/* chain_start: start chain and set internal values */
void chain_start(
    struct chain *self,
    const char *dst,
    const struct file_offset *start)
{
    self->dst = dst;
    self->start = start;
}

/* chain_create_dir:
   make new directory on destination file with start offset
   return chain success code if no errors happened
   return chain error code if an error happened */
enum chain_code chain_create_dir(
    const struct chain *self,
    const char *dirdesc,
    unsigned num_of_files,
    size_t relative_offset)
{
    struct bindir dir;
    FILE *ofp;
    struct bignumber dirsize;

    bindir_start(&dir);
    bindir_type_set(&dir, 'd');
    bindir_descsize_set(&dir, strlen(dirdesc));
    bindir_desc_set(&dir, dirdesc);
    bindir_num_of_files_set(&dir, 0);
    bindir_file_offset_set(&dir, 0);
    ofp = fopen(self->dst, "r+b");
    if (ofp == NULL)
        return CHAIN_ERROR_DIR_OPENFILE;
    if (!file_skip_to_offset(ofp, self->start)) {
        fclose(ofp);
        return CHAIN_ERROR_DIR_SKIPOFFSET;
    }
    if (!bignumber_set_value_int(&dirsize, bindir_get_size(&dir))) {
        fclose(ofp);
        return CHAIN_ERROR_DIR_FILESIZE;
    }
    if (!file_test_write_size(ofp, &dirsize)) {
        fclose(ofp);
        return CHAIN_ERROR_DIR_FILESIZE;
    }
    if (!node_write_dir(ofp, &dir)) {
        fclose(ofp);
        return CHAIN_ERROR_DIR_WRITENODE;
    }
    if (ferror(ofp)) {
        fclose(ofp);
        return CHAIN_ERROR_DIR_WRITEFILE;
    }
    if (fclose(ofp) != 0)
        return CHAIN_ERROR_DIR_FILESYS;
    bindir_end(&dir);
    return CHAIN_OK;
}

/* chain_append_file:
   append file to the chain on destination file with start offset
   return chain success code if no errors happened
   return chain error code is an error happened */
enum chain_code chain_append_file(
    const struct chain *self,
    const char *source,
    const char *filename,
    const char *filedesc,
    size_t filereloff)
{
    struct binfile file;
    FILE *ofp;
    FILE *ifp;
    unsigned long ctrlsum;
    struct bignumber filesize;
    char filesizestr[100];
    char datetime[DATETIMESIZE];

    fpos_t dirpos;
    struct bindir dirtmp;
    size_t tmp_num_of_files;

    fpos_t filepos;
    struct binfile filetmp;
    size_t tmp_dir_rel_off;

    size_t tmp_file_rel_off;
    size_t i;


    ofp = fopen(self->dst, "r+b");
    if (ofp == NULL)
        return CHAIN_ERROR_FILE_OPENFILE;

    if (!file_skip_to_offset(ofp, self->start)) {
        fclose(ofp);
        return CHAIN_ERROR_FILE_SKIPOFFSET;
    }

    if (!node_test_isdir(ofp)) {
        fclose(ofp);
        return CHAIN_ERROR_FILE_NODIR;
    }


    ifp = fopen(source, "rb");
    if (ifp == NULL) {
        fclose(ofp);
        return CHAIN_ERROR_FILE_OPENSOURCE;
    }

    binfile_start(&file);
    binfile_type_set(&file, 'f');
    binfile_namesize_set(&file, strlen(filename));
    binfile_name_set(&file, filename);
    binfile_descsize_set(&file, strlen(filedesc));
    binfile_desc_set(&file, filedesc);
    datetime_get_now(datetime);
    binfile_datetime_set(&file, datetime);
    file_get_ctrlsum(ifp, &ctrlsum);
    binfile_ctrlsum_set(&file, ctrlsum);
    file_get_size(ifp, &filesize);
    bignumber_tostr(&filesize, filesizestr);
    binfile_contentsize_set(&file, filesizestr);
    binfile_contentstream_set(&file, ifp);
    binfile_file_offset_set(&file, 0);

    if (!binfile_get_size(&file, &filesize)) {
        fclose(ifp);
        fclose(ofp);
        binfile_end(&file);
        return CHAIN_ERROR_FILE_FILESIZE;
    }


    fgetpos(ofp, &dirpos);

    bindir_start(&dirtmp);

    if (!node_read_dir_header(ofp, &dirtmp)) {
        fclose(ifp);
        fclose(ofp);
        bindir_end(&dirtmp);
        binfile_end(&file);
        return CHAIN_ERROR_FILE_READDIRHEADER;
    }

    if (!bindir_num_of_files_get(&dirtmp, &tmp_num_of_files)) {
        fclose(ifp);
        fclose(ofp);
        bindir_end(&dirtmp);
        binfile_end(&file);
        return CHAIN_ERROR_FILE_DIRGETNOF;
    }

    if (tmp_num_of_files == 0) {

        jumper_dir_jump_file_offset(ofp, filereloff);

        if (!file_test_write_size(ofp, &filesize)) {
            fclose(ifp);
            fclose(ofp);
            bindir_end(&dirtmp);
            binfile_end(&file);
            return CHAIN_ERROR_FILE_FILESIZE;
        }
        if (!node_write_file(ofp, &file)) {
            fclose(ifp);
            fclose(ofp);
            bindir_end(&dirtmp);
            binfile_end(&file);
            return CHAIN_ERROR_FILE_WRITENODE;
        }

        fsetpos(ofp, &dirpos);
        bindir_file_offset_set(&dirtmp, filereloff);
        node_write_dir_header_field(ofp, &dirtmp, DIRFLD_FILEOFFSET);

        fsetpos(ofp, &dirpos);
        bindir_num_of_files_set(&dirtmp, 1);
        node_write_dir_header_field(ofp, &dirtmp, DIRFLD_NUMOFFILES);

    }
    else {

        if (!bindir_file_offset_get(&dirtmp, &tmp_dir_rel_off)) {
            fclose(ifp);
            fclose(ofp);
            bindir_end(&dirtmp);
            binfile_end(&file);
            return CHAIN_ERROR_FILE_DIRGETOFFSET;
        }

        jumper_dir_jump_file_offset(ofp, tmp_dir_rel_off);

        binfile_start(&filetmp);

        for (i = 0; i < tmp_num_of_files; i++) {

            fgetpos(ofp, &filepos);

            if (!node_test_isfile(ofp)) {
                fclose(ifp);
                fclose(ofp);
                bindir_end(&dirtmp);
                binfile_end(&filetmp);
                binfile_end(&file);
                return CHAIN_ERROR_FILE_NOFILE;
            }
            if (!node_read_file_header(ofp, &filetmp)) {
                fclose(ifp);
                fclose(ofp);
                bindir_end(&dirtmp);
                binfile_end(&filetmp);
                binfile_end(&file);
                return CHAIN_ERROR_FILE_READFILEHEADER;
            }
            if (!binfile_file_offset_get(&filetmp, &tmp_file_rel_off)) {
                fclose(ifp);
                fclose(ofp);
                bindir_end(&dirtmp);
                binfile_end(&filetmp);
                binfile_end(&file);
                return CHAIN_ERROR_FILE_FILEGETOFFSET;
            }
            if (i + 1 < tmp_num_of_files)
                jumper_file_jump_file_offset(ofp, tmp_file_rel_off);
            else
                jumper_file_jump_file_offset(ofp, filereloff);

        }

        if (!file_test_write_size(ofp, &filesize)) {
            fclose(ifp);
            fclose(ofp);
            bindir_end(&dirtmp);
            binfile_end(&filetmp);
            binfile_end(&file);
            return CHAIN_ERROR_FILE_FILESIZE;
        }
        if (!node_write_file(ofp, &file)) {
            fclose(ifp);
            fclose(ofp);
            bindir_end(&dirtmp);
            binfile_end(&filetmp);
            binfile_end(&file);
            return CHAIN_ERROR_FILE_WRITENODE;
        }

        fsetpos(ofp, &filepos);
        binfile_file_offset_set(&filetmp, filereloff);
        node_write_file_header_field(ofp, &filetmp, FILFLD_FILEOFFSET);

        fsetpos(ofp, &dirpos);
        bindir_num_of_files_set(&dirtmp, tmp_num_of_files + 1);
        node_write_dir_header_field(ofp, &dirtmp, DIRFLD_NUMOFFILES);

        binfile_end(&filetmp);

    }

    bindir_end(&dirtmp);

    if (ferror(ofp)) {
        fclose(ifp);
        fclose(ofp);
        binfile_end(&file);
        return CHAIN_ERROR_FILE_WRITEFILE;
    }
    if (fclose(ofp) != 0) {
        fclose(ifp);
        binfile_end(&file);
        return CHAIN_ERROR_FILE_FILESYS;
    }
    if (ferror(ifp)) {
        fclose(ifp);
        binfile_end(&file);
        return CHAIN_ERROR_FILE_READSOURCE;
    }
    if (fclose(ifp) != 0) {
        binfile_end(&file);
        return CHAIN_ERROR_FILE_SOURCESYS;
    }
    binfile_end(&file);
    return CHAIN_OK;
}

/* chain_end: stop chain and clear internal values */
void chain_end(struct chain *self)
{
    self->dst = NULL;
    self->start = NULL;
}
