#include <iostream>
#include <cstdlib>
#include <climits>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>


using namespace std;

int objective_function(vector<vector<int>> &magic_square, int n, int M) {
    vector<int> row_sums(n, 0);
    vector<int> col_sums(n, 0);
    int diag_sum1 = 0;
    int diag_sum2 = 0;
    int n_minus_1 = n - 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            row_sums[i] += magic_square[i][j];
            col_sums[j] += magic_square[i][j];
            if (i == j) {
                diag_sum1 += magic_square[i][j];
            }
            if (i + j == n_minus_1) {
                diag_sum2 += magic_square[i][j];
            }
        }
    }
    int score = 0;
    for (int i = 0; i < n; i++) {
        score += abs(row_sums[i] - M);
        score += abs(col_sums[i] - M);
    }
    score += abs(diag_sum1 - M);
    score += abs(diag_sum2 - M);
    return score;
}

void swap_elements(vector<vector<int>> &magic_square, int n) {
    int i1, j1, i2, j2;
    i1 = rand() % n;
    j1 = rand() % n;
    i2 = rand() % n;
    j2 = rand() % n;
    while (i1 == i2 && j1 == j2) {
        i2 = rand() % n;
        j2 = rand() % n;
    }
    swap(magic_square[i1][j1], magic_square[i2][j2]);
}

void swap_LLH0(vector<vector<int>> &magic_square, int n, int M) {
    int deter = rand() % 4;
    if (deter == 0) {
        int row = 0;
        int sum = 0;
        while (row < n) {
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[row][i];
            }
            if (sum != M)
                break;
            row++;
        }
        if (row == n)
            return;
        if (sum > M) {
            int col1;
            do {
                col1 = rand() % n;
            } while (magic_square[row][col1] < M / n);

            int row2, col2, tempsum;
            do {
                tempsum = 0;
                row2 = rand() % n;
                col2 = rand() % n;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[row2][i];
                }
            } while (row2 == row || tempsum == M || magic_square[row2][col2] > M / n);
            swap(magic_square[row][col1], magic_square[row2][col2]);
        } else {
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
            swap(magic_square[row][col1], magic_square[row2][col2]);
        }
    } else if (deter == 1) {
        int col = 0;
        int sum = 0;
        while (col < n) {
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[i][col];
            }
            if (sum != M)
                break;
            col++;
        }
        if (col == n)
            return;
        if (sum > M) {
            int row1;
            do {
                row1 = rand() % n;
            } while (magic_square[row1][col] < M / n);

            int row2, col2, tempsum;
            do {
                tempsum = 0;
                row2 = rand() % n;
                col2 = rand() % n;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][col2];
                }
            } while (col2 == col || tempsum == M || magic_square[row2][col2] > M / n);
            swap(magic_square[row1][col], magic_square[row2][col2]);
        } else {
            int row1;
            do {
                row1 = rand() % n;
            } while (magic_square[row1][col] > M / n);

            int row2, col2, tempsum;
            do {
                tempsum = 0;
                row2 = rand() % n;
                col2 = rand() % n;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][col2];
                }
            } while (col2 == col || tempsum == M || magic_square[row2][col2] < M / n);
            swap(magic_square[row1][col], magic_square[row2][col2]);
        }
    } else if (deter == 2) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][i];
        }
        if (sum == M)
            return;
        if (sum > M) {
            int row1, col1;
            do {
                row1 = rand() % n;
                col1 = rand() % n;
            } while (magic_square[row1][col1] < M / n);
            int row2, col2;
//            int temp1, temp2;
            do {
//                temp1 = 0;
//                temp2 = 0;
                row2 = rand() % n;
                col2 = rand() % n;
//                for (int i = 0; i < n; i++)
//                {
//                    temp1 += magic_square[row2][i];
//                    temp2 += magic_square[i][col2];
//                }
            } while (row2 == col2 || magic_square[row2][col2] > M / n);
            swap(magic_square[row1][col1], magic_square[row2][col2]);
        } else {
            int row1, col1;
            do {
                row1 = rand() % n;
                col1 = rand() % n;
            } while (magic_square[row1][col1] > M / n);
            int row2, col2;
//            int temp1, temp2;
            do {
//                temp1 = 0;
//                temp2 = 0;
                row2 = rand() % n;
                col2 = rand() % n;
//                for (int i = 0; i < n; i++)
//                {
//                    temp1 += magic_square[row2][i];
//                    temp2 += magic_square[i][col2];
//                }
            } while (row2 == col2 || magic_square[row2][col2] < M / n);
            swap(magic_square[row1][col1], magic_square[row2][col2]);
        }
    } else {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][n - 1 - i];
        }
        if (sum == M)
            return;
        if (sum > M) {
            int row1, col1;
            do {
                row1 = rand() % n;
                col1 = rand() % n;
            } while (magic_square[row1][col1] < M / n);
            int row2, col2;
//            int temp1, temp2;
            do {
//                temp1 = 0;
//                temp2 = 0;
                row2 = rand() % n;
                col2 = rand() % n;
//                for (int i = 0; i < n; i++)
//                {
//                    temp1 += magic_square[row2][i];
//                    temp2 += magic_square[i][col2];
//                }
            } while (row2 == n - 1 - col2 || magic_square[row2][col2] > M / n);
            swap(magic_square[row1][col1], magic_square[row2][col2]);
        } else {
            int row1, col1;
            do {
                row1 = rand() % n;
                col1 = rand() % n;
            } while (magic_square[row1][col1] > M / n);
            int row2, col2;
//            int temp1, temp2;
            do {
//                temp1 = 0;
//                temp2 = 0;
                row2 = rand() % n;
                col2 = rand() % n;
//                for (int i = 0; i < n; i++)
//                {
//                    temp1 += magic_square[row2][i];
//                    temp2 += magic_square[i][col2];
//                }
            } while (row2 == n - 1 - col2 || magic_square[row2][col2] < M / n);
            swap(magic_square[row1][col1], magic_square[row2][col2]);
        }
    }
}

