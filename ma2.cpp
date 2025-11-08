
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
//Insertion sort
template <typename T>
void insertion_sort(vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        T key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j-1] > key) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }
}

//Quick Sort
template <typename T>
int partition(vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = low -1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[high]);

        }
    }
    swap(arr[i+1], arr[high]);
    return i + 1;
}

template <typename T>
void quicksort(vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);

    }
}


// Shell Sort
template <typename T>
void shell_sort(vector<T>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            T temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
                arr[j] = temp;
            }
        }
    }
}

//Helper Functions
vector<int> make_random_vector(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100000);
    vector<int> v(n);
    for (int& x : v) x = dis(gen);
    return v;
}

double time_sort(void(*sort_func)(vector<int>&), vector<int> data) {
    auto start = chrono::high_resolution_clock::now();
    sort_func(data);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    return elapsed.count();

}
void run_trials(const string& name, void(*sort_func)(vector<int>&), int n, int trials) {
    vector<double> times;
    for (int i = 0; i < trials; i++) {
        vector<int> data = make_random_vector(n);
        double t = time_sort(sort_func, data);
        times.push_back(t);
        cout << name << " Trial " << i+1 << ": " << fixed << setprecision(3) << t << "ms\n";

    }
    double avg = accumulate(times.begin(), times.end(), 0.0) / times.size();
    cout << name << " Avg: " << avg << "ms\n\n";

}
//Main
int main() {
    int n = 10000;
    int trials = 5;

    cout << "Sorting " << n << " random integers (" << trials << " trials each)\n\n";

    run_trials("Insertion Sort", insertion_sort<int>, n, trials);
    run_trials("Quicksort", [](vector<int>& v){ quicksort(v, 0, v.size()-1); }, n, trials);
    run_trials("Shell Sort", shell_sort<int>, n, trials);

    return 0;

}
