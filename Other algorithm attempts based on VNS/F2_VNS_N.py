import random
import time

import numpy as np

random.seed(0)
np.random.seed(0)

def generate_initial_solution(n):
    x0 = np.random.permutation(np.arange(1, n ** 2 + 1)).reshape((n, n))
    return x0

def objective_function(matrix):
    M = np.sum(matrix) / matrix.shape[0]
    row_sums = np.sum(matrix, axis=1)
    col_sums = np.sum(matrix, axis=0)
    diag_sum1 = np.trace(matrix)
    diag_sum2 = np.trace(np.fliplr(matrix))
    deviations = np.concatenate([np.abs(M - row_sums), np.abs(M - col_sums)])
    return np.sum(deviations) + np.abs(M - diag_sum1) + np.abs(M - diag_sum2)

def neighborhood_structure(matrix):
    matrixs = []
    matrixs.append(np.copy(matrix))
    # print(len(matrixs))
    structures = [swap_elements, swap_columns, swap_rows, shift_rows, shift_columns, swap_rows_max_min,
                  swap_cols_max_min, swap_rowmax_colmin, swap_rowmin_colmax, swap_row_min_col_max, swap_row_max_col_min,
                  swap_row_max_row_min, swap_col_min_col_max, swap_diagonal, swap_diagonals1, swap_diagonals2,
                  swap_diagonals3, swap_diagonals4, flip_random_row,
                  flip_random_col, shuffle_random_row, shuffle_random_col, swap_neighbors, shift_matrix_up,
                  shift_matrix_down, shift_matrix_left, shift_matrix_right]
    for structure in structures:
        stru_matrix = np.copy(structure(np.copy(matrix)))
        matrixs.append(np.copy(stru_matrix))
    # print(len(matrixs))
    for i in range(3,matrix.shape[0]+1):
        matrixs.append(np.copy(swap_n_elements(np.copy(matrix), i)))
    # print(len(matrixs))
    for j in range(matrix.shape[0]):
        matrixs.append(np.copy(swap_n_rows(np.copy(matrix),j)))
        matrixs.append(np.copy(swap_n_cols(np.copy(matrix),j)))
        matrixs.append(np.copy(rotate_submatrix1(np.copy(matrix),j)))
        matrixs.append(np.copy(rotate_submatrix2(np.copy(matrix),j)))
        matrixs.append(np.copy(rotate_submatrix3(np.copy(matrix), j)))

    for k in range((matrix.shape[0])**2):
        matrixs.append(np.copy(swap_elements(np.copy(matrix))))
        matrixs.append(np.copy(swap_diagonal_elements(np.copy(matrix), 2)))
    if matrix.shape[0]%2==1:
        for m in range(2,2*matrix.shape[0]):
            matrixs.append(np.copy(swap_diagonal_elements(np.copy(matrix), m)))
    else:
        for m in range(2,2*matrix.shape[0]+1):
            matrixs.append(np.copy(swap_diagonal_elements(np.copy(matrix), m)))
    # print(len(matrixs))
    best_matrix = np.copy(min(matrixs, key=lambda ma: objective_function(ma)))
    # print(objective_function(best_matrix))
    # print(best_matrix)
    return best_matrix

