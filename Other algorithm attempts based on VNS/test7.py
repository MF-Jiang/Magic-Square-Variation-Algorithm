import numpy as np
import random
import time
import sys

# random.seed(15)
# np.random.seed(15)


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




def swap_LLH0(magic_square, M):
    deter = random.randint(0, 3)
    if deter == 0:
        row = 0
        total = 0
        while row < n:
            total = 0
            for i in range(n):
                total += magic_square[row][i]
            if total != M:
                break
            row += 1
        if row == n:
            return
        if total > M:
            col1 = random.choice([i for i in range(n) if magic_square[row][i] >= M // n])
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                tempsum = sum(magic_square[row2])
                if row2 != row and tempsum != M and magic_square[row2][col2] <= M // n:
                    break
            magic_square[row][col1], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row][col1]
        else:
            col1 = random.choice([i for i in range(n) if magic_square[row][i] <= M // n])
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                tempsum = sum(magic_square[row2])
                if row2 != row and tempsum != M and magic_square[row2][col2] >= M // n:
                    break
            magic_square[row][col1], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row][col1]
    elif deter == 1:
        col = 0
        total = 0
        while col < n:
            total = 0
            for i in range(n):
                total += magic_square[i][col]
            if total != M:
                break
            col += 1
        if col == n:
            return
        if total > M:
            row1 = random.choice([i for i in range(n) if magic_square[i][col] >= M // n])
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                tempsum = sum(magic_square[i][col2] for i in range(n))
                if col2 != col and tempsum != M and magic_square[row2][col2] <= M // n:
                    break
            magic_square[row1][col], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row1][col]
        else:
            row1 = random.choice([i for i in range(n) if magic_square[i][col] <= M // n])
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                tempsum = sum(magic_square[i][col2] for i in range(n))
                if col2 != col and tempsum != M and magic_square[row2][col2] >= M // n:
                    break
            magic_square[row1][col], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row1][col]
    elif deter == 2:
        sum_diag = sum(magic_square[i][i] for i in range(n))
        if sum_diag == M:
            return
        if sum_diag > M:
            while True:
                row1 = random.randint(0, n - 1)
                col1 = random.randint(0, n - 1)
                if magic_square[row1][col1] >= M // n:
                    break
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                if row2 != col2 and magic_square[row2][col2] <= M // n:
                    break
            magic_square[row1][col1], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row1][col1]
        else:
            while True:
                row1 = random.randint(0, n - 1)
                col1 = random.randint(0, n - 1)
                if magic_square[row1][col1] <= M // n:
                    break
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                if row2 != col2 and magic_square[row2][col2] >= M // n:
                    break
            magic_square[row1][col1], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row1][col1]
    else:
        sum_diag_inv = sum(magic_square[i][n - 1 - i] for i in range(n))
        if sum_diag_inv == M:
            return
        if sum_diag_inv > M:
            while True:
                row1 = random.randint(0, n - 1)
                col1 = random.randint(0, n - 1)
                if magic_square[row1][col1] >= M // n:
                    break
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                if row2 != n - 1 - col2 and magic_square[row2][col2] <= M // n:
                    break
            magic_square[row1][col1], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row1][col1]
        else:
            while True:
                row1 = random.randint(0, n - 1)
                col1 = random.randint(0, n - 1)
                if magic_square[row1][col1] <= M // n:
                    break
            while True:
                row2 = random.randint(0, n - 1)
                col2 = random.randint(0, n - 1)
                if row2 != n - 1 - col2 and magic_square[row2][col2] >= M // n:
                    break
            magic_square[row1][col1], magic_square[row2][col2] = magic_square[row2][col2], magic_square[row1][col1]



def swap_LLH3(magic_square, M):
    deter = random.randint(0, 3)

    if deter == 0:
        row = 0
        while row < n:
            row_sum = sum(magic_square[row])
            if row_sum != M:
                break
            row += 1

        if row == n:
            return

        for times in range(100):
            row_sum = sum(magic_square[row])

            if row_sum > M:
                col1 = None
                while col1 is None or magic_square[row][col1] < M // n:
                    col1 = random.randint(0, n - 1)

                row2, col2, tempsum = None, None, None
                while (row2 is None or row2 == row or tempsum == M or
                        magic_square[row2][col2] > M // n):
                    tempsum = 0
                    row2 = random.randint(0, n - 1)
                    col2 = random.randint(0, n - 1)
                    for i in range(n):
                        tempsum += magic_square[row2][i]

                magic_square[row][col1], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row][col1]

                tempsum = sum(magic_square[row])
                if abs(tempsum - M) < abs(row_sum - M):
                    times = 0
                    row_sum = tempsum
                else:
                    magic_square[row][col1], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row][col1]

            elif row_sum < M:
                col1 = None
                while col1 is None or magic_square[row][col1] > M // n:
                    col1 = random.randint(0, n - 1)

                row2, col2, tempsum = None, None, None
                while (row2 is None or row2 == row or tempsum == M or
                        magic_square[row2][col2] < M // n):
                    tempsum = 0
                    row2 = random.randint(0, n - 1)
                    col2 = random.randint(0, n - 1)
                    for i in range(n):
                        tempsum += magic_square[row2][i]

                magic_square[row][col1], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row][col1]

                tempsum = sum(magic_square[row])
                if abs(tempsum - M) < abs(row_sum - M):
                    times = 0
                    row_sum = tempsum
                else:
                    magic_square[row][col1], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row][col1]

            else:
                return

    elif deter == 1:
        col = 0
        while col < n:
            col_sum = sum(magic_square[i][col] for i in range(n))
            if col_sum != M:
                break
            col += 1

        if col == n:
            return

        for times in range(100):
            col_sum = sum(magic_square[i][col] for i in range(n))

            if col_sum > M:
                row1 = None
                while row1 is None or magic_square[row1][col] < M // n:
                    row1 = random.randint(0, n - 1)

                row2, col2, tempsum = None, None, None
                while (row2 is None or col2 == col or tempsum == M or
                       magic_square[row2][col2] > M // n):
                    tempsum = 0
                    row2 = random.randint(0, n - 1)
                    col2 = random.randint(0, n - 1)
                    for i in range(n):
                        tempsum += magic_square[i][col2]

                magic_square[row1][col], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row1][col]

                tempsum = sum(magic_square[i][col] for i in range(n))
                if abs(tempsum - M) < abs(col_sum - M):
                    times = 0
                    col_sum = tempsum
                else:
                    magic_square[row1][col], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row1][col]

            elif col_sum < M:
                row1 = None
                while row1 is None or magic_square[row1][col] > M // n:
                    row1 = random.randint(0, n - 1)

                row2, col2, tempsum = None, None, None
                while (row2 is None or col2 == col or tempsum == M or
                       magic_square[row2][col2] < M // n):
                    tempsum = 0
                    row2 = random.randint(0, n - 1)
                    col2 = random.randint(0, n - 1)
                    for i in range(n):
                        tempsum += magic_square[i][col2]

                magic_square[row1][col], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row1][col]

                tempsum = sum(magic_square[i][col] for i in range(n))
                if abs(tempsum - M) < abs(col_sum - M):
                    times = 0
                    col_sum = tempsum
                else:
                    magic_square[row1][col], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row1][col]

            else:
                return

    elif deter == 2:
        diag_sum = sum(magic_square[i][i] for i in range(n))
        if diag_sum == M:
            return

        for times in range(100):
            if diag_sum > M:
                row1, col1 = None, None
                while row1 is None or col1 is None or magic_square[row1][col1] < M // n:
                    row1, col1 = random.randint(0, n - 1), random.randint(0, n - 1)

                row2, col2 = None, None
                while row2 is None or col2 is None or row2 == col2 or magic_square[row2][col2] > M // n:
                    row2, col2 = random.randint(0, n - 1), random.randint(0, n - 1)

                magic_square[row1][col1], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row1][col1]

                tempsum = sum(magic_square[i][i] for i in range(n))
                if abs(tempsum - M) < abs(diag_sum - M):
                    times = 0
                    diag_sum = tempsum
                else:
                    magic_square[row1][col1], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row1][col1]

            elif diag_sum < M:
                row1, col1 = None, None
                while row1 is None or col1 is None or magic_square[row1][col1] > M // n:
                    row1, col1 = random.randint(0, n - 1), random.randint(0, n - 1)

                row2, col2 = None, None
                while row2 is None or col2 is None or row2 == col2 or magic_square[row2][col2] < M // n:
                    row2, col2 = random.randint(0, n - 1), random.randint(0, n - 1)

                magic_square[row1][col1], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row1][col1]

                tempsum = sum(magic_square[i][i] for i in range(n))
                if abs(tempsum - M) < abs(diag_sum - M):
                    times = 0
                    diag_sum = tempsum
                else:
                    magic_square[row1][col1], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row1][col1]

            else:
                return

    elif deter == 3:
        diag_sum = sum(magic_square[i][n - i - 1] for i in range(n))
        if diag_sum == M:
            return

        for times in range(100):
            if diag_sum > M:
                row1, col1 = None, None
                while row1 is None or col1 is None or magic_square[row1][col1] < M // n:
                    row1, col1 = random.randint(0, n - 1), random.randint(0, n - 1)

                row2, col2 = None, None
                while row2 is None or col2 is None or col2 == n - row2 - 1 or magic_square[row2][col2] > M // n:
                    row2, col2 = random.randint(0, n - 1), random.randint(0, n - 1)

                magic_square[row1][col1], magic_square[row2][col2] = \
                    magic_square[row2][col2], magic_square[row1][col1]

                tempsum = sum(magic_square[i][n - i - 1] for i in range(n))
                if abs(tempsum - M) < abs(diag_sum - M):
                    times = 0
                    diag_sum = tempsum
                else:
                    magic_square[row1][col1], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row1][col1]

            elif diag_sum < M:
                row1, col1 = None, None
                while row1 is None or col1 is None or magic_square[row1][col1] > M // n:
                    row1, col1 = random.randint(0, n - 1), random.randint(0, n - 1)

                    row2, col2 = None, None
                    while row2 is None or col2 is None or col2 == n - row2 - 1 or magic_square[row2][col2] < M // n:
                        row2, col2 = random.randint(0, n - 1), random.randint(0, n - 1)

                    magic_square[row1][col1], magic_square[row2][col2] = \
                        magic_square[row2][col2], magic_square[row1][col1]

                    tempsum = sum(magic_square[i][n - i - 1] for i in range(n))
                    if abs(tempsum - M) < abs(diag_sum - M):
                        times = 0
                        diag_sum = tempsum
                    else:
                        magic_square[row1][col1], magic_square[row2][col2] = \
                            magic_square[row2][col2], magic_square[row1][col1]

                else:
                    return


