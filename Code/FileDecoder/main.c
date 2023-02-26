#include "stdio.h"
#include <stdlib.h>
#include "stdint.h"
#include "string.h"

int main(int argc, char *argv[])
{
    FILE *in, *out;
    uint16_t time;
    int16_t ch1, ch2, ch3, ch4;

    if (argc != 2 && argc != 3)
    {
        printf("Incorrect number of arguments, provided: %d, expected: 1 \n", argc - 1);
        return -1;
    }

    in = fopen(argv[1], "rb");

    // Same name as input file, but with .csv extension instead of .TXT for the output
    char *outName = malloc(strlen(argv[1]) + 5);
    strcpy(outName, argv[1]);
    outName[strlen(argv[1]) - 4] = '\0';
    strcat(outName, ".csv");
    out = fopen(outName, "w");

    if (in == NULL)
    {
        printf("Could not open file %s \n", argv[1]);
        return -1;
    }

    int offset = 0;
    if (argc == 3)
    {
        offset = atoi(argv[2]);
    }
    else
    {
        printf("No offset provided, trying to find offset in file \n");
        char c;
        while (fread(&c, sizeof(char), 1, in) == 1)
        {
            if (c == '#')
            {
                offset = ftell(in);
                // Try to see if the next 9 characters are also #
                int i;
                for (i = 0; i < 9; i++)
                {
                    fread(&c, sizeof(char), 1, in);
                    if (c != '#')
                    {
                        break;
                    }
                }
                if (i == 9)
                {
                    offset += 9;
                    printf("Found offset at %d \n", offset);
                    break;
                }
            }
        }
    }

    printf("Converting %s to out.csv, offset %d \n", argv[1], offset);
    fprintf(out, "Time,Channel 1,Channel 2,Channel 3,Channel 4\n");
    fseek(in, offset, SEEK_SET);
    while (fread(&time, sizeof(uint16_t), 1, in) == 1)
    {
        fread(&ch1, sizeof(int16_t), 1, in);
        fread(&ch2, sizeof(int16_t), 1, in);
        fread(&ch3, sizeof(int16_t), 1, in);
        fread(&ch4, sizeof(int16_t), 1, in);
        fprintf(out, "%d,%d,%d,%d,%d\n", time, ch1, ch2, ch3, ch4);
    }
}