def vns_algorithm(magic_square, time_limit):
    start_time = time.time()
    iteration = 0
    current_solution = np.copy(magic_square)
    best_solution = np.copy(current_solution)
    out_solution = np.copy(current_solution)
    best_objective_value = objective_function(np.copy(current_solution))
    out_value = objective_function(np.copy(out_solution))

    while time.time() - start_time < time_limit:
        i = 0
        while i < 100:
            i+=1
            candidate_solution = np.copy(neighborhood_structure(np.copy(current_solution)))
            candidate_objective_value = objective_function(np.copy(candidate_solution))
            iteration+=1
            if candidate_objective_value < best_objective_value:
                best_solution = np.copy(candidate_solution)
                best_objective_value = candidate_objective_value
                i = 0
                if best_objective_value < out_value:
                    out_solution = np.copy(best_solution)
                    out_value = best_objective_value
                if out_value == 0:
                    return out_solution

            # current_solution = np.copy(candidate_solution)
            current_solution = np.copy(best_solution)
            #print(best_objective_value)
            # print("--------------------------------------------")
            # print("Best: ", objective_function(best_solution))
            # print("Current: ", objective_function(current_solution))
            # print("--------------------------------------------")
            # print()
        # shake
        # current_solution = np.copy(swap_n_elements(np.copy(out_solution), 2*n))
        current_solution = np.copy(swap_elements(np.copy(out_solution)))
        best_solution = np.copy(current_solution)
        best_objective_value = objective_function(np.copy(best_solution))

        # print("*************************************************")
        # print("Out: ",objective_function(out_solution))
        # print("Best: ",objective_function(best_solution))
        # print("Current: ",objective_function(current_solution))
        # print("*************************************************")
        # print(time.time() - start_time)
        # print()
    # print("Iterations: ",iteration)
    return out_solution

def swap_elements(magic_square):
    n = magic_square.shape[0]
    i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0,
                                                                                                                  n - 1)
    while i1 == i2 and j1 == j2:
        i1, j1, i2, j2 = random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0, n - 1), random.randint(0,
                                                                                                                      n - 1)
    magic_square[i1][j1], magic_square[i2][j2] = magic_square[i2][j2], magic_square[i1][j1]
    return magic_square
def swap_rows(magic_square):
    n = magic_square.shape[0]
    i1, i2 = random.sample(range(n), 2)
    magic_square[[i1, i2], :] = magic_square[[i2, i1], :]
    return magic_square
def swap_columns(magic_square):
    n = magic_square.shape[0]
    j1, j2 = random.sample(range(n), 2)
    magic_square[:, [j1, j2]] = magic_square[:, [j2, j1]]
    return magic_square
def shift_rows(magic_square):
    n = magic_square.shape[0]
    i = random.randint(0, n - 1)
    k = random.randint(1, n - 1)
    magic_square[i, :] = np.roll(magic_square[i, :], k)
    return magic_square
def shift_columns(magic_square):
    n = magic_square.shape[0]
    j = random.randint(0, n - 1)
    k = random.randint(1, n - 1)
    magic_square[:, j] = np.roll(magic_square[:, j], k)
    return magic_square
def swap_rows_max_min(magic_square):
    row_sums = magic_square.sum(axis=1)
    max_sum_row = np.argmax(row_sums)
    min_sum_row = np.argmin(row_sums)
    max_val = np.max(magic_square[max_sum_row, :])
    min_val = np.min(magic_square[min_sum_row, :])
    max_val_idx = np.where(magic_square[max_sum_row, :] == max_val)[0][0]
    min_val_idx = np.where(magic_square[min_sum_row, :] == min_val)[0][0]
    magic_square[max_sum_row, max_val_idx], magic_square[min_sum_row, min_val_idx] = \
        magic_square[min_sum_row, min_val_idx], magic_square[max_sum_row, max_val_idx]
    return magic_square
def swap_cols_max_min(magic_square):
    col_sums = magic_square.sum(axis=0)
    max_sum_col = np.argmax(col_sums)
    min_sum_col = np.argmin(col_sums)
    max_val = np.max(magic_square[:, max_sum_col])
    min_val = np.min(magic_square[:, min_sum_col])
    max_val_idx = np.where(magic_square[:, max_sum_col] == max_val)[0][0]
    min_val_idx = np.where(magic_square[:, min_sum_col] == min_val)[0][0]
    magic_square[max_val_idx, max_sum_col], magic_square[min_val_idx, min_sum_col] = \
        magic_square[min_val_idx, min_sum_col], magic_square[max_val_idx, max_sum_col]
    return magic_square
def swap_rowmax_colmin(magic_square):
    row_sums = magic_square.sum(axis=1)
    col_sums = magic_square.sum(axis=0)
    max_sum_row = np.argmax(row_sums)
    min_sum_col = np.argmin(col_sums)
    max_val = np.max(magic_square[max_sum_row, :])
    min_val = np.min(magic_square[:, min_sum_col])
    max_val_idx = np.where(magic_square[max_sum_row, :] == max_val)[0][0]
    min_val_idx = np.where(magic_square[:, min_sum_col] == min_val)[0][0]
    magic_square[max_sum_row, max_val_idx], magic_square[min_val_idx, min_sum_col] = \
        magic_square[min_val_idx, min_sum_col], magic_square[max_sum_row, max_val_idx]
    return magic_square
