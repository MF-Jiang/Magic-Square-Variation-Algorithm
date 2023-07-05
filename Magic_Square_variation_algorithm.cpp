#include <iostream>
#include <cstdlib>
#include <climits>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <unistd.h>

int MAX_TIME = 0;

using namespace std;

// This function calculates the score of a given magic square
int objective_function(vector<vector<int>> &magic_square, int n, int M) {
    // Initialize vectors to store row and column sums
    vector<int> row_sums(n, 0);
    vector<int> col_sums(n, 0);
    // Initialize variables to store diagonal sums
    int diag_sum1 = 0;
    int diag_sum2 = 0;
    // Calculate n - 1 for use in diagonal sum calculation
    int n_minus_1 = n - 1;
    // Loop through each element in the magic square
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Add element to row and column sums
            row_sums[i] += magic_square[i][j];
            col_sums[j] += magic_square[i][j];
            // Check if element is on the main diagonal
            if (i == j) {
                diag_sum1 += magic_square[i][j];
            }
            // Check if element is on the secondary diagonal
            if (i + j == n_minus_1) {
                diag_sum2 += magic_square[i][j];
            }
        }
    }
    int score = 0;
    // Loop through row and column sums and add their absolute differences from M to the score
    for (int i = 0; i < n; i++) {
        score += abs(row_sums[i] - M);
        score += abs(col_sums[i] - M);
    }
    // Add absolute differences of diagonal sums from M to the score
    score += abs(diag_sum1 - M);
    score += abs(diag_sum2 - M);
    // Return final score
    return score;
}


// This function swaps two elements in a given magic square
void swap_elements(vector<vector<int>> &magic_square, int n) {
    // Initialize variables to store the indices of the elements to be swapped
    int i1, j1, i2, j2;
    // Generate random indices for the first element
    i1 = rand() % n;
    j1 = rand() % n;
    // Generate random indices for the second element
    i2 = rand() % n;
    j2 = rand() % n;
    // Ensure that the two elements are not the same
    while (i1 == i2 && j1 == j2) {
        // Generate new random indices for the second element
        i2 = rand() % n;
        j2 = rand() % n;
    }
    // Swap the two elements
    swap(magic_square[i1][j1], magic_square[i2][j2]);
}


