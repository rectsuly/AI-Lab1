#include<iostream>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<iomanip>

using namespace std;
 
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

//����ʽ������������ͻ�Ļʺ���� 
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

//���Խ������������������õ���������ֵ�Ⱦ�ֵС���򽻻���������״̬�����򷵻�ԭ��״̬ 
int try_swap(int* queen, int N, int a, int b, vector<int>& collisionList)
{
	vector<int> temp;
	int old_collision = evaluate(queen,N,collisionList);
	swap(queen[a],queen[b]);
	int new_collision = evaluate(queen,N,collisionList);
	if(new_collision > old_collision) 	//��ֵ�Ⱦ�ֵС 
		swap(queen[b],queen[a]);						//ȡ������
	else
		collisionList = temp;
	return new_collision;
}

//Ѱ����һ������ֵ���ŷ��� 
int next_step(int* queen, int N, vector<int>& collisionList)
{
	int old_collision = evaluate(queen,N, collisionList);
	int new_collision = old_collision;
//	int a,b;
	//ֻʣ���һ����ͻʱ 
	if(collisionList.size() == 1)
	{
		for(int i=1; i<=N; i++)
			if(i!=collisionList[0] && (try_swap(queen,N,i,collisionList[0],collisionList)==0))
				return 0;
	}
/*	do{
		srand((unsigned)time(NULL));
		a = rand()%N + 1;
		b = rand()%N + 1;
		while(b == a)
			b = rand()%N + 1;
		new_collision = try_swap(queen,N,a,b,collisionList);
	}while(new_collision > old_collision);*/
	for(int i=1; i<N; i++)
	{
	//	for(int j=i+1; j<=N; j++)
	//	{	
			int j = rand()%N + 1;
			while(j == i)
				j = rand()%N + 1;
			new_collision = try_swap(queen,N,i,j,collisionList);
			if(new_collision < old_collision)
				return new_collision;
	//	}
	}
	return new_collision;
}

void hill_climbing(int* queen, int N)
{
	int max_tries = N * 10000;
	int max_steps = N * 10000;
	int tries = 0, steps = 0;
	int collision;
	vector <int> collisionList;
	while(tries++ < max_tries)
	{
		srand((unsigned)time(NULL));
		generate_init_state(queen,N);
		collision = evaluate(queen,N,collisionList);
		while((collision != 0) && (steps++ < max_steps))
			collision = next_step(queen,N,collisionList);
		if(collision == 0)
		{
			if(N%2 == 1)
			{
				if(queen[(N+1)/2] == (N+1)/2)	//�������ĵ�� 
					continue;
				else
				{
					int flag = 1;
					for(int i=1; i<=N/2; i++)	
					{
						if((queen[i] + queen[N+1-i]) != (N+1))
							flag = 0;
					}
					if(flag == 1)				//���ڹ������ĵ�Գƽ� 
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
	fclose(stdin);
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
	if(N%2 == 0){					//��NΪż�����ڻʺ�Ľ��ͨ���׻ʺ�Ľ������߶ԳƵõ� 
		for(int i=N; i>=1; i--)
			cout << queen[i] << endl; 
	} 
	else
	{
		for(int i=1; i<=N; i++)
			cout << N+1 - queen[N+1-i] << endl;		//��NΪ�������ڻʺ�Ľ��ͨ���׻ʺ�Ľ����ĵ�ԳƵõ� 
	}
	cout << totaltime << endl;
	freopen("output_hill_climbing.txt", "w", stdout);
	for(int i=1; i<=N; i++)
		cout << queen[i] << endl;
	if(N%2 == 0){					//��NΪż�����ڻʺ�Ľ��ͨ���׻ʺ�Ľ������߶ԳƵõ� 
		for(int i=N; i>=1; i--)
			cout << queen[i] << endl; 
	} 
	else
	{
		for(int i=1; i<=N; i++)
			cout << N+1 - queen[N+1-i] << endl;		//��NΪ�������ڻʺ�Ľ��ͨ���׻ʺ�Ľ����ĵ�ԳƵõ� 
	}
	cout << setprecision(8) << totaltime << endl;
	fclose(stdout);
		
	return 0;
}
