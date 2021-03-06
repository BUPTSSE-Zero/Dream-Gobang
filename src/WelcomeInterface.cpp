#include "WelcomeInterface.h"
#include "tools/GradientAnimation.h"
#include "Gobang.h"
#include "image_game_logo.h"
#include "image_game_welcome_bg.h"
#include "image_game_special_thanks_bg.h"
#include "image_button_enter_game.h"
#include "image_button_hover_enter_game.h"
#include "image_button_press_enter_game.h"
#include "image_button_game_settings.h"
#include "image_button_hover_game_settings.h"
#include "image_button_press_game_settings.h"
#include "image_button_exit_game.h"
#include "image_button_hover_exit_game.h"
#include "image_button_press_exit_game.h"
#include "image_button_special_thanks.h"
#include "image_button_net_playing.h"

ImageTextButton* WelcomeInterface::button_enter_game_ = NULL;
ImageTextButton* WelcomeInterface::button_game_settings_ = NULL;
ImageTextButton* WelcomeInterface::button_exit_game_ = NULL;
ImageButton* WelcomeInterface::button_special_thanks_ = NULL;
ImageButton* WelcomeInterface::button_net_play_ = NULL;
Image* WelcomeInterface::game_welcome_bg_ = NULL;

WelcomeInterface::WelcomeInterface()
{
	//加载背景图片
	if (!game_welcome_bg_)
		game_welcome_bg_ = new Image(binary_game_welcome_bg_jpg, sizeof(binary_game_welcome_bg_jpg));

	//加载所有按钮
	if (!button_enter_game_)
	{
		button_enter_game_ = new ImageTextButton();
		button_enter_game_->add_normal_image(new Image(binary_button_enter_game_png, sizeof(binary_button_enter_game_png)), 0, 0);
		button_enter_game_->set_normal_text("进 入 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_enter_game_->add_hover_image(new Image(binary_button_hover_enter_game_png, sizeof(binary_button_hover_enter_game_png)), 0, 0);
		button_enter_game_->set_hover_text("进 入 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_enter_game_->add_press_image(new Image(binary_button_press_enter_game_png, sizeof(binary_button_press_enter_game_png)), 0, 0);
		button_enter_game_->set_press_text("进 入 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
	}

	if (!button_game_settings_)
	{
		button_game_settings_ = new ImageTextButton();
		button_game_settings_->add_normal_image(new Image(binary_button_game_settings_png, sizeof(binary_button_game_settings_png)), 0, 0);
		button_game_settings_->set_normal_text("游 戏 设 置", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_game_settings_->add_hover_image(new Image(binary_button_hover_game_settings_png, sizeof(binary_button_hover_game_settings_png)), 0, 0);
		button_game_settings_->set_hover_text("游 戏 设 置", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_game_settings_->add_press_image(new Image(binary_button_press_game_settings_png, sizeof(binary_button_press_game_settings_png)), 0, 0);
		button_game_settings_->set_press_text("游 戏 设 置", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
	}

	if (!button_exit_game_)
	{
		button_exit_game_ = new ImageTextButton();
		button_exit_game_->add_normal_image(new Image(binary_button_exit_game_png, sizeof(binary_button_exit_game_png)), 0, 0);
		button_exit_game_->set_normal_text("退 出 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_exit_game_->add_hover_image(new Image(binary_button_hover_exit_game_png, sizeof(binary_button_hover_exit_game_png)), 0, 0);
		button_exit_game_->set_hover_text("退 出 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
		button_exit_game_->add_press_image(new Image(binary_button_press_exit_game_png, sizeof(binary_button_press_exit_game_png)), 0, 0);
		button_exit_game_->set_press_text("退 出 游 戏", button_text_size_, text_x_offset_, -1, Gobang::font_llt);
	}

	if (!button_special_thanks_)
	{
		button_special_thanks_ = new ImageButton();
		button_special_thanks_->add_normal_image(new Image(binary_button_special_thanks_png, sizeof(binary_button_special_thanks_png)), 0, 0);
	}
	if (!button_net_play_)
	{
		button_net_play_ = new ImageButton();
		button_net_play_->add_normal_image(new Image(binary_button_net_playing_png, sizeof(binary_button_net_playing_png)), 0, 0);
	}
}

void WelcomeInterface::opening_animation()												//显示开场动画
{
	static Image game_logo(binary_game_logo_png, sizeof(binary_game_logo_png));
	const int image_width = 500;
	const int image_height = 222;
	int i = 0, colorpos = 0, color = 0, deltaAlpha = 12, deltaColor = 4;
	for (color = 0; color <= 255; color += deltaColor, delay_fps(60))
	{
		if (color > 128)
			i += deltaAlpha;
		if (i > 255)
			i = 255;
		setbkcolor_f(EGERGB(color, color, color));										//由黑到白逐渐改变背景色
		cleardevice();
		/*由完全透明到不透明渲染Logo图片*/
		game_logo.show_image_with_alpha(640 - image_width / 2, 320 - image_height / 2, (double)i / 255.0);
	}

	delay_ms(1500);
	deltaAlpha *= -1;
	deltaColor *= -1;
	/*下面的动画为上面动画的逆过程*/
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

void WelcomeInterface::enter_interface()
{
	GradientAnimation::transition_ease_out(game_welcome_bg_);
	show_welcome();
}

void WelcomeInterface::show_welcome()
{
	setrendermode(RENDER_MANUAL);
	game_welcome_bg_->show_image(0, 0);

	for(int x = 0; is_run() && x <= 80; delay_fps(60), x += 10)
	{
		button_enter_game_->set_position(x, 350);
		button_enter_game_->show();
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60), x += 10)
	{
		button_game_settings_->set_position(x, 450);
		button_game_settings_->show();
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60), x += 10)
	{
		button_exit_game_->set_position(x, 550);
		button_exit_game_->show();
	}

	button_special_thanks_->set_position(1000, 10);
	button_special_thanks_->show();
	button_net_play_->set_position(Gobang::WINDOW_WIDTH - 20 - button_net_play_->get_width(),
															 	 Gobang::WINDOW_HEIGHT - 20 - button_net_play_->get_height());
	button_net_play_->show();
}

void WelcomeInterface::curtain()
{
	setbkmode(TRANSPARENT);
	static Image game_thanks_bg(binary_game_special_thanks_bg_jpg, sizeof(binary_game_special_thanks_bg_jpg));
	GradientAnimation::transition_ease_out(&game_thanks_bg);
	Gobang::set_font(Gobang::font_llt, 40, true);

	setcolor(WHITE);

	xyprintf(10, 80, "特别鸣谢");

	delay_ms(1000);

	outtextxy(30, 180, "周圣云 = 大神带你飞");

	delay_ms(1000);

	outtextxy(30, 300, "陆爽秋 = 球球的编码新时代");

	delay_ms(1000);

	outtextxy(30, 400, "桂欢 = 欢神帮帮忙");

	delay_ms(1000);

	outtextxy(30, 500, "苏月 = 速度君");

	delay_ms(1000);

	outtextxy(30, 600, "何宇 = 图标小达人");
	delay_ms(1000);

	outtextxy(800, 600, "返回");
}

WelcomeInterface::action_type WelcomeInterface::action_judge(int x, int y)
{
	const int offset = 10;
	if (button_enter_game_->is_mouse_in_button(x, y))
		return ACTION_ENTER_GAME;
	else if (button_game_settings_->is_mouse_in_button(x, y))
		return ACTION_GAME_SETTINGS;
	else if (button_exit_game_->is_mouse_in_button(x, y))
		return ACTION_EXIT_GAME;
	else if (button_special_thanks_->is_mouse_in_button(x, y))
		return ACTION_SPECIAL_THANKS;
	else if (button_net_play_->is_mouse_in_button(x, y))
		return ACTION_NET_PLAY;
	else
		return ACTION_NONE;
}

void WelcomeInterface::on_mouse_move(WelcomeInterface::action_type action)
{
	switch (action)
	{
		case WelcomeInterface::ACTION_NONE:
			if (button_enter_game_->get_hover_status())
				button_enter_game_->show();
			if (button_game_settings_->get_hover_status())
				button_game_settings_->show();
			if (button_exit_game_->get_hover_status())
				button_exit_game_->show();
			break;
		case WelcomeInterface::ACTION_ENTER_GAME:
			button_enter_game_->on_mouse_hover();
			break;
		case WelcomeInterface::ACTION_GAME_SETTINGS:
			button_game_settings_->on_mouse_hover();
			break;
		case WelcomeInterface::ACTION_EXIT_GAME:
			button_exit_game_->on_mouse_hover();
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
			button_enter_game_->on_mouse_click();
			break;
		case WelcomeInterface::ACTION_GAME_SETTINGS:
			button_game_settings_->on_mouse_click();
			break;
		case WelcomeInterface::ACTION_EXIT_GAME:
			button_exit_game_->on_mouse_click();
			break;
		default:
			break;
	}
}
