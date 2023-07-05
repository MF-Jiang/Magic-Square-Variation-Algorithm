import numpy as np
import random
import time




# Define a function to calculate the objective value of a given magic square
def objective_function(x):
    n = len(magic_square)
    M = int(np.sum(x) / n)
    row_sums = np.sum(x, axis=1)
    col_sums = np.sum(x, axis=0)
    diag_sum1 = np.trace(x)
    diag_sum2 = np.trace(np.fliplr(x))
    deviations = np.concatenate([np.abs(M - row_sums), np.abs(M - col_sums)])
    return np.sum(deviations) + np.abs(M - diag_sum1) + np.abs(M - diag_sum2)


# Define a function to perform a random swap between two elements in the magic square
def random_swap(magic_square):
    n = magic_square.shape[0]
    i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0,n - 1)
    while i1 == i2 and j1 == j2:
        i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1)
    magic_square[i1][j1], magic_square[i2][j2] = magic_square[i2][j2], magic_square[i1][j1]


def shaking(magic_square):
    min_difference = float('inf')
    best_shake = np.copy(magic_square)

    for i in range((n+5) ** 2):  # Tweak this value based on the desired number of shaking attempts
        shake_candidate = np.copy(magic_square)
        random_swap(shake_candidate)
        difference = abs(objective_function(shake_candidate) - objective_function(magic_square))
        if difference < min_difference:
            min_difference = difference
            best_shake = np.copy(shake_candidate)

    return best_shake


# Define a function to perform the VNS algorithm on the magic square
def variable_neighborhood_search(magic_square):
    start_time = time.time()
    current_solution = np.copy(magic_square)
    current_cost = objective_function(current_solution)
    best_solution = np.copy(current_solution)
    best_cost = current_cost
    SA_TS = 500
    SA_TF = 0
    SA_BETA = 0.001
    temperature = SA_TS
    no_improvement_count = 0
    pre_best_obj = best_cost
    while time.time() - start_time <= 98 and temperature > SA_TF:
        current_solution = np.copy(best_solution)
        random_swap(current_solution)
        current_cost = objective_function(current_solution)
        if current_cost < best_cost:
            best_solution = np.copy(current_solution)
            best_cost = current_cost
            if best_cost == 0:
                return best_solution
        else:
            temperature = temperature / (1 + SA_BETA * temperature)
            delta = current_cost - best_cost
            probability = np.exp(-delta / temperature)
            if random.random() < probability:
                best_solution = np.copy(current_solution)
                best_cost = current_cost
        if pre_best_obj == best_cost:
            no_improvement_count += 1
        else:
            no_improvement_count = 0
        pre_best_obj = best_cost
        if (no_improvement_count > 10000 and n <= 10):
            print("shaking",best_cost)
            best_solution = shaking(best_solution)
            best_cost = objective_function(best_solution)
            no_improvement_count = 0
        #print(best_cost)
        elif(no_improvement_count > 400000 and n >= 20):
            print("shaking",best_cost)
            best_solution = shaking(best_solution)
            best_cost = objective_function(best_solution)
            no_improvement_count = 0
    return best_solution




# magic_square = np.arange(1, n*n+1).reshape(n, n)

# (11,4,10,(8000,15000)

x = 1 # 起始值
z = 3  # 公差
n = 30  # 矩阵大小

# 生成等差数列
a = np.arange(x, x + n ** 2 * z, z)

# np.random.shuffle(a)

# 将一维数组转换成二维矩阵
magic_square = np.reshape(a, (n, n))

print(magic_square)


solution = variable_neighborhood_search(magic_square)
print("Magic Square:\n", solution)
print("Objective Value:", objective_function(solution))