void swap_LLH3(vector<vector<int>> &magic_square, int n, int M) {
    //cout<<"HERE LLH3"<<endl;
    int deter = rand() % 4;
    //cout<<"deter: "<< deter<<endl;
    if (deter == 0) {
        int row = 0;
        int sum = 0;
        while (row < n) {
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[row][i];
            }
            if (sum != M)
                break;
            row++;
        }
        if (row == n)
            return;
        for (int times = 0; times < 100; times++) {
            //cout<<times<<endl;
            if (sum > M) {
                int col1;
                do {
                    col1 = rand() % n;
                } while (magic_square[row][col1] < M / n);

                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[row2][i];
                    }
                } while (row2 == row || tempsum == M || magic_square[row2][col2] > M / n);
                swap(magic_square[row][col1], magic_square[row2][col2]);

                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[row][i];
                }
                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row][col1], magic_square[row2][col2]);
                }
            } else if (sum < M) {
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
                swap(magic_square[row][col1], magic_square[row2][col2]);

                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[row][i];
                }
                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row][col1], magic_square[row2][col2]);
                }
            } else
                return;
        }
    } else if (deter == 1) {
        int col = 0;
        int sum = 0;
        while (col < n) {
            sum = 0;
            for (int i = 0; i < n; i++) {
                sum += magic_square[i][col];
            }
            if (sum != M)
                break;
            col++;
        }
        if (col == n)
            return;
        for (int times = 0; times < 100; times++) {

            if (sum > M) {
                int row1;
                do {
                    row1 = rand() % n;
                } while (magic_square[row1][col] < M / n);

                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[i][col2];
                    }
                } while (col2 == col || tempsum == M || magic_square[row2][col2] > M / n);

                swap(magic_square[row1][col], magic_square[row2][col2]);

                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][col];
                }

                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row1][col], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                int row1;
                do {
                    row1 = rand() % n;
                } while (magic_square[row1][col] > M / n);

                int row2, col2, tempsum;
                do {
                    tempsum = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
                    for (int i = 0; i < n; i++) {
                        tempsum += magic_square[i][col2];
                    }
                } while (col2 == col || tempsum == M || magic_square[row2][col2] < M / n);
                swap(magic_square[row1][col], magic_square[row2][col2]);

                tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][col];
                }

                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row1][col], magic_square[row2][col2]);
                }
            } else
                return;
        }
    } else if (deter == 2) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][i];
        }
        if (sum == M)
            return;
        for (int times = 0; times < 100; times++) {
            if (sum > M) {
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] < M / n);
                int row2, col2;
