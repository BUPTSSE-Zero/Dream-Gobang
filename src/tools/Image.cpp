#include "Image.h"
#include "../Gobang.h"

Image::Image()
{
	image_ = newimage(Gobang::WINDOW_WIDTH, Gobang::WINDOW_HEIGHT);
	putimage(image_, 0, 0, NULL);
}

Image::Image(const char* file_path)
{
	image_ = newimage();
	getimage(image_, file_path);
}

Image::~Image()
{
	delimage(image_);
}

void Image::show_image(int x, int y) const
{
	putimage(x, y, image_);
}

void Image::show_image_with_alpha(int x, int y, double alaph) const
{
	//������Ϊ�޸�EGE��������ĺ���
	putimage_alpha(NULL, image_, alaph, x, y);
}
