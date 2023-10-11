/**
 ***********************************************************************************
 * @file main.c
 * @author Matěj Křenek <xkrenem00@stud.fit.vutbr.cz>
 * @brief
 * @date 2023-10-11
 *
 * @copyright Copyright (c) 2023
 ***********************************************************************************
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "logger/logger.h"

#define MAX_INPUT_LENGTH 100
#define MAX_ADDRESS_LENGTH 100
#define MAX_HINT_LENGTH 128 // maximum number of chars in 7bit ASCII table

typedef enum
{
    FOUND,
    NOT_FOUND,
    PREFIX
} AddressResult;

char *trim(char *text);
char *lowercase(char *text);
char *argcat(char *input, char *args[], int argc);
AddressResult eval_address(char *input, char *address);

int main(int argc, char *argv[])
{
    logger_init(LOG_DEBUG);

    char input[MAX_INPUT_LENGTH] = "";
    char address[MAX_ADDRESS_LENGTH];
    char hint[MAX_HINT_LENGTH] = "";
    char *result;

    // Concatante passed arguments and convert the input to lowercase
    trim(lowercase(argcat(input, argv, argc)));

    // Loop through passed addreses
    while (fgets(address, sizeof(address), stdin))
    {
        switch (eval_address(input, trim(lowercase(address))))
        {
        case FOUND:
            LOG_DEBUG("address: %s\t", address);
            LOG_INFO("result: %s\n", "found");

            break;
        case PREFIX:

            break;

        default:
            break;
        }
    }

    LOG_DEBUG("address: %s\t", address);
    LOG_INFO("hint: %s\n", "hint");

    printf("Input: %s\n", input);

    return 0;
}

/**
 * @brief Converts passed string to lowercase
 * @param text String to be converted
 * @return Lowercased string
 */
char *lowercase(char *text)
{
    for (int i = 0; i < text[i]; i++)
    {
        text[i] = tolower(text[i]);
    }

    return text;
}

/**
 * @brief Concatanate list of arguments
 * @param input Variable where the concated arguments are stored into
 * @param args List of argumets to be concataned
 * @param argc Count of passed arguments
 * @return Concatenated arguments
 */
char *argcat(char *input, char *args[], int argc)
{

    for (int i = 1; i < argc; i++)
    {
        if (i > 1)
        {
            strcat(input, " ");
        }

        strcat(input, args[i]);
    }

    return input;
}

// Function to trim leading whitespace from a string
char *trim(char *text)
{
    int i = 0;
    int len = strlen(text);

    while (i < len && isspace(text[i]))
    {
        i++;
    }

    int len2 = strlen(text);

    while (len2 > 0 && isspace(text[len2 - 1]))
    {
        len2--;
    }

    text[len2] = '\0';

    return text;
}

/**
 * @brief Evaluate address result based on passed string and address
 * @param input Variable to search address
 * @param input_length Length of passeed input
 * @param address Address to be evaluated
 * @return Evaluated address result
 */
AddressResult eval_address(char *input, char *address)
{

    if (strcmp(address, input) == 0)
    {
        return FOUND;
    }
    else if (strncmp(address, input, strlen(input)) == 0)
    {
        return PREFIX;
    }

    return NOT_FOUND;
}