//                int  temp1, temp2;
                do {
//                    temp1 = 0;
//                    temp2 = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
//                    for (int i = 0; i < n; i++)
//                    {
//                        temp1 += magic_square[row2][i];
//                        temp2 += magic_square[i][col2];
//                    }
                } while (row2 == col2 || magic_square[row2][col2] > M / n);
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][i];
                }
                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] > M / n);
                int row2, col2;
//                int temp1, temp2;
                do {
//                    temp1 = 0;
//                    temp2 = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
//                    for (int i = 0; i < n; i++)
//                    {
//                        temp1 += magic_square[row2][i];
//                        temp2 += magic_square[i][col2];
//                    }
                } while (row2 == col2 || magic_square[row2][col2] < M / n);
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][i];
                }
                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else
                return;
        }
    } else {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += magic_square[i][n - 1 - i];
        }
        if (sum == M)
            return;
        for (int times = 0; times < 100; times++) {
            if (sum > M) {
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] < M / n);
                int row2, col2;
                //int temp1, temp2
                do {
//                    temp1 = 0;
//                    temp2 = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
//                    for (int i = 0; i < n; i++)
//                    {
//                        temp1 += magic_square[row2][i];
//                        temp2 += magic_square[i][col2];
//                    }
                } while (row2 == n - 1 - col2 || magic_square[row2][col2] > M / n);
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][n - 1 - i];
                }
                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else if (sum < M) {
                int row1, col1;
                do {
                    row1 = rand() % n;
                    col1 = rand() % n;
                } while (magic_square[row1][col1] > M / n);
                int row2, col2;
//                int temp1, temp2;
                do {
//                    temp1 = 0;
//                    temp2 = 0;
                    row2 = rand() % n;
                    col2 = rand() % n;
//                    for (int i = 0; i < n; i++)
//                    {
//                        temp1 += magic_square[row2][i];
//                        temp2 += magic_square[i][col2];
//                    }
                } while (row2 == n - 1 - col2 || magic_square[row2][col2] < M / n);
                swap(magic_square[row1][col1], magic_square[row2][col2]);

                int tempsum = 0;
                for (int i = 0; i < n; i++) {
                    tempsum += magic_square[i][n - 1 - i];
                }
                if (abs(tempsum - M) < abs(sum - M)) {
                    times = 0;
                    sum = tempsum;
                } else {
                    swap(magic_square[row1][col1], magic_square[row2][col2]);
                }
            } else
                return;
        }
    }
}

void swap_LLH5(vector<vector<int>> &magic_square, int n, int M) {
    int i1, j1, i2, j2;
    int row_sum1 = 0;
    int row_sum2 = 0;
    bool dia11 = false;
    int dia_1_1 = 0;
    bool dia21 = false;
    int dia_2_1 = 0;
    int col_sum1 = 0;
    int col_sum2 = 0;
    bool dia12 = false;
    int dia_1_2 = 0;
    bool dia22 = false;
    int dia_2_2 = 0;
    do {
        row_sum1 = 0;
        col_sum1 = 0;
        dia_1_1 = 0;
        dia_2_1 = 0;
        dia11 = false;
        dia21 = false;
        i1 = rand() % n;
        j1 = rand() % n;
        for (int i = 0; i < n; i++) {
            row_sum1 += magic_square[i][j1];
            col_sum1 += magic_square[i1][i];
            if (i1 == j1) {
                dia11 = true;
                for (int j = 0; j < n; j++) {
                    dia_1_1 += magic_square[j][j];
                }
            }
            if (i1 == n - 1 - j1) {
                dia21 = true;
                for (int j = 0; j < n; j++) {
                    dia_2_1 += magic_square[j][n - 1 - j];
                }
            }
        }
    } while (row_sum1 == M && col_sum1 == M && (!dia11 || dia_1_1 == M) && (!dia21 || dia_2_1 == M));
    do {
        row_sum2 = 0;
        col_sum2 = 0;
        dia12 = false;
        dia22 = false;
        dia_1_2 = 0;
        dia_2_2 = 0;
        i2 = rand() % n;
        j2 = rand() % n;
        for (int i = 0; i < n; i++) {
            row_sum2 += magic_square[i][j2];
            col_sum2 += magic_square[i2][i];
            if (i2 == j2) {
                dia12 = true;
                for (int j = 0; j < n; j++) {
                    dia_1_2 += magic_square[j][j];
                }
            }
            if (i2 == n - 1 - j2) {
                dia22 = true;
                for (int j = 0; j < n; j++) {
                    dia_2_2 += magic_square[j][n - 1 - j];
                }
            }
        }
    } while (row_sum2 == M && col_sum2 == M && (!dia12 || dia_1_2 == M) && (!dia22 || dia_2_2 == M));
    swap(magic_square[i1][j1], magic_square[i2][j2]);
}

