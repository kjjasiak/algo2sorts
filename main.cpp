#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>
#include "binheap.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ostream;
using std::vector;

/* struktura do eksperymentu
------------------------------*/
struct data {
	double num;
	char c;
};

/* generowanie struktury z losowymi danymi
--------------------------------------------- */
data generateData(int shl, double denom) {
	data d;
	d.num = (((rand() & 0x7FFF) << shl) + (rand() & 0x7FFF)) / denom;
	d.c = rand() % ('z' - 'a' + 1) + 'a'; // a - z
	return d;
}

/* funkcje pomocnicze
----------------------- */
ostream& operator<<(ostream& out, const data& n) {
	out << "double: " << std::fixed << std::setprecision(10) << n.num << ", char: " << n.c;
	return out;
}

string arrayToStr(int* arr, int size, int limit = 0) {
	stringstream s;
	int n;

	if ((limit != 0) && (limit < size))
		n = limit;
	else
		n = size;

	for (int i = 0; i < n; i++) {
		s << arr[i] << " ";
	}
	s << endl;

	return s.str();
}

template <typename T>
string arrayToStr(T* arr, int size, int limit = 0) {
	stringstream s;
	int n;

	if ((limit != 0) && (limit < size))
		n = limit;
	else
		n = size;

	for (int i = 0; i < n; i++) {
		s << arr[i] << endl;
	}
	s << endl;

	return s.str();
}

template <typename T>
bool compareArrays(T* arr1, T* arr2, int size, int(*compare)(const T&, const T&)) {
	for (int i = 0; i < size; i++) {
		if (compare(arr1[i], arr2[i]) != 0) {
			return false;
		}
	}
	return true;
}

int dataCompare(const int& i, const int& j) {
	return (i - j);
}

int dataCompare(const data& ob1, const data& ob2)
{
	double d = ob1.num - ob2.num;
	if (d != 0)
		return d < 0 ? -1 : 1;
	return ob1.c - ob2.c;
}

double getSortKey(const data& ob) {
	return ob.num;
}

