
past_calls = {}
def difference_counter(sequence1, sequence2, differences=0):
    """
    Returns whether or not the two values are unique by a factor of 2 differences
    substitutions, insertions, deletions included

    >>> bool = difference_counter(AAAAA,BBBBB)
    >>> return bool
    False
    >>> bool = difference_counter(AABAA,AAAAA)
    >>> return bool
    True
    >>> bool = diference_counter(AABBA,AAAAA)
    >>> return bool
    True
    >>> bool = difference_counter(ACADA,AAAAA)
    >>> return_bool
    True
    >>> bool = difference_counter(AAAAAA,ATATT)
    >>> return bool
    False
    """
    #returns true if values are identical (by factor of 2 differences)
    #returns false if values are not the same
    combined_sequences = sequence1 + sequence2
    if combined_sequences in past_calls:
        return past_calls[combined_sequences]
    if differences == 3:
        return False;
    if sequence1 == sequence2:
        return True;
    elif len(sequence1) == 0 or len(seqence2) == 0 and differences <=2:
        return True;
    elif sequence1[0] == sequence2[0]:
        return difference_counter(sequence1[1:],sequence2[1:],differences)
    else:
        differences +=1
        add_base = sequence2[0] + sequence1  # Fill in these lines
        remove_base = sequence1[1:]
        substitute_base = sequence2[0] + sequence1[1:]
        past_calls[combined_sequences] = difference_counter(add_base, sequence2, differences) or difference_counter(remove_base,sequence2,differences) or difference_counter(substitute_base,sequence2,differences) 
        return past_calls[combined_sequences]



def unique_sequences(sequence_list):
    exact_duplicate_dict = {i:sequence_list.count(i) for i in sequence_list}
    identical_values = []
    for i in sequence_list:
        past_bool = false
        bool = false
        for a in sequence_list:
            if a in sequence_list
                continue
            bool = or(difference_counter(sequence_list[i],sequence_list[a]),past_bool)
            past_bool = bool
        if bool:
            amount_duplicates = exact_duplicate_dict[i]
            while amount_duplicates > 0
                identical_values.append[i]
                amount_duplicates -= 1
    return len(sequence_list) - len(identical_values)







#            past_calls_test.append(sequence_list[i] + sequence_list[a])
#            past_calls_test.append(sequence_list[a] + sequence_list[i])
#            combined = sequence_list[i] + sequence_list[a]
#            if difference_counter(sequence_list[a],sequence_list[b]) == False and combined not in past_calls_test
#                count+=1
#    return count
