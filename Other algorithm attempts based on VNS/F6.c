#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_N 35
#define MAX_ITER 7000

int random_seed = 0;
int random_array[MAX_N * MAX_N];
int random_array_index = 0;

void random_seed_init()
{
    srand(random_seed);
    for (int i = 0; i < MAX_N * MAX_N; i++)
    {
        random_array[i] = rand();
    }
    random_array_index = 0;
}

int random_int(int a, int b)
{
    int r = random_array[random_array_index++];
    if (random_array_index == MAX_N * MAX_N)
    {
        random_array_index = 0;
    }
    return a + r % (b - a + 1);
}

void swap_elements(int **magic_square, int n)
{
    int i1, j1, i2, j2;
    i1 = random_int(0, n - 1);
    j1 = random_int(0, n - 1);
    i2 = random_int(0, n - 1);
    j2 = random_int(0, n - 1);
    while (i1 == i2 && j1 == j2)
    {
        i2 = random_int(0, n - 1);
        j2 = random_int(0, n - 1);
    }
    int temp = magic_square[i1][j1];
    magic_square[i1][j1] = magic_square[i2][j2];
    magic_square[i2][j2] = temp;
}


int objective_function(int **magic_square, int n)
{
    int M = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // printf("%d ", magic_square[i][j]);
            M += magic_square[i][j];
        }
    }
    M /= n;
    int row_sums[MAX_N] = {0};
    int col_sums[MAX_N] = {0};
    int diag_sum1 = 0;
    int diag_sum2 = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            row_sums[i] += magic_square[i][j];
            col_sums[j] += magic_square[i][j];
            if (i == j)
            {
                diag_sum1 += magic_square[i][j];
            }
            if (i + j == n - 1)
            {
                diag_sum2 += magic_square[i][j];
            }
        }
    }
    int score = 0;
    for (int i = 0; i < n; i++)
    {
        score += abs(row_sums[i] - M);
        score += abs(col_sums[i] - M);
    }
    score += abs(diag_sum1 - M);
    score += abs(diag_sum2 - M);
    // printf("Score: %d\n", score);
    return score;
}

void variable_neighborhood_search(int **magic_square, int n, int **output_square)
{
    // printf("VNS\n");
    clock_t start_time = clock();
        int **current_solution = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        current_solution[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            // printf("%d ", magic_square[i][j]);
            current_solution[i][j] = magic_square[i][j];
        }
        // printf("\n");
    }
    int current_cost = objective_function(current_solution, n);
    // printf("Initial cost: %d\n", current_cost);
    int **best_solution = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        best_solution[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            // printf("%d ", current_solution[i][j]);
            best_solution[i][j] = current_solution[i][j];
        }
        // printf("\n");
    }
    int best_cost = current_cost;
    int **temp_solution = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        temp_solution[i] = (int *)malloc(n * sizeof(int));
    }
    int output_cost = best_cost;
    int no_improve = 0;
    while ((double)(clock() - start_time) / CLOCKS_PER_SEC <= 98)
    {
        for (int i = 0; i < MAX_ITER; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    temp_solution[j][k] = best_solution[j][k];     
                }
            }

            swap_elements(temp_solution, n);

            int current_cost = objective_function(temp_solution, n);
            if (current_cost < best_cost)
            {
                for (int j = 0; j < n; j++)
                {
                    for (int k = 0; k < n; k++)
                    {
                        best_solution[j][k] = temp_solution[j][k];
                    }
                }
                best_cost = current_cost;
                if (best_cost == 0)
                {
                    *output_square = best_solution;
                    //free memory
                    for(int i = 0; i < n; i++)
                    {
                        free(current_solution[i]);
                        free(temp_solution[i]);
                    }
                    free(current_solution);
                    free(temp_solution);
                    return;
                }
                if (n < 10 || output_cost <= 10)
                {
                    continue;
                }
                break;
            }
            else
            {
                int delta = current_cost - best_cost;
                double temperature = fmax(0.005, 1.0 - (double)i / 2700);
                double probability = exp(-delta / temperature);
                if ((double)rand() / RAND_MAX < probability)
                {
                    for (int j = 0; j < n; j++)
                    {
                        for (int k = 0; k < n; k++)
                        {
                            best_solution[j][k] = temp_solution[j][k];
                        }
                    }
                    best_cost = current_cost;
                }
            }
        }
        if (best_cost < output_cost)
        {
            *output_square = best_solution;
            output_cost = best_cost;
            no_improve = 0;
        }
        else
        {
            no_improve++;
        }
        if (no_improve >= 100)
        {
            int **shake_magic(int magic_square[MAX_N][MAX_N], int n);
            int **shake_solution = shake_magic(best_solution, n);
            int shake_cost = objective_function(shake_solution, n);
            memcpy(best_solution, shake_solution, sizeof(best_solution));
            best_cost            = shake_cost;

            if (best_cost < output_cost)
            {
                *output_square = best_solution;
                output_cost = best_cost;
                if (best_cost == 0)
                {
                    //free memory
                    for(int i = 0; i < n; i++)
                    {
                        free(current_solution[i]);
                        free(temp_solution[i]);
                        //free(best_solution[i]);
                    }
                    free(current_solution);
                    free(temp_solution);
                    //free(best_solution);

                    return;
                }
            }
            //free memory
            for(int i = 0; i < n; i++)
            {
                free(shake_solution[i]);
            }
            free(shake_solution);
        }
    }
    *output_square = best_solution;
    //free memory
    for(int i = 0; i < n; i++)
    {
        free(current_solution[i]);
        free(temp_solution[i]);
        //free(best_solution[i]);
    }
    free(current_solution);
    free(temp_solution);
    //free(best_solution);

    return;
}