def swap_rowmin_colmax(magic_square):
    row_sums = magic_square.sum(axis=1)
    col_sums = magic_square.sum(axis=0)
    min_sum_row = np.argmin(row_sums)
    max_sum_col = np.argmax(col_sums)
    min_val = np.min(magic_square[min_sum_row, :])
    max_val = np.max(magic_square[:, max_sum_col])
    min_val_idx = np.where(magic_square[min_sum_row, :] == min_val)[0][0]
    max_val_idx = np.where(magic_square[:, max_sum_col] == max_val)[0][0]
    magic_square[min_sum_row, min_val_idx], magic_square[max_val_idx, max_sum_col] = \
        magic_square[max_val_idx, max_sum_col], magic_square[min_sum_row, min_val_idx]
    return magic_square
def swap_row_max_col_min(magic_square):
    row_sums = magic_square.sum(axis=1)
    col_sums = magic_square.sum(axis=0)
    max_sum_row = np.argmax(row_sums)
    min_sum_col = np.argmin(col_sums)
    min_val = np.min(magic_square[max_sum_row, :])
    max_val = np.max(magic_square[:, min_sum_col])
    min_val_idx = np.where(magic_square[max_sum_row, :] == min_val)[0][0]
    max_val_idx = np.where(magic_square[:, min_sum_col] == max_val)[0][0]
    magic_square[max_sum_row, min_val_idx], magic_square[max_val_idx, min_sum_col] = \
        magic_square[max_val_idx, min_sum_col], magic_square[max_sum_row, min_val_idx]
    return magic_square
def swap_row_min_col_max(magic_square):
    row_sums = magic_square.sum(axis=1)
    col_sums = magic_square.sum(axis=0)
    min_sum_row = np.argmin(row_sums)
    max_sum_col = np.argmax(col_sums)
    max_val = np.max(magic_square[min_sum_row, :])
    min_val = np.min(magic_square[:, max_sum_col])
    max_val_idx = np.where(magic_square[min_sum_row, :] == max_val)[0][0]
    min_val_idx = np.where(magic_square[:, max_sum_col] == min_val)[0][0]
    magic_square[min_sum_row, max_val_idx], magic_square[min_val_idx, max_sum_col] = \
        magic_square[min_val_idx, max_sum_col], magic_square[min_sum_row, max_val_idx]
    return magic_square
def swap_row_max_row_min(magic_square):
    row_sums = magic_square.sum(axis=1)
    max_sum_row = np.argmax(row_sums)
    min_sum_row = np.argmin(row_sums)
    max_val = np.max(magic_square[max_sum_row, :])
    min_val = np.min(magic_square[min_sum_row, :])
    max_val_idx = np.where(magic_square[max_sum_row, :] == max_val)[0][0]
    min_val_idx = np.where(magic_square[min_sum_row, :] == min_val)[0][0]
    magic_square[max_sum_row, max_val_idx], magic_square[min_sum_row, min_val_idx] = \
        magic_square[min_sum_row, min_val_idx], magic_square[max_sum_row, max_val_idx]
    return magic_square
def swap_col_min_col_max(magic_square):
    col_sums = magic_square.sum(axis=0)
    min_sum_col = np.argmin(col_sums)
    max_sum_col = np.argmax(col_sums)
    max_val = np.max(magic_square[:, min_sum_col])
    min_val = np.min(magic_square[:, max_sum_col])
    max_val_idx = np.where(magic_square[:, min_sum_col] == max_val)[0][0]
    min_val_idx = np.where(magic_square[:, max_sum_col] == min_val)[0][0]
    magic_square[max_val_idx, min_sum_col], magic_square[min_val_idx, max_sum_col] = \
        magic_square[min_val_idx, max_sum_col], magic_square[max_val_idx, min_sum_col]
    return magic_square
