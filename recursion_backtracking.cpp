/**
 * ============================================================================
 * RECURSION & BACKTRACKING
 * ============================================================================
 * * Core Philosophy: 
 * 1. Base Case (Stop condition + record answer)
 * 2. Iterate over valid choices
 * 3. CHOOSE (Update state/path)
 * 4. EXPLORE (Recursive call)
 * 5. UNCHOOSE (Backtrack / Revert state)
 * * Time Complexities to Remember:
 * - Subsets (Pick/Not Pick): O(2^N)
 * - Permutations (Arrangements): O(N!)
 * - Grid DFS/Paths: O(4^(R*C)) (without memoization)
 */

#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// 1. SUBSETS / COMBINATIONS (Pick / Not Pick Pattern)
// Use Case: Finding all combinations, Knapsack variations, Subset Sum.
// ============================================================================
class SubsetBacktracking {
public:
    vector<vector<int>> all_subsets;
    
    void generateSubsets(int index, vector<int>& nums, vector<int>& current_path) {
        // Base Case: Processed all elements
        if (index == nums.size()) {
            all_subsets.push_back(current_path);
            return;
        }
        
        // Choice 1: Include the current element
        current_path.push_back(nums[index]);
        generateSubsets(index + 1, nums, current_path);
        
        // Backtrack (Unchoose)
        current_path.pop_back();
        
        // Choice 2: Exclude the current element
        generateSubsets(index + 1, nums, current_path);
    }
    
    // Variation: Generate Subsets avoiding duplicates (nums must be sorted)
    void generateUniqueSubsets(int index, vector<int>& nums, vector<int>& current_path) {
        all_subsets.push_back(current_path);
        
        for (int i = index; i < nums.size(); ++i) {
            // Pruning: Skip duplicates at the same recursive depth
            if (i > index && nums[i] == nums[i - 1]) continue;
            
            current_path.push_back(nums[i]);
            generateUniqueSubsets(i + 1, nums, current_path);
            current_path.pop_back(); // Backtrack
        }
    }
};

// ============================================================================
// 2. PERMUTATIONS (Swapping Pattern)
// Use Case: Traveling Salesperson (TSP) brute force, arranging items.
// ============================================================================
class PermutationBacktracking {
public:
    vector<vector<int>> all_permutations;
    
    // Swapping technique (O(1) extra space for state, faster)
    void generatePermutations(int index, vector<int>& nums) {
        if (index == nums.size()) {
            all_permutations.push_back(nums);
            return;
        }
        
        for (int i = index; i < nums.size(); ++i) {
            swap(nums[index], nums[i]);             // Choose
            generatePermutations(index + 1, nums);  // Explore
            swap(nums[index], nums[i]);             // Unchoose (Backtrack)
        }
    }
};

// ============================================================================
// 3. GRID / MAZE TRAVERSAL (DFS with Backtracking)
// Use Case: Word Search, Rat in a Maze, Number of Islands (without backtrack).
// ============================================================================
class GridBacktracking {
private:
    int rows, cols;
    // Standard Direction Arrays: Up, Right, Down, Left
    int dRow[4] = {-1, 0, 1, 0};
    int dCol[4] = {0, 1, 0, -1};

    bool isValid(int r, int c, const vector<vector<int>>& grid, const vector<vector<bool>>& visited) {
        return (
            r >= 0 && r < rows &&
            c >= 0 && c < cols && 
            grid[r][c] == 1 && !visited[r][c]
        ); // Assuming 1 is a valid path
    }

public:
    void exploreGrid(int r, int c, const vector<vector<int>>& grid, vector<vector<bool>>& visited, string& path) {
        // Base Case Check (e.g., reached bottom-right corner)
        if (r == rows - 1 && c == cols - 1) {
            // Record answer/path
            cout << path << "
";
            return;
        }
        
        // Mark current cell as visited to prevent cycles in current path
        visited[r][c] = true;
        
        string dir_chars = "URDL";
        
        for (int i = 0; i < 4; ++i) {
            int next_r = r + dRow[i];
            int next_c = c + dCol[i];
            
            if (isValid(next_r, next_c, grid, visited)) {
                path.push_back(dir_chars[i]);              // Choose
                exploreGrid(next_r, next_c, grid, visited, path); // Explore
                path.pop_back();                           // Unchoose (Backtrack)
            }
        }
        
        // Unmark current cell so it can be used in OTHER paths
        visited[r][c] = false; 
    }
};

// ============================================================================
// 4. CONSTRAINT SATISFACTION (N-Queens Pattern)
// Use Case: Sudoku Solver, N-Queens, Graph Coloring.
// ============================================================================
class ConstraintBacktracking {
public:
    vector<vector<string>> solutions;
    
    bool isSafe(int row, int col, int n, const vector<string>& board) {
        // Check column (above)
        for (int i = 0; i < row; ++i)
            if (board[i][col] == 'Q') return false;
            
        // Check upper-left diagonal
        for (int i = row, j = col; i >= 0 && j >= 0; --i, --j)
            if (board[i][j] == 'Q') return false;
            
        // Check upper-right diagonal
        for (int i = row, j = col; i >= 0 && j < n; --i, ++j)
            if (board[i][j] == 'Q') return false;
            
        return true;
    }
    
    void solveNQueens(int row, int n, vector<string>& board) {
        if (row == n) {
            solutions.push_back(board);
            return;
        }
        
        for (int col = 0; col < n; ++col) {
            if (isSafe(row, col, n, board)) {
                board[row][col] = 'Q';              // Choose
                solveNQueens(row + 1, n, board);    // Explore next row
                board[row][col] = '.';              // Unchoose (Backtrack)
            }
        }
    }
};
