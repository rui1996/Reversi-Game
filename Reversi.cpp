// �ڰ��� ����ҵ.cpp : �������̨Ӧ�ó������ڵ㡣
//���ߣ� Ǯ�� 1500012777

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <windows.h>
using namespace std;

//����Ϊȫ�ֱ�����ʼ������
int chessboard[10][10];  //���� 0��ʾδ�� 1��ʾ��ɫ -1��ʾ��ɫ
int direct[8][2] = { { 0,1 },{ 0,-1 },{ 1,0 },{ -1,0 },{ 1,1 },{ -1,1 },{ 1,-1 },{ -1,-1 } };	//����
int turn[2] = { 1,-1 };	//��ʾ�������,��Ӧ���������
int turn_[2] = { 0, 0 };//��ʾ�������,��Ӧ�����AI
int changemax = 0;
int whitenum;
int blacknum;
int playercolor;//��ҵ���ɫ
int ai_x, ai_y;//ÿ��AI�ߵ�����
int depth = 3;//������ȣ���Ϊ�Ѷ�ϵ����()
int step = 1;//��¼����
int inputx, inputy;//����x,y
int result;//���

//��ӡ���̺���
void printchessboard()
{
	cout << "    1   2   3   4   5   6   7   8" << endl;
	cout << "  �����Щ��Щ��Щ��Щ��Щ��Щ��Щ���" << endl;
	for (int i = 1; i <= 8; i++) {
		cout << ' ' << i << "��";
		for (int j = 1; j <= 8;j++) {
			switch (chessboard[i][j]) {
			case 0:
				cout << "  ";
				break;
			case 1:
				cout << "��";
				break;
			case -1:
				cout << "��";
				break;
			}
			cout << "��";
		}
		if (i == 8)
			cout << endl << "  �����ة��ة��ة��ة��ة��ة��ة���" << endl;
		else
			cout << endl << "  �����੤�੤�੤�੤�੤�੤�੤��" << endl;
	}
	
}  

//�������̺���
void copy(int a[10][10], int b[10][10])
{
	int i, j;
	for (i = 1;i <=8; i++)
		for (j = 1; j <= 8; j++)
			b[i][j] = a[i][j];
}

//���Ӻ���
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

//�ж��ܷ����Ӻ���
bool judge(int x, int y, int color)
{
	int i, j, x_, y_;
	if (x == 0 || y == 0 || x > 8 || y > 8)
		return 0;
	if (chessboard[x][y] != 0)
		return 0;
	for (j = 0; j < 8; j++)	//����direct������˸���������
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
						return 1;	//������������Ӳ����м�������һ���Է��ӣ�˵��������
					break;	//��������������
				}
			i++;
			x_ = x_ + direct[j][1];	//�����һ��
			y_ = y_ + direct[j][0];
		}
	}
	return 0;
} 

//�����Ӻ���
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
	cout << "   ���ܲ˵���1.�浵  2.�˳�" << endl;
	cout << "   ���Ҫʹ�ò˵� ������-1��-1" << endl;
	cout << "   �׷�������" << whitenum << "          " << "�ڷ�������" << blacknum << endl;
}

int sum(int my_side)
{
	int i, j, s = 0;	//s��ʾ������
	for (i = 1;i <= 8; i++)
		for (j = 1;j <= 8;j++)
		{
			if (chessboard[i][j] == my_side)	//������ҷ�����
				s++;
		}
	return s;
}

//�浵����
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

//��������
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

//�������溯��
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

cout << "                      ����������������������������������" << endl;
cout << "                      ��     welcome to the game !    ��" << endl;
cout << "                      ����������������������������������" << endl << endl;
}

//�˵�����
int startmenu()
{
	int n;
	cout << "1.�¿�ʼ��Ϸ" << endl;
	cout << "2.��ȡ�浵" << endl;
	cout << "3.�˳�" << endl;
	cout << "��������Ҫ�Ĳ���" << endl;
	while (true)
	{
		cin >> n;
		if (n > 0 && n < 4)
		{
			return n;
			break;
		}
		else
			cout << "��ָ��Ƿ� ������һ��"<<endl;

	}
}

int gamemenu()
{
	if (inputx == -1 && inputy == -1)
	{
		int choose3;
		char name[1000];
		cout << "��ӭʹ�ò˵� ������ָ�� 1.�浵  2.�˳� " << endl;
		while (true)
		{
			cin >> choose3;
			if (choose3 > 0 && choose3 < 3)
			{
				break;
			}
			else
				cout << "��ָ��Ƿ� ������һ��" << endl;
		}
		if (choose3 == 1)
		{
			cout << "��������Ĵ浵������" << endl;
			cin >> name;
			cout << "����ɹ���" << endl;
			save_game(name);
			return 1;
		}
		if (choose3 == 2)
		{
			cout << "�����²���AI��Ҫ����Լ�������ˮƽ" << endl;
		}
		return 0;
	}
}