/* funkcje sortujace
----------------------- */
void insertionSort(vector<int>& v, int n) {
	int tmp;
	int j;

	for (int i = 1; i < n; i++) {
		tmp = v[i];
		j = i - 1;

		while ((j >= 0) && (v[j] > tmp)) {
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = tmp;
	}
}

template <typename T>
void insertionSort(vector<T>& v, int n, int(*compare)(const T&, const T&)) {
	T tmp;
	int j;

	for (int i = 1; i < n; i++) {
		tmp = v[i];
		j = i - 1;

		while ((j >= 0) && (compare(v[j], tmp) > 0)) {
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = tmp;
	}
}

void countingSort(int* arr, int n, int m) {
	int* c = new int[m];
	int* tmp = new int[n];

	// zainicjowanie tablicy zliczajacej zerami
	for (int i = 0; i < m; i++) {
		c[i] = 0;
	}

	// zliczanie
	for (int i = 0; i < n; i++) {
		c[arr[i]] = c[arr[i]] + 1;
	}

	// sumowanie zliczen
	for (int i = 0; i < m-1; i++) {
		c[i + 1] = c[i] + c[i + 1];
	}

	// uporzadkowanie elementow
	for (int i = 0; i < n; i++) {
		tmp[c[arr[i]] - 1] = arr[i];
		c[arr[i]] = c[arr[i]] - 1;
	}

	// przepisanie do pierwotnej tablicy
	for (int i = 0; i < n; i++) {
		arr[i] = tmp[i];
	}

	delete[] tmp;
	delete[] c;
}

void bucketSort(int* arr, int n, int m) {
	// utworzenie pustych kubelkow
	vector<vector<int>> bucketArr(n);

	// wpisanie wartosci do kubelkow
	for (int i = 0; i < n; i++) {
		bucketArr[int(arr[i] * double(n) / m)].push_back(arr[i]);
	}

	// sortowanie zawartosci poszczegolnych kubelkow
	for (int i = 0; i < n; i++) {
		//sort(bucketArr[i].begin(), bucketArr[i].end(), dataCompareBool);
		insertionSort(bucketArr[i], bucketArr[i].size());
	}

	// wyswietlenie zawartosci kubelkow
	/*for (int i = 0; i < n; i++) {
		cout << "Kubelek #" << i << ": ";
		for (int j : bucketArr[i]) {
			cout << j << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	// sklejanie posortowanej tablicy
	int j = 0;
	for (int k = 0; k < n; k++) {
		for (int i : bucketArr[k]) {
			arr[j] = i;
			j++;
		}
	}
}

template <typename T>
void bucketSort(T* arr, int n, double m, double(*sortKey)(const T&), int(*compare)(const T&, const T&)) {
	vector<vector<T>> bucketArr(n);

	// wpisanie wartosci do kubelkow
	for (int i = 0; i < n; i++) {
		//bucketArr[int(sortKey(arr[i]) * n)].push_back(arr[i]);
		bucketArr[int(sortKey(arr[i]) * double(n) / m)].push_back(arr[i]);
	}

	// wyswietlenie zawartosci kubelkow
	/*for (int i = 0; i < n; i++) {
		cout << "Kubelek #" << i << ":" << endl;
		for (T j : bucketArr[i]) {
			cout << j << endl;
		}
		cout << endl;
	}
	cout << endl;*/

	// sortowanie zawartosci poszczegolnych kubelkow
	for (int i = 0; i < n; i++) {
		insertionSort(bucketArr[i], bucketArr[i].size(), compare);
	}

	// sklejanie posortowanej tablicy
	int j = 0;
	for (int k = 0; k < n; k++) {
		for (T i : bucketArr[k]) {
			arr[j] = i;
			j++;
		}
	}
}



/* testy
----------------------- */
void testCSort(int* arr, int n, int m) {
	cout << "Sortowanie przez zliczanie" << endl;
	cout << "------------------------------" << endl;

	cout << "przed sortowaniem: ";
	cout << arrayToStr(arr, n, 3);

	countingSort(arr, n, m);

	cout << "po sortowaniu: ";
	cout << arrayToStr(arr, n, 3);
}

void testBSortInt(int* arr, int n, int m) {
	cout << "Sortowanie kubelkowe (dla int)" << endl;
	cout << "------------------------" << endl;

	cout << "przed sortowaniem: ";
	cout << arrayToStr(arr, n, 20);

	bucketSort(arr, n, m);

	cout << "po sortowaniu: ";
	cout << arrayToStr(arr, n, 20);
}

void testBSortData(data* arr, int n, double m, double(*getSortKey)(const data&), int(*compare)(const data&, const data&)) {
	cout << "Sortowanie kubelkowe (dla T)" << endl;
	cout << "------------------------" << endl;

	cout << "przed sortowaniem:" << endl;
	cout << arrayToStr(arr, n, 7);

	bucketSort<data>(arr, n, m, getSortKey, dataCompare);

	cout << "po sortowaniu:" << endl;
	cout << arrayToStr(arr, n, 7);
}

void testSorts() {
	int arr[] = { 2, 5, 9, 1, 3, 7, 4, 2, 3, 0 };
	int size = sizeof(arr) / sizeof(arr[0]);
	int* max = std::max_element(arr, arr + size);
	int m = *max + 1;

	// stworzenie tablicy struktur
	data* arr2 = new data[size];
	arr2[0] = { 0.78,'a' };
	arr2[1] = { 0.17,'f' };
	arr2[2] = { 0.39,'d' };
	arr2[3] = { 0.26,'a' };
	arr2[4] = { 0.72,'q' };
	arr2[5] = { 0.94,'c' };
	arr2[6] = { 0.21, 'b' };
	arr2[7] = { 0.12, 'n' };
	arr2[8] = { 0.23, 'l' };
	arr2[9] = { 0.68, 'p' };

	// stworzenie kopii arr
	int* arrC = new int[size];
	memcpy(arrC, arr, size * sizeof(int));

	int* arrB = new int[size];
	memcpy(arrB, arr, size * sizeof(int));

	// zliczanie
	testCSort(arrC, size, m);

	cout << endl;

	// kubelkowe (dla int)
	testBSortInt(arrB, size, m);

	cout << endl;

	// kubelkowe (dla T)
	testBSortData(arr2, size, 1.0, getSortKey, dataCompare);

	// sprawdzenie czy arrC == arrB
	cout << "Sprawdzenie rownosci tablic:" << endl;
	if (compareArrays(arrC, arrB, size, dataCompare)) {
		cout << "arrC == arrB" << endl;
	}
	else {
		cout << "arrC != arrB" << endl;
	}

	delete[] arrC;
	delete[] arrB;
	delete[] arr2;
}

void main_ints()
{
	srand(80);
	
	const int testStages[] = {
		10/*,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000*/
	};

	const int m = (int)pow(10, 7);
	double elapsedTime = 0;
	clock_t start, stop;

	int shl = 15;

	for (int i = 0; i < sizeof(testStages) / sizeof(testStages[0]); i++) {
		const int n = testStages[i];

		int* arr1 = new int[n];

		for (int i = 0; i < n; i++) {
			arr1[i] = (((rand() & 0x7FFF) << shl) + (rand() & 0x7FFF)) % m;
		}

		cout << "SORTOWANIE " << testStages[i] << " ELEMENTOW:" << endl << endl;
		cout << "Przed sortowaniem (skrot): " << arrayToStr(arr1, n, 10);
		cout << endl;

		int* arr2 = new int[n];
		int* arr3 = new int[n];
		memcpy(arr2, arr1, n * sizeof(int));
		memcpy(arr3, arr1, n * sizeof(int));

		// counting sort
		cout << "ZLICZANIE (" << testStages[i] << " elementow):" << endl << endl;

		start = clock();
		countingSort(arr1, n, m);
		stop = clock();

		cout << "Po sortowaniu przez zliczanie (skrot): " << arrayToStr(arr1, n, 10);

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);

		printf("Posortowanie %d elementow (przez zliczanie) zajelo %.6f s (%.3f ms)\n\n", testStages[i], elapsedTime, elapsedTime * 1000);
		
		// heap sort
		cout << "KOPCOWANIE (" << testStages[i] << " elementow):" << endl << endl;

		start = clock();
		BinHeap<int>* bheap = new BinHeap<int>(arr2, n, dataCompare, false);
		bheap->sort(dataCompare);
		stop = clock();

		cout << "Po sortowaniu przez kopcowanie (skrot): " << arrayToStr(arr2, n, 10);

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);

		printf("Posortowanie %d elementow (przez kopcowanie) zajelo %.6f s (%.3f ms)\n\n", testStages[i], elapsedTime, elapsedTime * 1000);

		// bucket sort
		cout << "KUBELKOWE (" << testStages[i] << " elementow):" << endl << endl;

		start = clock();
		bucketSort(arr3, n, m);
		stop = clock();

		cout << "Po sortowaniu kubelkowym (skrot): " << arrayToStr(arr3, n, 10);

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);

		printf("Posortowanie %d elementow (kubelkowo) zajelo %.6f s (%.3f ms)\n\n", testStages[i], elapsedTime, elapsedTime * 1000);

		int cmpArr12 = compareArrays(arr1, arr2, n, dataCompare);
		int cmpArr23 = compareArrays(arr2, arr3, n, dataCompare);

		if (cmpArr12 && cmpArr23) {
			cout << "Zawartosc wszystkich tablic jest rowna" << endl;
		}
		else {
			cout << "Blad sortowania - zawartosci tablic roznia sie" << endl;
		}
		cout << endl << endl;

		delete bheap;
		delete[] arr1;
		//Niepotrzebne bo wlasnosc nad arr2 przejal bheap
		//delete[] arr2;
		delete[] arr3;
	}
}

