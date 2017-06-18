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
	string path2;
}Node;

vector <Node> Open;
int target_pos[28];

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

int h2(Node cur_stat, Node target)
{
	int count = 0;
	int num = 0;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			for(int k=0; k<3; k++)
			{
				num = cur_stat.status[i*9 +j*3 +k];
				if(num != BARRIER && num != ZERO)
					count += abs(target_pos[num] - (i+j+k));
			}
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
	node.h= h2(cur_stat,target);
	node.f = node.g + node.h;
	node.path = cur_stat.path2;
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
void A_star_h2(Node& source, Node target)
{
	int x,y,z;
	Open.clear();
//	Open.push_back(source);
	while(source.h != 0)
	{
	//	cout << "source.h = " << source.h << endl;
		z = source.zero[0], y = source.zero[1], x = source.zero[2];
	//	cout << "x=" << x << " y=" << y << " z=" << z << endl;
		if(y<2 && source.status[z*9 + (y+1)*3 + x] != BARRIER)
		{
			move_state(source, UP);
			source.path2 = source.path + "U";
			generateChild(source, target);
			move_state(source, DOWN);
		}
		if(y>0 && source.status[z*9 + (y-1)*3 + x] != BARRIER)
		{
			move_state(source,DOWN);
			source.path2 = source.path + "D";
			generateChild(source, target);
			move_state(source,UP);
		}
		if(z>0 && source.status[(z-1)*9 + y*3 + x] != BARRIER)	//LEFT
		{
			move_state(source,LEFT);
			source.path2 = source.path + "L";
			generateChild(source, target);
			move_state(source,RIGHT);
		}
		if(z<2 && source.status[(z+1)*9 + y*3 + x] != BARRIER)	//RIGHT
		{
			move_state(source,RIGHT);
			source.path2 = source.path + "R";
			generateChild(source, target);
			move_state(source,LEFT);
		}
		if(x>0 && source.status[z*9 + y*3 + x - 1] != BARRIER)	//FORWARD
		{
			move_state(source,FORWARD);
			source.path2 = source.path + "F";
			generateChild(source, target);
			move_state(source, BACK);
		}
		if(x<2 && source.status[(z)*9 + y*3 + x+1] != BARRIER)
		{
			move_state(source,BACK);
			source.path2 = source.path + "B";
			generateChild(source, target);
			move_state(source, FORWARD);
		}
		sort(Open.begin(), Open.end(), &comp);
		
		source = Open.back();
		Open.pop_back();
//		cout << Open.size() << endl;
//		cout << source.g << endl;
//		cout << source.path2 << endl;
	}
//	for(int j=0; j<Open.size(); j++)
//		{
//			getChild(source);
//			cout <<"h=" << source.h << " g=" << source.g <<" path=" << source.path << endl;
//		}
	
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
				if(target.status[i*9 +j*3 +k] != BARRIER && target.status[i*9 + j*3 +k] != ZERO)
					target_pos[target.status[i*9 + j*3 +k]] = i + j + k;
			}
	source.g = 0;
	source.h = h2(source, target);
	source.f = source.g + source.f;
	source.path = "";
	source.path2 = "";
	clock_t start, finish;
	double totaltime;
	start = clock();
	//program
	A_star_h2(source, target);
	finish = clock();
	totaltime = (double)(finish - start)/CLOCKS_PER_SEC;
	 
	cout << totaltime << endl;
	cout << source.path << endl;
	
	freopen("output_Ah2.txt","w",stdout);
	cout << totaltime << endl;
	cout << source.path << endl;
	
	return 0;
}
