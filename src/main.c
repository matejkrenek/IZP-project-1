/**
 ***********************************************************************************
 * @file main.c
 * @author Matěj Křenek <xkrenem00@stud.fit.vutbr.cz>
 * @brief
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
#define MAX_HINT_LENGTH 128 // maximum number of chars in 7bit ASCII table

typedef enum
{
    FOUND,     // Search text fully corresponds to address
    NOT_FOUND, // Search text doesnt corresponds to address at all
    PREFIX     // Search text is prefix of address
} AddressResult;

char *uppercase(char *text);
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

    // Concatante passed arguments and convert the input to lowercase
    uppercase(argcat(input, argv, argc));

    // Check if any addresses have been passed to program
    if (isatty(STDIN_FILENO))
    {
        printf("Addresses havent been passed on input.\n");
        return 1;
    }

    // Loop through passed addreses
    while (fgets(address, sizeof(address), stdin))
    {
        char hint_temp[2];

        // Format address row
        uppercase(address);

        switch (eval_address(input, address))
        {
        case FOUND:
            strcpy(address_found, address);
            break;
        case PREFIX:
            hint_temp[0] = address[strlen(input)];
            hint_temp[1] = '\0';

            // Add letter to hint only of it does not alredy exists in the hint
            if (!strchr(hint, hint_temp[0]))
            {
                strcat(hint, hint_temp);
            }

            // If first of address prefix add it as found address else set found address to empty
            if (address_prefix_count <= 0)
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
 * @brief Converts passed string to uppercase
 * @param text String to be converted
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