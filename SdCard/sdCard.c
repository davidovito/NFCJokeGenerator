#include "sdCard.h"
#include "fatfs.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h> //for va_list var arg functions
#include <time.h>
#include <stdlib.h>

#define MAX_FILES 8
#define BUF_SIZE 256

FIL files[MAX_FILES]; // Handles for multiple files

/*void myprintf(const char *fmt, ...) {
    static char buffer[BUF_SIZE + 30];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    int len = strlen(buffer);
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, -1);
} */

uint8_t readRandomJokes(uint8_t mode, char *fillBuffer) {
    if (mode >= MAX_FILES || files[mode].fs == NULL) {
        //myprintf("Invalid file mode or file not opened!\r\n");
        return 1; // Error: some invalid file
    }

    FIL *selectedFile = &files[mode];
    BYTE buffer[BUF_SIZE];
    int total_lines = 0;

    // Count the number of lines in the file
    while (f_gets((TCHAR *)buffer, sizeof(buffer), selectedFile) != NULL) {
        total_lines++;
    }
    f_lseek(selectedFile, 0);

    if (total_lines == 0) {
        //myprintf("Selected joke database is empty!\r\n");
        return 1; // Error: empty file
    }

    srand(time(NULL)); // Initialize random number generator
    int random_line = (rand() % total_lines) + 1; // Random line number
    int current_line = 0;

    // Read the selected random line
    while (f_gets((TCHAR *)buffer, sizeof(buffer), selectedFile) != NULL) {
        current_line++;
        if (current_line == random_line) {
            strncpy(fillBuffer, (char *)buffer, BUF_SIZE - 1);
            fillBuffer[BUF_SIZE - 1] = '\0'; // Ensure null termination
            //myprintf("Random joke: %s\r\n", buffer);
            break;
        }
    }
    f_lseek(selectedFile, 0); // Reset file cursor

    if (current_line != random_line) {
        //myprintf("Failed to find the random line in the file.\r\n");
        return 1; // Error: reading failed
    }

    return 0; // Success
}

uint8_t sdCardInit() {
    FATFS FatFs;   // FatFs handle
    FRESULT fres;  // Result after operations

    // Mount the file system
    fres = f_mount(&FatFs, "", 1); // 1=mount now
    if (fres != FR_OK) {
        //myprintf("f_mount error (%i)\r\n", fres);
        return 1; // Error: failed to mount
    }

    const char *fileNames[MAX_FILES] = {"jokes.txt", "jokes2.txt"};

    for (int i = 0; i < MAX_FILES && fileNames[i] != NULL; i++) {
        fres = f_open(&files[i], fileNames[i], FA_READ);
        if (fres != FR_OK) {
            //myprintf("Failed to open %s (%i)\r\n", fileNames[i], fres);
            // Close all previously opened files
            for (int j = 0; j < i; j++) {
                f_close(&files[j]);
            }
            return 1; // Error: failed to open a file
        }
    }

    //myprintf("All joke files opened successfully!\r\n");
    return 0; // Success
}



