import random
import numpy as np
import time

random.seed(0)
np.random.seed(0)


# calculate obj
def objective_function(x):
    n = len(x)
    M = int(np.sum(x) / n)
    row_sums = np.sum(x, axis=1)
    col_sums = np.sum(x, axis=0)
    diag_sum1 = np.trace(x)
    diag_sum2 = np.trace(np.fliplr(x))
    deviations = np.concatenate([np.abs(M - row_sums), np.abs(M - col_sums)])
    return np.sum(deviations) + np.abs(M - diag_sum1) + np.abs(M - diag_sum2)


# swap elements to create neighborhood
def swap_elements(magic_square):
    n = magic_square.shape[0]
    i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0,
                                                                                                                  n - 1)
    while i1 == i2 and j1 == j2:
        i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0,
                                                                                                                      n - 1)
    magic_square[i1][j1], magic_square[i2][j2] = magic_square[i2][j2], magic_square[i1][j1]


def variable_neighborhood_search(magic_square):
    start_time = time.time()
    n = len(magic_square)
    current_solution = np.copy(magic_square)
    current_cost = objective_function(current_solution)
    best_solution = np.copy(current_solution)
    best_cost = current_cost
    output_square = np.copy(magic_square)
    output_cost = best_cost
    no_improve = 0
    # if n <= 20:
    #     max_iter = 7000
    # else:
    #     max_iter = 6500
    while time.time() - start_time <= 98:
        for i in range(7000): # max_iter
            current_solution = np.copy(best_solution)
            swap_elements(current_solution)
            current_cost = objective_function(current_solution)
            if current_cost < best_cost:
                best_solution = np.copy(current_solution)
                best_cost = current_cost
                if best_cost == 0:
                    return best_solution
                if n < 10 or output_cost <= 10:
                    continue
                break
            else:
                delta = current_cost - best_cost
                temperature = max(0.005, 1.0 - float(i) / 2700)
                probability = np.exp(-delta / temperature)
                if random.random() < probability:
                    best_solution = np.copy(current_solution)
                    best_cost = current_cost
        if best_cost < output_cost:
            output_square = np.copy(best_solution)
            output_cost = best_cost
            no_improve = 0
        else:
            no_improve += 1
        if no_improve >= 100:
            best_solution = shake(output_square)
            best_cost = objective_function(best_solution)
            no_improve = 0
    return output_square


def shake(magic_square):
    min_difference = float('inf')
    best_shake = np.copy(magic_square)
    n = len(magic_square)
    for i in range(n ** 2):
        shake_candidate = np.copy(magic_square)
        swap_elements(shake_candidate)
        difference = abs(objective_function(shake_candidate) - objective_function(magic_square))
        if difference < min_difference:
            min_difference = difference
            best_shake = np.copy(shake_candidate)

    return best_shake


def main():
    start = time.perf_counter()
    with open('ms-test.txt', 'r') as f:
        num_problems = int(f.readline())
        for i in range(num_problems):
            start_time = time.time()
            n, M = map(int, f.readline().split())
            v = list(map(int, f.readline().split()))
            v_array = np.array(v)
            x0 = v_array.reshape(n, n)

            best_result = variable_neighborhood_search(np.copy(x0))
            print("Initial solution:")
            print(x0)
            print("Objective function value:", objective_function(x0))
            print("Optimal solution:")
            print(best_result)
            print("Objective function value:", objective_function(best_result))
            print(time.time() - start_time)
    end = time.perf_counter()
    print("运行耗时", end - start)


if __name__ == "__main__":
    main()
