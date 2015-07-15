#include "Interface.h"

void Interface::animation()																	//��ʾ��������
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
	

void Interface::welcome()						
{
	static Image game_welcom_bg("res/game-welcom-bg.jpg");
	game_welcom_bg.show_image(0, 0);
	static Image button_enter_game("res/button-enter-game.png");
	static Image button_game_settings("res/button-game-settings.png");
	static Image button_exit_game("res/button-exit-game.png");
	static Image button_special_thanks("res/button-special-thanks.png");

	setrendermode(RENDER_MANUAL);

	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcom_bg.show_image(0, 0);
		button_enter_game.show_image_with_alpha(x, 350, 0.85);
		x += 10;
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcom_bg.show_image(0, 0);
		button_enter_game.show_image_with_alpha(80, 350, 0.85);
		button_game_settings.show_image_with_alpha(x, 450, 0.85);
		x += 10;
	}
	for(int x = 0; is_run() && x <= 80; delay_fps(60)) 
	{
		game_welcom_bg.show_image(0, 0);
		button_enter_game.show_image_with_alpha(80, 350, 0.85);
		button_game_settings.show_image_with_alpha(80, 450, 0.85);
		button_exit_game.show_image_with_alpha(x, 550, 0.85);
		x += 10;
	}
	
	button_special_thanks.show_image_with_alpha(1000, 10, 1.0);
}


void Interface::set_welcome()
{
	static Image game_welcom_bg("res/game-welcom-bg.jpg");
	game_welcom_bg.show_image(0, 0);

	static Image settings_select_black("res/game-settings-select-black.png");
	settings_select_black.show_image_with_alpha(30, 350, 0.9);

	static Image settings_select_white("res/game-settings-select-white.png");
	settings_select_white.show_image_with_alpha(260, 350, 0.9);

	static Image settings_audio("res/game-settings-audio.png");
	settings_audio.show_image_with_alpha(30, 520, 0.9);

	static Image settings_back("res/game-settings-back.png");
	settings_back.show_image_with_alpha(260, 500, 0.9);
}


void Interface::Curtain()
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



