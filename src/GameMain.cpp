#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "WelcomeInterface.h"
#include "SettingsInterface.h"
#include "NetSelectionInterface.h"
#include "NetSettingsInterface.h"
#include "NetWaitingInterface.h"
#include "NetPlayingInterface.h"
#include "tools/Music.h"
#include "Settings.h"
#include "widgets/MessageDialog.h"
#include "widgets/QuestionDialog.h"
#include "widgets/EditDialog.h"
#include "Gobang.h"
#include "tools/GradientAnimation.h"
#include "socket/ServerPlayer.h"
#include "socket/ServerPlayer.h"
#include <stdlib.h>
#include <direct.h>
#include <winuser.h>

Music* game_bgm = NULL;
Settings* game_settings = NULL;
LRESULT(CALLBACK *ege_wind_proc)(HWND, UINT, WPARAM, LPARAM lParam);			//ָ��EGE���ڴ�������ָ��

void main_loop();
void play_chess_interface();
void game_settings_interface();
void game_curtain();
void game_net_select_interface();
void game_net_settings_interface();
bool wait_open_room();
bool wait_enter_room();
void server_play(ServerPlayer* player, NetPlayerInfoView* self_player, NetPlayerInfoView* opposite_player);
void client_play(ClientPlayer* player, NetPlayerInfoView* self_player, NetPlayerInfoView* opposite_player);

void play_button_click_audio()
{
	static Music audio_button_click("res/button-click.wav", 1);
	if (game_settings && game_settings->is_audio_on())
		audio_button_click.start();
}

LRESULT CALLBACK wind_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool close_flag = false;
	if (message == WM_CLOSE)
	{
		if (!close_flag)
		{
			close_flag = true;
			QuestionDialog dialog(500, 150);
			dialog.set_title("Dream-Gobang");
			dialog.set_text("�Ƿ�Ҫ�˳� ���λ������塱 ��Ϸ��");
			dialog.show();
			if (dialog.get_response_result() == QuestionDialog::response_yes)
			{
				DestroyWindow(hWnd);
				return 0;
			}
			close_flag = false;
		}
		return 0;
	}
	if (message == WM_DESTROY)
		Gobang::remove_font_res();
	return ege_wind_proc(hWnd, message, wParam, lParam);
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
	setbkmode(TRANSPARENT);
	setcaption("�λ�������");

	WelcomeInterface::opening_animation();
	ege_wind_proc = (LRESULT(CALLBACK *)(HWND, UINT, WPARAM, LPARAM lParam))GetWindowLong(getHWnd(), GWL_WNDPROC);
	SetWindowLong(getHWnd(), GWL_WNDPROC, (LONG)wind_proc);
	Gobang::load_font_res();

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

