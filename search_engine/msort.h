#include <vector>
#include <iostream>



template <class T, class Compare>
void mergeSort(std::vector<T>& list, Compare comp);

template<class T, class Compare>
std::vector<T> msort(std::vector<T> list, Compare comp, int list_size);

template<class T, class Compare>
std::vector<T> merge(std::vector<T> &left_side, std::vector<T>& right_side, Compare comp);


template <class T, class Compare>
void mergeSort(std::vector<T>& list, Compare comp){
	if (list.empty()){
        return;
    }
    std::vector<T> copy_list = list;
	int t_size = (int) list.size();
	list = msort(list, comp, t_size);

}

template<class T, class Compare>
std::vector<T> msort(std::vector<T> list, Compare comp, int list_size){
	if (list.size() == 1){
		return list;
	}
	else{
		typename std::vector<T>::iterator it = list.begin() + (list_size / 2);
		std::vector<T> right_side(it, list.end());
		std::vector<T> left_side(list.begin(), it);
		left_side = msort(left_side, comp, left_side.size());
		right_side = msort(right_side, comp, right_side.size());
		return merge(left_side,right_side, comp);
	}
}

template<class T, class Compare>
std::vector<T> merge(std::vector<T> &left_side, std::vector<T>& right_side, Compare comp){
	std::vector<T> output; 
    int left = 0;
    int right = 0;
    while(left < (int) left_side.size() && right < (int) right_side.size()){  
        if(comp(right_side[right],left_side[left])){  
            output.push_back(right_side[right++]);  
        }  
        else{  
            output.push_back(left_side[left++]);   
        }  
    } 
    while(right < (int) right_side.size()){  
        output.push_back(right_side[right++]);   
    }   
    while(left < (int) left_side.size()){  
        output.push_back(left_side[left++]);  
    }  
    return output;
}

