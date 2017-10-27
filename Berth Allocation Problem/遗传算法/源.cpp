//#include <bits/stdc++.h>
#include "HeadFile.h"


bool ReadFile(string FileName)
{
	string DataFromFile;
	ifstream infile;
	memset(number, 0, sizeof(number));
	infile.open(FileName, ios::in);

	if (!infile)
	{
		infile.close();
		return false;
	}
	int index = 0;
	while (!infile.eof())
	{
		getline(infile, DataFromFile, '\n');

		for (unsigned int i = 0; i<DataFromFile.size(); i++)
		{
			while (DataFromFile[i] != ' ' && DataFromFile[i] != '\n' && i<DataFromFile.size())
			{
				number[index] = number[index] * 10 + DataFromFile[i] - '0';
				i++;
			}
			index++;
		}
	}

	TotalServiceTime = number[0];
	LargestWidth = number[1];
	NumberOfBoat = number[2];
	int j = 0;
	for (int i = 3; i<index; i += 3)
	{
		boats[j].arrive_time = number[i];
		boats[j].need_time = number[i + 1];
		boats[j].width = number[i + 2];
		j++;
	}
	infile.close();
	return true;
}

void ChangeParameter(int generation)
{
	if (generation < 50)
		return;
	Elite = 1.0 * 10 / generation;
	Mortality = 0.8 - 1.0 * 10 / generation;
	AberrationRate = 1 - 1.0 * 10 / generation;
	return;
}

/***********************************************************************************************************************/

unsigned int GetRandomPosition()
{
	return rand() % NumberOfBoat;
}

void Initialize()
{
	Individual FirstGene;
	Individual SecondGene;
	FirstGene.fitness = INF;
	SecondGene.fitness = INF;
	for (int i = 0; i<NumberOfBoat; i++)//���ɰ�˳�����е���������
	{
		FirstGene.Gene[i] = i;
		SecondGene.Gene[i] = NumberOfBoat - 1 - i;
	}
	Group.push_back(FirstGene);
	Group.push_back(SecondGene);
	/*for (int i = 0; i < NumberOfBoat; i++)
		cout << Group[1].Gene[i] << " ";
	cout << endl;*/

	for (int num = 2; num < 50; num++)//����ʣ���ǰ˸�������и���
	{
		Individual OtherGene;
		OtherGene.fitness = INF;
		::memcpy(OtherGene.Gene, Group[num - 1].Gene, sizeof(OtherGene.Gene));//���
		//OtherGene = Group[num-1];//������һ���������ٽ����������,ǳ����
		unsigned int r = 0;
		for (int i = 0; i < NumberOfBoat; i++)
		{
			r = GetRandomPosition();
			swap(OtherGene.Gene[i], OtherGene.Gene[r]);
		}
		Group.push_back(OtherGene);
	}
	return;
}

