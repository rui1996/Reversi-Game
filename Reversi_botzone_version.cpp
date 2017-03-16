// 黑白棋botzone版本.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "jsoncpp/json.h" // C++编译时默认包含此库

using namespace std;
# define INT_MAX 2147483647
# define INT_MIN (-2147483647-1)

int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[8][8] = { 0 }; // 先x后y，记录棋盘状态
int blackPieceCount = 2, whitePieceCount = 2;
int direct[8][2] = { { 0,1 },{ 0,-1 },{ 1,0 },{ -1,0 },{ 1,1 },{ -1,1 },{ 1,-1 },{ -1,-1 } };	//方向
int turn[2] = { 1,-1 };	//表示走棋次序,对应黑棋与白棋
int turn_[2] = { 0, 0 };//表示走棋次序,对应玩家与AI
int changemax = 0;
int whitenum;
int blacknum;
int playercolor;//玩家的颜色
int ai_x, ai_y;//每次AI走的棋子
int depth = 4;//搜索深度（视为难度系数）()
int step = 1;//记录步数
int inputx, inputy;//输入x,y
int result;//结果
// 向Direction方向改动坐标，并返回是否越界
inline bool MoveStep(int &x, int &y, int Direction)
{
	if (Direction == 0 || Direction == 6 || Direction == 7)
		x++;
	if (Direction == 0 || Direction == 1 || Direction == 2)
		y++;
	if (Direction == 2 || Direction == 3 || Direction == 4)
		x--;
	if (Direction == 4 || Direction == 5 || Direction == 6)
		y--;
	if (x < 0 || x > 7 || y < 0 || y > 7)
		return false;
	return true;
}

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int xPos, int yPos, int color, bool checkOnly = false)
{
	int effectivePoints[8][2];
	int dir, x, y, currCount;
	bool isValidMove = false;
	if (gridInfo[xPos][yPos] != 0)
		return false;
	for (dir = 0; dir < 8; dir++)
	{
		x = xPos;
		y = yPos;
		currCount = 0;
		while (1)
		{
			if (!MoveStep(x, y, dir))
			{
				currCount = 0;
				break;
			}
			if (gridInfo[x][y] == -color)
			{
				currCount++;
				effectivePoints[currCount][0] = x;
				effectivePoints[currCount][1] = y;
			}
			else if (gridInfo[x][y] == 0)
			{
				currCount = 0;
				break;
			}
			else
			{
				break;
			}
		}
		if (currCount != 0)
		{
			isValidMove = true;
			if (checkOnly)
				return true;
			if (color == 1)
			{
				blackPieceCount += currCount;
				whitePieceCount -= currCount;
			}
			else
			{
				whitePieceCount += currCount;
				blackPieceCount -= currCount;
			}
			while (currCount > 0)
			{
				x = effectivePoints[currCount][0];
				y = effectivePoints[currCount][1];
				gridInfo[x][y] *= -1;
				currCount--;
			}
		}
	}
	if (isValidMove)
	{
		gridInfo[xPos][yPos] = color;
		if (color == 1)
			blackPieceCount++;
		else
			whitePieceCount++;
		return true;
	}
	else
		return false;
}

// 检查color方有无合法棋步
bool CheckIfHasValidMove(int color)
{
	int x, y;
	for (y = 0; y < 8; y++)
		for (x = 0; x < 8; x++)
			if (ProcStep(x, y, color, true))
				return true;
	return false;
}
void copy(int a[8][8], int b[8][8])
{
	int i, j;
	for (i = 0;i <= 7; i++)
		for (j = 0; j <= 7; j++)
			b[i][j] = a[i][j];
}