// This function swaps elements in a given magic square to try to make the row, column and diagonal sums equal to M or wish they could be M
void swap_LLH0(vector<vector<int>> &magic_square, int n, int M) {
    // Generate a random number to determine which row, column or diagonal to work on
    int deter = rand() % 4;
    // If deter is 0, work on a row
    if (deter == 0) {
        // Initialize variables to store the row index and sum
        int row = 0;
        int sum = 0;
        // Find the first row whose sum is not equal to M
        while (row < n) {
            // Calculate the sum of the current row
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[row][i];
            }
            // If the sum is not equal to M, break out of the loop
            if (sum != M)
                break;
            // Move on to the next row
            row++;
        }
        // If all rows have a sum equal to M, return without making any changes
        if (row == n)
            return;
        // If the sum of the selected row is greater than M
        if (sum > M) {
            // Find all columns in the selected row whose value is greater than M/n
            vector<int> tempcol;
            for (int i = 0; i < n; i++) {
                if (magic_square[row][i] > M / n) {
                    tempcol.push_back(i);
                }
            }
            // Randomly select one of these columns
            int col1 = rand() % tempcol.size();
            // Find all valid coordinates for swapping
            vector<pair<int, int>> valid_coords;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // Check if the current element is not in the selected row and its value is less than or equal to M/n
                    if (i != row && magic_square[i][j] <= M / n) {
                        // Calculate the sum of the current row
                        int tempsum = 0;
                        for (int k = 0; k < n; k++) {
                            tempsum += magic_square[i][k];
                        }
                        // If the sum of the current row is not equal to M, add the current coordinates to valid_coords
                        if (tempsum != M) {
                            valid_coords.push_back(make_pair(i, j));
                        }
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element in the selected row
                int idx = rand() % valid_coords.size();
                swap(magic_square[row][col1], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        } else {
            // If the sum of the selected row is less than M
            // Find all columns in the selected row whose value is less than M/n
            vector<int> tempcol;
            for (int i = 0; i < n; i++) {
                if (magic_square[row][i] < M / n) {
                    tempcol.push_back(i);
                }
            }
            // Randomly select one of these columns
            int col1 = rand() % tempcol.size();
            // Find all valid coordinates for swapping
            vector<pair<int, int>> valid_coords;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // Check if the current element is not in the selected row and its value is greater than or equal to M/n
                    if (i != row && magic_square[i][j] >= M / n) {
                        // Calculate the sum of the current row
                        int tempsum = 0;
                        for (int k = 0; k < n; k++) {
                            tempsum += magic_square[i][k];
                        }
                        // If the sum of the current row is not equal to M, add the current coordinates to valid_coords
                        if (tempsum != M) {
                            valid_coords.push_back(make_pair(i, j));
                        }
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one to swap
                int idx = rand() % valid_coords.size();
                swap(magic_square[row][col1], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        }
    }
    // If deter is 1, work on a column
    else if (deter == 1)
    {
        // Initialize variables to store the column index and sum
        int col = 0;
        int sum = 0;
        // Find the first column whose sum is not equal to M
        while (col < n) {
            // Calculate the sum of the current column
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[i][col];
            }
            // If the sum is not equal to M, break out of the loop
            if (sum != M)
                break;
            // Move on to the next column
            col++;
        }
        // If all columns have a sum equal to M, return without making any changes
        if (col == n)
            return;
        // If the sum of the selected column is greater than M
        if (sum > M) {
            // Find all rows in the selected column whose value is greater than M/n
            vector<int> temprow;
            for(int i=0;i<n;i++){
                if(magic_square[i][col]>M/n){
                    temprow.push_back(i);
                }
            }
            // Randomly select one of these rows
            int row1 = rand() % temprow.size();

            // Find all valid coordinates for swapping
            vector<pair<int, int>> valid_coords;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // Check if the current element is not in the selected column and its value is less than or equal to M/n
                    if (i != col && magic_square[i][j] <= M / n) {
                        // Calculate the sum of the current row
                        int tempsum = 0;
                        for (int k = 0; k < n; k++) {
                            tempsum += magic_square[i][k];
                        }
                        // If the sum of the current row is not equal to M, add the current coordinates to valid_coords
                        if (tempsum != M) {
                            valid_coords.push_back(make_pair(i, j));
                        }
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element in the selected column
                int idx = rand() % valid_coords.size();
                swap(magic_square[row1][col], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        } else {
            // If the sum of the selected column is less than M
            // Find all rows in the selected column whose value is less than M/n
            vector<int> temprow;
            for(int i=0;i<n;i++){
                if(magic_square[i][col]<M/n){
                    temprow.push_back(i);
                }
            }
            // Randomly select one of these rows
            int row1 = rand() % temprow.size();

            // Find all valid coordinates for swapping
            vector<pair<int, int>> valid_coords;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // Check if the current element is not in the selected column and its value is greater than or equal to M/n
                    if (i != col && magic_square[i][j] >= M / n) {
                        // Calculate the sum of the current row
                        int tempsum = 0;
                        for (int k = 0; k < n; k++) {
                            tempsum += magic_square[i][k];
                        }
                        // If the sum of the current row is not equal to M, add the current coordinates to valid_coords
                        if (tempsum != M) {
                            valid_coords.push_back(make_pair(i, j));
                        }
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element in the selected column
                int idx = rand() % valid_coords.size();
                swap(magic_square[row1][col], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        }
    }
    // If deter is 2, work on the main diagonal
    else if (deter == 2) {
        // Calculate the sum of the main diagonal
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][i];
        }
        // If the sum of the main diagonal is equal to M, return without making any changes
        if (sum == M)
            return;
        // If the sum of the main diagonal is greater than M
        if (sum > M) {
            // Find all positions on the main diagonal whose value is greater than M/n
            vector<int> temppos;
            for(int i=0;i<n;i++){
                if(magic_square[i][i]>M/n){
                    temppos.push_back(i);
                }
            }
            // Randomly select one of these positions
            int pos = rand() % temppos.size();

            // Find all valid coordinates for swapping
            vector<pair<int,int>> valid_coords;
            for(int i=0; i<n;i++){
                for(int j=0;j<n;j++){
                    // Check if the current element is not on the main diagonal and its value is less than or equal to M/n
                    if(i!=j && magic_square[i][j]<=M/n){
                        valid_coords.push_back(make_pair(i,j));
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element on the main diagonal
                int idx = rand() % valid_coords.size();
                swap(magic_square[pos][pos], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }

        } else {
            // If the sum of the main diagonal is less than M
            // Find all positions on the main diagonal whose value is less than M/n
            vector<int> temppos;
            for(int i=0;i<n;i++){
                if(magic_square[i][i]<M/n){
                    temppos.push_back(i);
                }
            }
            // Randomly select one of these positions
            int pos = rand() % temppos.size();

            // Find all valid coordinates for swapping
            vector<pair<int,int>> valid_coords;
            for(int i=0; i<n;i++){
                for(int j=0;j<n;j++){
                    // Check if the current element is not on the main diagonal and its value is greater than or equal to M/n
                    if(i!=j && magic_square[i][j]>=M/n){
                        valid_coords.push_back(make_pair(i,j));
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element on the main diagonal
                int idx = rand() % valid_coords.size();
                swap(magic_square[pos][pos], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        }
    }
    else {
        // If deter is 3, work on the secondary diagonal
        // Calculate the sum of the secondary diagonal
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][n - 1 - i];
        }
        // If the sum of the secondary diagonal is equal to M, return without making any changes
        if (sum == M)
            return;
        // If the sum of the secondary diagonal is greater than M
        if (sum > M) {
            // Find all positions on the secondary diagonal whose value is greater than M/n
            vector<int> temppos;
            for(int i=0;i<n;i++){
                if(magic_square[i][n-1-i]>M/n){
                    temppos.push_back(i);
                }
            }
            // Randomly select one of these positions
            int pos = rand() % temppos.size();

            // Find all valid coordinates for swapping
            vector<pair<int,int>> valid_coords;
            for(int i=0; i<n;i++){
                for(int j=0;j<n;j++){
                    // Check if the current element is not on the secondary diagonal and its value is less than or equal to M/n
                    if(i!=n-1-j && magic_square[i][j]<=M/n){
                        valid_coords.push_back(make_pair(i,j));
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element on the secondary diagonal
                int idx = rand() % valid_coords.size();
                swap(magic_square[pos][n-1-pos], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        } else {
            // If the sum of the secondary diagonal is less than M
            // Find all positions on the secondary diagonal whose value is less than M/n
            vector<int> temppos;
            for(int i=0;i<n;i++){
                if(magic_square[i][n-1-i]<M/n){
                    temppos.push_back(i);
                }
            }
            // Randomly select one of these positions
            int pos = rand() % temppos.size();

            // Find all valid coordinates for swapping
            vector<pair<int,int>> valid_coords;
            for(int i=0; i<n;i++){
                for(int j=0;j<n;j++){
                    // Check if the current element is not on the secondary diagonal and its value is greater than or equal to M/n
                    if(i!=n-1-j && magic_square[i][j]>=M/n){
                        valid_coords.push_back(make_pair(i,j));
                    }
                }
            }
            // If there are valid coordinates for swapping
            if (!valid_coords.empty()) {
                // Randomly select one of these coordinates and swap it with the selected element on the secondary diagonal
                int idx = rand() % valid_coords.size();
                swap(magic_square[pos][n-1-pos], magic_square[valid_coords[idx].first][valid_coords[idx].second]);
            }
        }
    }
}


// This function swaps elements in a given magic square to try to make the row, column and diagonal sums equal to M (RUN LOOP)
void swap_LLH3(vector<vector<int>> &magic_square, int n, int M) {
    // Generate a random number to determine which row, column or diagonal to work on
    int deter = rand() % 4;
    // If deter is 0, work on a row
    if (deter == 0) {
        // Initialize variables to store the row index and sum
        int row = 0;
        int sum = 0;
        // Find the first row whose sum is not equal to M
        while (row < n) {
            // Calculate the sum of the current row
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[row][i];
            }
            // If the sum is not equal to M, break out of the loop
            if (sum != M)
                break;
            // Move on to the next row
            row++;
        }
        // If all rows have a sum equal to M, return without making any changes
        if (row == n)
            return;
        // Try swapping elements up to 100 times
        for (int times = 0; times < 100; times++) {
            // If the sum of the selected row is greater than M
            if (sum > M) {
                // Randomly select a column in the selected row whose value is greater than or equal to M/n
                int col1;
                do {
                    col1 = rand() % n;
                } while (magic_square[row][col1] < M / n);

                // Randomly select another element whose value is less than or equal to M/n and whose row sum is not equal to M
                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[row2][i];
                    }
                } while (row2 == row || tempsum == M || magic_square[row2][col2] > M / n);
                // Swap the two selected elements
                swap(magic_square[row][col1], magic_square[row2][col2]);

                // Calculate the new sum of the selected row
                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[row][i];
                }
                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row][col1], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                // If the sum of the selected row is less than M
                // Randomly select a column in the selected row whose value is less than or equal to M/n
                int col1;
                do {
                    col1 = rand() % n;
                } while (magic_square[row][col1] > M / n);

                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[row2][i];
                    }
                } while (row2 == row || tempsum == M || magic_square[row2][col2] < M / n);
                // Swap the two selected elements
                swap(magic_square[row][col1], magic_square[row2][col2]);

                // Calculate the new sum of the selected row
                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[row][i];
                }
                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row][col1], magic_square[row2][col2]);
                }
            } else
                // If the sum of the selected row is equal to M
                return;
        }
    }
    else if (deter == 1) {
        // If deter is 1, work on a column
        // Initialize variables to store the column index and sum
        int col = 0;
        int sum = 0;
        // Find the first column whose sum is not equal to M
        while (col < n) {
            // Calculate the sum of the current column
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[i][col];
            }
            // If the sum is not equal to M, break out of the loop
            if (sum != M)
                break;
            // Move on to the next column
            col++;
        }
        // If all columns have a sum equal to M, return without making any changes
        if (col == n)
            return;
        // Try swapping elements up to 100 times
        for (int times = 0; times < 100; times++) {
            // If the sum of the selected column is greater than M
            if (sum > M) {
                // Randomly select a row in the selected column whose value is greater than or equal to M/n
                int row1;
                do {
                    row1 = rand() % n;
                } while (magic_square[row1][col] < M / n);

                // Randomly select another element whose value is less than or equal to M/n and whose column sum is not equal to M
                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[i][col2];
                    }
                } while (col2 == col || tempsum == M || magic_square[row2][col2] > M / n);

                // Swap the two selected elements
                swap(magic_square[row1][col], magic_square[row2][col2]);

                // Calculate the new sum of the selected column
                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][col];
                }

                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row1][col], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                // If the sum of the selected column is less than M
                // Randomly select a row in the selected column whose value is less than or equal to M/n
                int row1;
                do {
                    row1 = rand() % n;
                } while (magic_square[row1][col] > M / n);

                // Randomly select another element whose value is greater than or equal to M/n and whose column sum is not equal to M
                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[i][col2];
                    }
                } while (col2 == col || tempsum == M || magic_square[row2][col2] < M / n);
                // Swap the two selected elements
                swap(magic_square[row1][col], magic_square[row2][col2]);

                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][col];
                }

                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row1][col], magic_square[row2][col2]);
                }
            } else
                // If the sum of the selected column is equal to M
                return;
        }
    }
    else if (deter == 2) {
        // If deter is 2, work on the main diagonal
        // Calculate the sum of the main diagonal
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][i];
        }
        // If the sum of the main diagonal is equal to M, return without making any changes
        if (sum == M)
            return;
        // Try swapping elements up to 100 times
        for (int times = 0; times < 100; times++) {
            // If the sum of the main diagonal is greater than M
            if (sum > M) {
                // Randomly select an element whose value is greater than or equal to M/n
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] < M / n);
                // Randomly select another element whose value is less than or equal to M/n and that is not on the main diagonal
                int row2, col2;
                do {
                    row2 = rand() % n;
                    col2 = rand() % n;
                } while (row2 == col2 || magic_square[row2][col2] > M / n);
                // Swap the two selected elements
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                // Calculate the new sum of the main diagonal
                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][i];
                }
                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                // If the sum of the main diagonal is less than M
                // Randomly select an element whose value is less than or equal to M/n
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] > M / n);
                // Randomly select another element whose value is greater than or equal to M/n and that is not on the main diagonal
                int row2, col2;
                do {
                    row2 = rand() % n;
                    col2 = rand() % n;
                } while (row2 == col2 || magic_square[row2][col2] < M / n);
                // Swap the two selected elements
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                // Calculate the new sum of the main diagonal
                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][i];
                }
                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else
                // If the sum of the main diagonal is equal to M
                return;
        }
    }
    else {
        // If deter is 3, work on the secondary diagonal
        // Calculate the sum of the secondary diagonal
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][n - 1 - i];
        }
        // If the sum of the secondary diagonal is equal to M, return without making any changes
        if (sum == M)
            return;
        // Try swapping elements up to 100 times
        for (int times = 0; times < 100; times++) {
            // If the sum of the secondary diagonal is greater than M
            if (sum > M) {
                // Randomly select an element whose value is greater than or equal to M/n
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] < M / n);
                // Randomly select another element whose value is less than or equal to M/n and that is not on the secondary diagonal
                int row2, col2;
                do {
                    row2 = rand() % n;
                    col2 = rand() % n;
                } while (row2 == n - 1 - col2 || magic_square[row2][col2] > M / n);
                // Swap the two selected elements
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                // Calculate the new sum of the secondary diagonal
                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][n - 1 - i];
                }
                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                // If the sum of the secondary diagonal is less than M
                // Randomly select an element whose value is less than or equal to M/n
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] > M / n);
                // Randomly select another element whose value is greater than or equal to M/n and that is not on the secondary diagonal
                int row2, col2;
                do {
                    row2 = rand() % n;
                    col2 = rand() % n;
                } while (row2 == n - 1 - col2 || magic_square[row2][col2] < M / n);
                // Swap the two selected elements
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                // Calculate the new sum of the secondary diagonal
                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][n - 1 - i];
                }
                // If the new sum is closer to M than the previous sum
                if (abs(tempsum - M) < abs(sum - M)) {
                    // Reset times and update sum
                    times = 0;
                    sum = tempsum;
                } else {
                    // If the new sum is not closer to M than the previous sum
                    // Swap back the two selected elements
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else
                // If the sum of the secondary diagonal is equal to M
                return;
        }
    }
}