int Fitness(const Individual one, bool ifDrawMap)
{
	if (one.fitness != INF && !ifDrawMap)
		return one.fitness;
	/*cout << Group.size() << endl;
	for (int j = 0; j < Group.size(); j++)
	{
		for (int i = 0; i < NumberOfBoat; i++)
			cout << Group[j].Gene[i] << " ";
		cout << endl;
	}*/
	/*for (int i = 0; i < NumberOfBoat; i++)
		cout << one.Gene[i] << " ";
	cout << endl;*/

	TotalWaitingTime = 0;
	LastDepartureTime = 0;
	
	for (int i = 0; i < NumberOfBoat; i++)
	{
		boats[one.Gene[i]].Location_x = boats[one.Gene[i]].Location_y = -1;
	}
	for (int i = 0; i<LargestWidth; i++)
	{
		for (int j = 0; j<TotalServiceTime; j++)
		{
			Map[i][j] = -1;
		}
	}
	for (int i = 0; i<NumberOfBoat; i++)
	{
		bool find = false;
		for (int j = boats[one.Gene[i]].arrive_time; j<TotalServiceTime; j++)
		{
			for (int k = 0; k<LargestWidth; k++)
			{
				if (j + boats[one.Gene[i]].need_time <= TotalServiceTime && k + boats[one.Gene[i]].width <= LargestWidth)
				{
					int x = k, y = j;
					bool flag = true;
					for (y = j; y<j + boats[one.Gene[i]].need_time; y++)
					{
						for (x = k; x<k + boats[one.Gene[i]].width; x++)
						{
							if (Map[x][y] != -1)
							{
								flag = false;
								break;
							}
						}
						if (flag == false) break;
					}
					if (flag == true) find = true;
					if (find == true)
					{
						boats[one.Gene[i]].Location_x = k;
						boats[one.Gene[i]].Location_y = j;
						if (LastDepartureTime<j + boats[one.Gene[i]].need_time) LastDepartureTime = j + boats[one.Gene[i]].need_time;
						TotalWaitingTime += j - boats[one.Gene[i]].arrive_time;
						for (x = k; x<k + boats[one.Gene[i]].width; x++)
						{
							for (y = j; y<j + boats[one.Gene[i]].need_time; y++)
							{
								Map[x][y] = one.Gene[i];
							}
						}
						/*if (ifDrawMap)
						{
							cout << "The berth allocation map of the best solution:" << endl;
							for (int a = 0; a<LargestWidth; a++)
							{
							for (int b = 0; b<TotalServiceTime; b++)
							{
							cout <<setw(2)<< Map[a][b] << " ";
							}
							cout << endl;
							}
						}*/
						break;
					}
				}
				if (find == true) break;
			}
			if (find == true) break;
		}
	}
	UnassignedVessel = 0;
	int TotalCost = 0;
	for (int i = 0; i<NumberOfBoat; i++)
	{
		if (boats[i].Location_x == -1) UnassignedVessel++;
	}
	/*for (int i = 0; i<NumberOfBoat; i++)
	{
		cout << boats[i].Location_x << "," << boats[i].Location_y << endl;
	}
	cout << "************************************************" << endl;
	cout << TotalWaitingTime << " " << LastDepartureTime << endl;*/
	TotalCost = UnassignedVessel * 100 + 2 * TotalWaitingTime + LastDepartureTime;
	/*cout << "total cost: " << TotalCost << endl;*/
	if (TheBest.fitness > TotalCost)
	{
		TheBest.fitness = TotalCost;
		memcpy(TheBest.Gene, one.Gene, sizeof(one.Gene));
	}

	if (ifDrawMap)
	{
		cout << "The berth allocation map of the best solution:" << endl;
		for (int a = 0; a<LargestWidth; a++)
		{
			for (int b = 0; b<TotalServiceTime; b++)
			{
				cout << setw(2) << Map[a][b] << " ";
			}
			cout << endl;
		}
		
	}

	return TotalCost;
}

//��̭����

void Selection()
{
	if (Group.size() < ProtectionPeriod)//��Ⱥ̫С��������̭���������������
		return;
	::memset(AccumulatedRate, 0, sizeof(AccumulatedRate));
	SelectionArray.clear();
	int TotalFit = 0;//�ܹ�����Ӧ��
	for (int i = 0; i < Group.size(); i++)
	{
		TotalFit += Group[i].fitness;
	}
	AccumulatedRate[0] = Group[0].fitness / TotalFit;
	for (int i = 1; i < Group.size(); i++)
	{
		AccumulatedRate[i] = 1.0 * Group[i].fitness / TotalFit + AccumulatedRate[i-1];//������۸���
		//cout << AccumulatedRate[i] << endl;
	}
	int SelectionNum =	1.0 * Mortality * Group.size();
	int Size = Group.size();
	//cout << "&&&&" << SelectionNum << endl;
	//getchar();
	//getchar();
	int SelectionIndex = 0;
	double r;
	while(SelectionArray.size() != SelectionNum)//�������̶ģ�ֱ����̭�ĸ����ﵽҪ��
	{
		r = rand() / (double)(RAND_MAX);
		/*
		for (; SelectionIndex < Size; SelectionIndex++)
		{
			if (r - AccumulatedRate[SelectionIndex] < 0.00001)
				break;
		}
		*///���ֲ������Ч��
		SelectionIndex = lower_bound(AccumulatedRate, AccumulatedRate + Size, r) - AccumulatedRate;
		if (SelectionIndex < Group.size())
		{
			SelectionArray.insert(SelectionIndex);
		}
		//cout << SelectionArray.size()<<"&&&&&&"<<r<<"&&&&&"<<SelectionIndex << endl;
	}
	for (set<int>::iterator it = SelectionArray.begin(); it != SelectionArray.end(); it++)//��ɾ����ʱ��Ͳ���Ӱ��ǰ��Ҫɾ����ֵ��λ��
	{
		Group.erase(Group.begin() + (*it));
	}
	return;
}

