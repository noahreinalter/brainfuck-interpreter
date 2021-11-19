#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printArgumentError();

int main(int argc, char *argv[])
{
    char *o_arg = NULL;
    char *i_arg = NULL;
    int c;

    FILE *outputFile;
    FILE *inputFile;

    while ((c = getopt(argc, argv, "i:o:")) != -1)
    {
        switch (c)
        {
        case 'o':
            o_arg = optarg;
            break;
        case 'i':
            i_arg = optarg;
            break;
        case '?':
            break;
        }
    }

    if (o_arg != NULL)
    {
        outputFile = fopen(o_arg, "w");
        if (outputFile == NULL)
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        printArgumentError();
        return EXIT_FAILURE;
    }

    if (i_arg != NULL)
    {
        inputFile = fopen(i_arg, "r");
        if (inputFile == NULL)
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        printArgumentError();
        return EXIT_FAILURE;
    }

    char array[30000] = {0};
    char *ptr = array;
    char *begin = &array[0];
    char *end = &array[30000];

    char *input = NULL;
    size_t inputSize = 0;
    ssize_t read;
    while ((read = getline(&input, &inputSize, inputFile)) != -1)
    {
        for (size_t i = 0; i < read; i++)
        {
            if (input[i] == '>' && ptr < end)
            {
                ptr++;
            }
            else if (input[i] == '<' && begin < ptr)
            {
                ptr--;
            }
            else if (input[i] == '+')
            {
                *ptr = *ptr + 1;
            }
            else if (input[i] == '-')
            {
                *ptr = *ptr - 1;
            }
            else if (input[i] == '.')
            {
                fprintf(outputFile, "%c", ptr[0]);
            }
            else if (input[i] == ',')
            {
                *ptr = getchar();
            }
            else if (input[i] == '[' && ptr[0] == 0)
            {
                while (input[i] != ']')
                {
                    if (i < read + 1)
                    {
                        i++;
                    }
                }
            }
            else if (input[i] == ']')
            {
                while (input[i] != '[')
                {
                    if (0 < i)
                    {
                        i--;
                    }
                    
                }
                i--; 
            }
            
        }
    }

    free(input);
    fclose(outputFile);
    fclose(inputFile);

    return EXIT_SUCCESS;
}

/**
 * @brief A function to print a standardized error for an argument error.
 *
 */
void printArgumentError()
{
    fprintf(stderr, "Error parsing arguments for brainfuck -i inputfile -o outfile\n");
}