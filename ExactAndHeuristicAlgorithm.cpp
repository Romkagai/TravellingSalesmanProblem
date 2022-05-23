#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <locale>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <string.h>

using namespace std;
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  АЛГОРИТМ ДЕЙКСТЫ  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//Создание матрицы стоимости путей
int** CreateDijkstraMatrix(int n)
{
	int** M = new int* [n];                 //Создаём двумерный дин. массив
	for (int i = 0; i < n; ++i)
	{
		M[i] = new int[n];
	}
	for (int i = 0; i < n; i++)             //Заполняем его
	{
		for (int j = 0; j < n; j++)
		{
			if (j == i) { M[i][j] = 0; }    //Если i=j, то 0 (диаг. элемент)
			else
			{
				M[i][j] = rand() % 100 + 1; //Иначе случайное число от 1 до 100
			}
		}
	}
	return M;                               //Функция возвращает созданный массив
}

//Вывод на экран матрицы стоимости путей
void PrintDijkstraMatrix(int** M, int n)
{
	cout << "Из/В\t ";              //Выводим все элементы в правильном порядке
	for (int i = 0; i < n; i++)     //С удобным для глаза оформлением
	{
		cout << i << " \t";
	}
	cout << endl << endl;
	for (int i = 0; i < n; i++)
	{
		cout << " " << i << "\t|";
		for (int j = 0; j < n; j++)
		{
			if (M[i][j] == 0) cout << " " << "\t|";
			else cout << M[i][j] << "\t|";
		}
		cout << endl << "\t";
		for (int i = 0; i < n; i++)
		{
			cout << "--------";
		}
		cout << endl;
	}

}

//Функция подсчета стоимости пути по данной перестановке
int TripCost(int* P, int** M, int n)
{
	int sum = 0;
	for (int i = 0; i < n - 1; i++)
	{
		sum += M[P[i]][P[i + 1]];   //Суммируем стоимость проезда по текущей перестановке (0+1; 1+2; ... n-1 + n)
	}
	sum += M[P[n - 1]][P[0]];         //Не забывая про путь из последнего города в город отправления (n + 0)
	return sum;                     //Функция возвращает стоимость пути
}

//Функция инвертирования хвоста перестановки
void TailInversion(int* P, int n, int i)
{
	while (i < n)
	{
		swap(P[i], P[n]);           //Получая индексы начала и конца хвоста меняем их в виде
		i++; n--;                   //Первый с последним, второй с предпоследним...
	}
}

//Функция поиска следующей перестановки
bool Permutation(int* P, int n)
{
	int i = n - 1;
	for (i = n - 1; i > 0; i--)
	{
		if (P[i] < P[i + 1])                                                            //Реализация алгоритма Дейкстры
		{                                                                               //Для создания новых перестановок
			for (int j = n; j > 0; j--)
			{
				if (P[i] < P[j])
				{
					swap(P[j], P[i]); TailInversion(P, n - 1, i + 1); return true;          //Если удалось создать перестановку возвращаем true
				}                                                                       //Выходим из цикла
			}
			return false;                                                               //Если не удалось выполнить if возвращаем false   
		}
	}
	return false;                                                                       //Если не удалось выполнить if возвращаем false 
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ЖАДНЫЙ АЛГОРИТМ ПОИСКА ПУТИ  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//Функция подсчета суммы элементов в строке матрицы
int MaxRowCost(int** N, int n, int i)
{
	int sum = 0;
	for (int j = 0; j < n; j++)
	{
		sum += N[i][j];
	}
	return sum;
}

//Функция удаления элементов строки и столбца по заданному элементу
void deleteRawColumn(int** N, int Raw, int Column, int n)
{
	for (int i = 0; i < n; i++)
	{
		N[Raw][i] = 0;
		N[i][Column] = 0;
	}
	N[Column][Raw] = 0;
}

//Функция проверки матрицы на содержание нулей
int MatrixCheck(int** N, int n)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			sum += N[i][j];
		}
	}
	return sum;
}

//Функция проверки гамильтонового цикла на полноту 
bool DCycleCheck(int* Towns, int n, int minColumn, int maxRow)
{
	int i = 0, j, start = maxRow, finish = minColumn;       //Хотим пойти из start в finish
	for (j = 0; j < n; j++)                                 //Проходим по всему массиву Towns и ожидаем 2 случая:
	{
		i = Towns[minColumn];                               //1) В массиве Towns встретили -1 - город не был посещен, выходим с true
		if (i == -1) return true;                           //2) Проходя по массиву Towns получилось так, что "start"..."start" =>
		minColumn = i;                                      // выходим с false
		if (i == start) { cout << "Дуга (" << start << "," << finish << ") образует цикл! "; return false; }
	}
	return false;
}


