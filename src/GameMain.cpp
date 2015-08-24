#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "WelcomeInterface.h"
#include "SettingsInterface.h"
#include "NetSelectionInterface.h"
#include "NetSettingsInterface.h"
#include "NetWaitingInterface.h"
#include "NetPlayerInfoView.h"
#include "tools/Music.h"
#include "Settings.h"
#include "widgets/MessageDialog.h"
#include "widgets/QuestionDialog.h"
#include "widgets/EditDialog.h"
#include "Gobang.h"
#include "tools/GradientAnimation.h"
#include <stdlib.h>
#include <direct.h>
#include <winuser.h>

Music* game_bgm = NULL;
Settings* game_settings = NULL;

void main_loop();
void play_chess_interface();
void game_settings_interface();
void game_curtain();
void game_net_select_interface();
void game_net_settings_interface();
bool wait_open_room();
bool wait_enter_room();

void play_button_click_audio()
{
	static Music audio_button_click("res/button-click.wav", 1);
	if (game_settings && game_settings->is_audio_on())
		audio_button_click.start();
}

int main()
{
	char cwd[1024];
	GetModuleFileName(NULL, cwd, sizeof(cwd));				//��ȡ��ǰ�������ڵ�����·��
	strrchr(cwd, '\\')[0] = '\0';											//ȥ��·���е�EXE�����ļ���
	chdir(cwd);																				//�л�����Ŀ¼�������ļ�����Ŀ¼

	//������Ϊ�޸�EGE��������ĺ���
	//���������ʽ�ļ������������ʽ����Ҫ�ڴ��ڳ�ʼ��ǰ���á�
	setcursorstyle("res/cursor.ani");

	initgraph(Gobang::WINDOW_WIDTH, Gobang::WINDOW_HEIGHT);
	setbkcolor(WHITE);
	setcaption("�λ�������");

	WelcomeInterface::opening_animation();

	game_bgm = new Music("res/game-bgm.wma", -1, 2);
	game_bgm->set_volume(0.85);
	game_settings = new Settings();
	if (game_settings->read_settings() == false)
		game_settings->write_settings();
	if (game_settings->is_audio_on())
		game_bgm->start();
	main_loop();
	//game_net_select_interface();
	return 0;
}


void main_loop()
{
	mouse_msg msg_;
	int k;
	bool welcome_flag;
	WelcomeInterface welcome_interface;
	WelcomeInterface::action_type welcome_action;
	bool enter_welcome_flag = true;

	while (is_run())
	{
		if (enter_welcome_flag)
			welcome_interface.enter_interface();
		else
			welcome_interface.show_welcome();
		welcome_flag = false;
		while (is_run())
		{
			msg_ = getmouse();
			welcome_action = welcome_interface.action_judge(msg_.x, msg_.y);
			welcome_interface.on_mouse_move(welcome_action);
			if (msg_.is_down() && msg_.is_left())
				welcome_interface.on_mouse_click(welcome_action);
			k = ((msg_.is_up() && msg_.is_left() == 1) ? 1 : 0);
			if (k == 1)
			{
				switch (welcome_action)
				{
					case WelcomeInterface::ACTION_ENTER_GAME:
					{
						play_button_click_audio();
						GradientAnimation::transition_ease_in();
						play_chess_interface();
						welcome_flag = true;
						enter_welcome_flag = true;
						break;
					}
					case WelcomeInterface::ACTION_GAME_SETTINGS:
					{
						play_button_click_audio();
						game_settings_interface();
						welcome_flag = true;
						enter_welcome_flag = false;
						break;
					}
					case WelcomeInterface::ACTION_EXIT_GAME:
					{
						QuestionDialog dialog(500, 150);
						dialog.set_title("Dream-Gobang");
						dialog.set_text("�Ƿ�Ҫ�˳� ���λ������塱 ��Ϸ��");
						dialog.show();
						if (dialog.get_response_result() == QuestionDialog::response_yes)
						{
							GradientAnimation::transition_ease_in();
							closegraph();
							exit(0);
						}
						break;
					}
					case WelcomeInterface::ACTION_SPECIAL_THANKS:
					{
						play_button_click_audio();
						GradientAnimation::transition_ease_in();
						game_curtain();
						welcome_flag = true;
						enter_welcome_flag = true;
						break;
					}
					default:
						break;
				}
				if (welcome_flag)
					break;
			}
		}
	}
}

