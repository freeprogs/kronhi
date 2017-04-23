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

#ifndef CMDSHELL_H
#define CMDSHELL_H

#define CMDSHELL_MAXINPUT  1000  /* maximum command shell input line length */

enum cmdshell_code {
    CMD_HELP,
    CMD_QUIT,
    CMD_UNKNOWN
};

void cmdshell_start(void);
enum cmdshell_code
cmdshell_prompt_command(const char *prompt, char in[], int maxsize);
void cmdshell_print_error(const char *fmt, ...);
void cmdshell_print_help(void);
void cmdshell_end(void);

#endif