/*������AI���� �����ȡalpha-beta��֦�㷨���ο��˺ܶ��������Ϻʹ���ָ�� ������ͬ �����ɺϣ�*/
/*----------------------------------------------------------------------------------------------------------------------------*/

//�ж������������ֺϷ��Ŀ����岽����)
int canput(int my_side)
{
	int i, j, s = 0;	//s��ʾ�Ϸ��Ŀ����岽����
	for (i = 1; i <=8; i++)
		for (j = 1; j <= 8; j++)
			if (judge(j, i, my_side))
				s++;
	return s;
}

//�ж�ʤ������
int checkwin(int my_side)
{
	int mychess, otherschess;	//mychess, otherschess�ֱ��ʾ�ҷ���Է���������
	if (canput(1) == 0 && canput(-1) == 0)	//���������ӿ���
	{
		mychess = sum(my_side);	//����˫��������
		otherschess =sum((-1)*my_side);
		if (mychess > otherschess)
			return 1;	//1��ʾʤ
		else
			if (mychess < otherschess)
				return -1;	//-1��ʾ��
			else
				return 0;	//0��ʾƽ
	}
	return 2;	//2��ʾδ����
}

//��ֵ�����������ж�����ռ��
int judgevalue(int my_side, int now_side)
{
	int mobility = now_side * my_side* canput(now_side);
	int	judge_value;	//mobility��ʾ�����ԣ�now_side * my_side���ڵ�������
	judge_value = mobility;
	judge_value = judge_value + (chessboard[1][1] * my_side + chessboard[8][8] * my_side + chessboard[1][8] * my_side + chessboard[8][1] * my_side) * 30 - (chessboard[2][2] * my_side + chessboard[7][7] * my_side + chessboard[2][7] * my_side + chessboard[7][2] * my_side) * 20;//���ǽ�������
	return judge_value;
}