void Cross()//����һ���ĸ��壬ͬʱ���ϸ�������Ų��ּ����¸���
{
	int start, end;
	int Num = Group.size() / 2;
	int select_1 = 0, select_2 = 0;
	for (int j = 0; j < Num; j++)//����Num���¸��壬�����µĻ���
	{
		select_1 = rand() % Group.size();
		memset(child1, 0, sizeof(child1));
		memset(child2, 0, sizeof(child2));
		start = GetRandomPosition();
		memset(child_1, 0, sizeof(child_1));
		select_2 = rand() % Group.size();
		memset(child_2, 0, sizeof(child_2));
		end = GetRandomPosition();
		if (start > end)
		{
			j--;
			continue;
		}
		//cout << start << "````" << end << endl;
		//getchar();
		for (int i = start; i <= end; i++)
		{
			child_1[Group[select_1].Gene[i]] = 1;
			child_2[Group[select_2].Gene[i]] = 1;
			child1[i] = Group[select_1].Gene[i];
			child2[i] = Group[select_2].Gene[i];
		}
		//cout << "###" << endl;
		for (int i = 0, index = 0; i<NumberOfBoat; i++)
		{
			if (index == start)
			{
				index = end + 1;
			}
			if (child_1[Group[select_2].Gene[i]] == 0)
			{
				child1[index] = Group[select_2].Gene[i];
				index++;
			}
		}
		//cout << "####" << endl;
		for (int i = 0, index = 0; i<NumberOfBoat; i++)
		{

			if (index == start)
			{
				index = end + 1;
			}
			if (child_2[Group[select_1].Gene[i]] == 0)
			{
				child2[index] = Group[select_1].Gene[i];
				index++;
			}
		}
		//cout << "#####" << endl;
		Individual OtherGene;
		OtherGene.fitness = INF;
		
		/*for (int i = 0; i < NumberOfBoat; i++)
			cout << child1[i] << " ";
		cout << endl;*/
		::memcpy(OtherGene.Gene, child1, sizeof(child1));//memcpy��C��׼�⺯����C����û�������ռ䣬���п⺯����������ȫ�ֿռ䡣
		NewGroup.push_back(OtherGene);//��������Ⱥ
		OtherGene.fitness = INF;
		::memcpy(OtherGene.Gene, child2, sizeof(child2));//::memcpy������ʽҪ�����C�⺯����������ʲô��������
		NewGroup.push_back(OtherGene);
	}
}

void Aberration()//����,�ڽ������֮����ã������������б������
{
	int ex = 0, change = 0;//exchange,������ɽ����ĵ㣬����������
	for (int i = 0; i < NewGroup.size(); i++)//�������ɵĸ�����б������
	{
		ex = GetRandomPosition();
		change = GetRandomPosition();
		if (rand() / (double)(RAND_MAX) - AberrationRate < 0.00001)
		{
			change = GetRandomPosition();
			swap(NewGroup[i].Gene[ex], NewGroup[i].Gene[change]);
			NewGroup[i].fitness = INF;
		}
	}
}