// This function swaps randomly two elements in a given magic square to try to make the row, column and diagonal sums equal to M
void swap_LLH5(vector<vector<int>> &magic_square, int n, int M) {
    // Initialize variables to store the indices of the two elements to be swapped
    int i1, j1, i2, j2;
    // Initialize variables to store the row and column sums of the two elements
    int row_sum1 = 0;
    int row_sum2 = 0;
    // Initialize variables to store the row and column sums of the two elements
    // Initialize variables to store whether the two elements are on the main diagonal
    // Initialize variables to store the sum of the main diagonal
    // Initialize variables to store whether the two elements are on the secondary diagonal
    // Initialize variables to store the sum of the secondary diagonal
    bool dia11 = false;
    int dia_1_1 = 0;
    bool dia21 = false;
    int dia_2_1 = 0;
    bool dia12 = false;
    int dia_1_2 = 0;
    bool dia22 = false;
    int dia_2_2 = 0;
    // Initialize variables to store the column sums of the two elements
    int col_sum1 = 0;
    int col_sum2 = 0;
    // Randomly select an element whose row sum, column sum and diagonal sums (if applicable) are not equal to M
    do {
        // Reset row_sum1 and col_sum1
        row_sum1 = 0;
        col_sum1 = 0;
        // Reset dia_1_1 and dia_2_1
        dia_1_1 = 0;
        dia_2_1 = 0;
        // Reset dia11 and dia21
        dia11 = false;
        dia21 = false;
        // Randomly select an element
        i1 = rand() % n;
        j1 = rand() % n;
        // Calculate its row sum and column sum
        for (int i = 0; i < n; i++) {
            row_sum1 += magic_square[i][j1];
            col_sum1 += magic_square[i1][i];
            // If it is on the main diagonal
            if (i1 == j1) {
                // Set dia11 to true and calculate the sum of the main diagonal
                dia11 = true;
                for (int j = 0; j < n; j++) {
                    dia_1_1 += magic_square[j][j];
                }
            }
            // If it is on the secondary diagonal
            if (i1 == n - 1 - j1) {
                // Set dia21 to true and calculate the sum of the secondary diagonal
                dia21 = true;
                for (int j = 0; j < n; j++) {
                    dia_2_1 += magic_square[j][n - 1 - j];
                }
            }
        }
    } while (row_sum1 == M && col_sum1 == M && (!dia11 || dia_1_1 == M) && (!dia21 || dia_2_1 == M));
    // Randomly select another element whose row sum, column sum and diagonal sums (if applicable) are not equal to M
    do {
        // Reset row_sum2 and col_sum2
        row_sum2 = 0;
        col_sum2 = 0;
        // Reset dia12 and dia22
        dia12 = false;
        dia22 = false;
        // Reset dia_1_2 and dia_2_2
        dia_1_2 = 0;
        dia_2_2 = 0;
        // Randomly select another element
        i2 = rand() % n;
        j2 = rand() % n;
        // Calculate its row sum and column sum
        for (int i = 0; i < n; i++) {
            row_sum2 += magic_square[i][j2];
            col_sum2 += magic_square[i2][i];
            // If it is on the main diagonal
            if (i2 == j2) {
                // Set dia12 to true and calculate the sum of the main diagonal
                dia12 = true;
                for (int j = 0; j < n; j++) {
                    dia_1_2 += magic_square[j][j];
                }
            }
            // If it is on the secondary diagonal
            if (i2 == n - 1 - j2) {
                // Set dia22 to true and calculate the sum of the secondary diagonal
                dia22 = true;
                for (int j = 0; j < n; j++) {
                    dia_2_2 += magic_square[j][n - 1 - j];
                }
            }
        }
    } while (row_sum2 == M && col_sum2 == M && (!dia12 || dia_1_2 == M) && (!dia22 || dia_2_2 == M));
    // Swap the two selected elements
    swap(magic_square[i1][j1], magic_square[i2][j2]);
}