void play_chess_interface()
{
	PlayChess::ACTION_TYPE n;
	int m;
	mouse_msg msg;
	bool recover_flag = false;
	bool first_enter_flag = true;
	ChessSaver saver;

	if (ChessSaver::is_recovery_data_exist())
	{
		QuestionDialog query_dialog(500, 150);
		query_dialog.set_title("Dream-Gobang");
		query_dialog.set_text("�����ϴ�δ��������֣��Ƿ�����ϴ���Ϸ��");
		query_dialog.show();
		if (query_dialog.get_response_result() == QuestionDialog::response_yes)
			recover_flag = true;
		else if (query_dialog.get_response_result() == QuestionDialog::response_close)
			return;
	}

	if (recover_flag)
	{
		if (saver.read_chess() == false)
		{
			MessageDialog message_dialog(500, 150, MessageDialog::icon_error);
			message_dialog.set_title("Error");
			message_dialog.set_text("��ȡ�о�����ʱ��������");
			message_dialog.show();
			recover_flag = false;
		}
	}

	if (ChessSaver::is_recovery_data_exist())
		ChessSaver::delete_recovery_file();

	while (is_run())
	{
		Chess c;
		Chess::PieceType player_type;
		Chess::PieceType computer_type;
		PlayChess* p = new PlayChess(c);
		bool game_end_flag = false;
		if (first_enter_flag)
		{
			p->enter_interface();
			first_enter_flag = false;
		}
		else
			p->show_chessboard();
		if (recover_flag)
		{
			player_type = saver.get_player_piece_type();
			p->show_last_game(saver);
		}
		else
			player_type = game_settings->get_piece_color();

		if (player_type == Chess::BLACK)
			computer_type = Chess::WHITE;
		else
			computer_type = Chess::BLACK;
		ComputerPlayer cp(computer_type, &c);

		int row, col;
		if (computer_type == Chess::BLACK && recover_flag == false)							//����ִ��������
		{
			cp.calc_next_step(row, col);
			p->play_chess_by_computer(row, col, computer_type);
			c.set_point(row, col, computer_type);
		}
		if (recover_flag)
			recover_flag = false;
		while (is_run())																											//ѭ�������������
		{
			msg = getmouse();
			n = p->action_judge(msg.x, msg.y);
			m = ((msg.is_up() && msg.is_left() == 1) ? 1 : 0);
      p->on_mouse_move(msg.x, msg.y, n);
      if(msg.is_down() && msg.is_left())
        p->on_mouse_click(n);

			if (n == PlayChess::ACTION_PLAY && m == 1 && !game_end_flag)								//����
			{
				play_button_click_audio();
				p->on_mouse_click(n);
				p->play_chess_by_man(msg.x, msg.y, player_type);
				if (p->show_outcome())
				{
					game_end_flag = true;
					continue;
				}
				cp.calc_next_step(row, col);
				p->play_chess_by_computer(row, col, computer_type);
				if (p->show_outcome())
				{
					game_end_flag = true;
					continue;
				}
				//c.show_chess();																						//�������
			}
			else if (n == PlayChess::ACTION_REPLAY && m == 1)							//���¿�ʼ
			{
				play_button_click_audio();
				delete p;
				break;
			}
			else if (n == PlayChess::ACTION_QUIT && m == 1)							//�˳���Ϸ
			{
				play_button_click_audio();
				GradientAnimation::transition_ease_in();
				if (game_end_flag == false && c.get_empty_grid_num() < Chess::SIZE * Chess::SIZE)
					ChessSaver::save_chess(c, player_type);
				return;
			}
		}
	}
}

void game_settings_interface()
{
	mouse_msg msg;
	int k;
	SettingsInterface settings_interface(game_settings);
	SettingsInterface::action_type settings_action;

	settings_interface.update_interface();
	while (is_run())
	{
		msg = getmouse();
		k = ((msg.is_up() && msg.is_left() == 1) ? 1 : 0);
		if (k == 1)
		{
			settings_action = settings_interface.action_judge(msg.x, msg.y);
			switch (settings_action)
			{
				case SettingsInterface::ACTION_SELECT_BLACK:
					play_button_click_audio();
					game_settings->set_piece_color(Chess::BLACK);
					break;
				case SettingsInterface::ACTION_SELECT_WHITE:
					play_button_click_audio();
					game_settings->set_piece_color(Chess::WHITE);
					break;
				case SettingsInterface::ACTION_SWITCH_AUDIO:
					if (game_settings->is_audio_on())
					{
						game_settings->set_audio(false);
						game_bgm->stop();
					}
					else
					{
						game_settings->set_audio(true);
						play_button_click_audio();
						game_bgm->start();
					}
					break;
				case SettingsInterface::ACTION_BACK:
					game_settings->write_settings();
					play_button_click_audio();
					return;
			}
			settings_interface.update_interface();
		}
	}
}

