#include <bits/stdc++.h>
using namespace std;

/*
    Stack
    
    Whenever problem says:
        nearest greater/smaller
        undo operations
        bracket matching
        recursion simulation
        maintain history
        process elements in reverse dependency

    your brain should think: STACK

    Monotonic Stack

    Whenever problem says:
        nearest greater/smaller
        sliding window maximum/minimum
        maintain a window of elements with a specific order (increasing/decreasing)
        process elements in reverse dependency but with a specific order
*/

void solve() {
    stack<int> st;

    for(int i = 0; i < n; i++) {
        while(!st.empty() && /*condition*/) st.pop();

        // answer using st.top()

        st.push(a[i]);
    }
    
    //... more code
}

void monotonicStackSkeleton() {
    stack<int> st;

    for(int i = direction; ... ) {}
        while(!st.empty() && bad(st.top(), a[i])) {
            st.pop();
        }

        // answer from top

        st.push(a[i]);
    }
}

vector<int> nextGreaterElement(const vector<int>& arr) {
    int n = arr.size();
    // Initialize with -1 (or size 'n' depending on the problem requirements)
    vector<int> nge(n, -1); 
    
    // The stack will store INDICES, not the actual values.
    // This allows us to calculate distances (e.g., span = i - st.top())
    stack<int> st; 

    for (int i = 0; i < n; ++i) {
        // Maintain monotonic decreasing stack:
        // While current element is GREATER than the element at stack top,
        // it means we found the NGE for the element at stack top.
        while (!st.empty() && arr[st.top()] < arr[i]) {
            nge[st.top()] = arr[i]; // Store the value (or store index 'i')
            st.pop();
        }
        st.push(i);
    }
    return nge;
}

/*
    Queue

    Whenever problem says:
        process elements in order
        sliding window
        maintain a window of elements
        BFS
        level order traversal
        scheduling
        simulations

    your brain should think: QUEUE
*/

/*
    Deque

    Whenever problem says:
        0-1 BFS shortest path
        sliding window maximum/minimum
        maintain a window of elements with both ends
        process elements in order but also need to remove from front and back
*/

vector<int> slidingWindowMaximum(const vector<int>& arr, int k) {
    vector<int> res;
    
    // The deque will store INDICES of array elements.
    // The front of the deque will always be the index of the MAXIMUM element in the current window.
    deque<int> dq; 

    for (int i = 0; i < arr.size(); ++i) {
        // 1. Remove elements that are no longer in the current window of size k
        if (!dq.empty() && dq.front() == i - k) {
            dq.pop_front();
        }
        
        // 2. Maintain the monotonic strictly decreasing order
        // Remove all indices whose values are LESS THAN OR EQUAL to the current element.
        // They are useless because they are smaller AND older than the current element.
        while (!dq.empty() && arr[dq.back()] <= arr[i]) {
            dq.pop_back();
        }
        
        // 3. Add current element's index
        dq.push_back(i);
        
        // 4. If our window has hit size k, record the maximum (at the front of deque)
        if (i >= k - 1) {
            res.push_back(arr[dq.front()]);
        }
    }
    return res;
}

void dequeSkeleton() { 
    deque<int> dq;

    for(int i = 0; i < n; i++) {

        while(!dq.empty() && outside_window) {
            dq.pop_front();
        }

        while(!dq.empty() && useless_elements) {
            dq.pop_back();
        }

        dq.push_back(i);
    }
}