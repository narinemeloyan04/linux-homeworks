#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "NO file provided."<< std::endl;
        exit(1);
    }

    char* filepath = argv[1];

    struct stat fileStat;
    if (stat(filepath, &fileStat) < 0) {
        std::cerr << strerror(erno) << std::endl;
        exit(errno);
    }

    if (!S_ISREG(fileStat.st_mode)) {
        std::cerr << strerror(erno) << std::endl;
        exit(errno);
    }

    int fd = open(filepath, O_WRONLY);
    if (fd < 0) {
    std::cerr << strerror(erno) << std::endl;
        exit(errno);
    }

    char nullChar = '\0';

    // Determine the file size and replace every byte with the null character
    ssize_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    for (ssize_t i = 0; i < fileSize; i++) {
        write(fd, &nullChar, sizeof(char));
    }

    close(fd);

    // Remove the file from the file system
    if (unlink(filepath) != 0) {
        std::cerr << strerror(erno) << std::endl;
        exit(errno);
    }

    std::cout << "File erased and deleted successfully." << std::endl;

    return 0;
}