void swap_LLH8(vector<vector<int>> &magic_square, int n, int M) {
    int deter = rand() % 3;
    if (deter == 0) { // row
        int max_row_sum = INT_MIN;
        int min_row_sum = INT_MAX;
        int max_row_index = -1;
        int min_row_index = -1;

        // find the row with the largest and smallest sum
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

        // swap each entry with a probability of 0.5
        for (int j = 0; j < n; j++) {
            if ((double) rand() / RAND_MAX <= 0.5) {
                swap(magic_square[max_row_index][j], magic_square[min_row_index][j]);
            }
        }
    } else if (deter == 1) { // column
        int max_col_sum = INT_MIN;
        int min_col_sum = INT_MAX;
        int max_col_index = -1;
        int min_col_index = -1;

        // find the column with the largest and smallest sum
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

        // swap each entry with a probability of 0.5
        for (int i = 0; i < n; i++) {
            if ((double) rand() / RAND_MAX <= 0.5) {
                swap(magic_square[i][max_col_index], magic_square[i][min_col_index]);
            }
        }
    } else { // diagonal
        int diagonal_sum_1 = 0;
        int diagonal_sum_2 = 0;
        for (int i = 0; i < n; i++) {
            diagonal_sum_1 += magic_square[i][i];
            diagonal_sum_2 += magic_square[i][n - 1 - i];
        }

        if (diagonal_sum_1 >= diagonal_sum_2) { // main diagonal
            for (int i = 0; i < n; i++) {
                if ((double) rand() / RAND_MAX <= 0.5) {
                    swap(magic_square[i][i], magic_square[n - 1 - i][n - 1 - i]);
                }
            }
        } else { // secondary diagonal
            for (int i = 0; i < n; i++) {
                if ((double) rand() / RAND_MAX <= 0.5) {
                    swap(magic_square[i][n - 1 - i], magic_square[n - 1 - i][i]);
                }
            }
        }
    }
}

void RP(vector<vector<int>> &magic_square, int n, int M, int obj) {
    int rand_num = rand() % 100;

    if(n>20 && obj<=20)
    {
        if (rand_num < 50) {
            swap_elements(magic_square, n);
        } else if (rand_num < 85) {
            swap_LLH0(magic_square, n, M);
        } else if (rand_num < 92) {
            swap_LLH3(magic_square, n, M);
        } else if (rand_num < 97) {
            swap_LLH8(magic_square, n, M);
        } else
            swap_LLH5(magic_square, n, M);
    }else{
        if (rand_num < 50) {
            swap_elements(magic_square, n);
        } else if (rand_num < 72) {
            swap_LLH0(magic_square, n, M);
        } else if (rand_num < 89) {
            swap_LLH3(magic_square, n, M);
        } else if (rand_num < 97) {
            swap_LLH8(magic_square, n, M);
        } else
            swap_LLH5(magic_square, n, M);
    }
}

