#include <bits/stdc++.h>

using namespace std;

struct Boat
{
	int ArrivalTime;
	int ServiceTime;
	int BerthsOccupied;
	int Location_x;
	int Location_y;
	Boat()
	{
		Location_x = -1;
		Location_y = -1;
	}
};

const int Max=105;
Boat boats[Max];
int number[3*Max];
int TotalServiceTime=0,LargestWidth=0,NumberOfBoat=0;

bool ReadFile(string FileName)
{
	string DataFromFile;
	ifstream infile;
	memset(number, 0, sizeof(number));
	infile.open(FileName,ios::in);

	if (!infile)
	{
		infile.close();
		return false;
	}
	int index=0;
	while(!infile.eof())
	{
		getline(infile,DataFromFile,'\n');
		
		for(unsigned int i=0;i<DataFromFile.size();i++)
		{
			while(DataFromFile[i]!=' ' && DataFromFile[i]!='\n' && i<DataFromFile.size())
			{
				number[index]=number[index]*10+DataFromFile[i]-'0';
				i++;
			}
			index++;
		}
	}

	TotalServiceTime=number[0];
	LargestWidth=number[1];
	NumberOfBoat=number[2];
	int j=0;
	for(int i=3;i<index;i+=3)
	{
		boats[j].ArrivalTime=number[i];
		boats[j].ServiceTime=number[i+1];
		boats[j].BerthsOccupied=number[i+2];
		j++;
	}
	infile.close();
	return true;
}

/***********************************************************************************************************************/

int Map[Max][Max];

int main(){
	string FileName;
	while(cin>>FileName)
	{
		int totalWaitingTime = 0;
		int lastDepartureTime = 0;
		memset(Map,0,sizeof(Map));
		if(!ReadFile(FileName))
		{
			cout << "Not such file!" << endl;
			continue;
		}
		cout<<TotalServiceTime<<" "<<LargestWidth<<" "<<NumberOfBoat<<endl;
//		sort(boats, boats+NumberOfBoat, cmp);
		cout << "*********************************************" << endl;
		for(int i=0; i<NumberOfBoat; i++)
		{
			cout << boats[i].ArrivalTime << " " << boats[i].ServiceTime << " " << boats[i].BerthsOccupied << endl;
		}
		cout << "*********************************************" << endl;


		for(int i=0; i<LargestWidth; i++)
		{
			for(int j=0; j<TotalServiceTime; j++)
			{
				Map[i][j] = -1;
			}
		}
		for(int i=0; i<NumberOfBoat; i++)
		{
			bool find = false;
			for(int j=boats[i].ArrivalTime; j<TotalServiceTime; j++)
			{
				for(int k=0; k<LargestWidth; k++)
				{
					if(j+boats[i].ServiceTime<=TotalServiceTime && k+boats[i].BerthsOccupied<=LargestWidth)
					{
						int x = k, y = j;
						bool flag = true;
						for(y=j; y<j+boats[i].ServiceTime; y++)
						{
							for(x=k; x<k+boats[i].BerthsOccupied; x++)
							{
								if(Map[x][y]!=-1)
								{
									flag = false;
									break;
								}
							}
							if(flag==false) break;
						}
						if(flag==true) find = true;
						if(find==true)
						{
				 			boats[i].Location_x = k;
							boats[i].Location_y = j;
							if(lastDepartureTime<j+boats[i].ServiceTime) lastDepartureTime = j+boats[i].ServiceTime;
							totalWaitingTime += j-boats[i].ArrivalTime;
							for(x=k; x<k+boats[i].BerthsOccupied; x++)
							{
								for(y=j; y<j+boats[i].ServiceTime; y++)
								{
									Map[x][y] = i;
								}
							}
							for(int a=0; a<LargestWidth; a++)
							{
								for(int b=0; b<TotalServiceTime; b++)
								{
									cout <<setw(2)<< Map[a][b] << " ";
								}
								cout << endl;
							}
							cout << "*********************************************" <<endl;
							break;
						}
					}
					if(find==true) break;
				}
				if(find==true) break;
			}
		}
		int unassignedVessel = 0;
		int totalCost = 0;
		for(int i=0; i<NumberOfBoat; i++)
		{
			if(boats[i].Location_x == -1) unassignedVessel++;
		} 
		for(int i=0; i<NumberOfBoat; i++)
		{
			cout << boats[i].Location_x << "," << boats[i].Location_y << endl;
		}
		cout << "************************************************" << endl;
		cout << totalWaitingTime << " " << lastDepartureTime << endl;
		totalCost = unassignedVessel*100 + 2*totalWaitingTime + lastDepartureTime;
		cout << "total cost: " << totalCost << endl;
	}
	return 0;
}