//��������
int chess_ai(int now_side, int now_depth, int alpha, int beta)	//now_depthָ���ǵ�ǰ����������ȣ�alpha�����ֵ����½磬beta����Сֵ����Ͻ�
{
	if (checkwin(now_side) == 1)	//���Ӯ��
	{
		int s = (now_depth % 2) * 2000000000 - 1000000000;
		return s;
	}
	if (checkwin(now_side) == -1)	//�������
	{
		int s = (1 - now_depth % 2) * 2000000000 - 1000000000;
		return s;
	}
	//�����ǰ��AI������10000000��������˷���-100000000��������ΪAIӮ���Ǳ��������ģ�����Ҫ����һ���ܴ��ֵ���Է�Ӯ�˾ͷ���һ����С�ģ�
	if (checkwin(now_side) == 0)	//���ƽ��
		return 0;
	if (now_depth == depth + 1)	//����������
	{
		if (now_depth % 2 == 1)
			return judgevalue(now_side, now_side);
		else
			return judgevalue(-now_side, now_side);
	}
	if (canput(now_side) == 0)
		return (chess_ai(-now_side, now_depth + 1, alpha, beta));	//�����ǰ�ִ��������
	int s;	//����ֵ
	int queue[64][3];
	int quelen = 0;	//�ڵ��������
	for (int i = 1;i <=8; i++)
	{
		for (int j = 1;j <=8;j++)
		{
			if (judge(j, i, now_side))	//�����������
			{
				int copychessboard[10][10];	//�������
				copy(chessboard, copychessboard);
				changecolor(j, i, now_side);
				queue[quelen][0] = judgevalue(-1, now_side);	//��¼�ڸô����ӵ���Ϣ
				queue[quelen][1] = j;
				queue[quelen][2] = i;
				quelen++;
				copy(copychessboard, chessboard);
			}
		}
	}
	for (int i = 0;i < quelen - 1; i++)	//����ÿ�������ߵ�λ�õĹ�ֵ�������򣬴Ӷ��ڽ������ӹ�ֵ�Ƚϴ�Ŀ�ʼ����
	{
		for (int j = 0; j < quelen - i - 1; j++)
		{
			if (queue[j][0] < queue[j + 1][0])
			{
				int temp;
				temp = queue[j][0];	//��һ�����ڽ�����Ϣ
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
	//����֮�����ÿ�δӿ����ߵĵ������ó�һ�����ݹ������ȥ�����ҽ���alpha-beta��֦��
	//˼·���¾����ڵ�ǰ״̬�Ѿ������Ѿ��ߵ������״̬��ʱ�򣬾Ͳ�Ҫ������ȥ�ˣ�ֱ�ӷ��ء�
	int putx, puty;//Ҫ�ߵĵ�
	if (now_depth % 2)
	{
		for (int j = 0; j < quelen; j++)
		{
			int copychessboard[10][10];	//�������
			copy(chessboard, copychessboard);
			changecolor(queue[j][1], queue[j][2], now_side);	//��ת
			int s1;
			s1 = chess_ai(-now_side, now_depth + 1, alpha, beta);
			if (s1 > alpha)
			{
				alpha = s1;
				putx = queue[j][1];
				puty = queue[j][2];
			}
			if (s1 > beta)	//��֦
			{
				return s1;
			}
			copy(copychessboard, chessboard);	//�ָ����
		}
		s = alpha;
	}
	else
	{
		for (int j = quelen - 1; j >= 0; j--)
		{
			int copychessboard[10][10];	//�������
			copy(chessboard, copychessboard);
			changecolor(queue[j][1], queue[j][2], now_side);	//��ת
			int s2;
			s2 = chess_ai(-now_side, now_depth + 1, alpha, beta);
			if (s2 < beta)	//��֦
			{
				beta = s2;
				putx = queue[j][1];
				puty = queue[j][2];
			}
			if (s2 < alpha)	//��֦
			{
				return s2;
			};
			copy(copychessboard, chessboard);	//�ָ����
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

//β����������8��ʱ��ȡ��������ʱӮ�����ķ���
int final(int now_side, int now_depth, int alpha, int beta)
{
	if (checkwin(now_side) != 2)	//�������
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
			int putx, puty;	//Ҫ�ߵĲ�
			int s;
			for (i = 1;i < 9; i++)
			{
				for (j = 1; j < 9; j++)
				{
					if (judge(j, i, now_side))	//�������
					{
						int copychessboard[10][10];	//�������
						copy(chessboard, copychessboard);
						changecolor(j, i, now_side);	//��ת
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
							if (s1 > beta)	//��֦
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
							if (s2 < alpha)	//��֦
							{
								return s2;
							}
							break;
						}
						copy(copychessboard, chessboard);	//�ָ����
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

//��Ϸ����
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
			cout << "��Ӯ����������" << endl;
			break;
		}
		if (result == -1)
		{
			printchessboard();
			cout << "��������������" << endl;
			break;
		}
		if (result == 0)
		{
			printchessboard();
			cout << "��AI���ƽ�֣����ϲ���Ŷ��������" << endl;
			break;
		}

		printchessboard();
		if (canput(1) > 0)
		{
			cout << "�������������꣺";
			cin >> inputx;
			cout << "�������������꣺";
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
						final(-1, 1, INT_MIN, INT_MAX);	//�о�����
					else
						chess_ai(-1, 1, INT_MIN, INT_MAX);	//��������
					changecolor(ai_x, ai_y, -1);
				}
				else
				{
					cout << "AI���ӿ��£�����������" << endl;
					system("pause");
				}
			}
			else
			{
				cout << "�������ӷǷ�" << endl;
				system("pause");
			}
		}
		else
		{
			cout << "�����ӿ��£�AI������" << endl;
			system("pause");
			if (canput(-1) > 0)
			{
				if (step >= 52)
					final(-1, 1, INT_MIN, INT_MAX);	//�о�����
				else
					chess_ai(-1, 1, INT_MIN, INT_MAX);	//��������
				changecolor(ai_x, ai_y, -1);
			}
			else
			{
				cout << "AI���ӿ��£�����������" << endl;
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
	chess_ai(1, 1, INT_MIN, INT_MAX);	//��������
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
			cout << "��Ӯ����������" << endl;
			break;
		}
		if (result == -1)
		{
			printchessboard();
			cout << "��������������" << endl;
			break;
		}
		if (result == 0)
		{
			printchessboard();
			cout << "��AI���ƽ�֣����ϲ���Ŷ��������" << endl;
			break;
		}

		printchessboard();
		if (canput(-1) > 0)
		{
			cout << "�������������꣺";
			cin >> inputx;
			cout << "�������������꣺";
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
						final(1, 1, INT_MIN, INT_MAX);	//�о�����
					else
						chess_ai(1, 1, INT_MIN, INT_MAX);	//��������
					changecolor(ai_x, ai_y, 1);
				}
				else
				{
					cout << "AI���ӿ��£�����������" << endl;
					system("pause");
				}
			}
			else
			{
				cout << "�������ӷǷ�" << endl;
				system("pause");
			}
		}
		else
		{
			cout << "�����ӿ��£�AI������" << endl;
			system("pause");
			if (canput(1) > 0)
			{
				if (step >= 52)
					final(1, 1, INT_MIN, INT_MAX);	//�о�����
				else
					chess_ai(1, 1, INT_MIN, INT_MAX);	//��������
				changecolor(ai_x, ai_y, 1);
			}
			else
			{
				cout << "AI���ӿ��£�����������" << endl;
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
	for (int i = 1;i <= 8;i++)    //���̳�ʼ��
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
		cout << "1.����ִ��" << endl;
		cout << "2.����ִ��" << endl;
		cout << "��������Ҫ�Ĳ���" << endl;
		while (true)
		{
			cin >> choose2;
			if (choose2 > 0 && choose2 < 3)
			{
				break;
			}
			else
				cout << "��ָ��Ƿ� ������һ��" << endl;
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
		cout << "��������Ĵ浵������" << endl;
		cin >> name;
		cout << "�����������ӵ���ɫ 1�Ǻ� -1�ǰ�" << endl;
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
			cout << "�������������꣺";
			cin >> inputx;
			cout << "�������������꣺";
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














			