// This function swaps elements(ROW MAX->Element MAX, ROW MIN->Element MIN) in a given magic square to try to make the row, column and diagonal sums equal to M
void swap_LLH8(vector<vector<int>> &magic_square, int n, int M) {
    // Generate a random number to determine whether to work on rows, columns or diagonals
    int deter = rand() % 3;
    // If deter is 0, work on rows
    if (deter == 0) {
        // Initialize variables to store the maximum and minimum row sums and their indices
        int max_row_sum = INT_MIN;
        int min_row_sum = INT_MAX;
        int max_row_index = -1;
        int min_row_index = -1;

        // Find the maximum and minimum row sums and their indices
        for (int i = 0; i < n; i++) {
            int row_sum = 0;
            for (int j = 0; j < n; j++) {
                row_sum += magic_square[i][j];
            }
            if (row_sum > max_row_sum) {
                max_row_sum = row_sum;
                max_row_index = i;
            }
            if (row_sum < min_row_sum) {
                min_row_sum = row_sum;
                min_row_index = i;
            }
        }

        // Swap elements in the maximum and minimum rows with a probability of 0.5
        for (int j = 0; j < n; j++) {
            if ((double) rand() / RAND_MAX <= 0.5) {
                swap(magic_square[max_row_index][j], magic_square[min_row_index][j]);
            }
        }
    } else if (deter == 1) {
        // If deter is 1, work on columns
        // Initialize variables to store the maximum and minimum column sums and their indices
        int max_col_sum = INT_MIN;
        int min_col_sum = INT_MAX;
        int max_col_index = -1;
        int min_col_index = -1;

        // Find the maximum and minimum column sums and their indices
        for (int j = 0; j < n; j++) {
            int col_sum = 0;
            for (int i = 0; i < n; i++) {
                col_sum += magic_square[i][j];
            }
            if (col_sum > max_col_sum) {
                max_col_sum = col_sum;
                max_col_index = j;
            }
            if (col_sum < min_col_sum) {
                min_col_sum = col_sum;
                min_col_index = j;
            }
        }

        // Swap elements in the maximum and minimum columns with a probability of 0.5
        for (int i = 0; i < n; i++) {
            if ((double) rand() / RAND_MAX <= 0.5) {
                swap(magic_square[i][max_col_index], magic_square[i][min_col_index]);
            }
        }
    } else {
        // If deter is 2, work on diagonals
        // Initialize variables to store the sums of the main and secondary diagonals
        int diagonal_sum_1 = 0;
        int diagonal_sum_2 = 0;
        // Calculate the sums of the main and secondary diagonals
        for (int i = 0; i < n; i++) {
            diagonal_sum_1 += magic_square[i][i];
            diagonal_sum_2 += magic_square[i][n - 1 - i];
        }

        // If the sum of the main diagonal is greater than or equal to the sum of the secondary diagonal
        if (diagonal_sum_1 >= diagonal_sum_2) {
            for (int i = 0; i < n; i++) {
                // Swap elements on the main and secondary diagonals with a probability of 0.5
                if ((double) rand() / RAND_MAX <= 0.5) {
                    swap(magic_square[i][i], magic_square[n - 1 - i][n - 1 - i]);
                }
            }
        } else {
            // If the sum of the main diagonal is less than the sum of the secondary diagonal
            // Swap elements on the main and secondary diagonals with a probability of 0.5
            for (int i = 0; i < n; i++) {
                if ((double) rand() / RAND_MAX <= 0.5) {
                    swap(magic_square[i][n - 1 - i], magic_square[n - 1 - i][i]);
                }
            }
        }
    }
}