int **shake_magic(int magic_square[MAX_N][MAX_N], int n)
{
    // printf("Shake\n");
    int min_difference = INT_MAX;
    int **best_shake = (int **)malloc(n * sizeof(int *));
    memcpy(best_shake, magic_square, sizeof(magic_square));
    for (int i = 0; i < n * n; i++)
    {
        int shake_candidate[MAX_N][MAX_N];
        memcpy(shake_candidate, magic_square, sizeof(magic_square));
        swap_elements(shake_candidate, n);
        int difference = abs(objective_function(shake_candidate, n) - objective_function(magic_square, n));
        if (difference < min_difference)
        {
            min_difference = difference;
            memcpy(best_shake, shake_candidate, sizeof(shake_candidate));
        }
    }
    int **ret = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        ret[i] = (int *)malloc(n * sizeof(int));
        memcpy(ret[i], best_shake[i], sizeof(best_shake[i]));
    }

    //free memory
    for(int i = 0; i < n; i++)
    {
        free(best_shake[i]);
    }
    free(best_shake);

    return ret;
}

int main(int argc, char const *argv[])
{
    random_seed = time(NULL);
    random_seed_init();


    FILE *fp = fopen("ms-test.txt", "r");
    int num_problems;
    fscanf(fp, "%d", &num_problems);
    printf("Number of problems: %d\n", num_problems);
    for (int i = 0; i < num_problems; i++)
    {
        int n, magic_constant;
        fscanf(fp, "%d %d", &n, &magic_constant);
        int **magic_square = (int **)malloc(n * sizeof(int *));
        for (int j = 0; j < n; j++)
        {
            magic_square[j] = (int *)malloc(n * sizeof(int));
            for (int k = 0; k < n; k++)
            {
                fscanf(fp, "%d", &magic_square[j][k]);
            }
        }
        int **output_square = (int **)malloc(n * sizeof(int *));
        variable_neighborhood_search(magic_square, n, &output_square);
        printf("Problem %d:\n", i + 1);
        printf("Input:\n");
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n - 1; k++)
            {
                printf("%d ", magic_square[j][k]);
            }
            printf("%d\n", magic_square[j][n - 1]);
        }
        printf("Output:\n");
                for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n - 1; k++)
            {
                printf("%d ", output_square[j][k]);
            }
            printf("%d\n", output_square[j][n - 1]);
        }
        printf("Cost: %d\n", objective_function(output_square, n));
        printf("\n");
        printf("%d\n",i);

        //rember to free memory
        for (int j = 0; j < n; j++)
        {
            free(magic_square[j]);
            free(output_square[j]);
        }
        free(magic_square);
        free(output_square);
    }
    fclose(fp);
    printf("End\n");
    return 0;
}