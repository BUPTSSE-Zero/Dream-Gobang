#pragma once
#include "Chess.h"
#include <fstream>
using namespace std;

class ChessSaver
{
private:
	Chess::PieceType player_piece_color_;
	Chess chess_buf_;
	static const char* file_name_;

	static bool chess_check(Chess& chess, Chess::PieceType player_type_);			//ͨ���Ա���ֵĺڰ��Ӹ����������������ִ���������Ƿ�ƥ��
public:
	bool read_chess();
	Chess::PieceType get_player_piece_type()
	{
		return player_piece_color_;
	}

	Chess get_chess()
	{
		return chess_buf_;
	}

	static bool save_chess(Chess& chess, Chess::PieceType player_type_);
	static bool is_recovery_data_exist();
	static bool delete_recovery_file();
};