// This function randomly selects one of several swap functions to modify a given magic square
void RP(vector<vector<int>> &magic_square, int n, int M, int obj) {
    // Generate a random number between 0 and 99
    int rand_num = rand() % 100;

    // If the random number is less than 45
    if (rand_num < 45) {
        // Call the swap_elements function
        swap_elements(magic_square, n);
    } else if (rand_num < 80) {
        // If the random number is between 45 and 79
        // Call the swap_LLH0 function
        swap_LLH0(magic_square, n, M);
    } else if (rand_num < 90) {
        // If the random number is between 80 and 89
        // Call the swap_LLH3 function
        swap_LLH3(magic_square, n, M);
    } else if (rand_num < 97) {
        // If the random number is between 90 and 96
        // Call the swap_LLH8 function
        swap_LLH8(magic_square, n, M);
    } else
        // If the random number is between 97 and 99
        // Call the swap_LLH5 function
        swap_LLH5(magic_square, n, M);
}


// This function tries to modify a given magic square by swapping elements to minimize the difference between the row, column and diagonal sums and M
// shake_magic is used to quit local_best but not too far from the magic square
vector<vector<int>> shake_magic(vector<vector<int>> magic_square, int n, int M) {
    // Initialize variables to store the minimum difference and the best shake candidate
    int min_difference = INT_MAX;
    vector<vector<int>> best_shake(n, vector<int>(n));
    vector<vector<int>> shake_candidate(n, vector<int>(n));
    // Calculate the cost of the given magic square using the objective_function
    int magic_square_cost = objective_function(magic_square, n, M);
    // Try swapping elements n*n times
    for (int i = 0; i < n * n; i++) {
        // Copy the given magic square to shake_candidate
        shake_candidate = magic_square;
        // Swap elements in shake_candidate
        swap_elements(shake_candidate, n);
        // Calculate the cost of shake_candidate using the objective_function
        int shake_candidate_cost = objective_function(shake_candidate, n, M);
        // Calculate the difference between the costs of shake_candidate and magic_square
        int difference = shake_candidate_cost - magic_square_cost;
        // If the difference is negative (i.e. shake_candidate has a lower cost than magic_square)
        if(difference<0){
            // Return shake_candidate
            return shake_candidate;
        }
        // Calculate the absolute value of the difference
        difference = abs(difference);
        // If the difference is less than min_difference
        if (difference < min_difference) {
            // Update min_difference and best_shake
            min_difference = difference;
            best_shake = shake_candidate;
        }
    }
    // Return best_shake
    return best_shake;
}