void main_data() {
	const int testStages[] = {
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000
	};

	const double m_double = (double)pow(2, 30);
	double elapsedTime = 0;
	clock_t start, stop;

	int shl = 15;

	for (int i = 0; i < sizeof(testStages) / sizeof(testStages[0]); i++) {
		const int n = testStages[i];

		data* arr1 = new data[n];
		for (int i = 0; i < n; i++) {
			arr1[i] = generateData(shl, m_double);
		}

		cout << "SORTOWANIE " << testStages[i] << " ELEMENTOW:" << endl << endl;
		cout << "Przed sortowaniem (skrot):" << endl << arrayToStr(arr1, n, 10);
		cout << endl;

		data* arr2 = new data[n];
		memcpy(arr2, arr1, n * sizeof(data));

		// heap sort
		cout << "KOPCOWANIE (" << testStages[i] << " elementow):" << endl << endl;

		start = clock();
		BinHeap<data>* bheap = new BinHeap<data>(arr1, n, dataCompare, false);
		bheap->sort(dataCompare);
		stop = clock();

		cout << "Po sortowaniu przez kopcowanie (skrot): " << arrayToStr(arr1, n, 10);

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);

		printf("Posortowanie %d elementow (przez kopcowanie) zajelo %.6f s (%.3f ms)\n\n", testStages[i], elapsedTime, elapsedTime * 1000);

		// bucket sort
		cout << "KUBELKOWE (" << testStages[i] << " elementow):" << endl << endl;

		start = clock();
		bucketSort<data>(arr2, n, 1.0, getSortKey, dataCompare);
		stop = clock();

		cout << "Po sortowaniu kubelkowym (skrot):" << endl << arrayToStr(arr2, n, 10);
		cout << endl;

		elapsedTime = double(stop - start) / (CLOCKS_PER_SEC);

		printf("Posortowanie %d elementow (kubelkowo) zajelo %.6f s (%.3f ms)\n\n", testStages[i], elapsedTime, elapsedTime * 1000);

		int cmpArr = compareArrays(arr1, arr2, n, dataCompare);
		
		if (cmpArr) {
			cout << "Zawartosc obu tablic jest rowna" << endl;
		}
		else {
			cout << "Blad sortowania - zawartosci tablic roznia sie" << endl;
		}
		cout << endl << endl;

		//Niepotrzebne bo wlasnosc nad arr1 przejal bheap
		//delete[] arr1;
		delete bheap;
		delete[] arr2;
	}
}

int main() {
	//testSorts();
	main_ints();
	main_data();

	getchar();
	return 0;
}