void show_error_dialog(const char* text, bool is_error = true)
{
	MessageDialog* dialog;
	if (is_error)
		dialog = new MessageDialog(500, 150, MessageDialog::icon_error);
	else
		dialog = new MessageDialog(500, 150);
	dialog->set_text(text);
	dialog->show();
	delete dialog;
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
							Gobang::remove_font_res();
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
					case WelcomeInterface::ACTION_NET_PLAY:
					{
						play_button_click_audio();
						GradientAnimation::transition_ease_in();
						if (game_settings->get_user_name().length() <= 0)
						{
							EditDialog edit_dialog(500, 200);
							edit_dialog.set_title("�����û���");
							edit_dialog.set_text("�״ν��������ս��������һ���µ��û�����\n�û����в��ܺ��пո�");
							edit_dialog.set_input_max_len(Gobang::USER_NAME_MAX_LEN);
							while (true)
							{
								edit_dialog.set_input_text(game_settings->get_user_name().c_str());
								edit_dialog.show();
								if (edit_dialog.get_response_type() == EditDialog::response_ok)
								{
									if (edit_dialog.get_input_text().length() <= 0)
										show_error_dialog("�û�������Ϊ�ա�");
									else if (game_settings->set_user_name(edit_dialog.get_input_text()))
									{
										game_settings->write_settings();
										break;
									}
									else
										show_error_dialog("������û�����ʽ�������������롣");
								}
								else
									goto back_main;
							}
						}
						game_net_select_interface();
						back_main:;
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
					edit_dialog.set_input_max_len(Gobang::USER_NAME_MAX_LEN);
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
	ServerPlayer player;
	ServerPlayer::ServerMessage server_message;
	player.set_server_port(game_settings->get_server_port().c_str());
	player.start();
	player.accept_new_connection();
	int complete_step = 0;
	string opposite_name;
	while (is_run())
	{ 
		server_message = player.get_message();
		if (complete_step > 1 && complete_step < 3)
			player.receive();
		if (server_message.msg_type == ServerPlayer::MESSAGE_ERROR)
		{
			if (server_message.msg_content.error_msg == ServerPlayer::ERROR_SOCKET_BIND_FAILED)
				show_error_dialog("���������ʧ�ܣ��������������еķ������˿ں��Ƿ��ѱ���������ռ�á�");
			else
				show_error_dialog("���������ʧ�ܣ����Ժ����ԡ�");
			player.stop();
			return false;
		}
		else if (server_message.msg_type == ServerPlayer::MESSAGE_COMPLETE)
		{
			if (server_message.msg_content.complete_msg == ServerPlayer::COMPLETE_LISTEN)
			{
				wait_interface.set_tip_text("�����Ѵ��������ڵȴ�������Ҽ��롭��");
				wait_interface.show_interface();
				complete_step++;
			}
			else if (server_message.msg_content.complete_msg == ServerPlayer::COMPLETE_ACCEPT)
			{
				wait_interface.set_tip_text("������Ҽ��룬���ڻ�ȡ������Ϣ����");
				wait_interface.show_interface();
				if (game_settings->get_piece_color() == Chess::BLACK)
					player.send_chess_type(Chess::WHITE);
				else
					player.send_chess_type(Chess::BLACK);
				Sleep(100);
				complete_step++;
			}
			else if (server_message.msg_content.complete_msg == ServerPlayer::COMPLETE_RECEIVE)
			{
				ServerPlayer::ActionInfo action = player.get_opposite_action(server_message);
				if (action.action_type == ServerPlayer::ACTION_TELL_USER_NAME)
				{
					opposite_name = action.ex_data.user_name;
					complete_step++;
					player.clean_mission_queue();
					player.send_user_name(game_settings->get_user_name().c_str());
				}
			}
		}
		if (complete_step >= 3)
		{
			NetPlayerInfoView self_player(false);
			self_player.set_player_name(game_settings->get_user_name().c_str());
			self_player.set_piece_type(game_settings->get_piece_color());
			NetPlayerInfoView opposite_player(true);
			opposite_player.set_player_name(opposite_name.c_str());
			if (game_settings->get_piece_color() == Chess::WHITE)
				opposite_player.set_piece_type(Chess::BLACK);
			else
				opposite_player.set_piece_type(Chess::WHITE);
			Sleep(100);
			GradientAnimation::transition_ease_in();
			server_play(&player, &self_player, &opposite_player);
			return true;
		}
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
		else
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
	ClientPlayer::ClientMessage socket_msg;
	ClientPlayer player;
	player.set_server_IP_addr(game_settings->get_client_connect_IP_addr().c_str());
	player.set_server_port(game_settings->get_client_connect_port().c_str());
	player.start();
	int complete_step = 0;
	Chess::PieceType self_type;
	string opposite_name;
	while (is_run())
	{
		if (complete_step > 0 && complete_step < 3)
			player.receive();
		socket_msg = player.get_message();
		if (socket_msg.msg_type == ClientPlayer::MESSAGE_COMPLETE)
		{
			if (socket_msg.msg_content.complete_msg == ClientPlayer::COMPLETE_CONNECT)
			{
				wait_interface.set_tip_text("���ӳɹ������ڻ�ȡ������Ϣ����");
				wait_interface.show_interface();
				player.send_user_name(game_settings->get_user_name().c_str());
				Sleep(100);
				complete_step++;
			}
			else if (socket_msg.msg_content.complete_msg == ClientPlayer::COMPLETE_RECEIVE)
			{
				ClientPlayer::ActionInfo action = player.get_opposite_action(socket_msg);
				if (action.action_type == ClientPlayer::ACTION_TELL_USER_NAME)
				{
					opposite_name = action.ex_data.user_name;
					complete_step++;
				}
				else if (action.action_type == ClientPlayer::ACTION_TELL_PIECE_TYPE)
				{
					self_type = action.ex_data.piece_type;
					complete_step++;
				}
			}
		}
		else if (socket_msg.msg_type == ClientPlayer::MESSAGE_ERROR)
		{
			MessageDialog error_dialog(500, 150, MessageDialog::icon_error);
			show_error_dialog("����ʧ�ܣ���ȷ����������״�����ü�������������������ԡ�");
			player.stop();
			return false;
		}
		if (complete_step >= 3)
		{
			NetPlayerInfoView self_player(false);
			self_player.set_player_name(game_settings->get_user_name().c_str());
			self_player.set_piece_type(self_type);
			NetPlayerInfoView opposite_player(true);
			opposite_player.set_player_name(opposite_name.c_str());
			if (self_type == Chess::WHITE)
				opposite_player.set_piece_type(Chess::BLACK);
			else
				opposite_player.set_piece_type(Chess::WHITE);
			Sleep(100);
			GradientAnimation::transition_ease_in();
			client_play(&player, &self_player, &opposite_player);
			return true;
		}
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
						player.stop();
						return false;
					default:
						break;
				}
			}
		}
		else
			Sleep(2);

	}
	return false;
}

