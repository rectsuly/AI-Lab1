#include<iostream>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<cmath>

using namespace std;

double origialTemperature = 50;
int tries_count = 300;
double coldRate = 0.8;
double temperature;
 
inline void swap(int* a, int* b)
{
	*a ^= *b ^= *a ^= *b;
}	

void generate_init_state(int *queen, int N)
{
	for(int i=1; i<=N; i++)
		queen[i] = i;
	
	for(int i=1; i<=N; i++)
	{
		int t = rand()%N + 1;
		swap(queen[t], queen[N+1-t]);
	}
}

//启发式函数，产生冲突的皇后对数 
inline int evaluate(int* queen, int N, vector<int>& collisionList)
{
	collisionList.clear();
	int collision = 0;
	for(int i=1; i<N; i++)
	{
		for(int j=i+1; j<=N; j++)
		{
			if(abs(queen[i]-queen[j]) == j-i)
			{
				collisionList.push_back(j);
				collision++;
			}
		}
	}
	return collision;
}

void hill_climbing(int* queen, int N)
{
	int max_tries = N * 1000;
	int max_steps = N * 1000;
	int tries = 0, steps = 0;
	int collision;
	int temp_collision,new_collision,a = 1,b = 1;
	temperature = origialTemperature;
	vector <int> collisionList;
	while(tries++ < max_tries)
	{
		
		srand((unsigned)time(NULL));
		generate_init_state(queen,N);
		collision = evaluate(queen,N,collisionList);
		while((collision != 0) && (steps++ < max_steps))
		{
			temp_collision = 2147483647;
		/*	collision = next_step(queen,N,collisionList,temperature);
			steps_count++;
			if(steps_count >= triesPT)
			{
				steps_count = 0;
				temperature *= coldRate;
			}*/
			for(int i=1; i<=N; i++)
			{
				for(int j=1; j<=N; j++)
				{
					if(j != i)
					{
						swap(queen[i],queen[j]);
						new_collision = evaluate(queen,N,collisionList);
						if(new_collision < temp_collision)
						{
							temp_collision = new_collision, a = i, b = j;
						}
					}
					swap(queen[i],queen[j]);
				}
			}
			if(--tries_count==0)
			{
				tries_count = 300;
				temperature *= 0.8;
			}
			if(!(temperature < 0.0000000001) && (temp_collision < collision || ((double)(rand() % 1000)/100.0)<exp(temp_collision/temperature)))
				swap(queen[a],queen[b]), collision = temp_collision;
			else
				return;
		}
	//	cout << "tries:" << tries << " " << "steps:" << steps << endl;		
		if(collision == 0)
		{
			if(N%2 == 1)
			{
				if(queen[(N+1)/2] == (N+1)/2)	//存在中心点解 
					continue;
				else
				{
					int flag = 1;
					for(int i=1; i<=N/2; i++)	
					{
						if((queen[i] + queen[N+1-i]) != (N+1))
							flag = 0;
					}
					if(flag == 1)				//存在关于中心点对称解 
						continue;
				} 	
					
			}
			
//			cout << "tries:" << tries << " " << "steps:" << steps << endl;	
//			freopen("output_hill_climbing.txt", "w", stdout);
//			cout << "tries:" << tries << " " << "steps:" << steps << endl;
//			freopen("CON","w",stdout);
			return ;
		}
	}
	

	

}
 
int main()
{
	int N = 1000;
	freopen("input.txt", "r", stdin);
	cin >> N;
	if(N <=3){ 
		cout << "No solution!" << endl;
		return 0;
	}
	int* queen = new int[N+1];
	
	clock_t start, finish;
	double totaltime;
	start = clock();
	
	hill_climbing(queen,N);
	
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	
	for(int i=1; i<=N; i++)
		cout << queen[i] << endl;
	if(N%2 == 0){					//若N为偶数，黑皇后的解可通过白皇后的解中心线对称得到 
		for(int i=N; i>=1; i--)
			cout << queen[i] << endl; 
	} 
	else
	{
		for(int i=1; i<=N; i++)
			cout << N+1 - queen[N+1-i] << endl;		//若N为奇数，黑皇后的解可通过白皇后的解中心点对称得到 
	}
	cout << totaltime << endl;
	freopen("output_hill_climbing.txt", "a", stdout);
	for(int i=1; i<=N; i++)
		cout << queen[i] << endl;
	if(N%2 == 0){					//若N为偶数，黑皇后的解可通过白皇后的解中心线对称得到 
		for(int i=N; i>=1; i--)
			cout << queen[i] << endl; 
	} 
	else
	{
		for(int i=1; i<=N; i++)
			cout << N+1 - queen[N+1-i] << endl;		//若N为奇数，黑皇后的解可通过白皇后的解中心点对称得到 
	}
	cout << totaltime << endl;
	fclose(stdout);
		
	return 0;
}
