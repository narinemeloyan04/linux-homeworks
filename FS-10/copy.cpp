#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "No file path provided." << std::endl;
        exit(1);
    }

    char* sourceFile = argv[1];
    char* destFile = argv[2];

    int source = open(sourceFile, O_RDONLY);

    if (source < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    int dest = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (dest < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    char buffer[BUFFER_SIZE];
    ssize_t readBytes;
    off_t copiedBytes = 0;
    off_t holeBytes = 0;

    while ((readBytes = read(source, buffer, sizeof(buffer))) > 0) {
        // Check for holes in the buffer
        int holes = 0;

        for (int i = 0; i < readBytes; ++i) {
            if (buffer[i] == '\0') {
                holes++;
            }
        }

        copiedBytes += readBytes - holes;
        holeBytes += holes;

        lseek(dest, holeBytes, SEEK_CUR);  // Move the destination file pointer by the number of holes
        write(dest, buffer, readBytes);
    }

    // Copying finished successfully
    if (readBytes == 0) {
        std::cout << "Total bytes copied: " << copiedBytes << " (Physical Data: " << copiedBytes - holeBytes << ", Holes: " << holeBytes << ")" << std::endl;
    } else {
        std::cerr << strerror(errno) << std::endl;
        exit(1);
    }

    close(source);
    close(dest);
}