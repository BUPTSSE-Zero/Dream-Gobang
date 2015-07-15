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
	PieceType get_point(int x, int y);
	int judge(int x, int y, Direction dire);          //1�������ң�2�������£�3�������ϵ����£�4�������ϵ�����
	bool is_chess_full();
	int get_empty_grid_num();
	PieceType judge_win();
	void show_chess();
	void set_man_picetype(PieceType picetype);
	PieceType get_man_piecetype();
	PieceType get_computer_piecetype();
private:
	PieceType chess_array_[SIZE][SIZE];
	static PieceType picetype_;
};
