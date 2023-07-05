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


# swap rows to create neighborhood
def swap_rows(magic_square):
    n = magic_square.shape[0]
    i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
    while i1 == i2:
        i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
    magic_square[[i1, i2], :] = magic_square[[i2, i1], :]
    return magic_square


# swap cols to create neighborhood
def swap_cols(magic_square):
    n = magic_square.shape[0]
    j1, j2 = random.randint(0, n - 1), random.randint(0, n - 1)
    while j1 == j2:
        j1, j2 = random.randint(0, n - 1), random.randint(0, n - 1)
    magic_square[:, [j1, j2]] = magic_square[:, [j2, j1]]
    return magic_square


# swap diagonals to create neighborhood
def swap_diagonals(magic_square):
    temp = np.diag(magic_square).copy()
    np.fill_diagonal(magic_square, magic_square[::-1].diagonal())
    np.fill_diagonal(magic_square[::-1], temp)
    return magic_square


def reverse_random_row(magic_square):
    n = magic_square.shape[0]
    row = random.randint(0, n - 1)
    magic_square[row] = np.flip(magic_square[row])
    return magic_square


def reverse_random_column(magic_square):
    n = magic_square.shape[0]
    col = random.randint(0, n - 1)
    magic_square[:, col] = np.flip(magic_square[:, col])
    return magic_square


def reverse_primary_diagonal(magic_square):
    magic_square = np.fliplr(magic_square).T
    return magic_square


def reverse_secondary_diagonal(magic_square):
    magic_square = np.flipud(magic_square).T
    return magic_square


# SA for local search
def simulated_annealing(magic_square, cost_func, max_iter, max_temp, cooling_factor, M):
    current_solution = np.copy(magic_square)
    current_cost = cost_func(current_solution, M)
    best_solution = np.copy(current_solution)
    best_cost = current_cost
    temperature = max_temp
    swap = 0
    for i in range(max_iter):
        if swap == 0:
            new_solution = swap_elements(np.copy(current_solution))
        # elif swap == 1:
        #     new_solution = swap_rows(np.copy(current_solution))
        # elif swap == 2:
        #     new_solution = swap_cols(np.copy(current_solution))
        # elif swap == 3:
        #     new_solution = swap_diagonals(np.copy(current_solution))
        # elif swap == 4:
        #     new_solution = reverse_random_row(np.copy(current_solution))
        # elif swap == 5:
        #     new_solution = reverse_random_column(np.copy(current_solution))
        # elif swap == 6:
        #     new_solution = reverse_primary_diagonal(np.copy(current_solution))
        # elif swap == 7:
        #     new_solution = reverse_secondary_diagonal(np.copy(current_solution))

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
        if temperature < 1:  # 温度过低时退出
            break

        swap += 1
        # if swap >= 8:
        #     swap = 0
        if swap >= 1:
            swap = 0

    return best_solution


# VNS use local search(SA)
def variable_neighborhood_search(matrix, sa_params, M):
    # first local search
    magic_square = simulated_annealing(np.copy(matrix), objective_function, *sa_params)
    best_cost = objective_function(magic_square, M)
    start_time = time.time()

    swap = 0
    while time.time() - start_time <= 98:
        print(time.time() - start_time)
        if best_cost == 0:
            return magic_square

        i = 0
        # shake
        while i <= (matrix.shape[0] + 5) ** 2:
            if swap == 0:
                new_magic_square = swap_elements(np.copy(magic_square))
            # elif swap == 1:
            #     new_magic_square = swap_rows(np.copy(magic_square))
            # elif swap == 2:
            #     new_magic_square = swap_cols(np.copy(magic_square))
            # elif swap == 3:
            #     new_magic_square = swap_diagonals(np.copy(magic_square))
            # elif swap == 4:
            #     new_magic_square = reverse_random_row(np.copy(magic_square))
            # elif swap == 5:
            #     new_magic_square = reverse_random_column(np.copy(magic_square))
            # elif swap == 6:
            #     new_magic_square = reverse_primary_diagonal(np.copy(magic_square))
            # elif swap == 7:
            #     new_magic_square = reverse_secondary_diagonal(np.copy(magic_square))

            new_cost = objective_function(new_magic_square, M)

            if new_cost < best_cost:
                magic_square = np.copy(new_magic_square)
                best_cost = new_cost
                i = 0
                if best_cost == 0:
                    return magic_square
            else:
                i += 1
            swap += 1
            # if swap >= 8:
            #     swap = 0
            if swap >= 1:
                swap = 0

        new_magic_square = simulated_annealing(magic_square, objective_function, *sa_params)
        new_cost = objective_function(new_magic_square, M)

        if new_cost < best_cost:
            magic_square = np.copy(new_magic_square)
            best_cost = new_cost

        if objective_function(magic_square, M) < best_cost:
            best_cost = objective_function(magic_square, M)

    return magic_square


def main():
    start = time.perf_counter()

    n = 30
    x0 = np.random.permutation(np.arange(1, n ** 2 + 1)).reshape((n, n))
    M = np.sum(x0) / x0.shape[0]
    x_optimal = variable_neighborhood_search(x0, (5000, 100000, 0.9, M), M)
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
