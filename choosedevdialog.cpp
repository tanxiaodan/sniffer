// =====================================================================================
// 
//       Filename:  choosedevdialog.cpp
//
//    Description:  选择适配器对话框类实现文件
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

#include <vector>

#include "choosedevdialog.h"
#include "Sniffer.h"

ChooseDevDialog::ChooseDevDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	// 固定对话框大小，不允许调整
	this->setFixedSize(this->width(),this->height()); 

	setWindowIcon(QIcon(":/res/images/corporation.png"));

	connect(helpButton, SIGNAL(clicked()), this, SLOT(helpDialog()));
	connect(netDevListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(setChoose(QListWidgetItem *)));
	connect(PromiscuousCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setPromiscuousFlag(int)));
	connect(beginCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setAutoBeginFlag(int)));
}

void ChooseDevDialog::addNetDevInfo()
{
	QListWidgetItem *elem;

	for (std::vector<NetDevInfo>::iterator index = sniffer->netDevInfo.begin();
			index < sniffer->netDevInfo.end(); ++index) {
		
		elem = new QListWidgetItem(QIcon(":/res/images/corporation.png"), 
									QString(index->strNetDevname.c_str()), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
		elem->setCheckState(Qt::Unchecked);
		devItemVector.push_back(elem);

		elem = new QListWidgetItem(QString(index->strNetDevDescribe.c_str()), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

		elem = new QListWidgetItem(QString(index->strIPV4FamilyName.c_str()), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

		elem = new QListWidgetItem(QString(index->strIPV4Addr.c_str()), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

		elem = new QListWidgetItem(QString(index->strIPV6FamilyName.c_str()), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

		elem = new QListWidgetItem(QString(index->strIPV6Addr.c_str()), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

		elem = new QListWidgetItem(QString("-----------------------------------------------"
										"-----------------------------------------------"), netDevListWidget);
		elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	}
}

void ChooseDevDialog::setChoose(QListWidgetItem *changedElem)
{
	iOpenDevNum = 0;
	
	int i = 1;
	if (changedElem->checkState() == Qt::Checked)
	{
		for (std::vector<QListWidgetItem *>::iterator index = devItemVector.begin();
			index < devItemVector.end(); ++index) {
			if (*index != changedElem) {
				(*index)->setCheckState(Qt::Unchecked);
			} else {
				iOpenDevNum = i;
			}
			i++;
		}
	}
}

void ChooseDevDialog::setPromiscuousFlag(int flag)
{
	bPromiscuous = (flag == Qt::Checked);
}

void ChooseDevDialog::setAutoBeginFlag(int flag)
{
	bAutoBegin = (flag == Qt::Checked);
}

void ChooseDevDialog::helpDialog()
{
	QMessageBox::information(this, tr("帮助信息 —— 选择网络适配器"),
				tr("<p><font size=4>以上列表中是您的计算机中所有活动的网络适配器。"
					"请选择一个适配器开始数据包捕获。</font></p>"
					"<p><font size=4>&nbsp;&nbsp;&nbsp;"
					"* 适配器的描述信息和 IP 地址也许能帮助您选择。</font></p>"
					"<p><font size=4>关于混杂模式：</FONT> </P>"
					"<p><font size=4>&nbsp;&nbsp;&nbsp;"
					"混杂模式就是接收所有经过网络适配器的数据包，包括不是发给本机的包。"
					"默认情况下网络适配器只把发给本机的包（包括广播包）传递给上层程序，其它的包一律丢弃。</font></p>"
					"<p><font size=4 color=#ff0000>&nbsp;&nbsp;&nbsp; "
					"* 如果您想在选择了捕获的适配器后设置过滤器，那么请不要勾选"
					"\"确定后直接开始捕获\"。</font></p>"));
}
