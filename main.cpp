// =====================================================================================
// 
//       Filename:  main.cpp
//
//    Description:  Sniffer 程序的主函数文件，仅 main 函数的实现
//
//        Version:  1.0
//        Created:  2013年01月20日 17时29分07秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// 处理中文乱码问题
	QTextCodec *codec = QTextCodec::codecForName("system");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);

	MainWindow *mainwindow = new MainWindow;
	
	mainwindow->showMaximized();

	return app.exec();
}

