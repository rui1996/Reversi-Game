// 黑白棋 大作业.cpp : 定义控制台应用程序的入口点。
//作者： 钱瑞 1500012777

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <windows.h>
using namespace std;

//以下为全局变量初始化部分
int chessboard[10][10];  //棋盘 0表示未填 1表示黑色 -1表示白色
int direct[8][2] = { { 0,1 },{ 0,-1 },{ 1,0 },{ -1,0 },{ 1,1 },{ -1,1 },{ 1,-1 },{ -1,-1 } };	//方向
int turn[2] = { 1,-1 };	//表示走棋次序,对应黑棋与白棋
int turn_[2] = { 0, 0 };//表示走棋次序,对应玩家与AI
int changemax = 0;
int whitenum;
int blacknum;
int playercolor;//玩家的颜色
int ai_x, ai_y;//每次AI走的棋子
int depth = 3;//搜索深度（视为难度系数）()
int step = 1;//记录步数
int inputx, inputy;//输入x,y
int result;//结果

//打印棋盘函数
void printchessboard()
{
	cout << "    1   2   3   4   5   6   7   8" << endl;
	cout << "  ┌─┬─┬─┬─┬─┬─┬─┬─┐" << endl;
	for (int i = 1; i <= 8; i++) {
		cout << ' ' << i << "│";
		for (int j = 1; j <= 8;j++) {
			switch (chessboard[i][j]) {
			case 0:
				cout << "  ";
				break;
			case 1:
				cout << "○";
				break;
			case -1:
				cout << "●";
				break;
			}
			cout << "│";
		}
		if (i == 8)
			cout << endl << "  └─┴─┴─┴─┴─┴─┴─┴─┘" << endl;
		else
			cout << endl << "  ├─┼─┼─┼─┼─┼─┼─┼─┤" << endl;
	}
	
}  

//复制棋盘函数
void copy(int a[10][10], int b[10][10])
{
	int i, j;
	for (i = 1;i <=8; i++)
		for (j = 1; j <= 8; j++)
			b[i][j] = a[i][j];
}