//吃子函数
void changecolor(int x, int y, int my_side)
{
	int i, j, k, x_, y_;	//i有计数的作用，x_, y_分别表示所检查点的坐标
	gridInfo[x][y] = my_side;	//将所下的点标记为我方子
	for (j = 0; j < 8; j++)	//借助direct数组向八个方向搜索
	{
		x_ = x + direct[j][1];
		y_ = y + direct[j][0];
		i = 1;
		while (x_>=0 && x_<8 && y_ >= 0 && y_ < 8)	//当不超过边界时
		{
			if (gridInfo[x_][y_] == 0)
				break;	//遇到空位置跳出
			else
				if (gridInfo[x_][y_] == my_side)	//如果找到我方的子
				{
					for (k = 1; k < i; k++)
						gridInfo[x + k * direct[j][1]][y + k * direct[j][0]] = my_side;	//翻转
					break;
				}
			i++;
			x_ = x_ + direct[j][1];	//检查下一点
			y_ = y_ + direct[j][0];
		}
	}
}
//判断能否落子函数
bool judge(int x, int y, int color)
{
	int i, j, x_, y_;
	if (x == -1 || y == -1 || x > 7 || y > 7)
		return 0;
	if (gridInfo[x][y] != 0)
		return 0;
	for (j = 0; j < 8; j++)	//借助direct数组向八个方向搜索
	{
		x_ = x + direct[j][1];
		y_ = y + direct[j][0];
		i = 1;
		while (x_ >= 0 && x_ < 8 && y_ >= 0 && y_<8)
		{
			if (gridInfo[x_][y_] == 0)
				break;
			else
				if (gridInfo[x_][y_] == color)
				{
					if (i > 1)
						return 1;	//如果遇到己方子并且中间至少有一个对方子，说明可以下
					break;	//遇到己方子跳出
				}
			i++;
			x_ = x_ + direct[j][1];	//检查下一点
			y_ = y_ + direct[j][0];
		}
	}
	return 0;
}
int sum(int my_side)
{
	int i, j, s = 0;	//s表示棋子数
	for (i = 0;i <= 7; i++)
		for (j = 0;j <= 7;j++)
		{
			if (gridInfo[i][j] == my_side)	//如果是我方棋子
				s++;
		}
	return s;
}
int canput(int my_side)
{
	int i, j, s = 0;	//s表示合法的可能棋步数量
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
			if (judge(j, i, my_side))
				s++;
	return s;
}

//判断胜负函数
int checkwin(int my_side)
{
	int mychess, otherschess;	//mychess, otherschess分别表示我方与对方的棋子数
	if (canput(1) == 0 && canput(-1) == 0)	//两方均无子可走
	{
		mychess = sum(my_side);	//计算双方棋子数
		otherschess = sum((-1)*my_side);
		if (mychess > otherschess)
			return 1;	//1表示胜
		else
			if (mychess < otherschess)
				return -1;	//-1表示负
			else
				return 0;	//0表示平
	}
	return 2;	//2表示未结束
}

//估值函数，考虑行动力和占角
int judgevalue(int my_side, int now_side)
{
	int mobility = now_side * my_side* canput(now_side);
	int	judge_value;	//mobility表示机动性，now_side * my_side用于调整符号
	judge_value = mobility;
	judge_value = judge_value + (gridInfo[0][0] * my_side + gridInfo[7][7] * my_side + gridInfo[0][7] * my_side + gridInfo[7][0] * my_side) * 30 - (gridInfo[1][1] * my_side + gridInfo[6][6] * my_side + gridInfo[1][6] * my_side + gridInfo[6][1] * my_side) * 20;//考虑角落的情况
	return judge_value;
}

