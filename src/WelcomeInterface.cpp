#include "WelcomeInterface.h"

Image* WelcomeInterface::button_enter_game = NULL;
Image* WelcomeInterface::button_hover_enter_game = NULL;
Image* WelcomeInterface::button_press_enter_game = NULL;
Image* WelcomeInterface::text_enter_game = NULL;
Image* WelcomeInterface::button_game_settings = NULL;
Image* WelcomeInterface::button_hover_game_settings = NULL;
Image* WelcomeInterface::button_press_game_settings = NULL;
Image* WelcomeInterface::text_game_settings = NULL;
Image* WelcomeInterface::button_exit_game = NULL;
Image* WelcomeInterface::button_hover_exit_game = NULL;
Image* WelcomeInterface::button_press_exit_game = NULL;
Image* WelcomeInterface::text_exit_game = NULL;
Image* WelcomeInterface::button_special_thanks = NULL;
Image* WelcomeInterface::game_welcome_bg = NULL;

const double WelcomeInterface::button_alpha = 1.0;

WelcomeInterface::WelcomeInterface()
{
	if (!game_welcome_bg)
		game_welcome_bg = new Image("res/game-welcom-bg.jpg");

	if (!button_enter_game)
		button_enter_game = new Image("res/button-enter-game.png");
	if (!button_hover_enter_game)
		button_hover_enter_game = new Image("res/button-hover-enter-game.png");
	if (!button_press_enter_game)
		button_press_enter_game = new Image("res/button-press-enter-game.png");
	if (!text_enter_game)
		text_enter_game = new Image("res/text-enter-game.png");

	if (!button_game_settings)
		button_game_settings = new Image("res/button-game-settings.png");
	if (!button_hover_game_settings)
		button_hover_game_settings = new Image("res/button-hover-game-settings.png");
	if (!button_press_game_settings)
		button_press_game_settings = new Image("res/button-press-game-settings.png");
	if (!text_game_settings)
		text_game_settings = new Image("res/text-game-settings.png");

	if (!button_exit_game)
		button_exit_game = new Image("res/button-exit-game.png");
	if (!button_hover_exit_game)
		button_hover_exit_game = new Image("res/button-hover-exit-game.png");
	if (!button_press_exit_game)
		button_press_exit_game = new Image("res/button-press-exit-game.png");
	if (!text_exit_game)
		text_exit_game = new Image("res/text-exit-game.png");
	if (!button_special_thanks)
		button_special_thanks = new Image("res/button-special-thanks.png");

	button_enter_game_flag_ = button_game_settings_flag_ = button_exit_game_flag_ = false;
}

void WelcomeInterface::opening_animation()												//��ʾ��������
{
	//��ʾ��������
	static Image game_logo("res/game-logo.png");
	const int image_width = 500;
	const int image_height = 222;
	int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
	for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i > 255)
			i = 255;
		setbkcolor_f(EGERGB(color, color, color));										//�ɺڵ����𽥸ı䱳��ɫ
		cleardevice();
		/*����ȫ͸������͸����ȾLogoͼƬ*/
		game_logo.show_image_with_alpha(640 - image_width / 2, 320 - image_height / 2, (double)i / 255.0);
	}

	delay_ms(1500);
	deltaAlpha *= -1;
	deltaColor *= -1;
	/*����Ķ���Ϊ���涯���������*/
	for (color = 255, i = 255 + (-1) * deltaAlpha; color >= 0; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i < 0)
			i = 0;
		setbkcolor_f(EGERGB(color, color, color));
		cleardevice();
		game_logo.show_image_with_alpha(640 - image_width / 2, 320 - image_height / 2, (double)i / 255.0);
	}
	delay_ms(100);
}
	

void WelcomeInterface::show_welcome()						
{
	game_welcome_bg->show_image(0, 0);

	setrendermode(RENDER_MANUAL);

	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcome_bg->show_image(0, 0);
		button_enter_game->show_image_with_alpha(x, 350, button_alpha);
		text_enter_game->show_image_with_alpha(x + text_x_offset, 350 + text_y_offset, button_alpha);
		x += 10;
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcome_bg->show_image(0, 0);
		button_enter_game->show_image_with_alpha(80, 350, button_alpha);
		text_enter_game->show_image_with_alpha(80 + text_x_offset, 350 + text_y_offset, button_alpha);
		button_game_settings->show_image_with_alpha(x, 450, button_alpha);
		text_game_settings->show_image_with_alpha(x + text_x_offset, 450 + text_y_offset, button_alpha);
		x += 10;
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcome_bg->show_image(0, 0);
		button_enter_game->show_image_with_alpha(80, 350, button_alpha);
		text_enter_game->show_image_with_alpha(80 + text_x_offset, 350 + text_y_offset, button_alpha);
		button_game_settings->show_image_with_alpha(80, 450, button_alpha);
		text_game_settings->show_image_with_alpha(80 + text_x_offset, 450 + text_y_offset, button_alpha);
		button_exit_game->show_image_with_alpha(x, 550, button_alpha);
		text_exit_game->show_image_with_alpha(x + text_x_offset, 550 + text_y_offset, button_alpha);
		x += 10;
	}
	
	button_special_thanks->show_image_with_alpha(1000, 10, 1.0);
}