void game_curtain()
{
	mouse_msg msg;
	int k;
	const int offset = 10;

	WelcomeInterface::curtain();
	while (is_run())
	{
		msg = getmouse();
		k = (msg.is_up() && msg.is_left() ? 1 : 0);
		if (k == 1 && msg.x > 800 - offset && msg.x < 950 - offset && msg.y > 600 - offset && msg.y < 650 + offset)
		{
			play_button_click_audio();
			GradientAnimation::transition_ease_in();
			return;
		}
	}
}

void game_net_select_interface()
{
	NetSelectionInterface net_select_interface;
	mouse_msg msg;
	NetSelectionInterface::ACTION_TYPE action_type;
	bool back_flag = false;
	bool enter_flag = true;
	while (is_run())
	{
		if (enter_flag)
			net_select_interface.enter_interface();
		else
			net_select_interface.show_interface();
		back_flag = false;
		enter_flag = true;
		while (is_run())
		{
			msg = getmouse();
			action_type = net_select_interface.action_judge(msg.x, msg.y);
			net_select_interface.on_mouse_move(action_type);
			if (msg.is_down() && msg.is_left())
				net_select_interface.on_mouse_click(action_type);
			else if (msg.is_up() && msg.is_left())
			{
				switch (action_type)
				{
					case NetSelectionInterface::ACTION_OPEN_ROOM:
						if (!wait_open_room())
							enter_flag = false;
						back_flag = true;
						break;
					case NetSelectionInterface::ACTION_ENTER_ROOM:
						if (!wait_enter_room())
							enter_flag = false;
						back_flag = true;
						break;
					case NetSelectionInterface::ACTION_NET_SETTINGS:
						game_net_settings_interface();
						back_flag = true;
						enter_flag = false;
						break;
					case NetSelectionInterface::ACTION_BACK:
						GradientAnimation::transition_ease_in();
						return;
					default:
						break;
				}
				if (back_flag)
					break;
			}
		}
	}
}

