#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>



#define BUFFER_SIZE 10


int main(int argc, char** argv)
{
    // Check if there are not enough command-line arguments.
    if(argc < 2)
    {
        std::cerr << "Provide file path." << std::endl;
        exit(1);
    }
    char* file = argv[1];
     
    // Open the file in read-only mode.
    int fd = open(file, O_RDONLY);

    // Check if the file couldn't be opened.
    if(fd < 0)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(errno);
    }

    char buf[BUFFER_SIZE];
    
    while(true)
    {
        // Read the file.
        ssize_t reading_bytes = read(fd, buf, BUFFER_SIZE);

        // Check if there was an error while reading.
        if(reading_bytes < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }   
        
        // Check if the end of the file has been reached.
        if(reading_bytes == 0)
        {
            break;
        }
        
        buf[reading_bytes] = '\0';

        // Print the content of the buffer to standard output. 
        std::cout << buf;        
    }

    // Close the file using the file descriptor.
    int close_result = close(fd);

    // Check if there was an error while closing the file.
    if(close_result < 0)
        {
            std::cerr << strerror(errno) << std::endl;
            exit(errno);
        }  
        
    return 0;    

}
