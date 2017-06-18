# AI-Lab1
Including cubic-digit problem and  2N-queen problem

27立方数码问题说明文档

一.实验题目

本问题包括一个3x3x3的立方体，24个写有数字(1-24)的单位立方体以及一个空位（由0表示），两个障碍位（涂黑的部分，可以用-1表示）。与空位上、下、左、右、前、后相邻的棋子可以滑动到空位中，任何棋子都不能移动到障碍位中，且障碍位不可移动。游戏的目的是要达到一个特定的目标状态。

二.问题表示

本次作业中，状态由一个3维矩阵表示，0表示空位置，1-24表示棋子，-1表示障碍物，本次作业障碍物的位置不固定，初始状态和目标状态都通过文件输入。

定义空格棋子0的6个动作， U代表up，即对棋子上移，D代表down，即对棋子下移，L代表left，即对棋子左移，R代表right，即对棋子右移，F代表forward，即对棋子前移（靠近自己的方向），B代表back，即对棋子后移（远离自己的方向）。所有动作均要合法。

本作业中，需要读取初始状态及目标状态，并实现两个求解八数码问题的算法：A搜索及迭代深入A搜索(IDAS)，使用以下两种启发函数：

	h1(n) = number of misplaced tiles（错位的棋子数） 

	h2(n) = total Manhattan distance（所有棋子到其目标位置的三个方向曼哈顿距离和） 

最后输出从初始状态到目标状态的动作序列，例如图2的初始状态到达图1的目标状态的移动序列是BRRB。

三.实验要求

使用C/C++实现4个算法，即，使用启发函数h1(n)的A*算法：Ah1.cpp，使用启发函数h2(n)的A*算法:Ah2.cpp，使用启发函数h1(n)的IDA*算法:IDAh1.cpp，使用启发函数h2(n)的IDA*算法:IDAh2.cpp。输出文件名与算法对应。以h1为启发函数的A*算法输出到output_Ah1.txt；以h2为启发函数的A*算法输出到output_Ah2.txt；以h1为启发函数的IDA*算法输出到output_IDAh1.txt；以h2为启发函数的IDA*算法输出到output_IDAh2.txt。

四.实验环境

操作系统：Windows 7 64位；

IDE： Dev-C++ 5.11

编译器： 

TMD-GCC 4.9.2 32-bit Profiling, MinGW64\bin\g++.exe

五.实验设计

1.算法主要代码

结构体：

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

求解h1启发式函数：

    int h1(Node cur_stat, Node target)
    {
    	int count = 0;
    	for(int i=0; i<27; i++)
    		if(cur_stat.status[i] != target.status[i])
    			count++;
    	return count;
    }

求解h2启发式函数：

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

生成后继节点：

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

下一步状态：

    /下一步状态 
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

2.A*算法主体部分思路：

当h值不为0时，循环进行下一步操作，即上下左右进行移动，然后恢复，恢复前结点进向量vector中，最后对向量中所有结点进行排序，然后取f值最小的继续循环。

3.IDAh算法主体部分思路：

首先source入向量vector，当向量vector不为空时进行循环，每次迭代，截断值是超过上一次迭代阶段值的节点中最小的f耗散值。

4.复杂度分析

A*h1和A*h2算法：

设最大深度为m，则空间复杂度为O(6^m)，时间复杂度为O(mN^3)；

IDA*h1和IDA*h2算法：

设最大深度为m，则空间复杂度为O(6m)，时间复杂度为O(mN^3).

六.实验结果

对于前20个测试样例均能够正常跑出结果，且速度大致为A*h1<A*h2<IDA*h1<IDA*h2.

2N皇后问题说明文档

一.实验题目

N*N的棋盘上摆放N个黑皇后和N个白皇后（共2N个），使任意两个同色的皇后不能相互攻击，即任意两个黑皇后都不能处于同一行、同一列或同一斜线上，任意两个黑皇后都不能处于同一行、同一列或同一斜线上。如上图为N=4时一种合理摆法（B表示黑皇后，W表示白皇后）。

二.实验难点

这个问题的难点在于，时间复杂度随着问题规模增加是指数型增长的，高效解决这个问题是本作业的重点。同时相比“N皇后”问题，“2N皇后”的难点在于，需要在N*N的棋盘中放置2N个棋子，而且仍要满足同色之间不能相互攻击。

三.实验要求

N个黑皇后和N个白皇后同时存在于棋盘上，同色之间不能相互攻击。

四.实验环境

操作系统：Windows 7 64位；

IDE： Dev-C++ 5.11

编译器： 

TMD-GCC 4.9.2 32-bit Profiling, MinGW64\bin\g++.exe

五.实验设计

1.算法实现

本次实验实现了随机重启爬山算法和模拟退火算法。

2.题目分析

仔细分析题目，易知，该2N皇后问题实际上是N皇后问题的简单变形，不需要在算法中真正求出2N个皇后的搜索状态，而是先求出N个白皇后的解，再利用对称的思想得到另外N个黑皇后的解。也就是说，当N为偶数时，黑皇后的解可根据白皇后的解中轴线对称得到；当N是奇数时，黑皇后的解可根据白皇后的解中心对称得到（必须去除中心点解和关于重心点对称的解）。这样一来，问题就简化为了求解N皇后的解，大大减少了问题的难度。

3.爬山算法思想

先生成随机状态queen[i] = i；

再求评价函数evaluate()：产生冲突的皇后对数； 

尝试交换函数try_swap()：如果交换后得到的新评价值比旧值小，则交换并返回新状态，否则返回原先状态 ；

下一步行动函数next_step()：寻找下一步评价值更优方案 ；

爬山算法函数hill_climbing()：初始状态行列均不冲突， 后继函数为交换两行。后继爬山行动为使用遍历加随机生成的方法，遇到局部最优解非全局最优解， 重新随机状态开始爬山。

空间复杂度分析：一维数组存储第i行的皇后位置queen[i]，冲突队列vector<int> collisionList，所需空间为O(N);

时间复杂度分析：搜索后继节点时，采用的是遍历加随机生成的方法，故时间复杂度为O(N)。

4.模拟退火算法思想

在爬山算法的基础上，对主要代码区域稍作修改即可得到模拟退火算法。首先设定退火状态参数， 在搜索遇到局部最优解非全局最优解的时候， 根据温度和 value 变化值设定概率接收“ 走一条较坏的路” ,调整参数值(初始温度， 退火速度， 每温度运行步数)可以实现1概率求解， 并且得到较好性能。 

空间复杂度：同爬山算法。

时间复杂度：同爬山算法。

六.实验结果

N=50：

爬山：0.013s  , 模拟退火：0.241s;

N=100:

爬山：0.09s , 模拟退火：8.109s;

N=200:

爬山：0.862s, 模拟退火：201.589s.


