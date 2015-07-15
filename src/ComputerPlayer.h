#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H
#include "Chess.h"

class ComputerPlayer
{
private:
  Chess* chess_;
  Chess::PieceType computer_piece_type_;
  Chess::PieceType person_piece_type_;

  struct GridScore
  {
    int row;
    int col;
    double row_score;
    double col_score;
    double left_right_score;
    double right_left_score;
    double sum_score;
  };

  GridScore computer_score_table_[Chess::SIZE][Chess::SIZE];
  GridScore computer_score_[Chess::SIZE * Chess::SIZE];
  GridScore person_score_table_[Chess::SIZE][Chess::SIZE];
  GridScore person_score_[Chess::SIZE * Chess::SIZE];

  static int max_search_step;
  static const int max_select_grid = 10;
  static const double empty_grid_discount_weight_[10];
  static const double linked_grid_score_[10];
  static const double linked_grid_weight_[10];
  static const int MAX_FLAG = 1;
  static const int MIN_FLAG = -1;
  static const double INFINITY_MAX;

  bool is_direction_valid(int row, int col, Chess::Direction dire, Chess::PieceType oposite_type);
  void calc_grid_score_direction(int row, int col, Chess::Direction dire, Chess::PieceType self_type, GridScore& grid_score);
  void calc_grid_score(GridScore& grid_score, Chess::PieceType seft_type);
  int quicksort_division(int left, int right, GridScore* array);
  void quicksort(int left, int right, GridScore* array);
  void attack(int& target_row, int& target_col);                            //����̰���㷨
  void defend(int& target_row, int& target_col);                            //����̰���㷨

  void show_score_tabel(GridScore score[][Chess::SIZE]);                    //�����������

  double calc_chess_score(Chess::PieceType self_type, int min_max_flag);
  void calc_score_table();
  double DFS(int nth_step, double ex_value, int min_max_flag, int& target_row, int& target_col);        //�����������㷨

public:
  ComputerPlayer(Chess::PieceType computer_type, Chess* c);
  void calc_next_step(int& target_row, int& target_col);
};

#endif // COMPUTERPLAYER_H
