#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_N 35

int random_seed = 0;
int random_array[MAX_N*MAX_N];
int random_array_index = 0;

void random_seed_init() {
    srand(random_seed);
    for (int i = 0; i < MAX_N*MAX_N; i++) {
        random_array[i] = rand();
    }
    random_array_index = 0;
}

int random_int(int a, int b) {
    int r = random_array[random_array_index++];
    if (random_array_index == MAX_N*MAX_N) {
        random_array_index = 0;
    }
    return a + r % (b-a+1);
}

void swap_elements(int magic_square[MAX_N][MAX_N], int n) {
    int i1, j1, i2, j2;
    i1 = random_int(0, n-1);
    j1 = random_int(0, n-1);
    i2 = random_int(0, n-1);
    j2 = random_int(0, n-1);
    while (i1 == i2 && j1 == j2) {
        i2 = random_int(0, n-1);
        j2 = random_int(0, n-1);
    }
    int temp = magic_square[i1][j1];
    magic_square[i1][j1] = magic_square[i2][j2];
    magic_square[i2][j2] = temp;
}

int objective_function(int magic_square[MAX_N][MAX_N], int n) {
    int M = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M += magic_square[i][j];
        }
    }
    M /= n;
    int row_sums[MAX_N] = {0};
    int col_sums[MAX_N] = {0};
    int diag_sum1 = 0;
    int diag_sum2 = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            row_sums[i] += magic_square[i][j];
            col_sums[j] += magic_square[i][j];
            if (i == j) diag_sum1 += magic_square[i][j];
            if (i+j == n-1) diag_sum2 += magic_square[i][j];
        }
    }
    int sum_deviation = 0;
    for (int i = 0; i < n; i++) {
        sum_deviation += abs(M - row_sums[i]);
        sum_deviation += abs(M - col_sums[i]);
    }
    sum_deviation += abs(M - diag_sum1);
    sum_deviation += abs(M - diag_sum2);
    return sum_deviation;
}

int solve(int magic_square[MAX_N][MAX_N], int n, int max_iterations) {
    random_seed_init();
    int best_objective = objective_function(magic_square, n);
    int iteration = 0;
    while (iteration < max_iterations) {
        int temp_magic_square[MAX_N][MAX_N];
        memcpy(temp_magic_square, magic_square, sizeof(temp_magic_square));
        swap_elements(temp_magic_square, n);
        int temp_objective = objective_function(temp_magic_square, n);
        if (temp_objective < best_objective) {
            memcpy(magic_square, temp_magic_square, sizeof(magic_square));
            best_objective = temp_objective;
        }
        iteration++;
    }
    return best_objective;
}

int main() {
//    int n, max_iterations;
//    scanf("%d %d %d", &n, &max_iterations, &random_seed);
//    int magic_square[MAX_N][MAX_N];
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            scanf("%d", &magic_square[i][j]);
//        }
//    }
//    int result = solve(magic_square, n, max_iterations);
//    printf("%d\n", result);
//    return 0;

    int magic_square[3][3] = {
            {8, 1, 6},
            {3, 5, 7},
            {4, 9, 2}
    };
    printf("%d\n", objective_function(magic_square, 3));

    // use objective function to write a instance
    // objective_function(magic_square, 3);
    // use solve to solve the instance
    // use objective function to check the result


}