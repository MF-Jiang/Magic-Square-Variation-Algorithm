import random
import numpy as np
import time


# calculate obj
def objective_function(x, M):
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
    return magic_square


# SA for local search
def simulated_annealing(magic_square, cost_func, max_iter, max_temp, cooling_factor, M):
    current_solution = np.copy(magic_square)
    current_cost = cost_func(current_solution, M)
    best_solution = np.copy(current_solution)
    best_cost = current_cost
    temperature = max_temp
    for i in range(max_iter):
        new_solution = swap_elements(np.copy(current_solution))

        new_cost = cost_func(new_solution, M)
        delta_cost = new_cost - current_cost

        if delta_cost < 0 or np.exp(-delta_cost / temperature) > random.uniform(0, 1):
            current_solution = np.copy(new_solution)
            current_cost = new_cost

        if current_cost < best_cost:
            best_solution = np.copy(current_solution)
            best_cost = current_cost
            if best_cost == 0:
                return best_solution

        temperature *= cooling_factor
        if temperature < 0.1:  # 温度过低时保持0.1
            temperature = 0.1

    return best_solution


# VNS use local search(SA)
def variable_neighborhood_search(matrix, sa_params, M):
    # first local search
    magic_square = simulated_annealing(np.copy(matrix), objective_function, *sa_params)
    best_cost = objective_function(magic_square, M)
    start_time = time.time()

    while time.time() - start_time <= 98:
        #print(time.time() - start_time)
        if best_cost == 0:
            return magic_square

        i = 0
        # shake
        while i <= (matrix.shape[0] + 5) ** 2:
            new_magic_square = swap_elements(np.copy(magic_square))

            new_cost = objective_function(new_magic_square, M)

            if new_cost < best_cost:
                magic_square = np.copy(new_magic_square)
                best_cost = new_cost
                i = 0
                if best_cost == 0:
                    return magic_square
            else:
                i += 1

        new_magic_square = simulated_annealing(magic_square, objective_function, *sa_params)
        new_cost = objective_function(new_magic_square, M)

        if new_cost < best_cost:
            magic_square = np.copy(new_magic_square)
            print("SA change")
            best_cost = new_cost

    return magic_square


def main():
    start = time.perf_counter()

    n = 30
    x0 = np.random.permutation(np.arange(1, n ** 2 + 1)).reshape((n, n))
    M = np.sum(x0) / x0.shape[0]
    x_optimal = variable_neighborhood_search(x0, (1000, 10000, 0.9, M), M)
    print(M)
    print("Initial solution:")
    print(x0)
    print("Objective function value:", objective_function(x0, M))
    print("Optimal solution:")
    print(x_optimal)
    print("Objective function value:", objective_function(x_optimal, M))

    end = time.perf_counter()
    print("运行耗时", end - start)

    # print(swap_diagonals(x_optimal))


if __name__ == "__main__":
    main()
