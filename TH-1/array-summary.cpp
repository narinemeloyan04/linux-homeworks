#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

// Function to compute the sum of an array using a regular for loop
long long computeSum(const vector<int>& arr) {
    long long sum = 0;
    for (int val : arr) {
        sum += val;
    }
    return sum;
}

// Function to compute the sum of an array using multiple threads
void computeSumThread(const vector<int>& arr, int start, int end, long long* result) {
    *result = 0;
    for (int i = start; i < end; ++i) {
        *result += arr[i];
    }
}

int main(int argc, char* argv[]) {

    // Check if the correct number of command-line arguments is provided
    if (argc != 3) 
    {
        cerr << "Usage: " << argv[0] << " N M" << endl;
        return 1;
    }

    // Parse command-line arguments
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);

    // Initialize array with 1 values
    vector<int> arr(N);
    for (int i = 0; i < N; ++i) 
    {
        arr[i] = 1; 
    }

    // Compute sum using regular for loop
    auto startTime = chrono::high_resolution_clock::now();
    long long regularSum = computeSum(arr);
    auto endTime = chrono::high_resolution_clock::now();
    auto durationRegular = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    // Compute sum using multiple threads
    vector<thread> threads;
    vector<long long> partialSums(M, 0);
    int chunkSize = N / M;
    startTime = chrono::high_resolution_clock::now();

    // Create threads for computing partial sums
    for (int i = 0; i < M - 1; ++i) {
        threads.push_back(thread(computeSumThread, ref(arr), i * chunkSize, (i + 1) * chunkSize, &partialSums[i]));
    }

    // Handle the last chunk which may be larger
    threads.push_back(thread(computeSumThread, ref(arr), (M - 1) * chunkSize, N, &partialSums[M - 1]));

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Compute the final sum from partial sums
    long long threadSum = 0;
    for (long long partialSum : partialSums) {
        threadSum += partialSum;
    }

    endTime = chrono::high_resolution_clock::now();
    auto durationThreads = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    // Print results
    cout << "Time spent without threads: " << durationRegular << " microseconds" << endl;
    cout << "Time spent with " << M << " threads: " << durationThreads << " microseconds" << endl;

    return 0;
}