//��ǰһ��ǰ50%����������ӵ�����Ⱥ��
//�ڽ���ͱ���֮����������Ⱥ
bool cmp(const Individual a, const Individual b)
{
	return a.fitness < b.fitness;//����
}
void GenerateNewGroup()//��������µ�Ⱥ��
{
	sort(Group.begin(), Group.begin() + Group.size(), cmp);
	if (Group.size() != 0)
	{
		if (TheBest.fitness > Group[0].fitness)
		{
			TheBest.fitness = Group[0].fitness;
			::memcpy(TheBest.Gene, Group[0].Gene, sizeof(TheBest.Gene));
		}
	}
	if (Group.size() != 0)
	{
		if (TheBest.fitness == Group[0].fitness)
		{
			CountStag++;
		}
		else
		{
			CountStag = 0;
		}

		if (CountStag == Stagnation)//Catastrophe�ֱ�
		{
			Group.clear();
			return;
		}
	}
	int EliteNum = 1.0 * Group.size() * Elite;
	for (int i = 0; i < EliteNum; i++)
	{
		NewGroup.push_back(Group[i]);
	}
	Group.clear();
	Group = NewGroup;
	NewGroup.clear();
}

void GA()
{
	Initialize();//��ʼ����һ��
	int generation = 0;
	if (NumberOfBoat < 9)
	{
		Individual Permutation;
		Permutation.fitness = INF;
		for (int i = 0; i < NumberOfBoat; i++)
		{
			Permutation.Gene[i] = i;
		}
		do 
		{
			Fitness(Permutation);
		} while (next_permutation(Permutation.Gene, Permutation.Gene + NumberOfBoat));
	}
	else
		while (generation < Generation)
		{
			//cout << Group.size() << "******" << endl;

			ChangeParameter(generation);
			for (int i = 0; i < Group.size(); i++)
			{
				Group[i].fitness = Fitness(Group[i]);//���������Ӧ��
			}
			//cout << "&" << generation << endl;
			Selection();//���̶�ѡ����̭20%�ĸ���
			//cout << "*" << generation << endl;
			Cross();//��������
			//cout << "@" << generation << endl;
			Aberration();//������������б������
			//cout << "#" << generation << endl;
			GenerateNewGroup();//����Ⱥ����50%����������������������Ⱥ��
			//cout << "^" << generation << endl;
			generation++;
		}
	return;
}

//void MemoryRecovery()
//{
//	int Lenght = Group.size();
//	for (int i = Lenght - 1; i >= 0; i--)
//		delete Group[i];
//	Lenght = NewGroup.size();
//	for (int i = Lenght - 1; i >= 0; i--)
//		delete NewGroup[i];
//}

/***********************************************************************************************************************/
//D:\\Desktop\\Algorithm\\project\\project instances\\12.txt

bool OpenFile(int Times)
{
	switch (Times % 12)
	{
	case 0:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\5.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 1:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\6.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 2:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\7.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 3:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\8.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 4:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\9.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 5:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\10.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 6:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\11.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;

	case 7:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\12.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;
	case 8:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\1.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;
	case 9:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\2.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;
	case 10:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\3.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;
	case 11:
	{
		if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\4.txt"))
		{
			cout << "Not such file!" << endl;
			return false;
		}
	}break;
	}
	return true;
}