void game_net_settings_interface()
{
	NetSettingsInterface settings_interface(game_settings);
	settings_interface.show_interface();
	mouse_msg msg;
	NetSettingsInterface::ACTION_TYPE action_type;
	EditDialog edit_dialog(500, 200);
	MessageDialog error_dialog(400, 150, MessageDialog::icon_error);
	error_dialog.set_title("Error");
	while (true)
	{
		msg = getmouse();
		action_type = settings_interface.action_judge(msg.x, msg.y);
		settings_interface.on_mouse_move(action_type);
		if (msg.is_down() && msg.is_left())
			settings_interface.on_mouse_click(action_type);
		if (msg.is_up() && msg.is_left())
		{
			switch (action_type)
			{
				case NetSettingsInterface::ACTION_SAVE:
					game_settings->write_settings();
					return;
				case NetSettingsInterface::ACTION_CANCEL:
					game_settings->read_settings();
					return;
				case NetSettingsInterface::ACTION_EDIT_USER_NAME:
					edit_dialog.set_title("�����û���");
					edit_dialog.set_text("�������µ��û�����\n�û����в��ܺ��пո�");
					edit_dialog.set_input_max_len(20);
					while (true)
					{
						edit_dialog.set_input_text(game_settings->get_user_name().c_str());
						edit_dialog.show();
						if (edit_dialog.get_response_type() == EditDialog::response_ok)
						{
							if (game_settings->set_user_name(edit_dialog.get_input_text()))
								break;
							else
							{
								error_dialog.set_text("������û������Ϸ������������롣");
								error_dialog.show();
							}
						}
						else
							break;
					}
					settings_interface.show_interface();
					break;
				case NetSettingsInterface::ACTION_EDIT_SERVER_PORT:
					edit_dialog.set_title("���÷���˶˿ں�");
					edit_dialog.set_text("�������µĶ˿ںţ���ΧΪ0-65535��\n��ע�ⲻҪ�뵱ǰϵͳ����ռ�õĶ˿ڳ�ͻ��");
					edit_dialog.set_input_max_len(5);
					while (true)
					{
						edit_dialog.set_input_text(game_settings->get_server_port().c_str());
						edit_dialog.show();
						if (edit_dialog.get_response_type() == EditDialog::response_ok)
						{
							if (game_settings->set_server_port(edit_dialog.get_input_text()))
								break;
							else
							{
								error_dialog.set_text("����Ķ˿ںŲ��Ϸ������������롣");
								error_dialog.show();
							}
						}
						else
							break;
					}
					settings_interface.show_interface();
					break;
				case NetSettingsInterface::ACTION_EDIT_CLIENT_CONNECT_IP:
					edit_dialog.set_title("���ÿͻ�������Ŀ��IP��ַ");
					edit_dialog.set_text("�������µ�IP��ַ����ʽΪa.b.c.d��\na��b��c��d�ķ�Χ��Ϊ0-255��");
					edit_dialog.set_input_max_len(15);
					while (true)
					{
						edit_dialog.set_input_text(game_settings->get_client_connect_IP_addr().c_str());
						edit_dialog.show();
						if (edit_dialog.get_response_type() == EditDialog::response_ok)
						{
							if (game_settings->set_client_connect_IP_addr(edit_dialog.get_input_text()))
								break;
							else
							{
								error_dialog.set_text("�����IP��ַ���Ϸ������������롣");
								error_dialog.show();
							}
						}
						else
							break;
					}
					settings_interface.show_interface();
					break;
				case NetSettingsInterface::ACTION_EDIT_CLIENT_CONNECT_PORT:
					edit_dialog.set_title("���ÿͻ�������Ŀ��˿ں�");
					edit_dialog.set_text("�������µĶ˿ںţ���ΧΪ0-65535��");
					edit_dialog.set_input_max_len(5);
					while (true)
					{
						edit_dialog.set_input_text(game_settings->get_client_connect_port().c_str());
						edit_dialog.show();
						if (edit_dialog.get_response_type() == EditDialog::response_ok)
						{
							if (game_settings->set_client_connect_port(edit_dialog.get_input_text()))
								break;
							else
							{
								error_dialog.set_text("����Ķ˿ںŲ��Ϸ������������롣");
								error_dialog.show();
							}
						}
						else
							break;
					}
					settings_interface.show_interface();
					break;
				default:
					break;
			}
		}
	}
}

bool wait_open_room()
{
	NetWaitingInterface wait_interface;
	wait_interface.set_tip_text("���ڴ������䣬���Ժ󡭡�");
	wait_interface.show_interface();
	mouse_msg msg;
	NetWaitingInterface::ACTION_TYPE action_type;
	while (is_run())
	{ 
		if (mousemsg())
		{
			msg = getmouse();
			action_type = wait_interface.action_judge(msg.x, msg.y);
			wait_interface.on_mouse_move(action_type);
			if (msg.is_down() && msg.is_left())
				wait_interface.on_mouse_click(action_type);
			else if (msg.is_up() && msg.is_left())
			{
				switch (action_type)
				{
					case NetWaitingInterface::ACTION_CANCEL:
						return false;
					default:
						break;
				}
			}
		}
		Sleep(2);
	}
	return false;
}

bool wait_enter_room()
{
	NetWaitingInterface wait_interface;
	wait_interface.set_tip_text("���ڳ��Խ��뷿�䣬���Ժ󡭡�");
	wait_interface.show_interface();
	mouse_msg msg;
	NetWaitingInterface::ACTION_TYPE action_type;
	while (true)
	{
		if (mousemsg())
		{
			msg = getmouse();
			action_type = wait_interface.action_judge(msg.x, msg.y);
			wait_interface.on_mouse_move(action_type);
			if (msg.is_down() && msg.is_left())
				wait_interface.on_mouse_click(action_type);
			else if (msg.is_up() && msg.is_left())
			{
				switch (action_type)
				{
					case NetWaitingInterface::ACTION_CANCEL:
						return false;
					default:
						break;
				}
			}
		}
		Sleep(2);
	}
	return false;
}
