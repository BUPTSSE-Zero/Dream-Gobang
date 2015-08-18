#pragma once

class Chess
{
public:
	Chess();

	enum PieceType
	{
		BLACK = 1,
		WHITE = -1,
		EMPTY = 0
	};
	enum Direction
	{
		COL = 1,
		ROW = 2,
		LEFT_RIGHT = 3,
		RIGHT_LEFT = 4
	};

	static const int SIZE = 15;

	void set_point(int x, int y, PieceType value);
	PieceType get_point(int x, int y) const;
	int judge(int x, int y, Direction dire) const;          //1�������ң�2�������£�3�������ϵ����£�4�������ϵ�����
	bool is_chess_full() const;
	int get_empty_grid_num() const;
	PieceType judge_win() const;
	void show_chess() const;
	bool judge_win_ex(PieceType piece) const;								//�жϵ�ǰ��ʤ������
private:
	PieceType chess_array_[SIZE][SIZE];
	int two_end(int x, int y, Direction dire) const;				//�����������ϰ�����Ŀ���ң�x,y)����Ϊ���ӷ���0
};
