// ConsoleApplication2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<string>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<vector> 
#include<cmath>
#include<fstream>
#include<iomanip>
#include<windows.h>
#include<ctime>
using namespace std;

//实现一个能够生成数独终局并且能求解数度问题的控制台程序
//命令行 suduku.exe -c
//存储在文件 suduku.txt 中
//左上角第一个数为 3
//Hint线索 Clue提示数

#define size 9
#define f_num 3

ofstream ofile;
ifstream ifile;

time_t t1, t2;

int countn = 0, limit = 0, jud = 0;
int Grid[9][9];
int Rol[9];
int Ful_que[100], low = 0, top = 0;
int Grid_ques[9][9];//数独开局

//交换函数
int Exchange(int s[], int i, int j)
{
	int temp;
	if (i == j)
		return 0;
	else
	{
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}
}
//行交换函数
int ExchangeRol(int i, int j)
{
	int Ruller[9];
	for (int f1 = 0; f1 < 9; f1++)
	{
		Ruller[f1] = Grid[i][f1];
		Grid[i][f1] = Grid[j][f1];
		Grid[j][f1] = Ruller[f1];
	}
	return 0;
}

//生成终局
int Cre_End()
{
	int i, j, k;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < 9; k++)
			{
				Grid[j + 3 * i][k] = Rol[(i + k + j * 3) % 9];
			}
				
		}
	}
	
	//获得一种终局，打印

	char *s = (char *)malloc(200);
	if (s == NULL)
		return 0;//分配空间不成功

	int sp = 0;

	for (int fy = 0; fy < 9; fy++)
	{
		for (int fx = 0; fx < 8; fx++)
		{
			*(s + sp) = Grid[fy][fx] + '0';
			sp++;
			
			*(s + sp) = ' ';
			sp++;
			

///			ofile << Grid[fy][fx] << " ";
//			cout << Grid[fy][fx] << ' ';
		}
		*(s + sp) = Grid[fy][8] + '0';
		sp++;

		*(s + sp) = '\n';
		sp++;
///		ofile << Grid[fy][8] << endl;
//		cout << Grid[fy][8] << endl;
	}
	*(s + sp) = '\0';
	ofile << s << endl;

	countn += 1;
	if (countn == limit) return -1;
	

	//之后交换4-6，7-9两行
	//34 35 45 67 68 78
	for (int cas0 = 0; cas0 < 2; cas0++)
	{
		for (int cas1 = 3; cas1 < 6; cas1++)
		{
			for (int cas2 = 4; cas2 < 6; cas2++)
			{
				ExchangeRol(cas1 + 3 * cas0, cas2 + 3 * cas0);
				//打印到文件
				
				memset(s, sp, sizeof(*s));
				sp = 0;

				for (int fy = 0; fy < 9; fy++)
				{
					for (int fx = 0; fx < 8; fx++)
					{
						*(s + sp) = Grid[fy][fx] + '0';
						sp++;

						*(s + sp) = ' ';
						sp++;
///						ofile << Grid[fy][fx] << " ";
//						cout << Grid[fy][fx] << " ";
					}
					*(s + sp) = Grid[fy][8] + '0';
					sp++;

					*(s + sp) = '\n';
					sp++;
///					ofile << Grid[fy][8] << endl;
//					cout << Grid[fy][8] << endl;
				}
				*(s + sp) = '\0';
				ofile << s << endl;
//				cout << endl;

				countn++;
				if (countn == limit) return -1;

				ExchangeRol(cas2 + 3 * cas0, cas1 + 3 * cas0);
			}
		}
	}
	//for (int i = 0; i < 9; i++) Grid[0][i] = Rol[i];
	//for (i = 3; i < 12; i++) Grid[][i - 3] = Rol[i % 9];//*****
}

//全排列函数
int Full_arrange(int n, int s[])
{
	if (jud == -1) return 0;
	if (n == 1)
	{
		Ful_que[top++] = s[0];
		for (int cas = 1; cas < 9; cas++)
		{
			Rol[cas] = Ful_que[cas - 1];
			//printf("%d", Rol[cas]);

			//生成数独终局
		}
		jud = Cre_End();
		if (jud == -1) return 0;
		Ful_que[--top] = 0;
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			Exchange(s, 0, i);
			Ful_que[top++] = *s;
			Full_arrange(n - 1, s + 1);
			Ful_que[top--] = 0;
			Exchange(s, 0, i);
		}
	}
	
	return 0;
}