def swap_LLH5(magic_square, M):
    while True:
        i1, j1 = random.randint(0, n - 1), random.randint(0, n - 1)
        row_sum1 = sum(magic_square[i][j1] for i in range(n))
        col_sum1 = sum(magic_square[i1][i] for i in range(n))
        dia_1_1 = sum(magic_square[i][i] for i in range(n)) if i1 == j1 else None
        dia_2_1 = sum(magic_square[i][n - 1 - i] for i in range(n)) if i1 == n - 1 - j1 else None

        if row_sum1 != M or col_sum1 != M or (dia_1_1 is not None and dia_1_1 != M) or (dia_2_1 is not None and dia_2_1 != M):
            break

    while True:
        i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1)
        row_sum2 = sum(magic_square[i][j2] for i in range(n))
        col_sum2 = sum(magic_square[i2][i] for i in range(n))
        dia_1_2 = sum(magic_square[i][i] for i in range(n)) if i2 == j2 else None
        dia_2_2 = sum(magic_square[i][n - 1 - i] for i in range(n)) if i2 == n - 1 - j2 else None

        if row_sum2 != M or col_sum2 != M or (dia_1_2 is not None and dia_1_2 != M) or (dia_2_2 is not None and dia_2_2 != M):
            break

    magic_square[i1][j1], magic_square[i2][j2] = magic_square[i2][j2], magic_square[i1][j1]





