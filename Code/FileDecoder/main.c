#include "stdio.h"
#include <stdlib.h>
#include "stdint.h"

/// This program will read a file that consist of raw bianry data. The data is a uint16_t for the delta time and 4 int16_t for the 4 channels of the ADC.
/// The program will create a new file with the same name as the input file but with the extension .csv. The new file will contain the data in a csv format.
/// The first line of the csv file will contain the header "Time,Channel 1,Channel 2,Channel 3,Channel 4"
/// The input file is the first argument and the output file is called out.csv
/// The second argument is the offset of the first sample in the file. This is used to skip the first samples in the file.
/// The real data is after ########## in the file.
int main(int argc, char *argv[]) {
    FILE *in, *out;
    uint16_t time;
    int16_t ch1, ch2, ch3, ch4;

    if(argc != 2 && argc != 3) {
        printf("Incorrect number of arguments, provided: %d, expected: 1 \n" , argc - 1);
        return -1;
    }

    in = fopen(argv[1], "rb");
    out = fopen("out.csv", "w");

    if(in == NULL) {
        printf("Could not open file %s \n", argv[1]);
        return -1;
    }
    

    int offset = 0;
    if(argc == 3) {
        offset = atoi(argv[2]);
    } else {
        printf("No offset provided, trying to find offset in file \n");
        char c;
        while (fread(&c, sizeof(char), 1, in) == 1) {
            if (c == '#') {
                offset = ftell(in);
                // Try to see if the next 9 characters are also #
                int i;
                for (i = 0; i < 9; i++) {
                    fread(&c, sizeof(char), 1, in);
                    if (c != '#') {
                        break;
                    }
                }
                if (i == 9) {
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
    while (fread(&time, sizeof(uint16_t), 1, in) == 1) {
        fread(&ch1, sizeof(int16_t), 1, in);
        fread(&ch2, sizeof(int16_t), 1, in);
        fread(&ch3, sizeof(int16_t), 1, in);
        fread(&ch4, sizeof(int16_t), 1, in);
        fprintf(out, "%d,%d,%d,%d,%d\n", time, ch1, ch2, ch3, ch4);
    }
}