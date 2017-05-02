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

#include <stdio.h>
#include "cmdshell.h"
#include "directory.h"
#include "input.h"

int run_command_shell(void);

int main(void)
{
    int retval;

    retval = run_command_shell();
    if (retval != 0)
        return 1;
    return 0;
}

/* run_command_shell: run command shell in a loop */
int run_command_shell(void)
{
    enum cmdshell_code retcmd;
    char reply[CMDSHELL_MAXINPUT];

    struct write_options wopts = { "", "", 0, W_CIPHER_NONE };
    struct read_options ropts = { "", "", 0, R_CIPHER_NONE };

    struct directory wdir;

    cmdshell_start();
    cmdshell_print_message("Input `help' for help or `quit' for exit.");
    write_options_clear(&wopts);
    read_options_clear(&ropts);
    while (1) {
        retcmd = cmdshell_prompt_command("Command: ", reply, sizeof reply);
        if (retcmd == CMD_INIT_WRITE) {
            char src[CMDSHELL_MAXINPUT];
            char dst[CMDSHELL_MAXINPUT];
            char offset[CMDSHELL_MAXINPUT];
            char cipher[CMDSHELL_MAXINPUT];
            write_options_tostr_source(&wopts, src);
            write_options_tostr_destination(&wopts, dst);
            write_options_tostr_offset(&wopts, offset);
            write_options_tostr_cipher(&wopts, cipher);
            if (cmdshell_init_write(src, dst, offset, cipher)) {
                if (!write_options_init(&wopts, src, dst, offset, cipher)) {
                    cmdshell_print_error("can't set write options");
                }
            }
            else {
                cmdshell_print_error("can't input write options");
            }
        }
        else if (retcmd == CMD_INIT_WRITE_DIR) {
            enum cmdshell_dir_code retdir;
            char dirdesc[CMDSHELL_MAXTEXTINPUT] = "";
            char dirdescfile[CMDSHELL_MAXINPUT] = "";

            retdir = cmdshell_init_write_dir(dirdesc, dirdescfile);
            if (retdir == CMD_DIR_INTER) {
                directory_description_set(&wdir, dirdesc);
            }
            else if (retdir == CMD_DIR_FILE) {
                if (input_from_file(dirdesc, sizeof dirdesc, dirdescfile)) {
                    directory_description_set(&wdir, dirdesc);
                    cmdshell_print_message(
                        "Directory description has loaded from \"%s\"",
                        dirdescfile);
                }
                else {
                    cmdshell_print_error(
                        "can't load directory description: \"%s\"",
                        dirdescfile);
                }
            }
            else if (retdir == CMD_DIR_NOOP) {
                cmdshell_print_message("Directory description hasn't changed");
            }
            else if (retdir == CMD_DIR_UNKNOWN) {
                cmdshell_print_error("can't input write directory contents");
            }
        }
        else if (retcmd == CMD_INIT_READ) {
            char src[CMDSHELL_MAXINPUT];
            char dst[CMDSHELL_MAXINPUT];
            char offset[CMDSHELL_MAXINPUT];
            char cipher[CMDSHELL_MAXINPUT];
            read_options_tostr_source(&ropts, src);
            read_options_tostr_destination(&ropts, dst);
            read_options_tostr_offset(&ropts, offset);
            read_options_tostr_cipher(&ropts, cipher);
            if (cmdshell_init_read(src, dst, offset, cipher)) {
                if (!read_options_init(&ropts, src, dst, offset, cipher)) {
                    cmdshell_print_error("can't set read options");
                }
            }
            else {
                cmdshell_print_error("can't input read options");
            }
        }
        else if (retcmd == CMD_STATUS_WRITE) {
            char src[CMDSHELL_MAXINPUT];
            char dst[CMDSHELL_MAXINPUT];
            char offset[CMDSHELL_MAXINPUT];
            char cipher[CMDSHELL_MAXINPUT];
            write_options_tostr_source(&wopts, src);
            write_options_tostr_destination(&wopts, dst);
            write_options_tostr_offset(&wopts, offset);
            write_options_tostr_cipher(&wopts, cipher);
            cmdshell_print_status_write(src, dst, offset, cipher);
        }
        else if (retcmd == CMD_STATUS_WRITE_DIR) {
            char desc[DIRECTORY_MAXDESCRIPTION];
            directory_description_get(&wdir, desc);
            cmdshell_print_status_write_dir(desc);
        }
        else if (retcmd == CMD_STATUS_READ) {
            char src[CMDSHELL_MAXINPUT];
            char dst[CMDSHELL_MAXINPUT];
            char offset[CMDSHELL_MAXINPUT];
            char cipher[CMDSHELL_MAXINPUT];
            read_options_tostr_source(&ropts, src);
            read_options_tostr_destination(&ropts, dst);
            read_options_tostr_offset(&ropts, offset);
            read_options_tostr_cipher(&ropts, cipher);
            cmdshell_print_status_read(src, dst, offset, cipher);
        }
        else if (retcmd == CMD_HELP) {
            cmdshell_print_help();
        }
        else if (retcmd == CMD_QUIT) {
            break;
        }
        else if (retcmd == CMD_UNKNOWN) {
            cmdshell_print_error(
                "unknown command \"%s\", input `help'", reply);
        }
    }
    write_options_clear(&wopts);
    read_options_clear(&ropts);
    cmdshell_print_message("Bye bye.");
    cmdshell_end();

    return 0;
}
