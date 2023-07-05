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
    magic_square[i1][j1], magic_square[i2][j2] = magic_square[i2][j2], magic_square[i1][j1]
    return magic_square


# swap rows to create neighborhood
def swap_rows(magic_square):
    n = magic_square.shape[0]
    i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
    magic_square[[i1, i2], :] = magic_square[[i2, i1], :]
    return magic_square


# swap cols to create neighborhood
def swap_cols(magic_square):
    n = magic_square.shape[0]
    j1, j2 = random.randint(0, n - 1), random.randint(0, n - 1)
    magic_square[:, [j1, j2]] = magic_square[:, [j2, j1]]
    return magic_square


# crossover two solutions to create offspring
def crossover(solution1, solution2):
    n = solution1.shape[0]
    child = np.zeros((n, n), dtype=int)
    for i in range(n):
        for j in range(n):
            if (i+j)%2 == 0:
                child[i,j] = solution1[i,j]
            else:
                child[i,j] = solution2[i,j]
    return child


# mutate a solution by randomly swapping elements
def mutation(solution):
    n = solution.shape[0]
    i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0,
                                                                                                                  n - 1)
    solution[i1][j1], solution[i2][j2] = solution[i2][j2], solution[i1][j1]
    return solution


# GA for local search
def genetic_algorithm(population, cost_func, max_iter, mutation_rate, M):
    population_size = population.shape[0]
    idx_list = list(range(population_size))
    best_solution = population[0].copy()
    best_cost = cost_func(best_solution, M)
    for i in range(max_iter):
        # selection
        parents_idx = random.sample(idx_list, 2)
        parent1, parent2 = population[parents_idx]
        # crossover
        child = crossover(parent1, parent2)
        # mutation
        if random.random() < mutation_rate:
            child = mutation(child)
        # selection
        child_cost = cost_func(child, M)
        if child_cost < best_cost:
            best_solution = child.copy()
            best_cost = child_cost
        # replace parent
        if child_cost < cost_func(parent1, M):
            population[parents_idx[0]] = child.copy()
        elif child_cost < cost_func(parent2, M):
            population[parents_idx[1]] = child.copy()
    return best_solution


# VNS use local search(GA)
def variable_neighborhood_search(matrix, max_k, ga_params, M):
    magic_square = matrix.copy()
    best_cost = objective_function(magic_square, M)
    k = 1
    while k <= max_k:
        if best_cost == 0:
            return magic_square
        i = 1
        swap = 0
        while i <= matrix.shape[0] ** 2:
            # if swap == 0:
            #     new_magic_square = swap_elements(magic_square.copy())
            # elif swap == 1:
            #     new_magic_square = swap_rows(magic_square.copy())
            # else:
            #     new_magic_square = swap_cols(magic_square.copy())
            new_magic_square = mutation(magic_square.copy())
            new_cost = objective_function(new_magic_square, M)
            if new_cost < best_cost:
                magic_square = new_magic_square
                best_cost = new_cost
                i = 1
            else:
                i += 1
        # GA local search
        population = np.array([mutation(magic_square.copy()) for _ in range(ga_params[0])])
        magic_square = genetic_algorithm(population, objective_function, *ga_params[1:], M)
        if objective_function(magic_square, M) < best_cost:
            best_cost = objective_function(magic_square, M)
        k += 1
    return magic_square

def main():
    # initialize magic square
    n = 30
    magic_square = np.random.permutation(np.arange(1, n ** 2 + 1)).reshape((n, n))
    M = np.sum(magic_square) / magic_square.shape[0]


    # run variable neighborhood search with genetic algorithm local search
    max_k = 1500
    ga_params = (550, 150, 0.3)
    start_time = time.time()
    result = variable_neighborhood_search(magic_square, max_k, ga_params, M)
    end_time = time.time()

    # output result
    print("Magic Square:")
    print(result)
    print("Objective Function Value:", objective_function(result, M))
    print("Execution Time:", end_time - start_time, "s")


if __name__ == "__main__":
    main()