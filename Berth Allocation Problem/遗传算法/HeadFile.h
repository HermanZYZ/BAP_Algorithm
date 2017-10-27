#pragma once
#ifndef HeadFile_H
#define HeadFile_H

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstring>
#include <string>
#include <ctime>
#include <set>
#include <iomanip>
#include <functional>//greater<>()

using namespace std;

struct Boat
{
	int arrive_time;
	int need_time;
	int width;
	int Location_x;
	int Location_y;
	Boat()
	{
		Location_x = -1;
		Location_y = -1;
		arrive_time = 0;
		need_time = 0;
		width = 0;
	}
	Boat(int a, int b, int c)//:arrive_time(a),need_time(b),width(c){};
	{
		arrive_time = a;
		need_time = b;
		width = c;
	}
};

const int Max = 105;
const int INF = 1 << 9;

struct Individual
{
	int fitness;
	int Gene[Max];
	Individual()
	{
		fitness = INF;
		memset(Gene, 0, sizeof(Gene));
	}
};

std::vector<Individual> Group;
std::vector<Individual> NewGroup;
Individual TheBest;//最优解保存
Boat boats[Max];//记录每一条船的信息

const string FilePath = "D:\\Desktop\\Algorithm\\project\\project instances\\";
const string TXTFormat = ".txt";

const int Generation = 250;//迭代的次数
						   //Elite = 5:<0.1s Elite = 4:0.2s Elite = 3: <40s
const int Stagnation = 80;//停滞
int CountStag = 0;
double Elite = 0.33;//精英比例，Group.size() / Elite, Elite: 1 - 10
int ProtectionPeriod = 100;//保护期
double Mortality = 0.2;//淘汰率
double AberrationRate = 0.01;//变异率

int number[3 * Max];//在读取文件的时候临时保存读取到的数据
int TotalServiceTime = 0, LargestWidth = 0, NumberOfBoat = 0;//港口服务时间，泊位数，船只数
int TotalWaitingTime = 0;//最优解的总共等待时间
int LastDepartureTime = 0;//最优解的最后离港时间
int UnassignedVessel = 0;//最优解的未分配船只数

															 //int NumerOfIndividuals = 0;//个体数

double AccumulatedRate[5000 * Max];//积累概率，必须限制种群容纳量为5000;
set<int, greater<int>> SelectionArray;//保存哪些个体的需要被淘汰；降序排序；避免重复的值

int child1[Max], child2[Max];//在交叉操作的时候用来临时记录新生体的染色体
bool child_1[Max], child_2[Max];//在交叉操作的时候用来辅助产生新一代

int Map[Max][Max];//记录港口船只安排的地图


bool ReadFile(string FileName);//从样例文件中读取数据
void ChangeParameter(int generation);
unsigned int GetRandomPosition();//随机从0-NumberOfBoat中挑出一个位置
void Initialize();//初始化种群，生成十个随机个体
int Fitness(const Individual one, bool ifDrawMap = false);//计算个体的适应度，适应度越大越容易被淘汰
void Selection();//物竞天择，优胜劣汰
void Cross();//增加一倍的个体，种群繁衍
void Aberration();//变异,在交叉操作之后调用，对新生代进行变异操作
//将前一代的优良个体加到新种群中
//在交叉和变异之后生成新种群
bool cmp(const Individual a, const Individual b);//比较条件为适应度
void GenerateNewGroup();//最后生成新的群体：Elite + (1 - Morality) * Parents Group
//void Catastrophe();

#endif // !HeadFile_H
