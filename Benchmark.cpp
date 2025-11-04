#include "SortingAlgorithms.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <algorithm>
#include <memory>
#include <fstream>

enum class DataType {
    RANDOM,
    SORTED,
    REVERSE_SORTED,
    NEARLY_SORTED,
    MANY_DUPLICATES,
    FEW_UNIQUE
};

class BenchmarkSuite {
private:
    std::vector<std::unique_ptr<SortingAlgorithm>> algorithms;
    std::ofstream csvFile;
    
    std::vector<int> generateData(int size, DataType type) {
        std::vector<int> data(size);
        std::random_device rd;
        std::mt19937 gen(42); // Fixed seed for reproducibility
        
        switch (type) {
            case DataType::RANDOM:
                std::uniform_int_distribution<> dis(0, size * 10);
                for (int i = 0; i < size; i++) {
                    data[i] = dis(gen);
                }
                break;
                
            case DataType::SORTED:
                for (int i = 0; i < size; i++) {
                    data[i] = i;
                }
                break;
                
            case DataType::REVERSE_SORTED:
                for (int i = 0; i < size; i++) {
                    data[i] = size - i;
                }
                break;
                
            case DataType::NEARLY_SORTED:
                for (int i = 0; i < size; i++) {
                    data[i] = i;
                }
                // Swap 5% of elements
                std::uniform_int_distribution<> swap_dis(0, size - 1);
                for (int i = 0; i < size / 20; i++) {
                    int idx1 = swap_dis(gen);
                    int idx2 = swap_dis(gen);
                    std::swap(data[idx1], data[idx2]);
                }
                break;
                
            case DataType::MANY_DUPLICATES:
                std::uniform_int_distribution<> dup_dis(0, 10);
                for (int i = 0; i < size; i++) {
                    data[i] = dup_dis(gen);
                }
                break;
                
            case DataType::FEW_UNIQUE:
                std::uniform_int_distribution<> unique_dis(0, 100);
                for (int i = 0; i < size; i++) {
                    data[i] = unique_dis(gen);
                }
                break;
        }
        
        return data;
    }
    
    std::string dataTypeToString(DataType type) {
        switch (type) {
            case DataType::RANDOM: return "Random";
            case DataType::SORTED: return "Sorted";
            case DataType::REVERSE_SORTED: return "Reverse Sorted";
            case DataType::NEARLY_SORTED: return "Nearly Sorted";
            case DataType::MANY_DUPLICATES: return "Many Duplicates";
            case DataType::FEW_UNIQUE: return "Few Unique";
            default: return "Unknown";
        }
    }
    
    bool isSorted(const std::vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); i++) {
            if (arr[i] < arr[i-1]) return false;
        }
        return true;
    }
    
public:
    BenchmarkSuite() {
        algorithms.push_back(std::make_unique<InsertionSort>());
        algorithms.push_back(std::make_unique<MergeSort>());
        algorithms.push_back(std::make_unique<QuickSort>());
        algorithms.push_back(std::make_unique<HeapSort>());
        algorithms.push_back(std::make_unique<IntroSort>());
        algorithms.push_back(std::make_unique<TimSort>());
        algorithms.push_back(std::make_unique<ShellSort>());
        algorithms.push_back(std::make_unique<CountingSort>());
        algorithms.push_back(std::make_unique<RadixSort>());
        
        csvFile.open("benchmark_results.csv");
        csvFile << "Algorithm,Data Type,Size,Time(ms),Comparisons,Swaps,Sorted Correctly\n";
    }
    
    ~BenchmarkSuite() {
        if (csvFile.is_open()) {
            csvFile.close();
        }
    }
    
    void runBenchmark(int size, DataType dataType) {
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "Testing with " << dataTypeToString(dataType) 
                  << " data, Size: " << size << "\n";
        std::cout << std::string(80, '=') << "\n\n";
        
        std::cout << std::left << std::setw(25) << "Algorithm"
                  << std::right << std::setw(12) << "Time(ms)"
                  << std::setw(15) << "Comparisons"
                  << std::setw(12) << "Swaps"
                  << std::setw(10) << "Status" << "\n";
        std::cout << std::string(80, '-') << "\n";
        
        for (auto& algo : algorithms) {
            std::vector<int> data = generateData(size, dataType);
            
            algo->resetStats();
            
            auto start = std::chrono::high_resolution_clock::now();
            algo->sort(data);
            auto end = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double, std::milli> duration = end - start;
            
            const SortStats& stats = algo->getStats();
            bool sorted = isSorted(data);
            
            std::cout << std::left << std::setw(25) << algo->getName()
                      << std::right << std::setw(12) << std::fixed << std::setprecision(3) << duration.count()
                      << std::setw(15) << stats.comparisons
                      << std::setw(12) << stats.swaps
                      << std::setw(10) << (sorted ? "✓" : "✗") << "\n";
            
            csvFile << algo->getName() << ","
                    << dataTypeToString(dataType) << ","
                    << size << ","
                    << duration.count() << ","
                    << stats.comparisons << ","
                    << stats.swaps << ","
                    << (sorted ? "Yes" : "No") << "\n";
        }
    }
    
    void runFullBenchmark() {
        std::vector<int> sizes = {100, 1000, 5000, 10000, 50000};
        std::vector<DataType> dataTypes = {
            DataType::RANDOM,
            DataType::SORTED,
            DataType::REVERSE_SORTED,
            DataType::NEARLY_SORTED,
            DataType::MANY_DUPLICATES,
            DataType::FEW_UNIQUE
        };
        
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "COMPREHENSIVE SORTING ALGORITHM BENCHMARK\n";
        std::cout << "Algorithms tested: " << algorithms.size() << "\n";
        std::cout << std::string(80, '=') << "\n";
        
        for (int size : sizes) {
            for (DataType type : dataTypes) {
                runBenchmark(size, type);
            }
        }
        
        std::cout << "\n" << std::string(80, '=') << "\n";
        std::cout << "Benchmark complete! Results saved to benchmark_results.csv\n";
        std::cout << std::string(80, '=') << "\n";
    }
};

int main() {
    std::cout << "Sorting Algorithms Benchmark Suite\n";
    std::cout << "===================================\n\n";
    
    std::cout << "This benchmark tests the following algorithms:\n";
    std::cout << "  1. Insertion Sort - O(n²) - Used for small arrays\n";
    std::cout << "  2. Merge Sort - O(n log n) - Stable, used in Python/Java\n";
    std::cout << "  3. Quick Sort - O(n log n) avg - Part of C++ STL\n";
    std::cout << "  4. Heap Sort - O(n log n) - Part of C++ STL\n";
    std::cout << "  5. Introsort - O(n log n) - Used in C++ std::sort\n";
    std::cout << "  6. Timsort - O(n log n) - Used in Python sorted()\n";
    std::cout << "  7. Shell Sort - O(n^1.5) - Used in embedded systems\n";
    std::cout << "  8. Counting Sort - O(n+k) - For limited range integers\n";
    std::cout << "  9. Radix Sort - O(d*n) - For large integer datasets\n\n";
    
    std::cout << "Data patterns tested:\n";
    std::cout << "  - Random data\n";
    std::cout << "  - Already sorted\n";
    std::cout << "  - Reverse sorted\n";
    std::cout << "  - Nearly sorted (95% sorted)\n";
    std::cout << "  - Many duplicates\n";
    std::cout << "  - Few unique values\n\n";
    
    std::cout << "Press Enter to start benchmark...\n";
    std::cin.get();
    
    BenchmarkSuite suite;
    suite.runFullBenchmark();
    
    return 0;
}
