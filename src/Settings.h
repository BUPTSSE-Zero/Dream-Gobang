#include"string"
using namespace std;
class Settings{
public:
	enum color
	{
		BLACK=1,
		WHITE=-1
	};//����Ϊ1������Ϊ-1��
	Settings();
	Settings(color color_);
	color getter();
	void setter(color color_);
	bool readfine();
	void writefine(color color_);
	static const string fill_name;
private:
	color color_;
	};