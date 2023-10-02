import numpy as np

def construct_matrix(first_array, second_array):
    """
    Construct matrix from pair of arrays
    :param first_array: first array
    :param second_array: second array
    :return: constructed matrix
    """
    return np.vstack([first_array, second_array]).T # <- your first right code here

x = [1, 2, 3, 4]
y = [5, 6, 7, 8]
result_matrix = construct_matrix(x, y)
print(result_matrix)