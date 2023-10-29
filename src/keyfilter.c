/**
 ***********************************************************************************
 * @file keyfilter.c
 * @author Matěj Křenek <xkrenem00@stud.fit.vutbr.cz>
 * @brief Program to emulate the algorithm for virtual navigation keyboard.
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 ***********************************************************************************
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 101
#define MAX_ADDRESS_LENGTH 101
#define MAX_HINT_LENGTH 128 // Maximum number of chars in 7bit ASCII table

typedef enum
{
    FOUND,     // Search text fully corresponds to address
    NOT_FOUND, // Search text doesnt corresponds to address at all
    PREFIX     // Search text is prefix of address
} AddressResult;

char *uppercase(char *text);
char *trim(char *text);
char *argcat(char *input, char *args[], int argc);
char *bsort(char *input);
AddressResult eval_address(char *input, char *address);

int main(int argc, char *argv[])
{
    char input[MAX_INPUT_LENGTH] = "";
    char address[MAX_ADDRESS_LENGTH];
    char address_found[MAX_ADDRESS_LENGTH];
    int address_prefix_count = 0;
    char hint[MAX_HINT_LENGTH] = "";

    // Concatenate passed arguments and convert the input to uppercase
    uppercase(argcat(input, argv, argc));

    // Check if any addresses have been passed to input of program
    if (isatty(STDIN_FILENO))
    {
        fprintf(stderr, "Addresses haven't been passed to input of program.\n");
        return 1;
    }

    // Loop through passed addreses
    while (fgets(address, sizeof(address), stdin))
    {
        char hint_temp[2];

        // Convert address to uppercase and trim trailing spaces
        trim(uppercase(address));

        switch (eval_address(input, address))
        {
        case FOUND:
            strcpy(address_found, address);
            break;
        case PREFIX:
            hint_temp[0] = address[strlen(input)];
            hint_temp[1] = '\0';

            // Add only a hint letter that does not already exist in hint
            if (!strchr(hint, hint_temp[0]))
            {
                strcat(hint, hint_temp);
            }

            // If first of address prefix or found address is same as address add it as found address else set found address to empty
            if (address_prefix_count <= 0 || strcmp(address_found, address) == 0)
            {
                strcpy(address_found, address);
            }
            else
            {
                strcpy(address_found, "");
            }

            address_prefix_count++;
            break;

        default:
            break;
        }
    }

    // Inform user about address search result
    if (strlen(address_found))
    {
        printf("Found: %s\n", address_found);
    }
    else if (strlen(hint))
    {
        printf("Enable: %s\n", bsort(hint));
    }
    else
    {
        printf("Not found\n");
    }

    return 0;
}

/**
 * @brief Trim trailing spaces from the passed input string
 * @param input String to be trimmed
 * @return Trimmed string
 */
char *trim(char *text)
{
    for (int i = strlen(text) - 1; i >= 0; i--)
    {
        if (text[i] == '\n')
        {
            text[i] = '\0';
        }
        else
        {
            break;
        }
    }

    return text;
}

/**
 * @brief Converts passed string to uppercase
 * @param text String to be converted to uppercase
 * @return Uppercased string
 */
char *uppercase(char *text)
{
    for (int i = 0; i < (int)strlen(text); i++)
    {
        text[i] = toupper(text[i]);
    }

    return text;
}

/**
 * @brief Concatanate list of arguments
 * @param input String where the concated arguments are stored into
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

/**
 * @brief Sort passed text alphabetically using bubble sort
 * @param text String to be sorted
 * @return Sorted text
 */
char *bsort(char *text)
{
    int length = strlen(text);
    char temp;

    for (int i = 0; i < length - 1; i++)
    {
        for (int j = 0; j < length - i - 1; j++)
        {
            if (text[j] > text[j + 1])
            {
                temp = text[j];
                text[j] = text[j + 1];
                text[j + 1] = temp;
            }
        }
    }

    return text;
}

/**
 * @brief Evaluate address result based on passed search input and address
 * @param input String to search address
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