//获取开局
int Get_ques()
{
	//运行到该行
	int line = 0;
	int i = 0, j = 0;
	int flag = 0;
//	char* a = (char *)malloc(20);
//	while (!ifile.eof())
//	{
//		if (line == r * 10) break;
//		ifile.getline(a, 1e9,'\n');
//		line++;
//	}
	while (i != 9)
	{
		ifile >> flag;
		if (flag >= 0 && flag <= 9)
		{
			Grid_ques[i][j] = flag;
			j++;
		}
		if (j == 9)
		{
			i++;
			j = 0;
		}
	}
	

	return 0;
}

//解数独
void Solve(int i, int j)
{
	int k = 0;
	for (; i < 9; i++)
	{
		for (; j < 9; j++)
		{
			if (Grid_ques[i][j] == 0)
			{
				int i1 = i / 3, j1 = j / 3;
				i1 = i1 * 3;
				j1 = j1 * 3;
				int f2 = 0;
				for (k = 1; k <= 9; k++)
				{
					f2 = 0;
					//判断该数符合条件，f2==-1不符合，继续下一轮搜索
					for (int cas = 0; cas < 9; cas++)
					{
						if (Grid_ques[cas][j] == k || Grid_ques[i][cas] == k)
						{
							f2 = -1;
							break;
						}
					}

					while(1)
					{
						while(1)
						{
							if (Grid_ques[i1%9][j1%9] == k)
							{
								f2 = -1;
								break;
							}
							j1++;
							if (j1 % 3 == 0)
							{
								j1 -= 3;
								break;
							}
						}
						if (f2 == -1) break;
						j1++;
						if (i1 % 3 == 0) break;

					}
					if (f2 == 0)
					{
						Grid_ques[i][j] = k;
						Solve(i + (j + 1) / 9, (j + 1) % 9);
					}
				}
				return;
			}
		}
		j = 0;
	}
	for (int cas3 = 0; cas3 < 9; cas3++)
	{
		for (int cas4 = 0; cas4 < 8; cas4++)
		{
			cout << Grid_ques[cas3][cas4] << ' ';
		}
		cout << Grid_ques[cas3][8] << endl;
	}
	return;
}

int main(int argc, char* argv[])
{
	t1 = clock();

	Rol[0] = f_num;
	int c[] = {1,2,4,5,6,7,8,9};
	int N = 8;
//	int r, r1;

	int l0;
	int suanzi = 1;

//	argc = 3;
//	char a[] = "-c", b[] = "1000000";
//	argv[1] = a;
//	argv[2] = b;

	cout << argc << endl;
	if (argc != 3)
	{
		cout << "输入格式错误" << endl;
		return 0;
	}
	
	l0 = (int)strlen(argv[2]);
	int l1 = l0;

	if (strcmp(argv[1],"-c") == 0)
	{
		while (l1--)
		{
			if (l1 != l0 - 1)
				suanzi *= 10;
			limit += suanzi * (argv[2][l1] - '0');
			
			//test module
			cout << limit << endl;

			if (argv[2][l1] < '0' || argv[2][l1] > '9')
			{
				cout << "输入格式错误" << endl;
				return 0;
			}
		}
		cout << "进入终局生成模块" << endl;
		//生成终局，将结果记录到sudoku.txt中
		ofile.open("sudoku.txt");
		Full_arrange(N, c);
		ofile.close();
	}
	
	else {
		if (strcmp(argv[1], "-s") == 0)
		{
			cout << "进入解数独模块" << endl;
			//解数独，思路，从sudoku.txt中随机读取终局，
			//线索Hint，提示数Clue，提示数在23~30之间
			//利用终局产生题目，利用分支限界求解
			char address[100] = { 0 };
			int x = 0, y = 0;
			while (argv[2][y] != '\0')
			{
				if (argv[2][y] == '\\')
				{
					address[x++] = '\\';
				}
				address[x++] = argv[2][y++];
			}
			printf("%s\n", address);
			ifile.open(argv[2]);
			Get_ques();
			Solve(0, 0);
			ifile.close();
		}
		else cout << "输入格式错误" << endl;
	}

	t2 = clock();

	cout << "程序运行结束, 运行时间为 " << 0.1*(t2 - t1) / 100.0 << 's' << endl;
	return 1;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
