#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <iostream>
#include <vector>

using namespace std;
int M, N;

int Area(int **B, int i, int j)
{
	int TempArea = 0;
	TempArea++;
	*(*(B + i) + j) = 0;

	if (i > 0)
		if (*(*(B + i - 1) + j) == 1)
		{
			int tempI = i-1;
			int tempJ = j;
			TempArea += Area(B, tempI, tempJ);
		}
	
	if (j<N-1)
		if (*(*(B + i) + j+1) == 1)
		{
			int tempI = i;
			int tempJ = j+1;
			TempArea += Area(B, tempI, tempJ);
		}

	if (j > 0)
		if (*(*(B + i) + j - 1) == 1)
		{
			int tempI = i;
			int tempJ = j - 1;
			TempArea += Area(B, tempI, tempJ);
		}

	if (i < M-1)
		if (*(*(B + i+1) + j) == 1)
		{
			int tempI = i+1;
			int tempJ = j;
			TempArea += Area(B, tempI, tempJ);
		}

	return TempArea;
}

void main()
{
	int MaxArea = 0;
	srand((unsigned)time(NULL));
	cout << "Input number of lines\n";
	cin >> M;
	cout << "Input number of colonne\n";
	cin >> N;

	int** B;
	B = new int* [M];
	for (int i = 0; i < M; i++)
	{
		B[i] = new int[N];
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			*(*(B + i) + j) = (rand() % 2) * (rand() % 2);
			cout << *(*(B + i) + j) << ' ';
		}
		cout<<'\n';
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (*(*(B + i) + j) == 1)
				MaxArea = max(Area(B,i,j), MaxArea);
		}
	}
	cout << MaxArea;
}