// This function uses a variable neighborhood search algorithm to modify a given magic square to minimize the difference between the row, column and diagonal sums and M
void variable_neighborhood_search(vector<vector<int>> magic_square, int n, vector<vector<int>> &output_square, int M) {
    // Record the start time of the function
    clock_t start_time = clock();
    // Initialize variables to store the current solution and its cost
    vector<vector<int>> current_solution(n, vector<int>(n));
    current_solution = magic_square;
    int current_cost = objective_function(current_solution, n, M);
    // Initialize variables to store the best solution and its cost
    vector<vector<int>> best_solution(n, vector<int>(n));
    best_solution = current_solution;
    int best_cost = current_cost;
    // Initialize a variable to store the previous cost
    int pre_cost=best_cost;
    // Initialize a variable to store the output cost
    int output_cost = best_cost;
    // Initialize a variable to count the number of iterations without improvement
    int no_improve = 0;
    // Initialize variables for simulated annealing
    float SA_TS = 500;
    float SA_TF = 0;
    float SA_BETA = 0.001;
    float temperature = SA_TS;
    bool halfmatrix=false;

    // While the time limit has not been reached and the temperature is greater than SA_TF
    while (((double) (clock() - start_time) / CLOCKS_PER_SEC <= MAX_TIME) && (temperature > SA_TF)) {;
        // Set current_solution to best_solution
        current_solution = best_solution;
        // Call the RP function to modify current_solution
        RP(current_solution, n, M,best_cost);
        // Calculate the cost of current_solution using the objective_function
        current_cost = objective_function(current_solution, n, M);
        // If current_cost is less than best_cost
        if (current_cost < best_cost) {
            // Update best_solution and best_cost
            best_solution = current_solution;
            best_cost = current_cost;
            // If best_cost is 0 (i.e. all row, column and diagonal sums are equal to M)
            if (best_cost == 0) {
                // Update output_cost and output_square and return
                output_cost = best_cost;
                output_square = best_solution;
                return;
            }
        } else {
            // If current_cost is not less than best_cost
            // Update temperature using simulated annealing
            temperature = temperature/(1+SA_BETA*temperature);
            int delta = current_cost - best_cost;
            double probability = exp(-delta / temperature);
            if ((double) rand() / RAND_MAX < probability)
            {
                best_solution = current_solution;
                best_cost = current_cost;
            }
        }
        // If pre_cost is equal to best_cost (i.e. there was no improvement)
        if(pre_cost==best_cost){
            // Increment no_improve
            no_improve++;
        }else{
            // If pre_cost is not equal to best_cost (i.e. there was improvement)
            // Reset no_improve
            no_improve = 0;
        }
        // Update pre_cost
        pre_cost = best_cost;
        // If no_improve is greater than or equal to 10000 and n is less than or equal to 10
        if (no_improve >= 10000 && n<=10) {
            static vector<vector<int>> shake_candidate(n, vector<int>(n));
            shake_candidate = output_square;
            // Call the shake_magic function on shake_candidate and update best_solution and best_cost accordingly
            best_solution = shake_magic(shake_candidate, n, M);
            best_cost = objective_function(best_solution, n, M);
            if (best_cost < output_cost) {
                output_cost = best_cost;
                output_square = best_solution;
            }
            no_improve = 0;
        }else if(no_improve>800000 && n<=20 && n>10)
        {
            static vector<vector<int>> shake_candidate(n, vector<int>(n));
            shake_candidate = output_square;
            // Call the shake_magic function on shake_candidate and update best_solution and best_cost accordingly
            best_solution = shake_magic(shake_candidate, n, M);
            best_cost = objective_function(best_solution, n, M);
            if (best_cost < output_cost) {
                output_cost = best_cost;
                output_square = best_solution;
            }
            no_improve = 0;
        }else if(no_improve>3000000 &&  n>20)
        {
            static vector<vector<int>> shake_candidate(n, vector<int>(n));
            shake_candidate = output_square;
            // Call the shake_magic function on shake_candidate and update best_solution and best_cost accordingly
            best_solution = shake_magic(shake_candidate, n, M);
            best_cost = objective_function(best_solution, n, M);
            if (best_cost < output_cost) {
                output_cost = best_cost;
                output_square = best_solution;
            }
            no_improve = 0;
        }
    }
    output_cost = best_cost;
    output_square = best_solution;
}

