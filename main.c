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
    char *end = &array[30000] - 1;

    for (char c = getc(inputFile); c != EOF; c = getc(inputFile))
    {
        if (c == '>' && ptr < end)
        {
            ptr++;
        }
        else if (c == '>' && !(ptr < end))
        {
            fprintf(stderr, "%s Error can't move more to the right\n", argv[0]);
            break;
        }
        else if (c == '<' && begin < ptr)
        {
            ptr--;
        }
        else if (c == '<' && !(begin < ptr))
        {
            fprintf(stderr, "%s Error can't move more to the left\n", argv[0]);
            break;
        }
        else if (c == '+')
        {
            *ptr = *ptr + 1;
        }
        else if (c == '-')
        {
            *ptr = *ptr - 1;
        }
        else if (c == '.')
        {
            fprintf(outputFile, "%c", ptr[0]);
        }
        else if (c == ',')
        {
            *ptr = getchar();
        }
        else if (c == '[' && ptr[0] == 0)
        {
            while (c != ']' && c != EOF)
            {
                c = getc(inputFile);
            }
        }
        else if (c == ']')
        {
            while (fseek(inputFile, -1, SEEK_CUR) == 0 && getc(inputFile) != '[')
            {
                fseek(inputFile, -1, SEEK_CUR);
            }
            if (fseek(inputFile, -1, SEEK_CUR) != 0)
            {
                fprintf(stderr, "%s Error ] without matching [\n", argv[0]);
                break;
            }
        }
    }

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