void WelcomeInterface::curtain()
{
	setbkmode(TRANSPARENT);
	static Image game_thanks_bg("res/game-special-thanks-bg.png");
	game_thanks_bg.show_image(0, 0);
	//setfont(40, 30, "����������");
	setfont(-40, 0, "΢���ź�");
	setcolor(WHITE);

	xyprintf(10, 80, "�ر���л");

	delay_ms(1000);

	outtextxy(30, 180, "��ʥ�� = ��������");

	delay_ms(1000);

	outtextxy(30, 300, "½ˬ�� = ����ı�����ʱ��");

	delay_ms(1000);

	outtextxy(30, 400, "�� = ������æ");

	delay_ms(1000);

	outtextxy(30, 500, "���� = �ٶȾ�");

	delay_ms(1000);

	outtextxy(30, 600, "���� = ͼ��С����");
	delay_ms(1000);

	outtextxy(800, 600, "����");

}

WelcomeInterface::action_type WelcomeInterface::action_judge(int x, int y)
{
	const int offset = 10;
	if (x > 80 - offset && x < 320 + offset && y > 350 - offset && y < 410 + offset)
		return ACTION_ENTER_GAME;
	else if (x > 80 - offset && x < 320 + offset && y >450 - offset && y < 510 + offset)
		return ACTION_GAME_SETTINGS;
	else if (x  > 80 - offset && x < 320 + offset && y>550 - offset &&  y < 610 + offset)
		return ACTION_EXIT_GAME;
	else if (x > 1000 - offset && x < 1200 + offset && y > 30 - offset && y < 230 + offset)
		return ACTION_SPECIAL_THANKS;
	else
		return ACTION_NONE;
}

void WelcomeInterface::on_mouse_move(WelcomeInterface::action_type action)
{
	switch (action)
	{
		case WelcomeInterface::ACTION_NONE:
			if (button_enter_game_flag_)
			{
				button_enter_game_flag_ = false;
				button_enter_game->show_image_with_alpha(80, 350, button_alpha);
				text_enter_game->show_image_with_alpha(80 + text_x_offset, 350 + text_y_offset, button_alpha);
			}
			if (button_game_settings_flag_)
			{
				button_game_settings_flag_ = false;
				button_game_settings->show_image_with_alpha(80, 450, button_alpha);
				text_game_settings->show_image_with_alpha(80 + text_x_offset, 450 + text_y_offset, button_alpha);
			}
			if (button_exit_game_flag_)
			{
				button_exit_game_flag_ = false;
				button_exit_game->show_image_with_alpha(80, 550, button_alpha);
				text_exit_game->show_image_with_alpha(80 + text_x_offset, 550 + text_y_offset, button_alpha);
			}
			break;
		case WelcomeInterface::ACTION_ENTER_GAME:
			if (button_enter_game_flag_ == false)
			{
				button_enter_game_flag_ = true;
				button_hover_enter_game->show_image_with_alpha(80, 350, button_alpha);
				text_enter_game->show_image_with_alpha(80 + text_x_offset, 350 + text_y_offset, button_alpha);
			}
			break;
		case WelcomeInterface::ACTION_GAME_SETTINGS:
			if (button_game_settings_flag_ == false)
			{
				button_game_settings_flag_ = true;
				button_hover_game_settings->show_image_with_alpha(80, 450, button_alpha);
				text_game_settings->show_image_with_alpha(80 + text_x_offset, 450 + text_y_offset, button_alpha);
			}
			break;
		case WelcomeInterface::ACTION_EXIT_GAME:
			if (button_exit_game_flag_ == false)
			{
				button_exit_game_flag_ = true;
				button_hover_exit_game->show_image_with_alpha(80, 550, button_alpha);
				text_exit_game->show_image_with_alpha(80 + text_x_offset, 550 + text_y_offset, button_alpha);
			}
			break;
		default:
			break;
	}
}

void WelcomeInterface::on_mouse_click(WelcomeInterface::action_type action)
{
	switch (action)
	{
		case WelcomeInterface::ACTION_ENTER_GAME:
			button_press_enter_game->show_image_with_alpha(80, 350, button_alpha);
			text_enter_game->show_image_with_alpha(80 + text_x_offset, 350 + text_y_offset, button_alpha);
			break;
		case WelcomeInterface::ACTION_GAME_SETTINGS:
			button_press_game_settings->show_image_with_alpha(80, 450, button_alpha);
			text_game_settings->show_image_with_alpha(80 + text_x_offset, 450 + text_y_offset, button_alpha);
			break;
		case WelcomeInterface::ACTION_EXIT_GAME:
			button_press_exit_game->show_image_with_alpha(80, 550, button_alpha);
			text_exit_game->show_image_with_alpha(80 + text_x_offset, 550 + text_y_offset, button_alpha);
			break;
		default:
			break;
	}
}
