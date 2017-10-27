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
Individual TheBest;//���ŽⱣ��
Boat boats[Max];//��¼ÿһ��������Ϣ

const string FilePath = "D:\\Desktop\\Algorithm\\project\\project instances\\";
const string TXTFormat = ".txt";

const int Generation = 250;//�����Ĵ���
						   //Elite = 5:<0.1s Elite = 4:0.2s Elite = 3: <40s
const int Stagnation = 80;//ͣ��
int CountStag = 0;
double Elite = 0.33;//��Ӣ������Group.size() / Elite, Elite: 1 - 10
int ProtectionPeriod = 100;//������
double Mortality = 0.2;//��̭��
double AberrationRate = 0.01;//������

int number[3 * Max];//�ڶ�ȡ�ļ���ʱ����ʱ�����ȡ��������
int TotalServiceTime = 0, LargestWidth = 0, NumberOfBoat = 0;//�ۿڷ���ʱ�䣬��λ������ֻ��
int TotalWaitingTime = 0;//���Ž���ܹ��ȴ�ʱ��
int LastDepartureTime = 0;//���Ž��������ʱ��
int UnassignedVessel = 0;//���Ž��δ���䴬ֻ��

															 //int NumerOfIndividuals = 0;//������

double AccumulatedRate[5000 * Max];//���۸��ʣ�����������Ⱥ������Ϊ5000;
set<int, greater<int>> SelectionArray;//������Щ�������Ҫ����̭���������򣻱����ظ���ֵ

int child1[Max], child2[Max];//�ڽ��������ʱ��������ʱ��¼�������Ⱦɫ��
bool child_1[Max], child_2[Max];//�ڽ��������ʱ����������������һ��

int Map[Max][Max];//��¼�ۿڴ�ֻ���ŵĵ�ͼ


bool ReadFile(string FileName);//�������ļ��ж�ȡ����
void ChangeParameter(int generation);
unsigned int GetRandomPosition();//�����0-NumberOfBoat������һ��λ��
void Initialize();//��ʼ����Ⱥ������ʮ���������
int Fitness(const Individual one, bool ifDrawMap = false);//����������Ӧ�ȣ���Ӧ��Խ��Խ���ױ���̭
void Selection();//�ﾺ������ʤ��̭
void Cross();//����һ���ĸ��壬��Ⱥ����
void Aberration();//����,�ڽ������֮����ã������������б������
//��ǰһ������������ӵ�����Ⱥ��
//�ڽ���ͱ���֮����������Ⱥ
bool cmp(const Individual a, const Individual b);//�Ƚ�����Ϊ��Ӧ��
void GenerateNewGroup();//��������µ�Ⱥ�壺Elite + (1 - Morality) * Parents Group
//void Catastrophe();

#endif // !HeadFile_H
