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

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include "bindir.h"
#include "binfile.h"
#include "file_operation.h"
#include "binfield.h"

struct node {
    struct binfield *field;
};

struct node_state {
    int has_cryptor;
    size_t cryptor_password_position;
};

enum node_dir_field_flags {
    DIRFLD_TYPESIGN = 0x1,
    DIRFLD_DESCSIZE = 0x2,
    DIRFLD_DESC = 0x4,
    DIRFLD_NUMOFFILES = 0x8,
    DIRFLD_FILEOFFSET = 0x10
};

enum node_file_field_flags {
    FILFLD_TYPESIGN = 0x1,
    FILFLD_NAMESIZE = 0x2,
    FILFLD_NAME = 0x4,
    FILFLD_DESCSIZE = 0x8,
    FILFLD_DESC = 0x10,
    FILFLD_DATETIME = 0x20,
    FILFLD_CTRLSUM = 0x40,
    FILFLD_CONTENTSIZE = 0x80,
    FILFLD_CONTENTSTREAM = 0x100,
    FILFLD_FILEOFFSET = 0x200
};

void node_start(struct node *self, struct binfield *field);
int node_write_dir(struct node *self, FILE *ofp, const struct bindir *dir);
int node_test_isdir(struct node *self, FILE *ifp);
int node_read_dir_header(struct node *self, FILE *ifp, struct bindir *dir);
int node_write_dir_header_field(
    struct node *self,
    FILE *ofp,
    const struct bindir *dir,
    enum node_dir_field_flags fieldflags);
int node_write_file(struct node *self, FILE *ofp, const struct binfile *file);
int node_test_isfile(struct node *self, FILE *ifp);
int node_read_file_header(struct node *self, FILE *ifp, struct binfile *file);
int node_write_file_header_field(
    struct node *self,
    FILE *ofp,
    const struct binfile *file,
    enum node_file_field_flags fieldflags);
void node_end(struct node *self);

#endif
