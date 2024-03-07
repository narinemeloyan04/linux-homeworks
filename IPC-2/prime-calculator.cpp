#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


// Function to check if a number is prime
bool isPrime(int num)
 {
    if (num < 2) 
    {
        return false;
    }

    for (int i = 2; i <= num / 2; ++i) 
    {
        if (num % i == 0) 
        {
            return false;
        }
    }

    return true;
}

// Function to calculate the m-th prime number
int calculateMthPrime(int m) 
{
    int count = 0;
    int num = 2;

    while (count < m) 
    {
        if (isPrime(num)) 
        {
            ++count;
        }

        if (count < m) 
        {
            ++num;
        }
    }

    return num;
}

int main() 
{
    int pipe_fd[2];

    // Creating a pipe for communication between parent and child processes
    if (pipe(pipe_fd) == -1) 
    {
        std::cerr << "Error creating pipe." << std::endl;
        exit(errno);
    }

    // Forking to create a child process
    pid_t child_pid = fork();

    if (child_pid == -1) 
    {
        std::cerr << "Error creating child process." << std::endl;
        exit(errno);    }

    if (child_pid == 0) 
    {  
        close(pipe_fd[1]);  

        int m;

        // Read user input from the pipe
        read(pipe_fd[0], &m, sizeof(m));  

        // Calculate the m-th prime number
        std::cout << "[Child] Calculating " << m << "-th prime number..." << std::endl;
        int result = calculateMthPrime(m);
        close(pipe_fd[0]);

        // Send the result back to the parent process
        std::cout << "[Child] Sending calculation result of prime(" << m << ")..." << std::endl;
        write(pipe_fd[1], &result, sizeof(result));
        close(pipe_fd[1]);  

        exit(0);  
    } 
    else 
    {  
        close(pipe_fd[0]); 

        while (true) 
        {
            std::cout << "[Parent] Please enter the number: ";
            std::string input;
            std::cin >> input;

            if (input == "exit") 
            {
                // Close write end to signal the child process
                close(pipe_fd[1]); 

                // Wait for the child process to finish
                waitpid(child_pid, NULL, 0);  
                break;
            }

            int m = std::stoi(input);

            // Send the user input to the child process through the pipe
            std::cout << "[Parent] Sending " << m << " to the child process..." << std::endl;
            write(pipe_fd[1], &m, sizeof(m));

            // Wait for and read the response from the child process
            std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;
            int result;
            read(pipe_fd[0], &result, sizeof(result));
            
            // Print the result received from the child process
            std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << std::endl;
        }

        close(pipe_fd[1]);  
    }

    return 0;
}
