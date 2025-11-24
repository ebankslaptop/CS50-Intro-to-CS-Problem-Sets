// Apply a grayscale filter to 24-bit BMP files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// Lets us use number types like uint8_t (1 byte)
#include <string.h>

#pragma pack(push, 1)
// Tells the computer: do not put extra empty space inside these structs
typedef struct {
    uint16_t bfType;
    // First part of a BMP file. bfType should say "BM"
    uint32_t bfSize;
    // How big the whole BMP file is
    uint16_t bfReserved1;
    // Reserved space. Must be zero
    uint16_t bfReserved2;
    // Another zero value slot.
    uint32_t bfOffBits;
    // Tells us where the pixel data starts in the file
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    // Size of this info header
    int32_t  biWidth;
    // Picture width in pixels
    int32_t  biHeight;
    // Picture height in pixels. Negative means image is stored top-to-bottom
    uint16_t biPlanes;
    // BMP always uses 1 plane
    uint16_t biBitCount;
    // How many bits per pixel. We need 24
    uint32_t biCompression;
    //Should be zero (means no compression)
    uint32_t biSizeImage;
    // Size of the pixel area
    int32_t  biXPelsPerMeter;
    // Picture resolution sideways
    int32_t  biYPelsPerMeter;
    // Picture resolution up and down
    uint32_t biClrUsed;
    // Number of colors used (not needed for 24-bit BMP)
    uint32_t biClrImportant;
    // Number of important colors (not needed here)
} BITMAPINFOHEADER;
#pragma pack(pop)
// Return to normal struct spacing rules
void apply_grayscale(uint8_t* pixelData, int width, int height, int padding, int topDown) {
// Turns every pixel into a gray pixel
    int rowSize = width * 3 + padding;
    // Each pixel uses 3 bytes (B, G, R)
    // padding fixes row length to a multiple of 4 byte

    for (int y = 0; y < height; y++) {
    // Go through each row of the image
        int row = topDown ? y : (height - 1 - y);
        // If topDown is true, use rows normally
        // If not, BMP rows are stored upside-down, so we flip them
        uint8_t* rowPtr = pixelData + row * rowSize;
        // Pointer to the start of this row
        for (int x = 0; x < width; x++) {
            uint8_t* pixel = rowPtr + x * 3;
            // Pointer to this pixel’s 3 colors
            uint8_t gray = (uint8_t)(0.299 * pixel[2] +
                                     0.587 * pixel[1] +
                                     0.114 * pixel[0]);
          // Make a gray value using red, green, and blue

            pixel[0] = gray;
            //Set blue to gray
            pixel[1] = gray;
            //Set green to gray
            pixel[2] = gray;
            //Set red to gray
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
    // Make sure the user typed two filenames, 3 because of the cwd being first
        printf("Usage: %s <input BMP> <output BMP>\n", argv[0]);
        // Tell them how to run the program
        return 1;
    }

    FILE* inFile = fopen(argv[1], "rb");
    if (!inFile) {
        perror("Error opening input file");
        return 1;
    }

    BITMAPFILEHEADER fileHeader;
    // Make space to store file header
    BITMAPINFOHEADER infoHeader;
    // Make space to store info header

    if (fread(&fileHeader, sizeof(fileHeader), 1, inFile) != 1 ||
        fread(&infoHeader, sizeof(infoHeader), 1, inFile) != 1) {
        printf("Error reading BMP headers\n");
        fclose(inFile);
        return 1;
    }

    if (fileHeader.bfType != 0x4D42 ||
        infoHeader.biBitCount != 24 ||
        infoHeader.biCompression != 0) {
    // Make sure it is a real BMP, 24-bit, and not compressed
        printf("Unsupported BMP format\n");
        fclose(inFile);
        return 1;
    }

    int width  = infoHeader.biWidth;
    // Get picture width
    int height = abs(infoHeader.biHeight);
    // Get picture height. If negative, turn it positive

    int padding = (4 - (width * 3) % 4) % 4;
    // Compute how many padding bytes each row has
  
    size_t rowSize = width * 3 + padding;
    // Row length with padding included
    size_t dataSize = rowSize * height;
    // Total size of all pixel rows together

    uint8_t* pixelData = malloc(dataSize);
    // Ask the computer for memory to hold the pixels
  
    if (!pixelData) {
        printf("Memory allocation failed\n");
        fclose(inFile);
        return 1;
    }

    fseek(inFile, fileHeader.bfOffBits, SEEK_SET);
    // Jump to where pixel data starts in the file
    if (fread(pixelData, 1, dataSize, inFile) != dataSize) {
    // Read all the pixel data into memory
        printf("Error reading pixel data\n");
        free(pixelData);
        fclose(inFile);
        return 1;
    }

    fclose(inFile);
    // Close the input file because we no longer need it
    apply_grayscale(pixelData, width, height, padding,
                    infoHeader.biHeight < 0);
   // Call our function to turn the picture gray

    infoHeader.biSizeImage = dataSize;
    // Update header with the correct pixel data size
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
                        sizeof(BITMAPINFOHEADER) +
                        dataSize;
    // Update header with the new total file size
    FILE* outFile = fopen(argv[2], "wb");
    // Open the output file for writing in binary mode
    if (!outFile) {
        perror("Error opening output file");
        free(pixelData);
        return 1;
    }

    fwrite(&fileHeader, sizeof(fileHeader), 1, outFile);
    // Write the file header to the new file
    fwrite(&infoHeader, sizeof(infoHeader), 1, outFile);
    // Write the info header
    fwrite(pixelData, 1, dataSize, outFile);
    // Write the grayscale pixel data

    fclose(outFile);
    free(pixelData);

    return 0;
}