int main()
{
start:
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	//Создание необходимых переменных и массивов
	int n;										//Кол-во городов
	int** M; 									//Матрица путей
	int k;										//Город отправления
	int minSum;									//Минимальная стоимость проезда
	int Count = 1; int minCount = 1;			//Номер текущей перестановки и номер минимальной по стоимости перестановки
	
	clock_t start, stop; double T1, T2;				//Создаем таймер

	cout << "Точное решение задачи коммивояжера" << endl
		<< "Введите исходное количество городов: ";
	cin >> n;
	cout << "Создаем матрицу стоимости проезда по городам:" << endl << endl;

	int* P = new int[n];						//Выделяем память под перестановки
	int* Pmin = new int[n];						//Выделяем память под min перестановку
	M = CreateDijkstraMatrix(n);				//Создаем 
	PrintDijkstraMatrix(M, n);					//И выводим матрицу стоимости путей
	cout << endl;

	cout << "Введите город отправления k: ";
	cin >> k;

	start = clock();
	cout << "Маршруты:\tСтоимость:" << endl;

	P[0] = k;									//Заполняем первоначальную перестановку
	for (int i = 1; i < n; i++)					//В виде k0123...(k-1)(k+1)...(n-1)
	{
		P[i] = i - 1;
		if (i >= k + 1) { P[i] = i; }
	}

	cout << Count << ". "; Count++;

	for (int i = 0; i < n; i++)					//Выводим первоначальную перестановку
	{
		cout << P[i];
	}

	cout << "\t\t";

	minSum = TripCost(P, M, n);					//Определяем стоимость первой перестановки и выводим её
	cout << minSum << endl;

	for (int i = 0; i < n; i++)					//Сохраняем начальный маршрут как минимальный
	{
		Pmin[i] = P[i];
	}

	while (Permutation(P, n))					//Пока удается создать новую перестановку
	{
		cout << Count << ". ";

		for (int i = 0; i < n; i++)				//Выводим новую перестановку
		{
			cout << P[i];
		}
		cout << "\t\t" << TripCost(P, M, n);	//Выводим стоимость новой перестановки		
		cout << endl;
		if (TripCost(P, M, n) < minSum)			//Если новая перестановка "дешевле"
		{
			for (int i = 0; i < n; i++)
			{
				Pmin[i] = P[i];					//Сохраняем её в качестве минимальной				
			}
			minCount = Count;					//Сохраняем номер min перестановки
			minSum = TripCost(P, M, n);			//И сохраняем её минимальную стоимость
		}
		Count++;
	}
	stop = clock();
	T1 = (stop - start + .0) / CLOCKS_PER_SEC;

	cout << "Перестановка минимальной находится стоимости под номером " << minCount << ": ";

	for (int i = 0; i < n; i++)					//Вывод перестановки min стоимости и её стоимости
	{
		cout << Pmin[i];
	}

	cout << endl << "Её минимальная стоимость: " << minSum << endl << endl;

	//-----Вторая эвристика----- 

	cout << "Второй эвристический алгоритм:" << endl;
	cout << "В строке с максимальной суммой дуг, выбираем минимальную дугу" << endl << endl;

	start = clock();

	int* Towns = new int[n];		//Массив посещенных городов для проверки зацикливания
	for (int i = 0; i < n; i++)
	{
		Towns[i] = -1;
	}

	int FinalWay = 0;				//Длина конечного пути
	int count = 1;					//Счетчик шагов
	while (MatrixCheck(M, n) != 0)	//Пока в матрице есть элементы
	{
		cout << "Шаг №" << count << endl;
		PrintDijkstraMatrix(M, n);	//Печатаем текущую матрицу
		int maxSum = 0;											//Возьмем max строку меньше минимального - 0
		int maxRow = 0;
		for (int i = 0; i < n; i++)
		{
			if (MaxRowCost(M, n, i) > maxSum)
			{
				maxSum = MaxRowCost(M, n, i);					//Поиск max строки, откуда едем
				maxRow = i;										//Если сумма строки > предыдущей она max
			}													//Запоминаем индекс
		}

		int minWay = 101;										//Возьмем min дугу больше максимального - 100
		int minColumn = 0;

		for (int j = 0; j < n; j++)
		{
			if (M[maxRow][j] < minWay && M[maxRow][j] != 0)			//Поиск min элемента в max строке, куда едем
			{
				if (count == n || DCycleCheck(Towns, n, j, maxRow))	//Проверка на зацикливание, учитывая, что последняя дуга всегда
				{													//образует цикл
					Towns[maxRow] = j;								//Записываем город в массив посещенных городов
					minColumn = j;									//Запоминаем индекс минимальной дуги
					minWay = M[maxRow][j];							//Запоминаем длину минимальной дуги
				}											
			}														
		}
			FinalWay += minWay;
			deleteRawColumn(M, maxRow, minColumn, n);
			count++;
			cout << "Добавляем дугу (" << maxRow << "," << minColumn << ")" << endl << endl;		
	}
	stop = clock();
	T2 = (stop - start + .0) / CLOCKS_PER_SEC;
	
	//-----Вторая эвристика----- 

	cout << endl << "Длина пути точного решения: " << minSum << endl;
	cout << "Длина пути по второму эвристическому алгоритму: " << FinalWay << endl;
	cout << "Получены дуги:";
	for (int i = 0; i < n; i++)
	{
		cout << "(" << i << "," << Towns[i] << ") ";
	}
	cout << endl << "Маршрут: " << k;
	for (int a = 0; a < n-1; a++)
	{ 
		cout << /*"->" <<*/ Towns[k];
		k = Towns[k];
	}
	cout << endl << endl;
	
	cout << "Время работы точного решения: " << T1 << endl <<
		"Время работы второго эвристического алгоритма: " << T2 << endl <<
		"P.S. Вывод матриц на экран задействует достаточно много времени" << endl << endl;

	delete[]P;
	delete[]M;									//Очистка памяти 
	delete[]Pmin;
	

	cout << "Хотите запустить программу заново? (y - да, любая другая клавиша - выйти из программы)" << endl;
	char ch;									//Повторный запуск программы
	cin >> ch;
	if (ch == 'y') { system("cls"); goto start; }

	return(0);
}
