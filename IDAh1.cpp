#include<iostream>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cstring>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define FORWARD 4
#define BACK 5
#define BARRIER -1
#define ZERO 0
using namespace std;

typedef struct Node
{
	int status[28];
	int zero[3];
	int f;
	int g;
	int h;
	string path;
	string path1;
}Node;

vector <Node> Open;

void print(Node cur_stat)
{
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{	
			for(int k=0; k<3; k++)
				cout << (int)cur_stat.status[i*9 + j*3 + k] << ' ';
			cout << endl;
		}
		cout << endl;
	} 
}

int h1(Node cur_stat, Node target)
{
	int count = 0;
	for(int i=0; i<27; i++)
		if(cur_stat.status[i] != target.status[i])
			count++;
	return count;
}
//生成后继结点 
void generateChild(Node& cur_stat, Node target)
{
	Node node;
	for(int i=0; i<27; i++)
	{
		node.status[i] = cur_stat.status[i];
	}
	for(int i=0; i<3; i++)
		node.zero[i] = cur_stat.zero[i];
	node.g = cur_stat.g + 1;
	node.h= h1(cur_stat,target);
	node.f = node.g + node.h;
	node.path = cur_stat.path1;
	Open.push_back(node);
}
//下一步状态 
void move_state(Node& p, int dir)
{
	int z = p.zero[0];	//最外层 
	int y = p.zero[1];
	int x = p.zero[2];	//最内层 
	switch(dir)
	{
		case DOWN:
				p.status[z*9 + y*3 + x] = p.status[z*9 + (y-1)*3 + x];
				p.status[z*9 + (y-1)*3 + x] = ZERO;
				p.zero[1] = y-1;
			break;
		case UP:
				p.status[z*9 + y*3 + x] = p.status[z*9 + (y+1)*3 + x];
				p.status[z*9 + (y+1)*3 + x] = ZERO;
				p.zero[1] = y+1;
			break;
		case LEFT:
				p.status[z*9 + y*3 + x] = p.status[(z-1)*9 + y*3 + x];
				p.status[(z-1)*9 + y*3 + x] = ZERO;
				p.zero[0] = z-1;
			break;
		case RIGHT:
				p.status[z*9 + y*3 + x] = p.status[(z+1)*9 + y*3 + x];
				p.status[(z+1)*9 + y*3 + x] = ZERO;
				p.zero[0] = z+1;
			break;
		case FORWARD:
				p.status[z*9 + y*3 + x] = p.status[z*9 + y*3 + x-1];
				p.status[z*9 + y*3 + x-1] = ZERO;
				p.zero[2] = x-1;
			break;
		case BACK:
				p.status[z*9 + y*3 + x] = p.status[z*9 + y*3 + x+1];
				p.status[z*9 + y*3 + x+1] = ZERO;
				p.zero[2] = x+1;
			break;
		default:
			break;
	}
}

bool comp(Node a, Node b) {
	return (a.f) > (b.f);
}
//主算法 
void IDA_h1(Node& source, Node target)
{
	int x,y,z;
	int limit = source.f;
	int next_limit;
	Open.clear();
//	Open.push_back(source);
	while(limit < 100)
	{
		next_limit = 100;
		Open.push_back(source);
		while(!Open.empty())
		{
			source = Open.back();
			Open.pop_back();
			source.f = source.h + source.g;
			if(source.h == 0)
				return;
			else if((source.g+source.h) > limit)
				next_limit = min(next_limit,(source.g+source.h));
			else
			{
				z = source.zero[0], y = source.zero[1], x = source.zero[2];
				if(y<2 && source.status[z*9 + (y+1)*3 + x] != BARRIER)
				{
					move_state(source, UP);
					source.path1 = source.path + "U";
					generateChild(source, target);
					move_state(source, DOWN);
				}
				if(y>0 && source.status[z*9 + (y-1)*3 + x] != BARRIER)
				{
					move_state(source,DOWN);
					source.path1 = source.path + "D";
					generateChild(source, target);
					move_state(source,UP);
				}
				if(z>0 && source.status[(z-1)*9 + y*3 + x] != BARRIER)	//LEFT
				{
					move_state(source,LEFT);
					source.path1 = source.path + "L";
					generateChild(source, target);
					move_state(source,RIGHT);
				}
				if(z<2 && source.status[(z+1)*9 + y*3 + x] != BARRIER)	//RIGHT
				{
					move_state(source,RIGHT);
					source.path1 = source.path + "R";
					generateChild(source, target);
					move_state(source,LEFT);
				}
				if(x>0 && source.status[z*9 + y*3 + x - 1] != BARRIER)	//FORWARD
				{
					move_state(source,FORWARD);
					source.path1 = source.path + "F";
					generateChild(source, target);
					move_state(source, BACK);
				}
				if(x<2 && source.status[(z)*9 + y*3 + x+1] != BARRIER)
				{
					move_state(source,BACK);
					source.path1 = source.path + "B";
					generateChild(source, target);
					move_state(source, FORWARD);
				}
			}
		}
		limit = next_limit;
	}
	cout << "No solution!" << endl; 
}

int main()
{
	
	ifstream fin("source.txt");
	ifstream fout("target.txt");
	string buff;
	int col1,col2,col3;
	int dim1 = 0, dim2 = 0;
	Node source;
	Node target;
	
	while(getline(fin, buff))
	{
		if(buff != "" && buff != "\r" && buff != "\n")
		{
			istringstream str(buff);
			str >> col1 >> col2 >> col3;
	
			source . status[dim2*9 + dim1*3 + 0] = col1;
			source . status[dim2*9 + dim1*3 + 1] = col2;
			source . status[dim2*9 + dim1*3 + 2] = col3;
			dim1++;
		}
		else
		{
			dim1 = 0;
			dim2++;
		}
	}
	dim1 = dim2 =0;
	while(getline(fout, buff))
	{
		if(buff != "" && buff != "\r" && buff != "\n")
		{
			istringstream str(buff);
			str >> col1 >> col2 >> col3;
	
			target . status[dim2*9 + dim1*3 + 0] = col1;
			target . status[dim2*9 + dim1*3 + 1] = col2;
			target . status[dim2*9 + dim1*3 + 2] = col3;
			dim1++;
		}
		else
		{
			dim1 = 0;
			dim2++;
		}
	}
//	print(source);
//	print(target);
	//find the zero position
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
			{
				if(source.status[i*9 + j*3 +k] == 0)
				{
					source.zero[0] = i;
					source.zero[1] = j;
					source.zero[2] = k;
				}
			}
	source.g = 0;
	source.h = h1(source, target);
	source.f = source.g + source.h;
	source.path = "";
	source.path1 = "";
	clock_t start, finish;
	double totaltime;
	start = clock();
	//program
	IDA_h1(source, target);
	finish = clock();
	totaltime = (double)(finish - start)/CLOCKS_PER_SEC;
	 
	cout << totaltime << endl;
	cout << source.path << endl;
	
	freopen("output_IDAh1.txt","w",stdout);
	cout << totaltime << endl;
	cout << source.path << endl;
	cout << Open.size() << endl;
 	return 0;
}
