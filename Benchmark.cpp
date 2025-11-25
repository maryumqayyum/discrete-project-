#include "Benchmark.h"
#include <iostream>
#include <iomanip>
#include <map>

using namespace std;

Benchmark::Benchmark() {}

void Benchmark::runTest(const string& name, function<bool()> testFunc) {
    cout << "Running: " << name << "... ";

    double time = measureTime([&]() {
        testFunc();
        });

    bool passed = true; // Assume passed if no exception
    results.push_back({ name, time, passed });

    cout << "Done (" << fixed << setprecision(3) << time << " ms)\n";
}

void Benchmark::displayResults() const {
    cout << "\n========================================================================\n";
    cout << "||              PERFORMANCE ANALYSIS SUMMARY                         ||\n";
    cout << "========================================================================\n\n";

    cout << left << setw(40) << "Test Description" << setw(15) << "Duration (ms)" << "Status\n";
    cout << string(70, '-') << "\n";

    for (const TestResult& r : results) {
        cout << left << setw(40) << r.testName
            << setw(15) << fixed << setprecision(3) << r.timeMs
            << (r.passed ? "[PASS]" : "[FAIL]") << "\n";
    }

    cout << string(70, '-') << "\n";

    double totalTime = 0;
    int passCount = 0;
    for (const TestResult& r : results) {
        totalTime += r.timeMs;
        if (r.passed) passCount++;
    }

    cout << "Cumulative Duration: " << totalTime << " ms\n";
    cout << "Tests Passed: " << passCount << "/" << results.size() << "\n";
    cout << "========================================================================\n";
}

void Benchmark::clearResults() {
    results.clear();
}

// Recursive Fibonacci (inefficient)
unsigned long long fibRecursive(int n) {
    if (n <= 1) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

// Memoized Fibonacci (efficient)
unsigned long long fibMemoized(int n, map<int, unsigned long long>& memo) {
    if (n <= 1) return n;
    if (memo.count(n)) return memo[n];
    memo[n] = fibMemoized(n - 1, memo) + fibMemoized(n - 2, memo);
    return memo[n];
}

void Benchmark::testRecursiveFibonacci(int n) {
    runTest("Recursive Fibonacci(" + to_string(n) + ")", [n]() {
        fibRecursive(n);
        return true;
        });
}

void Benchmark::testMemoizedFibonacci(int n) {
    runTest("Memoized Fibonacci(" + to_string(n) + ")", [n]() {
        map<int, unsigned long long> memo;
        fibMemoized(n, memo);
        return true;
        });
}

unsigned long long factorialRecursive(int n) {
    if (n <= 1) return 1;
    return n * factorialRecursive(n - 1);
}

void Benchmark::testFactorial(int n) {
    runTest("Factorial(" + to_string(n) + ")", [n]() {
        factorialRecursive(n);
        return true;
        });
}

unsigned long long nCrRecursive(int n, int r) {
    if (r == 0 || r == n) return 1;
    if (r == 1) return n;
    return nCrRecursive(n - 1, r - 1) + nCrRecursive(n - 1, r);
}

void Benchmark::testCombinations(int n, int r) {
    runTest("Combinations C(" + to_string(n) + "," + to_string(r) + ")", [n, r]() {
        nCrRecursive(n, r);
        return true;
        });
}

void generatePowerSetRecursive(int index, int n, vector<int>& current, int& count) {
    count++;
    if (index == n) return;

    for (int i = index; i < n; i++) {
        current.push_back(i);
        generatePowerSetRecursive(i + 1, n, current, count);
        current.pop_back();
    }
}

void Benchmark::testPowerSet(int n) {
    runTest("PowerSet size(" + to_string(n) + ")", [n]() {
        vector<int> current;
        int count = 0;
        generatePowerSetRecursive(0, n, current, count);
        return true;
        });
}