def swap_diagonal(magic_square):
    n = magic_square.shape[0]
    diagonal_choice = random.randint(0, 1)
    if diagonal_choice == 0:
        i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
        while i1 == i2:
            i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
        magic_square[i1][i1], magic_square[i2][i2] = magic_square[i2][i2], magic_square[i1][i1]
    else:
        i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
        while i1 == i2:
            i1, i2 = random.randint(0, n - 1), random.randint(0, n - 1)
        magic_square[i1][n - i1 - 1], magic_square[i2][n - i2 - 1] = magic_square[i2][n - i2 - 1], magic_square[i1][
            n - i1 - 1]
    return magic_square
def swap_diagonals1(magic_square):
    diagonal1 = np.diag(magic_square)
    diagonal2 = np.diag(np.fliplr(magic_square))
    temp = np.copy(diagonal1)
    np.fill_diagonal(magic_square, diagonal2)
    np.fill_diagonal(np.fliplr(magic_square), temp)
    return magic_square
def swap_diagonals2(magic_square):
    diagonal1 = np.diag(magic_square)
    diagonal2 = np.diag(np.fliplr(magic_square))
    temp = np.copy(diagonal1)
    diagonal2 = diagonal2[::-1]
    np.fill_diagonal(magic_square, diagonal2)
    np.fill_diagonal(np.fliplr(magic_square), temp)
    return magic_square
def swap_diagonals3(magic_square):
    diagonal1 = np.diag(magic_square)
    diagonal2 = np.diag(np.fliplr(magic_square))
    temp = np.copy(diagonal1[::-1])
    np.fill_diagonal(magic_square, diagonal2)
    np.fill_diagonal(np.fliplr(magic_square), temp)
    return magic_square
def swap_diagonals4(magic_square):
    diagonal1 = np.diag(magic_square)
    diagonal2 = np.diag(np.fliplr(magic_square))
    temp = np.copy(diagonal1[::-1])
    diagonal2 = diagonal2[::-1]
    np.fill_diagonal(magic_square, diagonal2)
    np.fill_diagonal(np.fliplr(magic_square), temp)
    return magic_square
def flip_random_row(matrix):
    row_idx = random.randint(0, matrix.shape[0] - 1)
    matrix[row_idx, :] = matrix[row_idx, ::-1]
    return matrix
def flip_random_col(matrix):
    col_idx = random.randint(0, matrix.shape[1] - 1)
    matrix[:, col_idx] = matrix[::-1, col_idx]
    return matrix
def shuffle_random_row(matrix):
    row_idx = random.randint(0, matrix.shape[0] - 1)
    np.random.shuffle(matrix[row_idx, :])
    return matrix
def shuffle_random_col(matrix):
    col_idx = random.randint(0, matrix.shape[1] - 1)
    np.random.shuffle(matrix[:, col_idx])
    return matrix
def swap_n_elements(magic_square, n):
    shape = magic_square.shape[0]
    indices = set()
    while len(indices) < n:
        indices.add((random.randint(0, shape - 1), random.randint(0, shape - 1)))
    values = [magic_square[i] for i in indices]
    for i in range(n-1):
        magic_square[indices.pop()] = values[i+1]
    magic_square[indices.pop()] = values[0]
    return magic_square
def swap_n_rows(magic_square, n):
    shape = magic_square.shape[0]
    indices = np.random.choice(shape, n, replace=False)
    magic_square[indices] = magic_square[indices[::-1]]
    return magic_square
def swap_n_cols(magic_square, n):
    shape = magic_square.shape[0]
    indices = np.random.choice(shape, n, replace=False)
    magic_square[:, indices] = magic_square[:, indices[::-1]]
    return magic_square
def rotate_submatrix1(matrix, n):
    rows, cols = matrix.shape
    row_start = random.randint(0, rows - n)
    col_start = random.randint(0, cols - n)
    submatrix = matrix[row_start:row_start+n, col_start:col_start+n]
    rotated_submatrix = np.rot90(submatrix)
    matrix[row_start:row_start+n, col_start:col_start+n] = rotated_submatrix
    return matrix
