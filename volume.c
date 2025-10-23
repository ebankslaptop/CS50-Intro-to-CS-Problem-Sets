#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// WAV header structure
typedef uint8_t BYTE;
typedef int16_t SAMPLE;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    float factor = atof(argv[3]);

    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open file %s.\n", argv[1]);
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open file %s.\n", argv[2]);
        fclose(input);
        return 1;
    }

    BYTE header[44];
    fread(header, sizeof(BYTE), 44, input);
    fwrite(header, sizeof(BYTE), 44, output);

    SAMPLE buffer;
    while (fread(&buffer, sizeof(SAMPLE), 1, input))
    {
        buffer = buffer * factor;
        fwrite(&buffer, sizeof(SAMPLE), 1, output);
    }

    fclose(input);
    fclose(output);
    
    return 0;
}
