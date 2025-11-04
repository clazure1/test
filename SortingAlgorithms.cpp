#include "SortingAlgorithms.h"
#include <algorithm>
#include <cmath>
#include <climits>

// ============= Insertion Sort =============
void InsertionSort::sort(std::vector<int>& arr) {
    sortRange(arr, 0, arr.size() - 1);
}

void InsertionSort::sortRange(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && compare(key, arr[j])) {
            arr[j + 1] = arr[j];
            stats.swaps++;
            j--;
        }
        arr[j + 1] = key;
    }
}

// ============= Merge Sort =============
void MergeSort::sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    std::vector<int> temp(arr.size());
    mergeSort(arr, 0, arr.size() - 1, temp);
}

void MergeSort::mergeSort(std::vector<int>& arr, int left, int right, std::vector<int>& temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, temp);
        mergeSort(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

void MergeSort::merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& temp) {
    int i = left;
    int j = mid + 1;
    int k = left;
    
    while (i <= mid && j <= right) {
        if (compare(arr[i], arr[j]) || !compare(arr[j], arr[i])) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    
    for (int i = left; i <= right; i++) {
        arr[i] = temp[i];
        stats.swaps++;
    }
}

// ============= Quick Sort =============
void QuickSort::sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    quickSort(arr, 0, arr.size() - 1);
}

void QuickSort::quickSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int pi = partition(arr, left, right);
        quickSort(arr, left, pi - 1);
        quickSort(arr, pi + 1, right);
    }
}

int QuickSort::partition(std::vector<int>& arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;
    
    for (int j = left; j < right; j++) {
        if (compare(arr[j], pivot)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1;
}

// ============= Heap Sort =============
void HeapSort::sort(std::vector<int>& arr) {
    int n = arr.size();
    
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void HeapSort::heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && compare(arr[largest], arr[left])) {
        largest = left;
    }
    
    if (right < n && compare(arr[largest], arr[right])) {
        largest = right;
    }
    
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// ============= Introsort (C++ STL style) =============
void IntroSort::sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    int depthLimit = getMaxDepth(arr.size());
    introSort(arr, 0, arr.size() - 1, depthLimit);
}

void IntroSort::introSort(std::vector<int>& arr, int left, int right, int depthLimit) {
    int size = right - left + 1;
    
    if (size <= 16) {
        insertionSort.sortRange(arr, left, right);
        stats.comparisons += insertionSort.getStats().comparisons;
        stats.swaps += insertionSort.getStats().swaps;
        insertionSort.resetStats();
        return;
    }
    
    if (depthLimit == 0) {
        std::vector<int> temp(arr.begin() + left, arr.begin() + right + 1);
        heapSort.sort(temp);
        stats.comparisons += heapSort.getStats().comparisons;
        stats.swaps += heapSort.getStats().swaps;
        heapSort.resetStats();
        
        for (int i = 0; i < temp.size(); i++) {
            arr[left + i] = temp[i];
        }
        return;
    }
    
    int pi = partition(arr, left, right);
    introSort(arr, left, pi - 1, depthLimit - 1);
    introSort(arr, pi + 1, right, depthLimit - 1);
}

int IntroSort::partition(std::vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (compare(arr[right], arr[left])) {
        swap(arr[left], arr[right]);
    }
    if (compare(arr[mid], arr[left])) {
        swap(arr[left], arr[mid]);
    }
    if (compare(arr[right], arr[mid])) {
        swap(arr[mid], arr[right]);
    }
    
    int pivot = arr[mid];
    swap(arr[mid], arr[right - 1]);
    
    int i = left;
    int j = right - 1;
    
    while (true) {
        while (compare(arr[++i], pivot)) {}
        while (compare(pivot, arr[--j])) {}
        
        if (i >= j) break;
        swap(arr[i], arr[j]);
    }
    
    swap(arr[i], arr[right - 1]);
    return i;
}

int IntroSort::getMaxDepth(int n) {
    return 2 * static_cast<int>(std::log2(n));
}

// ============= Timsort (Python/Java style) =============
void TimSort::sort(std::vector<int>& arr) {
    timSort(arr);
}

void TimSort::timSort(std::vector<int>& arr) {
    int n = arr.size();
    int minRun = calcMinRun(n);
    
    for (int start = 0; start < n; start += minRun) {
        int end = std::min(start + minRun - 1, n - 1);
        insertionSort.sortRange(arr, start, end);
        stats.comparisons += insertionSort.getStats().comparisons;
        stats.swaps += insertionSort.getStats().swaps;
        insertionSort.resetStats();
    }
    
    for (int size = minRun; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);
            
            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}

int TimSort::calcMinRun(int n) {
    int r = 0;
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

void TimSort::merge(std::vector<int>& arr, int left, int mid, int right) {
    int len1 = mid - left + 1;
    int len2 = right - mid;
    
    std::vector<int> leftArr(len1);
    std::vector<int> rightArr(len2);
    
    for (int i = 0; i < len1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int i = 0; i < len2; i++) {
        rightArr[i] = arr[mid + 1 + i];
    }
    
    int i = 0, j = 0, k = left;
    
    while (i < len1 && j < len2) {
        if (compare(leftArr[i], rightArr[j]) || !compare(rightArr[j], leftArr[i])) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
        stats.swaps++;
    }
    
    while (i < len1) {
        arr[k++] = leftArr[i++];
        stats.swaps++;
    }
    
    while (j < len2) {
        arr[k++] = rightArr[j++];
        stats.swaps++;
    }
}

// ============= Shell Sort =============
void ShellSort::sort(std::vector<int>& arr) {
    int n = arr.size();
    
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap && compare(temp, arr[j - gap]); j -= gap) {
                arr[j] = arr[j - gap];
                stats.swaps++;
            }
            arr[j] = temp;
        }
    }
}

// ============= Counting Sort =============
void CountingSort::sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    
    int minVal = *std::min_element(arr.begin(), arr.end());
    int maxVal = *std::max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;
    
    std::vector<int> count(range, 0);
    std::vector<int> output(arr.size());
    
    for (int i = 0; i < arr.size(); i++) {
        count[arr[i] - minVal]++;
        stats.comparisons++;
    }
    
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
        stats.swaps++;
    }
    
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = output[i];
    }
}

// ============= Radix Sort =============
void RadixSort::sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    
    int maxVal = *std::max_element(arr.begin(), arr.end());
    
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSort(arr, exp);
    }
}

void RadixSort::countingSort(std::vector<int>& arr, int exp) {
    int n = arr.size();
    std::vector<int> output(n);
    std::vector<int> count(10, 0);
    
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
        stats.comparisons++;
    }
    
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        stats.swaps++;
    }
    
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}