//开局搜索
int chess_ai(int now_side, int now_depth, int alpha, int beta)	//now_depth指的是当前搜索树的深度，alpha是最大值层的下界，beta是最小值层的上界
{
	if (checkwin(now_side) == 1)	//如果赢了
	{
		int s = (now_depth % 2) * 2000000000 - 1000000000;
		return s;
	}
	if (checkwin(now_side) == -1)	//如果输了
	{
		int s = (1 - now_depth % 2) * 2000000000 - 1000000000;
		return s;
	}
	//如果当前是AI，返回10000000，如果是人返回-100000000（这是因为AI赢了是本人期望的，所以要返回一个很大的值，对方赢了就返回一个很小的）
	if (checkwin(now_side) == 0)	//如果平局
		return 0;
	if (now_depth == depth + 1)	//如果到达深度
	{
		if (now_depth % 2 == 1)
			return judgevalue(now_side, now_side);
		else
			return judgevalue(-now_side, now_side);
	}
	if (canput(now_side) == 0)
		return (chess_ai(-now_side, now_depth + 1, alpha, beta));	//如果当前轮次无棋可走
	int s;	//返回值
	int queue[64][3];
	int quelen = 0;	//节点排序队列
	for (int i = 0;i <= 7; i++)
	{
		for (int j = 0;j <= 7;j++)
		{
			if (judge(j, i, now_side))	//如果可以落子
			{
				int copygridInfo[8][8];	//备份棋局
				copy(gridInfo, copygridInfo);
				changecolor(j, i, now_side);
				queue[quelen][0] = judgevalue(-1, now_side);	//记录在该处落子的信息
				queue[quelen][1] = j;
				queue[quelen][2] = i;
				quelen++;
				copy(copygridInfo, gridInfo);
			}
		}
	}
	for (int i = 0;i < quelen - 1; i++)	//按照每个可以走的位置的估值进行排序，从而在接下来从估值比较大的开始试验
	{
		for (int j = 0; j < quelen - i - 1; j++)
		{
			if (queue[j][0] < queue[j + 1][0])
			{
				int temp;
				temp = queue[j][0];	//这一段用于交换信息
				queue[j][0] = queue[j + 1][0];
				queue[j + 1][0] = temp;
				temp = queue[j][1];
				queue[j][1] = queue[j + 1][1];
				queue[j + 1][1] = temp;
				temp = queue[j][2];
				queue[j][2] = queue[j + 1][2];
				queue[j + 1][2] = temp;
			}
		}
	}
	//这里之后就是每次从可以走的点里面拿出一个，递归的走下去，并且进行alpha-beta剪枝，
	//思路大致就是在当前状态已经不如已经走到的最好状态的时候，就不要再走下去了，直接返回。
	int putx, puty;//要走的点
	if (now_depth % 2)
	{
		for (int j = 0; j < quelen; j++)
		{
			int copygridInfo[8][8];	//备份棋局
			copy(gridInfo, copygridInfo);
			changecolor(queue[j][1], queue[j][2], now_side);	//翻转
			int s1;
			s1 = chess_ai(-now_side, now_depth + 1, alpha, beta);
			if (s1 > alpha)
			{
				alpha = s1;
				putx = queue[j][1];
				puty = queue[j][2];
			}
			if (s1 > beta)	//剪枝
			{
				return s1;
			}
			copy(copygridInfo, gridInfo);	//恢复棋局
		}
		s = alpha;
	}
	else
	{
		for (int j = quelen - 1; j >= 0; j--)
		{
			int copygridInfo[8][8];	//备份棋局
			copy(gridInfo, copygridInfo);
			changecolor(queue[j][1], queue[j][2], now_side);	//翻转
			int s2;
			s2 = chess_ai(-now_side, now_depth + 1, alpha, beta);
			if (s2 < beta)	//剪枝
			{
				beta = s2;
				putx = queue[j][1];
				puty = queue[j][2];
			}
			if (s2 < alpha)	//剪枝
			{
				return s2;
			};
			copy(copygridInfo, gridInfo);	//恢复棋局
		}
		s = beta;
	}
	if (now_depth == 1)
	{
		ai_x = putx;
		ai_y = puty;
	}
	return s;
}

