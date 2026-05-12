#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

class Parallel_bubble_merge
{
public:

    // ================= PARALLEL BUBBLE SORT =================

    void parallel_bubble(vector<int> &arr)
{
    int n = arr.size();

    for (int i = 0; i < n; i++)
    {

        // Even Phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd Phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

    // ================= MERGE FUNCTION =================

    void merge(vector<int> &arr, int low, int mid, int high)
    {
        vector<int> temp;

        int i = low;
        int j = mid + 1;

        while (i <= mid && j <= high)
        {
            if (arr[i] < arr[j])
            {
                temp.push_back(arr[i]);
                i++;
            }
            else
            {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid)
        {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= high)
        {
            temp.push_back(arr[j]);
            j++;
        }

        for (int i = low; i <= high; i++)
        {
            arr[i] = temp[i - low];
        }
    }

    // ================= PARALLEL MERGE SORT =================

    void parallel_merge(vector<int> &arr, int low, int high)
    {
        if (low < high)
        {
            int mid = (low + high) / 2;

#pragma omp parallel sections
            {

#pragma omp section
                parallel_merge(arr, low, mid);

#pragma omp section
                parallel_merge(arr, mid + 1, high);
            }

            merge(arr, low, mid, high);
        }
    }

    void print_array_parallel(vector<int> &arr)
    {
        for (int n : arr)
        {
            cout << n << " ";
        }

        cout << endl;
    }
};

// ==========================================================

class sequential_bubble_merge
{
public:

    // ================= SEQUENTIAL BUBBLE SORT =================

    void sequential_bubble(vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    // ================= MERGE FUNCTION =================

    void merge(vector<int> &arr, int low, int mid, int high)
    {
        vector<int> temp;

        int i = low;
        int j = mid + 1;

        while (i <= mid && j <= high)
        {
            if (arr[i] < arr[j])
            {
                temp.push_back(arr[i]);
                i++;
            }
            else
            {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid)
        {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= high)
        {
            temp.push_back(arr[j]);
            j++;
        }

        for (int i = low; i <= high; i++)
        {
            arr[i] = temp[i - low];
        }
    }

    // ================= SEQUENTIAL MERGE SORT =================

    void sequential_merge(vector<int> &arr, int low, int high)
    {
        if (low < high)
        {
            int mid = (low + high) / 2;

            sequential_merge(arr, low, mid);
            sequential_merge(arr, mid + 1, high);

            merge(arr, low, mid, high);
        }
    }

    void print_array_sequential(vector<int> &arr)
    {
        for (int n : arr)
        {
            cout << n << " ";
        }

        cout << endl;
    }
};

// ==========================================================

int main()
{
    Parallel_bubble_merge p;
    sequential_bubble_merge s;

    vector<int> arr1;

    for (int i = 500; i >= 1; i--)
    {
        arr1.push_back(i);
    }

    vector<int> arr2 = arr1;
    vector<int> arr3 = arr1;
    vector<int> arr4 = arr1;

    // ================= SEQUENTIAL BUBBLE =================

    auto start = high_resolution_clock::now();

    s.sequential_bubble(arr1);

    auto end = high_resolution_clock::now();

    auto duration =
        duration_cast<microseconds>(end - start);

    cout << "\nSequential Bubble Sort\n";

    // s.print_array_sequential(arr1);

    cout << "Time : "
         << duration.count()
         << " microseconds\n";

    // ================= PARALLEL BUBBLE =================

    auto start1 = high_resolution_clock::now();

    p.parallel_bubble(arr2);

    auto end1 = high_resolution_clock::now();

    auto duration2 =
        duration_cast<microseconds>(end1 - start1);

    cout << "\nParallel Bubble Sort\n";

    // p.print_array_parallel(arr2);

    cout << "Time : "
         << duration2.count()
         << " microseconds\n";

    // ================= PARALLEL MERGE =================

    auto start2 = high_resolution_clock::now();

    p.parallel_merge(arr3, 0, 499);

    auto end2 = high_resolution_clock::now();

    auto duration3 =
        duration_cast<microseconds>(end2 - start2);

    cout << "\nParallel Merge Sort\n";

    // p.print_array_parallel(arr3);

    cout << "Time : "
         << duration3.count()
         << " microseconds\n";

    // ================= SEQUENTIAL MERGE =================

    auto start3 = high_resolution_clock::now();

    s.sequential_merge(arr4, 0, 499);

    auto end3 = high_resolution_clock::now();

    auto duration4 =
        duration_cast<microseconds>(end3 - start3);

    cout << "\nSequential Merge Sort\n";

    // s.print_array_sequential(arr4);

    cout << "Time : "
         << duration4.count()
         << " microseconds\n";

    return 0;
}