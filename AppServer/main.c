#include <stdio.h>
#include <stdlib.h>

// The size of the buffer we will use for input processing.
// For the test system, 1 billion bytes should be decent buffer size.
// With a buffer size of 1 billion bytes, we will be processing 250 million integers per buffer.
// Adjust this constant as needed.
//static const int BUF_SIZE = 1000000000;

// Use this buffer size instead since our machine is low on RAM.
// Otherwise, malloc call would generate segmentation fault.
static const int BUF_SIZE = 256000000;

int main(int argc, char** argv) {
    // Ensure program is executed with at least one argument.
    if(argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    // Ensure specified file name is valid.
    FILE* inputFile = fopen(argv[1], "rb");
    if(inputFile == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Allocate a buffer with the appropriate size and close the file handle.
    int bytesRead = 0;
    char* buf = malloc(BUF_SIZE);
    unsigned long long runningTotal = 0;
    while(1) {
        // Attempt to read 250 million integers into the buffer.
        bytesRead = fread(buf, sizeof(char), 20, inputFile);

        // Parse the little-endian binary data into integers.
        // Since each integer data is represented by 4 bytes.
        // The amount of numbers in the file = size / 4.
        // Store the running total as a 64-bit unsigned integer.
        // Since the data is in little-endian...
        // the data is stored from least significant bytes to the most significant bytes.
        int i = 0;
        for(i = 0; i < (bytesRead / 4); i++) {
            // Find out the number represented by the current 4 bytes.
            // Shift each byte to the correct position and add it to the currentNumber.
            // Finally, add the currentNumber to the running total.
            int j, currentNumber = 0;
            for(j = 0; j < 4; j++) {
                currentNumber += buf[(i*4) + j] << (16 * j);
            }
            runningTotal += currentNumber;
        }

        // Either an error or eof has been reached, stop processing.
        if(bytesRead != BUF_SIZE) {
            break;
        }
    }

    // Perform cleanups.
    // Close file handle and deallocate memory allocated on the heap.
    free(buf);
    fclose(inputFile);
    buf = NULL;
    inputFile = NULL;

    // Display the total sum.
    printf("%llu\n", runningTotal);
    return EXIT_SUCCESS;
}