//残局搜索，后8步时采取比赛结束时赢子最多的估值方法，并搜索到游戏结束为止
int final(int now_side, int now_depth, int alpha, int beta)
{
	if (checkwin(now_side) != 2)	//如果结束
	{
		if (depth % 2 == 1)
			return sum(now_side) - sum(-now_side);
		else
			return sum(-now_side) - sum(now_side);
	}
	else
		if (canput(now_side) == 0)
			return (final(-now_side, now_depth + 1, alpha, beta));
		else
		{
			int i, j;
			int putx, puty;	//要走的步
			int s;
			for (i = 0;i < 8; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (judge(j, i, now_side))	//如果可下
					{
						int copygridInfo[8][8];	//备份棋局
						copy(gridInfo, copygridInfo);
						changecolor(j, i, now_side);	//翻转
						int s1, s2;
						switch (now_depth % 2)
						{
						case 1:
							s1 = final(-now_side, now_depth + 1, alpha, beta);
							if (s1 > alpha)
							{
								alpha = s1;
								putx = j;
								puty = i;
							}
							if (s1 > beta)	//剪枝
							{
								return s1;
							}
							break;
						case 0:
							s2 = final(-now_side, now_depth + 1, alpha, beta);
							if (s2 < beta)
							{
								beta = s2;
								putx = j;
								puty = i;
							}
							if (s2 < alpha)	//剪枝
							{
								return s2;
							}
							break;
						}
						copy(copygridInfo, gridInfo);	//恢复棋局
					}
				}
			}
			if (now_depth % 2)
				s = alpha;
			else
				s = beta;
			if (now_depth == 1)
			{
				ai_x = putx;
				ai_y = puty;
			}
			return s;
		}
}

int main()
{
	int x, y;

	// 初始化棋盘
	gridInfo[3][4] = gridInfo[4][3] = 1;  //|白|黑|
	gridInfo[3][3] = gridInfo[4][4] = -1; //|黑|白|

										  // 读入JSON
	string str;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);

	// 分析自己收到的输入和自己过往的输出，并恢复状态
	int turnID = input["responses"].size();
	currBotColor = input["requests"][(Json::Value::UInt) 0]["x"].asInt() < 0 ? 1 : -1; // 第一回合收到坐标是-1, -1，说明我是黑方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合
		x = input["requests"][i]["x"].asInt();
		y = input["requests"][i]["y"].asInt();
		if (x >= 0)
			ProcStep(x, y, -currBotColor); // 模拟对方落子
		x = input["responses"][i]["x"].asInt();
		y = input["responses"][i]["y"].asInt();
		if (x >= 0)
			ProcStep(x, y, currBotColor); // 模拟己方落子
	}

	// 看看自己本回合输入
	x = input["requests"][turnID]["x"].asInt();
	y = input["requests"][turnID]["y"].asInt();
	if (x >= 0)
		ProcStep(x, y, -currBotColor); // 模拟对方落子

									   // 找出合法落子点
	int possiblePos[64][2], posCount = 0, choice;

	for (y = 0; y < 8; y++)
		for (x = 0; x < 8; x++)
			if (ProcStep(x, y, currBotColor, true))
			{
				possiblePos[posCount][0] = x;
				possiblePos[posCount++][1] = y;
			}

	// 做出决策（你只需修改以下部分）

	int resultX, resultY;
	int cnt = 0;
	for (int i = 0;i <= 7;i++)
		for (int j = 0;j <= 7;j++)
			if (gridInfo[i][j] == 1 || gridInfo[i][j] == (-1))
				cnt++;
	if (cnt <= 56)
	{
		if (canput(currBotColor) > 0)
		{
			chess_ai(currBotColor, 1, INT_MIN, INT_MAX);	//开局搜索
			resultX = ai_x;
			resultY = ai_y;
		}
		else
		{
			resultX = -1;
			resultY = -1;
		}
	}
	else
	{
			if (canput(currBotColor) > 0)
			{
				final(currBotColor, 1, INT_MIN, INT_MAX);	//开局搜索
				resultX = ai_x;
				resultY = ai_y;
			}
			else
			{
				resultX = -1;
				resultY = -1;
			}
	}


	// 决策结束，输出结果（你只需修改以上部分）

	Json::Value ret;
	ret["response"]["x"] = resultX;
	ret["response"]["y"] = resultY;
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;
	return 0;
}