def swap_LLH8(magic_square):
    deter = random.randint(0, 2)

    if deter == 0:  # row
        max_row_sum = -sys.maxsize
        min_row_sum = sys.maxsize
        max_row_index = -1
        min_row_index = -1

        # find the row with the largest and smallest sum
        for i in range(n):
            row_sum = sum(magic_square[i])
            if row_sum > max_row_sum:
                max_row_sum = row_sum
                max_row_index = i
            if row_sum < min_row_sum:
                min_row_sum = row_sum
                min_row_index = i

        # swap each entry with a probability of 0.5
        for j in range(n):
            if random.random() <= 0.5:
                magic_square[max_row_index][j], magic_square[min_row_index][j] = magic_square[min_row_index][j], magic_square[max_row_index][j]

    elif deter == 1:  # column
        max_col_sum = -sys.maxsize
        min_col_sum = sys.maxsize
        max_col_index = -1
        min_col_index = -1

        # find the column with the largest and smallest sum
        for j in range(n):
            col_sum = sum(magic_square[i][j] for i in range(n))
            if col_sum > max_col_sum:
                max_col_sum = col_sum
                max_col_index = j
            if col_sum < min_col_sum:
                min_col_sum = col_sum
                min_col_index = j

        # swap each entry with a probability of 0.5
        for i in range(n):
            if random.random() <= 0.5:
                magic_square[i][max_col_index], magic_square[i][min_col_index] = magic_square[i][min_col_index], magic_square[i][max_col_index]

    else:  # diagonal
        diagonal_sum_1 = sum(magic_square[i][i] for i in range(n))
        diagonal_sum_2 = sum(magic_square[i][n - 1 - i] for i in range(n))

        if diagonal_sum_1 >= diagonal_sum_2:  # main diagonal
            for i in range(n):
                if random.random() <= 0.5:
                    magic_square[i][i], magic_square[n - 1 - i][n - 1 - i] = magic_square[n - 1 - i][n - 1 - i], magic_square[i][i]

        else:  # secondary diagonal
            for i in range(n):
                if random.random() <= 0.5:
                    magic_square[i][n - 1 - i], magic_square[n - 1 - i][i] = magic_square[n - 1 - i][i], magic_square[i][n - 1 - i]



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
    output_square = np.copy(magic_square)
    output_cost = best_cost
    M = int(np.sum(magic_square) / n)
    no_improve = 0
    no_improvement_count = 0
    pre_best_obj = best_cost
    while time.time() - start_time <= 98:
        for i in range(7000):  # max_iter 7000
            current_solution = np.copy(best_solution)
            rand_num = random.randint(0, 99)
            if rand_num < 50:
                random_swap(current_solution)
            elif rand_num < 72:
                swap_LLH0(current_solution, M)
            elif rand_num < 89:
                swap_LLH3(current_solution, M)
            elif rand_num < 97:
                swap_LLH8(current_solution)
            else:
                swap_LLH5(current_solution, M)
            current_cost = objective_function(current_solution)
            if current_cost < best_cost:
                best_solution = np.copy(current_solution)
                best_cost = current_cost
                if best_cost == 0:
                    return best_solution
                # if n < 10 or output_cost <= 10:
                #     continue
                break
            else:
                delta = current_cost - best_cost
                temperature = max(0.01, 1.0 - float(i) / 2700)  # 2700
                probability = np.exp(-delta / temperature)
                if random.random() < probability:
                    best_solution = np.copy(current_solution)
                    best_cost = current_cost
        print(best_cost)
        if pre_best_obj == best_cost:
            no_improvement_count += 1
        else:
            no_improvement_count = 0
        pre_best_obj = best_cost
        if best_cost < output_cost:
            output_square = np.copy(best_solution)
            output_cost = best_cost
            no_improve = 0
        else:
            no_improve += 1
        if no_improve >= 50 and no_improvement_count >= 50:     #50
            print("shaking")
            best_solution = shaking(best_solution)
            best_cost = objective_function(best_solution)
            no_improve = 0
            no_improvement_count = 0
    return output_square




# magic_square = np.arange(1, n*n+1).reshape(n, n)

# (11,4,10,(8000,15000)

x = 11 # 起始值
z = 4  # 公差
n = 10  # 矩阵大小

# 生成等差数列
a = np.arange(x, x + n ** 2 * z, z)

# np.random.shuffle(a)

# 将一维数组转换成二维矩阵
magic_square = np.reshape(a, (n, n))

print(magic_square)



solution = variable_neighborhood_search(magic_square)
print("Magic Square:\n", solution)
print("Objective Value:", objective_function(solution))

