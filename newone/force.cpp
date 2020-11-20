#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <map>

enum state
{
	zerohour,
	ld,
	q,
	resize,
	blur,
	s,
	h
};

struct offstring
{
	static const std::string disclaimer;
};

int main(int argc , char *argv[])
{

	std::map<std::string ,state> braze;
	std::map<std::string ,cv::Mat> matmap;

	braze["quit"]   	= q;
	braze["exit"]   	= q;
	braze["q"]   		= q;
	braze["h"]   		= h;
	braze["help"]   	= h;
	braze["resize"]   	= resize;
	braze["blur"]   	= blur;
	braze["store"]   	= s;
	braze["s"]   		= s;
	braze["load"]   	= ld;
	braze["ld"]   		= ld;

	bool quitsignal = false;

	while(quitsignal == false) {
		std::string f;
		char com_r[200] = {};
		std::string filenm;
		std::string socalled;
		std::string socalledn2;
		int sq = 0, width = 0, height = 0;

		scanf("%[^\n]%*c" ,com_r);
		std::stringstream ff(com_r);
		ff >> f;

		switch(braze[f])
		{
			case q:
			quitsignal = !false;
			break;

			case h:
			std::cout << offstring::disclaimer <<  std::endl;
			break;

			case resize:
			ff >> socalled >> socalledn2 >> width  >> height;			
			cv::resize(matmap[socalled] ,matmap[socalledn2] ,cv::Size(width ,height));

			break;

			case blur:
			ff >> socalled >> socalledn2 >> sq;
			if (ff.fail()) { std::cerr << "turns out bad" << std::endl; break;}
			cv::blur(matmap[socalled] ,matmap[socalledn2] ,cv::Size(sq ,sq));
			break;

			case s:
			ff >> socalled >> filenm;
			cv::imwrite(filenm ,matmap[socalled] ,std::vector<int>{cv::IMWRITE_JPEG_QUALITY ,95});
			break;

			case ld:			
			ff >> socalled >> filenm;
			matmap[socalled] = cv::imread(filenm, cv::IMREAD_UNCHANGED);
			break;

			default:
			std::cout << "nothing" << std::endl;
			break;
		}
	}
	std::cout << "and then " <<  std::endl;
}

const std::string offstring::disclaimer = R"(load, ld
<name> - имя изображения, по имени оно доступно в других командах
<filename> - имя файла для загрузки

Загрузка 
the image в формате JPG

store, s
<name> - имя изображения
<filename> - имя файла для сохранениея

Сохранение изображения в формате jpg

blur
<from_name> - имя изображения
 <to_name> - имя изображения
<lsize> - размер основы lsize X lsize ,число. 

Сглаживание изображения

resize
<from_name> - имя изображения
<to_name> - имя изображения
<new_width>
<new_height>

Изменение размера изображения

help, h

Выводит справку о поддерживаемых командах

exit, quit, q

выход)";
