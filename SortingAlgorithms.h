#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <vector>
#include <string>
#include <cstdint>

// Statistics structure to track algorithm performance
struct SortStats {
    uint64_t comparisons;
    uint64_t swaps;
    double time_ms;
    std::string algorithm_name;
    
    SortStats() : comparisons(0), swaps(0), time_ms(0.0) {}
    
    void reset() {
        comparisons = 0;
        swaps = 0;
        time_ms = 0.0;
    }
};

// Base sorting interface
class SortingAlgorithm {
protected:
    SortStats stats;
    
    template<typename T>
    bool compare(const T& a, const T& b) {
        stats.comparisons++;
        return a < b;
    }
    
    template<typename T>
    void swap(T& a, T& b) {
        stats.swaps++;
        T temp = std::move(a);
        a = std::move(b);
        b = std::move(temp);
    }
    
public:
    virtual ~SortingAlgorithm() = default;
    virtual void sort(std::vector<int>& arr) = 0;
    virtual std::string getName() const = 0;
    
    const SortStats& getStats() const { return stats; }
    void resetStats() { stats.reset(); }
};

// Insertion Sort - used for small arrays in hybrid algorithms
class InsertionSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr) override;
    void sortRange(std::vector<int>& arr, int left, int right);
    std::string getName() const override { return "Insertion Sort"; }
};

// Merge Sort - stable, O(n log n), used in Python/Java (Timsort)
class MergeSort : public SortingAlgorithm {
private:
    void mergeSort(std::vector<int>& arr, int left, int right, std::vector<int>& temp);
    void merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& temp);
    
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Merge Sort"; }
};

// Quick Sort - average O(n log n), used in C++ STL (as part of Introsort)
class QuickSort : public SortingAlgorithm {
private:
    void quickSort(std::vector<int>& arr, int left, int right);
    int partition(std::vector<int>& arr, int left, int right);
    
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Quick Sort"; }
};

// Heap Sort - O(n log n) worst case, used in C++ STL (as part of Introsort)
class HeapSort : public SortingAlgorithm {
private:
    void heapify(std::vector<int>& arr, int n, int i);
    
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Heap Sort"; }
};

// Introsort - hybrid algorithm used in C++ STL std::sort
class IntroSort : public SortingAlgorithm {
private:
    InsertionSort insertionSort;
    HeapSort heapSort;
    
    void introSort(std::vector<int>& arr, int left, int right, int depthLimit);
    int partition(std::vector<int>& arr, int left, int right);
    int getMaxDepth(int n);
    
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Intro Sort (STL-style)"; }
};

// Timsort - hybrid algorithm used in Python and Java
class TimSort : public SortingAlgorithm {
private:
    static const int MIN_MERGE = 32;
    
    InsertionSort insertionSort;
    
    void timSort(std::vector<int>& arr);
    int calcMinRun(int n);
    void merge(std::vector<int>& arr, int left, int mid, int right);
    
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Tim Sort (Python-style)"; }
};

// Shell Sort - improved insertion sort, used in embedded systems
class ShellSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Shell Sort"; }
};

// Counting Sort - O(n+k) for integers in limited range
class CountingSort : public SortingAlgorithm {
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Counting Sort"; }
};

// Radix Sort - O(d*n) for integers, used for large datasets
class RadixSort : public SortingAlgorithm {
private:
    void countingSort(std::vector<int>& arr, int exp);
    
public:
    void sort(std::vector<int>& arr) override;
    std::string getName() const override { return "Radix Sort"; }
};

#endif // SORTING_ALGORITHMS_H
