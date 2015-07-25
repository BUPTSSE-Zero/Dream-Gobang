#pragma once
#include "Chess.h"
#include "Save.h"
#include "graphics.h"
#include "Image.h"
#include "ImageButton.h"

class PlayChess
{
public:
	enum ACTION_TYPE
	{
		ACTION_PLAY = 1,
		ACTION_REPLAY = 2,
		ACTION_QUIT = 3,
		ACTION_NONE = 4
	};
	PlayChess(Chess* c);
	~PlayChess();

	int mouse_to_row(int x, int y);//���������תΪ������
	int mouse_to_col(int x, int y);//���������תΪ������
	void show_chessboard();//��ʾ������
	void update_windows(Chess &c);//ˢ�½���
	PlayChess::ACTION_TYPE action_judge(int x, int y);//������������ж����Ӧ���򣬲�������Ӧ��������
	void save_last_game(Save &save, Chess &chess_);//����о�
	void show_last_game();//��ʾ�о�
	bool show_outcome(Chess &chess);//��ʾ��Ϸ���
	void play_chess_by_man(int x, int y, Chess::PieceType value);//������괫�������꼰��������ִ�����嶯��
	void play_chess_by_computer(int row, int col, Chess::PieceType value);//���ݼ����������������꼰��������

	void on_mouse_move(PlayChess::ACTION_TYPE action);
	void on_mouse_click(PlayChess::ACTION_TYPE action);

private:
  static ImageButton* button_game_replay_;
  static ImageButton* button_game_quit_;

  static const int text_x_offset_ = 65;
	static const int text_y_offset_ = 12;
	Chess* p;

	bool judge_zone(int x, int y);//�ж�����Ƿ���������Ч������
};
