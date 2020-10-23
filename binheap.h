#ifndef BINHEAP_H
#define BINHEAP_H

#include <string>
#include <sstream>
#include "dynarr.h"

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::stringstream;

template <typename T>
class BinHeap {
	DynArr<T>* arr;
	T rTmp; // przechowuje dane usuwanego korzenia

public:
	BinHeap() {
		arr = new DynArr<T>();
		arr->setLength(0);
	}

	BinHeap(T* arr, int size, int(*compare)(const T&, const T&), bool topDown) {
		this->arr = new DynArr<T>(arr, size);

		if (topDown) { // top-down
			for (int i = 0; i < size; i++) {
				heapifyUp(i, compare);
			}

		}
		else { // bottom-up
			for (int i = (size - 1) / 2; i >= 0; i--) {
				heapifyDown(i, compare);
			}
		}
	}

	~BinHeap() {
		delete arr;
		//cout << "[ Usunieto kopiec... ]" << endl;
	}

	T* getRoot() {
		return arr->getByIndex(0);
	}

	void add(const T&, int(*compare)(const T&, const T&));
	T* getAndDeleteRoot(int(*compare)(const T&, const T&));
	void clear();
	string heapToStr(int numOfEl = -1);
	void showHeap(int numOfEl = -1);
	void sort(int(*compare)(const T&, const T&));

private:
	void heapifyUp(int, int(*compare)(const T&, const T&));
	void heapifyDown(int, int(*compare)(const T&, const T&));
};

template <typename T>
void BinHeap<T>::add(const T& data, int(*compare)(const T&, const T&)) {
	arr->add(data);
	//cout << "[ Dodano element... ]" << endl;
	//cout << "[ " << data << " ]" << endl;

	if (arr->getLength() > 1) {		
		//cout << "[ Przekopcowanie w gore... ]" << endl;
		heapifyUp(arr->getLength() - 1, compare);
	}
}

template <typename T>
T* BinHeap<T>::getAndDeleteRoot(int(*compare)(const T&, const T&)) {
	if (arr->getLength() == 0) {
		return nullptr;
	}

	rTmp = *this->getRoot();
	T* last = arr->getByIndex(arr->getLength() - 1);

	arr->setByIndex(0, *last);
	arr->deleteLast();
	//cout << "[ Usunieto korzen... ]" << endl;

	//cout << "[ Przekopcowanie w dol... ]" << endl;
	if (arr->getLength() > 0) {
		heapifyDown(0, compare);
	}
	
	last = nullptr;

	return &rTmp;
}

template <typename T>
void BinHeap<T>::clear() {
	arr->clearArray();
	//cout << "[ Wyczyszczono kopiec... ]" << endl;
}

template <typename T>
string BinHeap<T>::heapToStr(int numOfEl) {
	string s = "Rozmiar kopca: " + to_string(arr->getLength()) + "\nZawartosc kopca (tablica dynamiczna):\n\n" + arr->arrayToStr(numOfEl);
	return s;
}

template <typename T>
void BinHeap<T>::showHeap(int numOfEl) {
	arr->showArray(numOfEl);
}

// DZIALA, THE BEST
template <typename T>
void BinHeap<T>::sort(int(*compare)(const T&, const T&)) {
	int length = arr->getLength();
	for (int i = 0; i < length; i++) {
		T root = *this->getAndDeleteRoot(compare);
		//cout << "root: " << root << endl;
		arr->add(root);
		//arr->setLength(arr->getLength() - 1);
		arr->deleteLast();
	}
	arr->setLength(length);
}

template <typename T>
void BinHeap<T>::heapifyUp(int index, int(*compare)(const T&, const T&)) {
	if (arr->getByIndex(index) == nullptr) {
		return;
	}

	int indexP, indexMax;

	indexP = (index - 1) / 2;
	indexMax = indexP;

	if (compare(*arr->getByIndex(index), *arr->getByIndex(indexP)) > 0) {
		indexMax = index;
	}

	if (indexMax != indexP) {
		// zamieniamy miejscami arr[indexP] <-> arr[indexMax]
		T tmp = *arr->getByIndex(indexP);
		arr->setByIndex(indexP, *arr->getByIndex(indexMax));
		arr->setByIndex(indexMax, tmp);

		heapifyUp(indexP, compare);
	}
}

template <typename T>
void BinHeap<T>::heapifyDown(int index, int(*compare)(const T&, const T&)) {
	if (arr->getByIndex(index) == nullptr) {
		return;
	}

	int indexL, indexR, indexMax;

	indexMax = index;
	indexL = 2 * index + 1; // indeks lewego potomka
	indexR = 2 * index + 2; // indeks prawego potomka

	if (indexL < arr->getLength() && compare(*arr->getByIndex(indexL), *arr->getByIndex(index)) > 0) { // jesli lewy potomek istnieje i jest "wiekszy" niz rodzic
		indexMax = indexL;
	}

	if (indexR < arr->getLength() && compare(*arr->getByIndex(indexR), *arr->getByIndex(indexMax)) > 0) {  // jesli prawy potomek istnieje i jest "wiekszy" niz element max
		indexMax = indexR;
	}

	if (indexMax != index) {
		// zamieniamy miejscami arr[index] <-> arr[indexMax]
		T tmp = *arr->getByIndex(index);
		arr->setByIndex(index, *arr->getByIndex(indexMax));
		arr->setByIndex(indexMax, tmp);

		heapifyDown(indexMax, compare);
	}
}

#endif