//吃子函数
void changecolor(int x, int y,int color)
{
	int x1,y1;
	int count;

	chessboard[x][y] = color;
	
	x1 = x;
	y1 = y;
	count = 0;
	if(chessboard[x-1][y]==0-color)
		while (x1>=2)
		{
			if (chessboard[x1-1][y] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x1-1][y] == color)
				break;
			count++;
			x1--;
		}
	if (count == x - 1)
		count = 0;
	if (count != 0 && count != x - 1)
		for (int i = x - 1;i >= x - count;i--)
			chessboard[i][y] = color;
	if (count > changemax)
		changemax = count;

	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x + 1][y] == 0-color)
		while (x1 <= 7)
		{
			if (chessboard[x1 + 1][y] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x1 + 1][y] == color)
				break;
			count++;
			x1++;
		}
	if (count == 8-x)
		count = 0;
	if (count != 0 && count != 8 - x)
		for (int i = x +1;i <= x + count;i++)
			chessboard[i][y] = color;
	if (count > changemax)
		changemax = count;
		
	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x ][y-1] == 0-color)
		while (y1 >= 2)
		{
			if (chessboard[x][y1-1] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x][y1-1] ==color)
				break;
			count++;
			y1--;
		}
	if (count == y- 1)
		count = 0;
	if (count != 0 && count != y - 1)
		for (int i = y - 1;i >= y  - count;i--)
			chessboard[x][i] = color;
	if (count > changemax)
		changemax = count;

	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x][y+1] == 0-color)
		while (y1 <= 7)
		{
			if (chessboard[x][y1+1] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x][y1+1] == color)
				break;
			count++;
			y1++;
		}
	if (count == 8 - y)
		count = 0;
	if (count != 0 && count != 8 - y)
		for (int i = y + 1;i <= y + count;i++)
			chessboard[x][i] = color;
	if (count > changemax)
		changemax = count;

	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x - 1][y-1] == 0-color)
		while ((x1 >= 2)&&(y1 >= 2))
		{
			if (chessboard[x1 - 1][y1 - 1] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x1 - 1][y1-1] == color)
				break;
			count++;
			x1--;
			y1--;
		}
	if (count ==(x-1)|| count == (y-1))
		count = 0;
	if (count != 0 && count != (x - 1) && count != (y - 1))
		for (int i = x - 1;i >= x  - count;i--)
			chessboard[i][i+y-x]=color;
	if (count > changemax)
		changemax = count;

	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x +1][y + 1] == 0-color)
		while ((x1 <= 7) && (y1 <= 7))
		{
			if (chessboard[x1 + 1][y1 + 1] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x1 + 1][y1 + 1] == color)
				break;
			count++;
			x1++;
			y1++;
		}
	if (count == (8-x) || count == (8-y))
		count = 0;
	if (count != 0 && count != (8-x) && count != (8-y))
		for (int i = x +1;i <= x +  count;i++)
			chessboard[i][i + y - x] = color;
	if (count > changemax)
		changemax = count;

	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x - 1][y + 1] == 0-color)
		while ((x1 >= 2) && (y1 <= 7))
		{
			if (chessboard[x1 - 1][y1 + 1] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x1 - 1][y1 + 1] == color)
				break;
			count++;
			x1--;
			y1++;
		}
	if (count == (x - 1) || count == (8-y))
		count = 0;
	if (count != 0 && count != (x - 1) && count != (8-y))
		for (int i = x - 1;i >= x - count;i--)
			chessboard[i][x+y-i] =color;
	if (count > changemax)
		changemax = count;

	x1 = x;
	y1 = y;
	count = 0;
	if (chessboard[x +1][y - 1] == 0-color)
		while ((y1 >= 2) && (x1 <= 7))
		{
			if (chessboard[x1 + 1][y1 - 1] == 0)
			{
				count = 0;
				break;
			}
			if (chessboard[x1 + 1][y1 - 1] == color)
				break;
			count++;
			x1++;
			y1--;
		}
	if (count == (y - 1) || count == (8 - x))
		count = 0;
	if (count != 0 && count != (y - 1) && count != (8 - x))
		for (int i = y - 1;i >= y - count;i--)
			chessboard[x + y - i][i] = color;
	if (count > changemax)
		changemax = count;

}