int main(int argc, const char* argv[]) {
    // Seed the random number generator
    srand(time(0));
    int opt;
    // Initialize variables to store the input and output file names
    std::string input_file, output_file;
    // Parse command line arguments
    while ((opt = getopt(argc, const_cast<char* const*>(argv), "s:c:t:")) != -1) {
        switch (opt) {
            case 's':
                // Set input_file to the argument of the -s option
                input_file = optarg;
                break;
            case 'c':
                // Set output_file to the argument of the -c option
                output_file = optarg;
                break;
            case 't':
                // Set MAX_TIME to the argument of the -t option
                MAX_TIME = std::stoi(optarg);
                if (MAX_TIME > 2) {
                    MAX_TIME -= 2;
                }
                break;
            default:
                // Print usage information and exit with an error
                std::cerr << "Usage: " << argv[0] << " -s input_file -c output_file -t max_time" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    // Open input file
    std::ifstream file(input_file);
    std::ofstream output_stream(output_file);
    if (!file) {
        // If the input file could not be opened, print an error message and exit with an error
        std::cerr << "Error: could not open input file " << input_file << std::endl;
        exit(EXIT_FAILURE);
    }
    int num_process;
    // Read the number of processes from the input file
    file >> num_process;
    // Write the number of processes to the output file
    output_stream << num_process<< std::endl;
    for (int i = 0; i < num_process; i++) {
        int n, m;
        // Read n and m from the input file
        file >> n >> m;
        vector<vector<int>> magic_square(n, vector<int>(n));
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                // Read the magic square from the input file
                file >> magic_square[j][k];

        vector<vector<int>> output_square(n, vector<int>(n));
        output_square = magic_square;
        // Call the variable_neighborhood_search function on magic_square and store the result in output_square
        variable_neighborhood_search(magic_square, n, output_square, m);
        if (!output_stream) {
            // If the output file could not be opened, print an error message and exit with an error
            std::cerr << "Error: could not open output file " << output_file << std::endl;
            exit(EXIT_FAILURE);
        }
        // Write the cost of output_square to the output file
        output_stream << objective_function(output_square, n, m) << std::endl;
        // Write output to output file
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n - 1; j++) {
                output_stream << output_square[k][j] << " ";
            }
            output_stream << output_square[k][n - 1] << std::endl;
        }
        magic_square.clear();
        output_square.clear();
    }
    // Close output file
    output_stream.close();
}