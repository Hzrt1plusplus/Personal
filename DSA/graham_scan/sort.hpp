#ifndef SORT_HPP
#define SORT_HPP

#include <cstdlib> 
#include <vector> 
#include <stdexcept> 


template <typename T, typename Less = std::less<T>, typename Greater = std::greater<T>>
class QuickSort {

	static constexpr Less less = Less(); 
	static constexpr Greater greater = Greater(); 

	static int __hoare_partition(std::vector<T>& objs, int low, int high) {
		
		int index = low + (rand() % (high - low + 1));
		T pivot = objs[low]; 

		int i = low - 1;
		int j = high + 1;

		while (true) {
			do {
				i++;
			} while (less(objs[i], pivot)); 
			
			do {
				j--;
			} while (greater(objs[j], pivot));
			if (i >= j) return j;
			swap(objs, i, j); 
		}

	}

	static inline void swap(std::vector<T>& v, int i, int j) noexcept {
		T tmp = v[i]; 
		v[i] = v[j]; 
		v[j] = tmp;
	}

	static void __quickSort_impl(std::vector<T>& objs, int start, int end) {
		
		if (start < end) {
		
			int pi = __hoare_partition(objs, start, end);
			__quickSort_impl(objs, start, pi); 
			__quickSort_impl(objs,pi + 1, end);

		}

	}

public: 

	QuickSort() = delete;

	static void sort(std::vector<T>& elems) {
		srand(time(NULL)); 
		__quickSort_impl(elems, 0, elems.size() - 1); 
	}

	static void partial_sort(std::vector<T>& elems, int start, int end) {
		if (start < 0 || end >= elems.size() || start > end)
			throw std::runtime_error("Invalid bounds!");
		srand(time(NULL));
		__quickSort_impl(elems, start, end); 
	}

};


template <typename T, typename Compare = std::less_equal<T>>
class MergeSort {

	static constexpr Compare compare = Compare(); 

	static void merge_sorted_arrays(std::vector<T>& arr, int l, int m, int r) {

		const int left_size = m - l + 1;
		const int right_size = r - m;

		std::vector<T> left; 
		std::vector<T> right;

		int i, j, k;

		for (i = 0; i < left_size; i++)
			left.push_back(arr[l + i]); 

		for (j = 0; j < right_size; j++) 
			right.push_back(arr[m + j + 1]);
		
		for (i = 0, j = 0, k = l; k <= r; k++) {
			if ((i < left_size) && ((j >= right_size) || compare(left[i], right[j])))
				arr[k] = left[i++]; 
			else 
				arr[k] = right[j++]; 
		}

	}

	static void __merge_sort_impl(std::vector<T>& v, int l, int r) {

		if (l < r) {
			int m = l + (r - l)/2;
			__merge_sort_impl(v, l, m); 
			__merge_sort_impl(v, m+1, r); 
			merge_sorted_arrays(v, l, m , r); 
		}

	}

public: 

	MergeSort() = delete;

	static void sort(std::vector<T>& elems) {
		__merge_sort_impl(elems, 0, elems.size() - 1); 
	}

	static void partial(std::vector<T>& elems, int start, int end) {
		if (start < 0 || end >= elems.size() || start > end)
			throw std::runtime_error("invalid partial sort!");
		__merge_sort_impl(elems, start, end); 
	}

};

#endif 
