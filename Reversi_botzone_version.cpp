// �ڰ���botzone�汾.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "jsoncpp/json.h" // C++����ʱĬ�ϰ����˿�

using namespace std;
# define INT_MAX 2147483647
# define INT_MIN (-2147483647-1)

int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[8][8] = { 0 }; // ��x��y����¼����״̬
int blackPieceCount = 2, whitePieceCount = 2;
int direct[8][2] = { { 0,1 },{ 0,-1 },{ 1,0 },{ -1,0 },{ 1,1 },{ -1,1 },{ 1,-1 },{ -1,-1 } };	//����
int turn[2] = { 1,-1 };	//��ʾ�������,��Ӧ���������
int turn_[2] = { 0, 0 };//��ʾ�������,��Ӧ�����AI
int changemax = 0;
int whitenum;
int blacknum;
int playercolor;//��ҵ���ɫ
int ai_x, ai_y;//ÿ��AI�ߵ�����
int depth = 4;//������ȣ���Ϊ�Ѷ�ϵ����()
int step = 1;//��¼����
int inputx, inputy;//����x,y
int result;//���
// ��Direction����Ķ����꣬�������Ƿ�Խ��
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

// �����괦���ӣ�����Ƿ�Ϸ���ģ������
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

// ���color�����޺Ϸ��岽
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

//���Ӻ���
void changecolor(int x, int y, int my_side)
{
	int i, j, k, x_, y_;	//i�м��������ã�x_, y_�ֱ��ʾ�����������
	gridInfo[x][y] = my_side;	//�����µĵ���Ϊ�ҷ���
	for (j = 0; j < 8; j++)	//����direct������˸���������
	{
		x_ = x + direct[j][1];
		y_ = y + direct[j][0];
		i = 1;
		while (x_>=0 && x_<8 && y_ >= 0 && y_ < 8)	//���������߽�ʱ
		{
			if (gridInfo[x_][y_] == 0)
				break;	//������λ������
			else
				if (gridInfo[x_][y_] == my_side)	//����ҵ��ҷ�����
				{
					for (k = 1; k < i; k++)
						gridInfo[x + k * direct[j][1]][y + k * direct[j][0]] = my_side;	//��ת
					break;
				}
			i++;
			x_ = x_ + direct[j][1];	//�����һ��
			y_ = y_ + direct[j][0];
		}
	}
}
//�ж��ܷ����Ӻ���
bool judge(int x, int y, int color)
{
	int i, j, x_, y_;
	if (x == -1 || y == -1 || x > 7 || y > 7)
		return 0;
	if (gridInfo[x][y] != 0)
		return 0;
	for (j = 0; j < 8; j++)	//����direct������˸���������
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
int sum(int my_side)
{
	int i, j, s = 0;	//s��ʾ������
	for (i = 0;i <= 7; i++)
		for (j = 0;j <= 7;j++)
		{
			if (gridInfo[i][j] == my_side)	//������ҷ�����
				s++;
		}
	return s;
}
int canput(int my_side)
{
	int i, j, s = 0;	//s��ʾ�Ϸ��Ŀ����岽����
	for (i = 0; i <= 7; i++)
		for (j = 0; j <= 7; j++)
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
		otherschess = sum((-1)*my_side);
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
	judge_value = judge_value + (gridInfo[0][0] * my_side + gridInfo[7][7] * my_side + gridInfo[0][7] * my_side + gridInfo[7][0] * my_side) * 30 - (gridInfo[1][1] * my_side + gridInfo[6][6] * my_side + gridInfo[1][6] * my_side + gridInfo[6][1] * my_side) * 20;//���ǽ�������
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
	for (int i = 0;i <= 7; i++)
	{
		for (int j = 0;j <= 7;j++)
		{
			if (judge(j, i, now_side))	//�����������
			{
				int copygridInfo[8][8];	//�������
				copy(gridInfo, copygridInfo);
				changecolor(j, i, now_side);
				queue[quelen][0] = judgevalue(-1, now_side);	//��¼�ڸô����ӵ���Ϣ
				queue[quelen][1] = j;
				queue[quelen][2] = i;
				quelen++;
				copy(copygridInfo, gridInfo);
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
			int copygridInfo[8][8];	//�������
			copy(gridInfo, copygridInfo);
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
			copy(copygridInfo, gridInfo);	//�ָ����
		}
		s = alpha;
	}
	else
	{
		for (int j = quelen - 1; j >= 0; j--)
		{
			int copygridInfo[8][8];	//�������
			copy(gridInfo, copygridInfo);
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
			copy(copygridInfo, gridInfo);	//�ָ����
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

//�о���������8��ʱ��ȡ��������ʱӮ�����Ĺ�ֵ����������������Ϸ����Ϊֹ
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
			for (i = 0;i < 8; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (judge(j, i, now_side))	//�������
					{
						int copygridInfo[8][8];	//�������
						copy(gridInfo, copygridInfo);
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
						copy(copygridInfo, gridInfo);	//�ָ����
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

	// ��ʼ������
	gridInfo[3][4] = gridInfo[4][3] = 1;  //|��|��|
	gridInfo[3][3] = gridInfo[4][4] = -1; //|��|��|

										  // ����JSON
	string str;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);

	// �����Լ��յ���������Լ���������������ָ�״̬
	int turnID = input["responses"].size();
	currBotColor = input["requests"][(Json::Value::UInt) 0]["x"].asInt() < 0 ? 1 : -1; // ��һ�غ��յ�������-1, -1��˵�����Ǻڷ�
	for (int i = 0; i < turnID; i++)
	{
		// ������Щ��������𽥻ָ�״̬����ǰ�غ�
		x = input["requests"][i]["x"].asInt();
		y = input["requests"][i]["y"].asInt();
		if (x >= 0)
			ProcStep(x, y, -currBotColor); // ģ��Է�����
		x = input["responses"][i]["x"].asInt();
		y = input["responses"][i]["y"].asInt();
		if (x >= 0)
			ProcStep(x, y, currBotColor); // ģ�⼺������
	}

	// �����Լ����غ�����
	x = input["requests"][turnID]["x"].asInt();
	y = input["requests"][turnID]["y"].asInt();
	if (x >= 0)
		ProcStep(x, y, -currBotColor); // ģ��Է�����

									   // �ҳ��Ϸ����ӵ�
	int possiblePos[64][2], posCount = 0, choice;

	for (y = 0; y < 8; y++)
		for (x = 0; x < 8; x++)
			if (ProcStep(x, y, currBotColor, true))
			{
				possiblePos[posCount][0] = x;
				possiblePos[posCount++][1] = y;
			}

	// �������ߣ���ֻ���޸����²��֣�

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
			chess_ai(currBotColor, 1, INT_MIN, INT_MAX);	//��������
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
				final(currBotColor, 1, INT_MIN, INT_MAX);	//��������
				resultX = ai_x;
				resultY = ai_y;
			}
			else
			{
				resultX = -1;
				resultY = -1;
			}
	}


	// ���߽���������������ֻ���޸����ϲ��֣�

	Json::Value ret;
	ret["response"]["x"] = resultX;
	ret["response"]["y"] = resultY;
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;
	return 0;
}