bool query_quit(const char* text)
{
	QuestionDialog dialog(500, 150);
	dialog.set_title("��ʾ");
	dialog.set_text(text);
	dialog.show();
	if (dialog.get_response_result() == QuestionDialog::response_yes)
		return true;
	return false;
}

void client_play(ClientPlayer* player, NetPlayerInfoView* self_player, NetPlayerInfoView* opposite_player)
{
	Chess chess;
	NetPlayingInterface play_interface(self_player, opposite_player, &chess);
	play_interface.enter_interface();
	mouse_msg msg;
	NetPlayingInterface::ACTION_TYPE action;
	ClientPlayer::ClientMessage socket_msg;
	while (is_run())
	{
		chess = Chess();
		self_player->set_ready_state(false);
		opposite_player->set_ready_state(false);
		play_interface.show_interface();
		player->clean_mission_queue();
		bool start_flag = false;
		bool is_self_playing = false;
		while (is_run())
		{
			player->receive();
			socket_msg = player->get_message();
			if (socket_msg.msg_type == ClientPlayer::MESSAGE_ERROR || !player->is_run())
			{
				show_error_dialog("���ӷ��������жϡ�");
				player->stop();
				GradientAnimation::transition_ease_in();
				return;
			}
			else if (socket_msg.msg_type == ClientPlayer::MESSAGE_COMPLETE && socket_msg.msg_content.complete_msg == ClientPlayer::COMPLETE_RECEIVE)
			{
				ClientPlayer::ActionInfo action_info = player->get_opposite_action(socket_msg);
				if (action_info.action_type == ClientPlayer::ACTION_QUIT)
				{
					show_error_dialog("�������˳���Ϸ��", false);
					player->stop();
					GradientAnimation::transition_ease_in();
					return;
				}
				if (action_info.action_type == ClientPlayer::ACTION_READY)
				{
					opposite_player->set_ready_state(true);
					play_interface.show_interface();
				}
				if (start_flag && !is_self_playing && action_info.action_type == ServerPlayer::ACTION_PLAY_CHESS)
				{
					play_interface.on_mouse_click(NetPlayingInterface::ACTION_PLAY_CHESS);
					chess.set_point(action_info.ex_data.chess_pos[0], action_info.ex_data.chess_pos[1],
						opposite_player->get_piece_type());
					play_interface.play_chess_by_opposite(action_info.ex_data.chess_pos[0], action_info.ex_data.chess_pos[1],
						opposite_player->get_piece_type());
					if (play_interface.show_result())
						break;
					is_self_playing = true;
				}
			}
			if (mousemsg())
			{
				msg = getmouse();
				action = play_interface.action_judge(msg.x, msg.y);
				play_interface.on_mouse_move(msg.x, msg.y, action);
				if (msg.is_down() && msg.is_left())
					play_interface.on_mouse_click(action);
				else if (msg.is_up() && msg.is_left())
				{
					switch (action)
					{
						case NetPlayingInterface::ACTION_READY:
							self_player->set_ready_state(true);
							player->clean_mission_queue();
							player->send_ready_message();
							play_interface.show_interface();
							break;
						case NetPlayingInterface::ACTION_QUIT:
							if (query_quit("ȷ��Ҫ�˳������ս��"))
							{
								player->clean_mission_queue();
								player->send_quit_message();
								while (is_run() && player->is_run())
								{
									socket_msg = player->get_message();
									if (socket_msg.msg_type == ClientPlayer::MESSAGE_COMPLETE && socket_msg.msg_content.complete_msg == ClientPlayer::COMPLETE_SEND)
									{
										player->stop();
										GradientAnimation::transition_ease_in();
										return;
									}
								}
							}
							break;
						case NetPlayingInterface::ACTION_PLAY_CHESS:
							if (!start_flag || !is_self_playing)
								break;
							int row, col;
							play_interface.on_mouse_click(NetPlayingInterface::ACTION_PLAY_CHESS);
							play_interface.mouse_to_coor(msg.x, msg.y, row, col);
							chess.set_point(row, col, self_player->get_piece_type());
							play_interface.play_chess_by_self(msg.x, msg.y, self_player->get_piece_type());
							player->clean_mission_queue();
							player->send_chess_pos(row, col);
							is_self_playing = false;
							if (play_interface.show_result())
								goto game_end;
							break;
						default:
							break;
					}
				}
			}
			else
				Sleep(5);
			if (!start_flag && self_player->is_ready() && opposite_player->is_ready())
			{
				start_flag = true;
				if (self_player->get_piece_type() == Chess::BLACK)
					is_self_playing = true;
				else
					is_self_playing = false;
			}
		}
	game_end:;
	}
}

