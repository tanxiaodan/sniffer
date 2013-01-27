// =====================================================================================
// 
//       Filename:  findqqdialog.cpp
//
//    Description:  捕获QQ信息对话框类实现文件
//
//        Version:  1.0
//        Created:  2013年01月26日 21时02分42秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "findqqdialog.h"

FindQQDialog::FindQQDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	// 固定对话框大小，不允许调整
	this->setFixedSize(this->width(),this->height());

	connect(infoPushButton, SIGNAL(clicked()), this, SLOT(showHelpInfo()));
}

void FindQQDialog::showHelpInfo()
{
	QMessageBox::information(this, tr("关于捕获QQ号码"),
				tr("<p><span style=\" font-size:12pt;\">"
					"工具简介：</span></p><p><span style=\" font-size:10pt;\">"
					"这个小工具可以发现局域网内进行通信的QQ号码。但由于受到通"
					"信方式的限制，现代网络常常采用交换机作为网络连接设备"
					"枢纽，在通常情况下，交换机不会让网络中每一台主机侦听"
					"到其他主机的通讯，因此Sniffer技术在这时必须结合网络端"
					"口镜像技术或者进行ARP欺骗等方式获取数据包。</span></p>"
					"<p><span style=\" font-size:10pt; color:#ff0000;\">* 因为实现"
					"ARP欺骗容易被杀毒软件识别为病毒，故在此不予实现，"
					"仅仅演示捕获，无线局域网一般可以嗅探全网段数据包。</span>"
					"</p>"));
}