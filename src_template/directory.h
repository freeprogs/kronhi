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

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string.h>

/* maximum length of directory description */
#define DIRECTORY_MAXDESCRIPTION  65535

struct directory {
    char description[DIRECTORY_MAXDESCRIPTION];
    size_t nfiles;
};

void directory_description_set(struct directory *dir, const char *s);
char *directory_description_get(struct directory *dir, char out[]);

#endif
