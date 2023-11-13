def levenshtein_distance(str1, str2):
    distance = [[0] * (len(str2) + 1) for _ in range(len(str1) + 1)]

    for i in range(len(str1) + 1):
        distance[i][0] = i
    for j in range(len(str2) + 1):
        distance[0][j] = j

    for i in range(1, len(str1) + 1):
        for j in range(1, len(str2) + 1):
            substitution_cost = 0 if str1[i - 1] == str2[j - 1] else 1
            deletion = distance[i - 1][j] + 1
            insertion = distance[i][j - 1] + 1
            substitution = distance[i - 1][j - 1] + substitution_cost

            distance[i][j] = min(deletion, insertion, substitution)

    res_number = distance[len(str1)][len(str2)]

    return res_number


val = levenshtein_distance('abcdefg', 'accefg')
print(val)