#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <climits>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include<chrono>

#define MAX_N 35
#define MAX_ITER 7000

using namespace std;

int objective_function(int **magic_square, int n)
{
    int M = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
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
    return score;
}

void swap_elements(int **magic_square, int n)
{
    int i1, j1, i2, j2;
    i1 = rand() % n;
    j1 = rand() % n;
    i2 = rand() % n;
    j2 = rand() % n;
    while (i1 == i2 && j1 == j2)
    {
        i2 = rand() % n;
        j2 = rand() % n;
    }
    int temp = magic_square[i1][j1];
    magic_square[i1][j1] = magic_square[i2][j2];
    magic_square[i2][j2] = temp;
}

int **shake_magic(int **magic_square, int n)
{
    int min_difference = INT_MAX;
    int **best_shake = new int *[n];
    for (int i = 0; i < n; i++)
    {
        best_shake[i] = new int[n];
        memcpy(best_shake[i], magic_square[i], n * sizeof(int));
    }
    int **shake_candidate = new int *[n];
    for (int i = 0; i < n; i++)
    {
        shake_candidate[i] = new int[n];
    }

    for (int i = 0; i < n * n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            memcpy(shake_candidate[j], magic_square[j], n * sizeof(int));
        }
        swap_elements(shake_candidate, n);
        int difference = abs(objective_function(shake_candidate, n) - objective_function(magic_square, n));
        if (difference < min_difference)
        {
            min_difference = difference;
            memcpy(best_shake, shake_candidate, sizeof(shake_candidate));
        }
    }
    for (int i = 0; i < n; i++)
    {
        delete[] shake_candidate[i];
    }
    delete[] shake_candidate;

    int **ret = new int *[n];
    for (int i = 0; i < n; i++)
    {
        ret[i] = new int[n];
        memcpy(ret[i], best_shake[i], sizeof(best_shake[i]));
    }
    return ret;
}

void variable_neighborhood_search(int **magic_square, int n, int **output_square)
{
    clock_t start_time = clock();
    int **current_solution = new int *[n];
    for (int i = 0; i < n; i++)
    {
        current_solution[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            current_solution[i][j] = magic_square[i][j];
        }
    }
    int current_cost = objective_function(current_solution, n);
    int **best_solution = new int *[n];
    for (int i = 0; i < n; i++)
    {
        best_solution[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            best_solution[i][j] = current_solution[i][j];
        }
    }
    int best_cost = current_cost;
    int **temp_solution = new int *[n];
    for (int i = 0; i < n; i++)
    {
        temp_solution[i] = new int[n];
    }

    int output_cost = best_cost;
    int no_improve = 0;
    while ((double)(clock() - start_time) / CLOCKS_PER_SEC <= 98)
    {
        for (int i = 0; i < MAX_ITER; i++)
        {
            for (int j = 0; j < n; j++)
            {
                memcpy(temp_solution[j], best_solution[j], n * sizeof(int));
            }

            swap_elements(temp_solution, n);
            int current_cost = objective_function(temp_solution, n);
            if (current_cost < best_cost)
            {
                for (int j = 0; j < n; j++)
                {
                    memcpy(best_solution[j], temp_solution[j], n * sizeof(int));
                }
                best_cost = current_cost;
                if (best_cost == 0)
                {
                    output_cost = best_cost;
                    for (int j = 0; j < n; j++)
                    {
                        memcpy(output_square[j], best_solution[j], n * sizeof(int));
                    }
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
                        memcpy(best_solution[j], temp_solution[j], n * sizeof(int));
                    }
                    best_cost = current_cost;
                }
            }
        }
        if (best_cost < output_cost)
        {
            output_cost = best_cost;
            for (int j = 0; j < n; j++)
            {
                memcpy(output_square[j], best_solution[j], n * sizeof(int));
            }
            no_improve = 0;
        }
        else
        {
            no_improve++;
        }
        if (no_improve >= 100)
        {
            int **shake_candidate = new int *[n];
            for (int i = 0; i < n; i++)
            {
                shake_candidate[i] = new int[n];
                memcpy(shake_candidate[i], output_square[i], n * sizeof(int));
            }
            int **shaked_solution = shake_magic(shake_candidate, n);
            int shaked_cost = objective_function(shaked_solution, n);
            if (shaked_cost < output_cost)
            {
                output_cost = shaked_cost;
                for (int j = 0; j < n; j++)
                {
                    memcpy(output_square[j], shaked_solution[j], n * sizeof(int));
                }
            }
            for (int i = 0; i < n; i++)
            {
                delete[] shake_candidate[i];
                delete[] shaked_solution[i];
            }
            delete[] shake_candidate;
            delete[] shaked_solution;
            no_improve = 0;
        }
    }
    output_cost = best_cost;
    for (int j = 0; j < n; j++)
    {
        for (int k = 0; k < n; k++)
        {
            output_square[j][k] = best_solution[j][k];
        }
    }
    return;
}

int main(int argc, char const *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file("ms-test.txt");
    int num_process;
    file >> num_process;
    cout << num_process << endl;
    for (int i = 0; i < num_process; i++)
    {
        int n, m;
        file >> n >> m;
        cout << n << " " << m << endl;
        int **magic_square = new int *[n];
        for (int j = 0; j < n; j++)
        {
            magic_square[j] = new int[n];
            for (int k = 0; k < n; k++)
            {
                // magic_square[i][j] = i * n + j + 1;
                file >> magic_square[j][k];
                // cout<<magic_square[j][k]<<" ";
            }
            // cout<<endl;
        }

        int **output_square = new int *[n];
        for (int i = 0; i < n; i++)
        {
            output_square[i] = new int[n];
            memcpy(output_square[i], magic_square[i], n * sizeof(int));
        }
        variable_neighborhood_search(magic_square, n, output_square);
        cout << "output_square:" << endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - 1; j++)
            {
                cout << output_square[i][j] << " ";
            }
            cout << output_square[i][n - 1] << endl;
        }
        cout << "Cost: " << objective_function(output_square, n) << endl;
        cout << "End" << endl;

        for (int i = 0; i < n; i++)
        {
            delete[] magic_square[i];
            delete[] output_square[i];
        }
        delete[] magic_square;
        delete[] output_square;
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}