vector<vector<int>> shake_magic(vector<vector<int>> magic_square, int n, int M) {
    int min_difference = INT_MAX;
    vector<vector<int>> best_shake(n, vector<int>(n));
    vector<vector<int>> shake_candidate(n, vector<int>(n));
    int magic_square_cost = objective_function(magic_square, n, M);
    for (int i = 0; i < n * n; i++) {
        //std::swap(shake_candidate, magic_square);
        shake_candidate = magic_square;
        swap_elements(shake_candidate, n);
        int shake_candidate_cost = objective_function(shake_candidate, n, M);
        int difference = abs(shake_candidate_cost - magic_square_cost);
        if (difference < min_difference) {
            min_difference = difference;
            best_shake = shake_candidate;
        }
    }
    return best_shake;
}

void variable_neighborhood_search(vector<vector<int>> magic_square, int n, vector<vector<int>> &output_square, int M) {
    clock_t start_time = clock();
    vector<vector<int>> current_solution(n, vector<int>(n));
    current_solution = magic_square;
    int current_cost = objective_function(current_solution, n, M);
    vector<vector<int>> best_solution(n, vector<int>(n));
    best_solution = current_solution;
    int best_cost = current_cost;
    int pre_cost=best_cost;
    int output_cost = best_cost;
    int no_improve = 0;
    float SA_TS = 500;
    float SA_TF = 0;
    float SA_BETA = 0.001;
    float temperature = SA_TS;

    while (((double) (clock() - start_time) / CLOCKS_PER_SEC <= 98) && (temperature > SA_TF)) {;
        current_solution = best_solution;
        RP(current_solution, n, M,best_cost);
        current_cost = objective_function(current_solution, n, M);
        if (current_cost < best_cost) {
            best_solution = current_solution;
            best_cost = current_cost;
            if (best_cost == 0) {
                output_cost = best_cost;
                output_square = best_solution;
                return;
            }
        } else {
            temperature = temperature/(1+SA_BETA*temperature);
            int delta = current_cost - best_cost;
            double probability = exp(-delta / temperature);
            if ((double) rand() / RAND_MAX < probability)
            {
                best_solution = current_solution;
                best_cost = current_cost;
            }
        }
        if(pre_cost==best_cost){
            no_improve++;
        }else{
            no_improve = 0;
        }
        pre_cost = best_cost;
        if (no_improve >= 10000 && n<=10) {
            static vector<vector<int>> shake_candidate(n, vector<int>(n));
            shake_candidate = output_square;
            best_solution = shake_magic(shake_candidate, n, M);
            best_cost = objective_function(best_solution, n, M);
            if (best_cost < output_cost) {
                output_cost = best_cost;
                output_square = best_solution;
            }
            no_improve = 0;
        }else if(no_improve>500000 && n<=20 && n>10)
        {
            static vector<vector<int>> shake_candidate(n, vector<int>(n));
            shake_candidate = output_square;
            best_solution = shake_magic(shake_candidate, n, M);
            best_cost = objective_function(best_solution, n, M);
            if (best_cost < output_cost) {
                output_cost = best_cost;
                output_square = best_solution;
            }
            no_improve = 0;
        }else if(no_improve>900000 &&  n>20)
        {
            static vector<vector<int>> shake_candidate(n, vector<int>(n));
            shake_candidate = output_square;
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

int main(int argc, char const *argv[]) {
    // srand(0);

    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file("ms-test.txt");
    int num_process;
    file >> num_process;
    cout << num_process << endl;
    for (int i = 0; i < num_process; i++) {
        auto start_task = std::chrono::high_resolution_clock::now();
        int n, m;
        file >> n >> m;
        cout << n << " " << m << endl;
        vector<vector<int>> magic_square(n, vector<int>(n));
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                file >> magic_square[j][k];

        vector<vector<int>> output_square(n, vector<int>(n));
        output_square = magic_square;

        variable_neighborhood_search(magic_square, n, output_square, m);
        //RP_search(magic_square, n, output_square, m);

        cout << "output_square:" << endl;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                cout << output_square[i][j] << " ";
            }
            cout << output_square[i][n - 1] << endl;
        }

        cout << "Cost: " << objective_function(output_square, n, m) << endl;

        cout << "End" << endl;
        auto finish_task = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_task = finish_task - start_task;
        std::cout << "Task elapsed time: " << elapsed_task.count() << " s\n";
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}