//判断能否落子函数
bool judge(int x, int y, int color)
{
	int i, j, x_, y_;
	if (x == 0 || y == 0 || x > 8 || y > 8)
		return 0;
	if (chessboard[x][y] != 0)
		return 0;
	for (j = 0; j < 8; j++)	//借助direct数组向八个方向搜索
	{
		x_ = x + direct[j][1];
		y_ = y + direct[j][0];
		i = 1;
		while (x_ > 0 && x_ < 9 && y_ > 0 && y_<9)
		{
			if (chessboard[x_][y_] == 0)
				break;
			else
				if (chessboard[x_][y_] == color)
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

//数棋子函数
void count()
{
	whitenum = 0;
	blacknum = 0;
	for (int i = 1;i <= 8;i++)
		for (int j = 1;j <= 8;j++)
		{
			if (chessboard[i][j] == -1)
				whitenum++;
			if (chessboard[i][j] == 1)
				blacknum++;
		}
	cout << "   功能菜单：1.存档  2.退出" << endl;
	cout << "   如果要使用菜单 请输入-1，-1" << endl;
	cout << "   白方子数：" << whitenum << "          " << "黑方子数：" << blacknum << endl;
}

int sum(int my_side)
{
	int i, j, s = 0;	//s表示棋子数
	for (i = 1;i <= 8; i++)
		for (j = 1;j <= 8;j++)
		{
			if (chessboard[i][j] == my_side)	//如果是我方棋子
				s++;
		}
	return s;
}

//存档函数
void save_game(const char*file_name)
{
	ofstream out_file_stream;
	out_file_stream.open(file_name, ios::out | ios::trunc);
	for (int i = 1;i <= 8;i++)
		for (int j = 1;j <= 8;j++)
		{
			if (j == 8)
				out_file_stream << chessboard[i][j] << endl;
			else
				out_file_stream << chessboard[i][j] << ' ';
		}
	out_file_stream.close();
}

//读档函数
void load_game(const char*file_name)
{
	ifstream in_file_stream;
	in_file_stream.open(file_name, ios::in);
	for (int i = 1;i <= 8;i++)
		for (int j = 1;j <= 8;j++)
		{
			in_file_stream >> chessboard[i][j];
		}
	in_file_stream.close();
}

//美化界面函数
void UI()
{
cout<<"      ,-.----."<<endl;
cout<<"      \\    /  \\                                                      ,--,"<<endl;
cout<<"      ;   :    \\                                __  ,-.            ,--.'|"<<endl;
cout<<"      |   | .\\ :                .---.         ,' ,'/ /|  .--.--.   |  |,"<<endl;
cout<<"      .   : |: |    ,---.     /.  ./|  ,---.  '  | |' | /  /    '  `--'_"<<endl;
cout<<"      |   |  \\ :   /     \\  .-' . ' | /     \\ |  |   ,'|  :  /`./  ,' ,'|"<<endl;
cout<<"      |   : .  /  /    /  |/___/ \\: |/    /  |'  :  /  |  :  ;_    '  | |"<<endl;
cout<<"      ;   | |  \\ .    ' / |.   \\  ' .    ' / ||  | '    \\  \\    `. |  | :"<<endl;
cout<<"      |   | ;\\  \\'   ;   /| \\   \\   '   ;   /|;  : |     `----.   \\'  : |__"<<endl;
cout<<"      :   ' | \\.''   |  / |  \\   \\  '   |  / ||  , ;    /  /`--'  /|  | '.'|"<<endl;
cout<<"      :   : :-'  |   :    |   \\   \\ |   :    | ---'    '--'.     / ;  :    ;"<<endl;
cout<<"      |   |.'     \\   \\  /     '---' \\   \\  /            `--'---'  |  ,   / "<<endl;
cout<<"      `---'        `----'             `----'                        ---`-'"<<endl;

cout << "                      ┌───────────────┐" << endl;
cout << "                      │     welcome to the game !    │" << endl;
cout << "                      └───────────────┘" << endl << endl;
}

//菜单函数
int startmenu()
{
	int n;
	cout << "1.新开始游戏" << endl;
	cout << "2.读取存档" << endl;
	cout << "3.退出" << endl;
	cout << "请输入你要的操作" << endl;
	while (true)
	{
		cin >> n;
		if (n > 0 && n < 4)
		{
			return n;
			break;
		}
		else
			cout << "该指令非法 请再输一遍"<<endl;

	}
}

int gamemenu()
{
	if (inputx == -1 && inputy == -1)
	{
		int choose3;
		char name[1000];
		cout << "欢迎使用菜单 请输入指令 1.存档  2.退出 " << endl;
		while (true)
		{
			cin >> choose3;
			if (choose3 > 0 && choose3 < 3)
			{
				break;
			}
			else
				cout << "该指令非法 请再输一遍" << endl;
		}
		if (choose3 == 1)
		{
			cout << "请输入你的存档的名字" << endl;
			cin >> name;
			cout << "保存成功！" << endl;
			save_game(name);
			return 1;
		}
		if (choose3 == 2)
		{
			cout << "少年下不过AI还要提高自己的姿势水平" << endl;
		}
		return 0;
	}
}

/*以下是AI部分 这里采取alpha-beta剪枝算法（参考了很多网上资料和大神指导 如有雷同 纯属巧合）*/
/*----------------------------------------------------------------------------------------------------------------------------*/

//行动力函数（棋手合法的可能棋步数量)
int canput(int my_side)
{
	int i, j, s = 0;	//s表示合法的可能棋步数量
	for (i = 1; i <=8; i++)
		for (j = 1; j <= 8; j++)
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
		otherschess =sum((-1)*my_side);
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
	judge_value = judge_value + (chessboard[1][1] * my_side + chessboard[8][8] * my_side + chessboard[1][8] * my_side + chessboard[8][1] * my_side) * 30 - (chessboard[2][2] * my_side + chessboard[7][7] * my_side + chessboard[2][7] * my_side + chessboard[7][2] * my_side) * 20;//考虑角落的情况
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
	for (int i = 1;i <=8; i++)
	{
		for (int j = 1;j <=8;j++)
		{
			if (judge(j, i, now_side))	//如果可以落子
			{
				int copychessboard[10][10];	//备份棋局
				copy(chessboard, copychessboard);
				changecolor(j, i, now_side);
				queue[quelen][0] = judgevalue(-1, now_side);	//记录在该处落子的信息
				queue[quelen][1] = j;
				queue[quelen][2] = i;
				quelen++;
				copy(copychessboard, chessboard);
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
			int copychessboard[10][10];	//备份棋局
			copy(chessboard, copychessboard);
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
			copy(copychessboard, chessboard);	//恢复棋局
		}
		s = alpha;
	}
	else
	{
		for (int j = quelen - 1; j >= 0; j--)
		{
			int copychessboard[10][10];	//备份棋局
			copy(chessboard, copychessboard);
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
			copy(copychessboard, chessboard);	//恢复棋局
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

//尾局搜索，后8步时采取比赛结束时赢子最多的方法
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
			for (i = 1;i < 9; i++)
			{
				for (j = 1; j < 9; j++)
				{
					if (judge(j, i, now_side))	//如果可下
					{
						int copychessboard[10][10];	//备份棋局
						copy(chessboard, copychessboard);
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
						copy(copychessboard, chessboard);	//恢复棋局
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
/*----------------------------------------------------------------------------------------------------------------------------*/

//游戏函数
void game1()
{
	int choose4;
	while (true)
	{
		count();
		result = checkwin(1);
		if (result == 1)
		{
			printchessboard();
			cout << "你赢啦！！！！" << endl;
			break;
		}
		if (result == -1)
		{
			printchessboard();
			cout << "你输啦！！！！" << endl;
			break;
		}
		if (result == 0)
		{
			printchessboard();
			cout << "和AI打个平手，哎呦不错哦！！！！" << endl;
			break;
		}

		printchessboard();
		if (canput(1) > 0)
		{
			cout << "请输入左栏坐标：";
			cin >> inputx;
			cout << "请输入上栏坐标：";
			cin >> inputy;
			choose4 = gamemenu();
			if (choose4 == 0||choose4==1)
				break;
			if (judge(inputx, inputy, 1))
			{
				system("cls");
				changecolor(inputx, inputy, 1);
				count();
				printchessboard();
				if (canput(-1) > 0)
				{
					if (step >= 52)
						final(-1, 1, INT_MIN, INT_MAX);	//残局搜索
					else
						chess_ai(-1, 1, INT_MIN, INT_MAX);	//开局搜索
					changecolor(ai_x, ai_y, -1);
				}
				else
				{
					cout << "AI无子可下，请您继续下" << endl;
					system("pause");
				}
			}
			else
			{
				cout << "您的落子非法" << endl;
				system("pause");
			}
		}
		else
		{
			cout << "您无子可下，AI继续下" << endl;
			system("pause");
			if (canput(-1) > 0)
			{
				if (step >= 52)
					final(-1, 1, INT_MIN, INT_MAX);	//残局搜索
				else
					chess_ai(-1, 1, INT_MIN, INT_MAX);	//开局搜索
				changecolor(ai_x, ai_y, -1);
			}
			else
			{
				cout << "AI无子可下，请您继续下" << endl;
				system("pause");
			}
		}
		system("cls");
		step++;
	}
}

void game2()
{
	int choose4;
	count();
	printchessboard();
	chess_ai(1, 1, INT_MIN, INT_MAX);	//开局搜索
	changecolor(ai_x, ai_y, 1);
	system("pause");
	system("cls");
	while (true)
	{
		count();
		result = checkwin(-1);
		if (result == 1)
		{
			printchessboard();
			cout << "你赢啦！！！！" << endl;
			break;
		}
		if (result == -1)
		{
			printchessboard();
			cout << "你输啦！！！！" << endl;
			break;
		}
		if (result == 0)
		{
			printchessboard();
			cout << "和AI打个平手，哎呦不错哦！！！！" << endl;
			break;
		}

		printchessboard();
		if (canput(-1) > 0)
		{
			cout << "请输入左栏坐标：";
			cin >> inputx;
			cout << "请输入上栏坐标：";
			cin >> inputy;
			choose4 = gamemenu();
			if (choose4 == 0||choose4==1)
				break;
			if (judge(inputx, inputy, -1))
			{
				system("cls");
				changecolor(inputx, inputy, -1);
				count();
				printchessboard();
				if (canput(1) > 0)
				{
					if (step >= 52)
						final(1, 1, INT_MIN, INT_MAX);	//残局搜索
					else
						chess_ai(1, 1, INT_MIN, INT_MAX);	//开局搜索
					changecolor(ai_x, ai_y, 1);
				}
				else
				{
					cout << "AI无子可下，请您继续下" << endl;
					system("pause");
				}
			}
			else
			{
				cout << "您的落子非法" << endl;
				system("pause");
			}
		}
		else
		{
			cout << "您无子可下，AI继续下" << endl;
			system("pause");
			if (canput(1) > 0)
			{
				if (step >= 52)
					final(1, 1, INT_MIN, INT_MAX);	//残局搜索
				else
					chess_ai(1, 1, INT_MIN, INT_MAX);	//开局搜索
				changecolor(ai_x, ai_y, 1);
			}
			else
			{
				cout << "AI无子可下，请您继续下" << endl;
				system("pause");
			}
		}
		system("cls");
		step++;
	}
}

int main()
{
	int n;
	char name[100];
	for (int i = 1;i <= 8;i++)    //棋盘初始化
		for (int j = 1;j <= 8;j++)
			chessboard[i][j] = 0;
	chessboard[4][4] = -1;
	chessboard[5][5] = -1;
	chessboard[5][4] = 1;
	chessboard[4][5] = 1;
	UI();
	system("pause");
	system("cls");
	int choose1= startmenu();
	if (choose1 == 1)
	{
		int choose2;
		cout << "1.先手执黑" << endl;
		cout << "2.后手执白" << endl;
		cout << "请输入你要的操作" << endl;
		while (true)
		{
			cin >> choose2;
			if (choose2 > 0 && choose2 < 3)
			{
				break;
			}
			else
				cout << "该指令非法 请再输一遍" << endl;
		}
		if (choose2 == 1)
		{
			system("pause");
			system("cls");
			game1();
		}
		if (choose2 == 2)
		{
			system("pause");
			system("cls");
			game2();
		}
	}
	if (choose1 == 2) 
	{
		cout << "请输入你的存档的名字" << endl;
		cin >> name;
		cout << "请输入你走子的颜色 1是黑 -1是白" << endl;
		cin >> n;
		load_game(name);
		system("pause");
		system("cls");
		if(n==1)
		game1();
		if (n == (-1))
		{
			count();
			printchessboard();
			cout << "请输入左栏坐标：";
			cin >> inputx;
			cout << "请输入上栏坐标：";
			cin >> inputy;
			if (judge(inputx, inputy, -1))
			{
				system("cls");
				changecolor(inputx, inputy, -1);
				game2();
			}
		}
	}
	if (choose1 == 3)
	{
		system("pause");
		return 0;
	}
	
	system("pause");
    return 0;
}














			
