#include <bits/stdc++.h>
using namespace std;

int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; 

        if (arr[mid] == target) return mid; 
        else if (arr[mid] < target) left = mid + 1; 
        else right = mid - 1;
    }
    return -1; 
}

// This function contains the logic to check if a proposed 'mid' answer is possible
bool check(int mid /* pass necessary data structures */) {
    // Implement problem-specific logic here
    // Example: Can we divide the array into K parts such that max sum is <= mid?
    
    // return true if 'mid' satisfies conditions, else false
    return true; 
}

int binarySearchOnAnswer(int minPossible, int maxPossible) {
    int left = minPossible;
    int right = maxPossible;
    int ans = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (check(mid)) {
            ans = mid;
            
            // DEPENDING ON THE PROBLEM:
            // If you want to MINIMIZE the answer, discard the right half
            right = mid - 1; 
            
            // If you want to MAXIMIZE the answer, discard the left half
            // left = mid + 1; 
        } else {
            // DEPENDING ON THE PROBLEM:
            // If 'mid' was too small to be valid, search right
            left = mid + 1;  
            
            // If 'mid' was too large to be valid, search left
            // right = mid - 1; 
        }
    }
    return ans;
}

// Define the unimodal function
double f(double x) {
    // Return the value of the function at x
    return x * x - 4 * x + 4; // Example: Valley at x = 2
}

double ternarySearchContinuous(double left, double right) {
    double eps = 1e-9;

    while (right - left > eps) {
        double m1 = left + (right - left) / 3.0;
        double m2 = right - (right - left) / 3.0;

        if (f(m1) < f(m2)) {
            // For finding the MINIMUM: the minimum cannot be in the [m2, right] range
            right = m2; 
            
            // Note: If finding the MAXIMUM, swap the logic: left = m1;
        } else {
            // For finding the MINIMUM
            left = m1; 
        }
    }
    return left; // left, right, or (left+right)/2 are all acceptable here
}

// Define the unimodal function or array access
long long f(long long x) {
    // Return array[x] or function calculation
    return 0; 
}

long long ternarySearchDiscrete(long long left, long long right) {
    // Stop when the range is 3 elements or smaller
    while (right - left > 2) {
        long long m1 = left + (right - left) / 3;
        long long m2 = right - (right - left) / 3;

        if (f(m1) < f(m2)) {
            // Finding MINIMUM
            right = m2; 
        } else {
            left = m1;
        }
    }

    // Linearly scan the remaining few elements for the absolute minimum
    long long best_x = left;
    for (long long i = left + 1; i <= right; i++) {
        if (f(i) < f(best_x)) { // < for minimum, > for maximum
            best_x = i;
        }
    }
    return best_x;
}