def rotate_submatrix2(matrix, n):
    rows, cols = matrix.shape
    row_start = random.randint(0, rows - n)
    col_start = random.randint(0, cols - n)
    submatrix = matrix[row_start:row_start+n, col_start:col_start+n]
    rotated_submatrix = np.rot90(np.rot90(submatrix))
    matrix[row_start:row_start+n, col_start:col_start+n] = rotated_submatrix
    return matrix
def rotate_submatrix3(matrix, n):
    rows, cols = matrix.shape
    row_start = random.randint(0, rows - n)
    col_start = random.randint(0, cols - n)
    submatrix = matrix[row_start:row_start+n, col_start:col_start+n]
    rotated_submatrix = np.rot90(np.rot90(np.rot90(submatrix)))
    matrix[row_start:row_start+n, col_start:col_start+n] = rotated_submatrix
    return matrix
def swap_neighbors(matrix):
    rows, cols = matrix.shape
    row = random.randint(1, rows - 2)
    col = random.randint(1, cols - 2)
    submatrix = matrix[row-1:row+2, col-1:col+2]
    submatrix[[0, 2]] = submatrix[[2, 0]]
    submatrix[:, [0, 2]] = submatrix[:, [2, 0]]
    return matrix
def shift_matrix_up(matrix):
    matrix = np.roll(matrix, -1, axis=0)
    return matrix
def shift_matrix_down(matrix):
    matrix = np.roll(matrix, 1, axis=0)
    return matrix
def shift_matrix_left(matrix):
    matrix = np.roll(matrix, -1, axis=1)
    return matrix
def shift_matrix_right(matrix):
    matrix = np.roll(matrix, 1, axis=1)
    return matrix
def swap_diagonal_elements(matrix, n):
    diagonal_elements = set(np.diag(matrix))
    diagonal_elements.update(np.diag(np.fliplr(matrix)))
    # selected_elements = set(random.sample(diagonal_elements, n))
    selected_elements = set(random.sample(list(diagonal_elements), n))
    positions = []
    for element in selected_elements:
        positions.extend(list(zip(*np.where(matrix == element))))
    new_positions = [positions[-1]]+positions[:-1]
    for x,y in new_positions:
        matrix[x,y] = selected_elements.pop()
    return matrix

# def find_arithmetic_sequence_parameters(seq):
#     a = min(seq)
#     d = min([seq[i+1]-seq[i] for i in range(len(seq)-1) if seq[i+1]-seq[i] > 0])
#     return a, d
# def change_arithmetic_sequence(sequence,a,b):
#   new_sequence = [1]
#   for i in range(1, len(sequence)):
#     new_sequence.append(sequence[i] - (b*i) + i -(a-1))
#   return new_sequence


if __name__ == "__main__":

    x0 = generate_initial_solution(5)
    print(x0)
    print((x0))

    # start = time.perf_counter()
    # with open('ms-test.txt', 'r') as f:
    #     num_problems = int(f.readline())
    #     for i in range(num_problems):
    #         start_time = time.time()
    #         n, M = map(int, f.readline().split())
    #         v = list(map(int, f.readline().split()))
    #         v_array = np.array(v)
    #         # v_array = np.sort(v_array)
    #         # a,b = find_arithmetic_sequence_parameters(v_array)
    #         # print(a,b)
    #         # v_array = change_arithmetic_sequence(v_array, a, b)
    #         # v_array = np.array(v_array)
    #         x0 = v_array.reshape(n, n)
    #
    #         best_result = vns_algorithm(np.copy(x0), 95)
    #         # if a==1:
    #         #     best_result = (a - 1) + (b * best_result)
    #         # else:
    #         #     best_result = (a - 1) + (b * best_result) - 1
    #
    #         print("Initial solution:")
    #         print(x0)
    #         print("Objective function value:", objective_function(x0))
    #         print("Optimal solution:")
    #         print(best_result)
    #         print("Objective function value:", objective_function(best_result))
    #         print(time.time() - start_time)
    # end = time.perf_counter()
    # print("运行耗时", end - start)
