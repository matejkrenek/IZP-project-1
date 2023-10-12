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
#include <time.h>

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
char *uppercase(char *text);
char *argcat(char *input, char *args[], int argc);
char *bsort(char *input);
AddressResult eval_address(char *input, char *address);

int main(int argc, char *argv[])
{
    char input[MAX_INPUT_LENGTH] = "";
    char address[MAX_ADDRESS_LENGTH];
    char address_found[MAX_ADDRESS_LENGTH];
    char hint[MAX_HINT_LENGTH] = "";

    // Concatante passed arguments and convert the input to lowercase
    trim(uppercase(argcat(input, argv, argc)));

    // Loop through passed addreses
    while (fgets(address, sizeof(address), stdin))
    {
        char hint_temp[2];
        trim(uppercase(address));

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
            break;

        default:
            break;
        }
    }
    if (strlen(address_found))
    {
        printf("Found: %s\n", address_found);
    }

    if (strlen(hint))
    {
        printf("ENABLED: %s\n", bsort(hint));
    }

    if (!strlen(hint) && !strlen(address_found))
    {
        printf("NOT FOUND\n");
    }

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
 * @brief Converts passed string to uppercase
 * @param text String to be converted
 * @return Uppercased string
 */
char *uppercase(char *text)
{
    for (int i = 0; i < text[i]; i++)
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

/**
 * @brief Trim all leading whitespaces and trailing spaces
 * @param text String to be
 * @return Trimed text
 *
 */
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