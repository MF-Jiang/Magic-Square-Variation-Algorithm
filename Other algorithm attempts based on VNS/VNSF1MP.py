import random
import numpy as np
import time

random.seed(0)


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
    magic_square = np.copy(matrix)
    best_cost = objective_function(magic_square, M)
    start_time = time.time()
    output_square = np.copy(matrix)
    output_cost = best_cost
    t = 0
    while time.time() - start_time <= 98:
        # print(time.time() - start_time)
        if best_cost == 0:
            return magic_square
        t += 1
        #print("times: ", t, " ", best_cost)

        # shake操作：在当前解的邻域中生成一个新解
        new_magic_square = swap_elements(np.copy(magic_square))
        # if best_cost<20:
        #     magic_square = swap_elements(np.copy(magic_square))

        # 使用模拟退火算法进行局部搜索
        new_magic_square = simulated_annealing(new_magic_square, objective_function, *sa_params)
        new_cost = objective_function(new_magic_square, M)

        if new_cost < best_cost:
            magic_square = np.copy(new_magic_square)
            best_cost = new_cost

        if best_cost < output_cost:
            output_square = np.copy(magic_square)
            output_cost = best_cost

    return output_square


def parallel_vns(matrix):
    M = np.sum(matrix) / matrix.shape[0]
    x = 0
    if matrix.shape[0] < 10:
        x = 1
    elif 10 <= matrix.shape[0] < 20:
        x = 2
    else:
        x = 3
    result = variable_neighborhood_search(np.copy(matrix), (8000 * x, 2700 * x, 0.9, M), M)

    return result


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
            x = 0
            if x0.shape[0] < 10:
                x = 1
            elif 10 <= x0.shape[0] < 20:
                x = 2
            else:
                x = 3
            best_result = variable_neighborhood_search(np.copy(x0), (8000*x, 2700*x, 0.9, M), M)
            print("Initial solution:")
            print(x0)
            print("Objective function value:", objective_function(x0, M))
            print("Optimal solution:")
            print(best_result)
            print("Objective function value:", objective_function(best_result, M))
            print(time.time() - start_time)
    end = time.perf_counter()
    print("运行耗时", end - start)


if __name__ == "__main__":
    main()
