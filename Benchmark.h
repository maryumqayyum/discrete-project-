#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <string>
#include <chrono>
#include <functional>
#include <vector>

class Benchmark {
private:
    struct TestResult {
        std::string testName;
        double timeMs;
        bool passed;
    };

    std::vector<TestResult> results;

public:
    Benchmark();

    template<typename Func>
    double measureTime(Func func);

    void runTest(const std::string& name, std::function<bool()> testFunc);
    void displayResults() const;
    void clearResults();

    // Algorithmic efficiency tests
    void testRecursiveFibonacci(int n);
    void testMemoizedFibonacci(int n);
    void testFactorial(int n);
    void testCombinations(int n, int r);
    void testPowerSet(int n);
};

// Template implementation
template<typename Func>
double Benchmark::measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

#endif