#pragma once
#include "Chess.h"
#include "graphics.h"
#include "tools/Image.h"
#include "widgets/ImageTextButton.h"
#include "ChessSaver.h"
#include "ChessBoardView.h"

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
	PlayChess(Chess& c);
	~PlayChess();

	void enter_interface();
	void show_chessboard();															//��ʾ������
	void update_windows();															//ˢ�½���
	PlayChess::ACTION_TYPE action_judge(int x, int y);	//������������ж����Ӧ���򣬲�������Ӧ��������
	void show_last_game(ChessSaver& saver);							//��ʾ�о�
	bool show_outcome();																//��ʾ��Ϸ���
	void play_chess_by_man(int x, int y, Chess::PieceType value);//������괫�������꼰��������ִ�����嶯��
	void play_chess_by_computer(int row, int col, Chess::PieceType value);//���ݼ����������������꼰��������

	void on_mouse_move(int x, int y, PlayChess::ACTION_TYPE action);
	void on_mouse_click(PlayChess::ACTION_TYPE action);

private:
  static ImageTextButton* button_game_replay_;
  static ImageTextButton* button_game_quit_;
	ChessBoardView chess_board_;
	Chess& chess_;

	static const int button_text_size_ = 28;
	static const int text_x_offset_ = 75;
};
