#include "main.h"

/**
 * display_prompt - Display the shell prompt
 */
void display_prompt(void)
{
    write(STDOUT_FILENO, "$ ", 2);
}

