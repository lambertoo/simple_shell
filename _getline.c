#include "header.h"

/**
 * _getline - Reads input from standard input by the user.
 * Return: The input on a buffer.
 */
char *_getline() {
    int i, rd, buffer_size = BUFSIZE;
    char c = 0, *input_buffer, *processed_input;

    input_buffer = malloc(buffer_size);
    if (input_buffer == NULL) {
        free(input_buffer);
        return NULL;
    }

    for (i = 0; c != EOF && c != '\n'; i++) {
        fflush(stdin);
        rd = read(STDIN_FILENO, &c, 1);

        if (rd == 0) {
            free(input_buffer);
            exit(EXIT_SUCCESS);
        }

        input_buffer[i] = c;

        if (input_buffer[0] == '\n') {
            return enter(input_buffer);
        }

        if (i >= buffer_size) {
            input_buffer = realloc(input_buffer, (buffer_size + 2) * sizeof(char));
            if (input_buffer == NULL) {
                free(input_buffer);
                return NULL;
            }
        }
    }

    input_buffer[i] = '\0';
    processed_input = remove_whitespace(input_buffer);
    free(input_buffer);
    remove_comments(processed_input);
    return processed_input;
}

/**
 * enter - Handles newline character input.
 * @string: String to be handled.
 * Return: Empty string.
 */
char *enter(char *string) {
    free(string);
    return "\0";
}

/**
 * remove_whitespace - Modifies the input string to remove preceding whitespaces.
 * @str: Input to be modified.
 * Return: Returns the modified string.
 */
char *remove_whitespace(char *str) {
    int i, j = 0;
    char *processed_str;

    processed_str = malloc((strlen(str) + 1) * sizeof(char));
    if (processed_str == NULL) {
        free(processed_str);
        return NULL;
    }

    for (i = 0; str[i] == ' '; i++);

    for (; str[i + 1] != '\0'; i++) {
        processed_str[j] = str[i];
        j++;
    }

    processed_str[j] = '\0';

    if (processed_str[0] == '\0' || processed_str[0] == '#') {
        free(processed_str);
        return "\0";
    }

    return processed_str;
}

/**
 * remove_comments - Removes everything after '#'.
 * @input: Input buffer.
 * Return: Nothing.
 */
void remove_comments(char *input) {
    int i;

    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == '#' && input[i - 1] == ' ' && input[i + 1] == ' ') {
            input[i] = '\0';
        }
    }
}
