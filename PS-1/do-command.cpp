#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void do_command(char** argv)
{
    clock_t startTime = clock();

    pid_t pid = fork();

    if (pid == -1) 
    {
        cerr << " Fork failed." << endl;
        exit(1);
    } 
    else if (pid == 0) 
    {
        // Child process
        if (execvp(argv[0], argv) == -1)
        {
            cerr << " Execution failed." << endl;
            exit(1);
        }
    } 
    else 
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        clock_t endTime = clock();
        double duration = static_cast<double>(endTime - startTime) / 
         CLOCKS_PER_SEC;

        cout << "Command exited with status: " << status << endl;
        cout << "Duration: " << duration  << endl;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
        cerr << "Usage: " << argv[0] <<  endl;
        return 1;
    }

    // Building proper argument vector
    char** command_args = new char*[argc];
    for (int i = 0; i < argc; ++i) 
    {
        command_args[i] = argv[i + 1];
    }
    command_args[argc - 1] = nullptr;

    // Execute the command
    do_command(command_args);

    delete[] command_args;
    return 0;
}
