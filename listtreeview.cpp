// =====================================================================================
// 
//       Filename:  listtreeview.cpp
//
//    Description:  主窗口数据捕获控件的类实现文件
//
//        Version:  1.0
//        Created:  2013年01月22日 14时53分49秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "listtreeview.h"

ListTreeView::ListTreeView()
{
	mainModel = new QStandardItemModel;

	mainModel->setColumnCount(6);
	mainModel->setHeaderData(0, Qt::Horizontal, tr("序号"));
	mainModel->setHeaderData(1, Qt::Horizontal, tr("时间"));
	mainModel->setHeaderData(2, Qt::Horizontal, tr("源IP地址"));
	mainModel->setHeaderData(3, Qt::Horizontal, tr("目标IP地址"));
	mainModel->setHeaderData(4, Qt::Horizontal, tr("协议"));
	mainModel->setHeaderData(5, Qt::Horizontal, tr("发送长度"));

	QStandardItem *item;

	item = new QStandardItem(QString("1"));
	mainModel->setItem(0, 0, item);
	item = new QStandardItem(QString("17:00:00"));
	mainModel->setItem(0, 1, item);
	item = new QStandardItem(QString("192.168.0.1"));
	mainModel->setItem(0, 2, item);
	item = new QStandardItem(QString("192.168.0.255"));
	mainModel->setItem(0, 3, item);
	item = new QStandardItem(QString("HTTP"));
	mainModel->setItem(0, 4, item);
	item = new QStandardItem(QString("20"));
	mainModel->setItem(0, 5, item);

	this->setModel(mainModel);
}

ListTreeView::~ListTreeView()
{

}

void ListTreeView::rebuildInfo()
{
	mainModel->clear();

	mainModel->setColumnCount(6);
	mainModel->setHeaderData(0, Qt::Horizontal, tr("序号"));
	mainModel->setHeaderData(1, Qt::Horizontal, tr("时间"));
	mainModel->setHeaderData(2, Qt::Horizontal, tr("源IP地址"));
	mainModel->setHeaderData(3, Qt::Horizontal, tr("目标IP地址"));
	mainModel->setHeaderData(4, Qt::Horizontal, tr("协议"));
	mainModel->setHeaderData(5, Qt::Horizontal, tr("发送长度"));
}

bool ListTreeView::isChanged()
{
	QList<QStandardItem *> tmp = mainModel->findItems(tr("1"));

	if (tmp.size() != 0) {
		return true;
	}

	return false;
}