int main()
{
	clock_t Start, Finish;//����ʱ�����
	double TotalTime;
	srand((unsigned)time(NULL));//�Ե�ǰϵͳʱ����Ϊ�������
	string FileName;//�ļ���
	//string File;//·��������
	//File = FilePath;
	ofstream outfile;//д�ļ�������
	int Times = -1;

	//ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\12.txt");
	//TheBest.fitness = INF;
	//int t[Max] = {5,3,9,7,24,8,13,12,10,11,19,14,16,15,2,6,17,4,21,1,23,18,20,22};
	//for (int i = 24; i < Max; i++)
	//	t[i] = 0;
	//::memcpy(TheBest.Gene, t, sizeof(t));
	//for (int i = 0; i < NumberOfBoat; i++)
	//	TheBest.Gene[i] -= 1;
	//cout << endl;
	//TheBest.fitness = Fitness(TheBest, true);
	//cout << "," << UnassignedVessel << "," << TotalWaitingTime << "," << LastDepartureTime << "," << TheBest.fitness << '\n';
	////outfile << "Locations of vessels:" << endl;
	//sort(TheBest.Gene, TheBest.Gene + NumberOfBoat);
	//for (int i = 0; i < NumberOfBoat; i++)
	//	cout << "(" << boats[TheBest.Gene[i]].Location_x << " " << boats[TheBest.Gene[i]].Location_y << ")\n";
	//
	//while (1);
	while (Times < 10000)//(getline(cin, FileName))
	{
		Times++;
		/*cout << FileName << endl;
		File += FileName;
		File += TXTFormat;
		cout << File << endl;*/
		TheBest.fitness = INF;
		Group.clear();
		NewGroup.clear();
		//if (!ReadFile(FileName.c_str()));
		
		if (!OpenFile(Times))
			continue;

		/*if (!ReadFile("D:\\Desktop\\Algorithm\\project\\project instances\\1.txt"))
		{
			cout << "Not such file!" << endl;
			continue;
		}*/
		cout << "Number of boats:" << NumberOfBoat << endl;
		for (int i = 0; i < NumberOfBoat; i++)
			cout << boats[i].arrive_time << " " << boats[i].need_time << " " << boats[i].width << endl;
		Start = clock();//�㷨��ʼʱ��
		GA();//�Ŵ��㷨
		Finish = clock();//�㷨����ʱ��
		cout << "******************" << endl;
		sort(Group.begin(), Group.begin() + Group.size(), cmp);
		if (Group.size() != 0)
		{
			if (TheBest.fitness > Group[0].fitness)
			{
				TheBest.fitness = Group[0].fitness;
				::memcpy(TheBest.Gene, Group[0].Gene, sizeof(TheBest.Gene));
			}
		}
		cout << "The lowest number found: ";
		cout << TheBest.fitness << endl;
		cout << "Unsigned Vessel: " << UnassignedVessel << " ";
		cout << "Total Waiting Time: " << TotalWaitingTime << " ";
		cout << "Last Departure Time: " << LastDepartureTime << endl;
		cout << "Sequence of the arrange: " << endl;
		for (int i = 0; i < NumberOfBoat; i++)
			cout << TheBest.Gene[i] + 1 << " ";
		cout << endl;
		
		TotalTime = (double)(Finish - Start) / CLOCKS_PER_SEC;//����ʱ�俪��
		cout << "Running Time:" << TotalTime << " Seconds��" << endl << endl;
	
		switch (Times % 12)
		{
		case 0:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game5.csv", ios_base::app); break;
		case 1:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game6.csv", ios_base::app); break;
		case 2:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game7.csv", ios_base::app); break;
		case 3:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game8.csv", ios_base::app); break;
		case 4:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game9.csv", ios_base::app); break;
		case 5:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game10.csv", ios_base::app); break;
		case 6:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game11.csv", ios_base::app); break;
		case 7:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game12.csv", ios_base::app); break;
		case 8:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game1.csv", ios_base::app); break;
		case 9:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game2.csv", ios_base::app); break;
		case 10:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game3.csv", ios_base::app); break;
		case 11:outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\game4.csv", ios_base::app); break;
		
		}
		//outfile.open("D:\\Desktop\\Algorithm\\project\\project instances\\result.txt", ios_base::app);

		//outfile << Times / 8 << " time:" << endl;
		//outfile << "The lowest cost:" << endl;
		/*outfile << TheBest.fitness << endl;
		outfile << "Unsigned Vessel: " << UnassignedVessel << " ";
		outfile << "Total Waiting Time: " << TotalWaitingTime << " ";
		outfile << "Last Departure Time: " << LastDepartureTime << endl;*/
		
		Fitness(TheBest, true);
		outfile << "," << UnassignedVessel << "," << TotalWaitingTime << "," << LastDepartureTime << "," << TheBest.fitness << ",";
		//outfile << "Locations of vessels:" << endl;
		sort(TheBest.Gene, TheBest.Gene + NumberOfBoat);
		for (int i = 0; i < NumberOfBoat; i++)
			outfile << "(" << boats[TheBest.Gene[i]].Location_x << " " << boats[TheBest.Gene[i]].Location_y << "),";
		//outfile << '\n';
		
		//outfile << "Running Time:" << endl;
		outfile << TotalTime << '\n';
		//outfile << "***************** End *****************" << endl;
		outfile.close();
		//MemoryRecovery();
	}

}