void server_play(ServerPlayer* player, NetPlayerInfoView* self_player, NetPlayerInfoView* opposite_player)
{
	Chess chess;
	NetPlayingInterface play_interface(self_player, opposite_player, &chess);
	play_interface.enter_interface();
	mouse_msg msg;
	NetPlayingInterface::ACTION_TYPE action;
	ServerPlayer::ServerMessage socket_msg;
	while (is_run())
	{
		chess = Chess();
		self_player->set_ready_state(false);
		opposite_player->set_ready_state(false);
		play_interface.show_interface();
		player->clean_mission_queue();
		bool start_flag = false;
		bool is_self_playing = false;
		while (is_run())
		{
			player->receive();
			socket_msg = player->get_message();
			if (socket_msg.msg_type == ServerPlayer::MESSAGE_ERROR || !player->is_run())
			{
				show_error_dialog("���ӷ��������жϡ�");
				player->stop();
				GradientAnimation::transition_ease_in();
				return;
			}
			else if (socket_msg.msg_type == ServerPlayer::MESSAGE_COMPLETE && socket_msg.msg_content.complete_msg == ServerPlayer::COMPLETE_RECEIVE)
			{
				ServerPlayer::ActionInfo action_info = player->get_opposite_action(socket_msg);
				if (action_info.action_type == ServerPlayer::ACTION_QUIT)
				{
					show_error_dialog("�������˳���Ϸ��", false);
					player->stop();
					GradientAnimation::transition_ease_in();
					return;
				}
				if (action_info.action_type == ServerPlayer::ACTION_READY)
				{
					opposite_player->set_ready_state(true);
					play_interface.show_interface();
				}
				if (start_flag && !is_self_playing && action_info.action_type == ServerPlayer::ACTION_PLAY_CHESS)
				{
					play_interface.on_mouse_click(NetPlayingInterface::ACTION_PLAY_CHESS);
					chess.set_point(action_info.ex_data.chess_pos[0], action_info.ex_data.chess_pos[1],
						opposite_player->get_piece_type());
					play_interface.play_chess_by_opposite(action_info.ex_data.chess_pos[0], action_info.ex_data.chess_pos[1],
																								opposite_player->get_piece_type());
					if (play_interface.show_result())
						break;
					is_self_playing = true;
				}
			}
			if (mousemsg())
			{
				msg = getmouse();
				action = play_interface.action_judge(msg.x, msg.y);
				play_interface.on_mouse_move(msg.x, msg.y, action);
				if (msg.is_down() && msg.is_left())
					play_interface.on_mouse_click(action);
				else if (msg.is_up() && msg.is_left())
				{
					switch (action)
					{
						case NetPlayingInterface::ACTION_READY:
							self_player->set_ready_state(true);
							player->clean_mission_queue();
							player->send_ready_message();
							play_interface.show_interface();
							break;
						case NetPlayingInterface::ACTION_QUIT:
							if (query_quit("ȷ��Ҫ�˳������ս��"))
							{
								player->clean_mission_queue();
								player->send_quit_message();
								while (is_run() && player->is_run())
								{
									socket_msg = player->get_message();
									if (socket_msg.msg_type == ServerPlayer::MESSAGE_COMPLETE && socket_msg.msg_content.complete_msg == ServerPlayer::COMPLETE_SEND)
									{
										player->stop();
										GradientAnimation::transition_ease_in();
										return;
									}
								}
							}
							break;
						case NetPlayingInterface::ACTION_PLAY_CHESS:
							if (!start_flag || !is_self_playing)
								break;
							int row, col;
							play_interface.on_mouse_click(NetPlayingInterface::ACTION_PLAY_CHESS);
							play_interface.mouse_to_coor(msg.x, msg.y, row, col);
							chess.set_point(row, col, self_player->get_piece_type());
							play_interface.play_chess_by_self(msg.x, msg.y, self_player->get_piece_type());
							player->clean_mission_queue();
							player->send_chess_pos(row, col);
							is_self_playing = false;
							if (play_interface.show_result())
								goto game_end;
							break;
						default:
							break;
					}
				}
			}
			else
				Sleep(5);
			if (!start_flag && self_player->is_ready() && opposite_player->is_ready())
			{
				start_flag = true;
				if (self_player->get_piece_type() == Chess::BLACK)
					is_self_playing = true;
				else
					is_self_playing = false;
			}
		}
	game_end:;
	}
}
