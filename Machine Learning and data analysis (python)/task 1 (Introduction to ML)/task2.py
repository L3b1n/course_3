def most_frequent(nums):
    count_dict = {}
    
    for num in nums:
        if num in count_dict:
            count_dict[num] += 1
        else:
            count_dict[num] = 1
    
    most_frequent = max(count_dict, key=count_dict.get)
    
    return most_frequent

nums = [1, 2, 2, 3, 3, 3, 4, 4, 4, 4]
result = most_frequent(nums)
print("Most frequent element:", result)