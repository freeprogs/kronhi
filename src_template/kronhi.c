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

int run_command_shell(void);

int main(void)
{
    int retval;

    printf("Hello, Kronhi\n");

    retval = run_command_shell();
    if (retval != 0)
        return 1;
    return 0;
}

int run_command_shell(void)
{
    enum cmdshell_code retcmd;
    char reply[CMDSHELL_MAXINPUT];

    cmdshell_start();
    while (1) {
        retcmd = cmdshell_prompt_command("Command: ", reply, sizeof reply);
        if (retcmd == CMD_HELP) {
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
    cmdshell_end();

    return 0;
}
