// =====================================================================================
// 
//       Filename:  mainwindow.cpp
//
//    Description:  主窗口的类实现文件
//
//        Version:  1.0
//        Created:  2013年01月20日 17时06分16秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "mainwindow.h"
#include "listtreeview.h"
#include "sniffer.h"

#include "choosedevdialog.h"
#include "findqqdialog.h"

#ifndef PCAP_OPENFLAG_NOCAPTURE_LOCAL
	// Defines if the local adapter will capture its own generated traffic.
	#define PCAP_OPENFLAG_NOCAPTURE_LOCAL   8
#endif

MainWindow::MainWindow()		
{
	captureThread = NULL;

	sniffer = new Sniffer;
	settingInfo = new SettingInfo;

	createMainWeiget();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	readSettings();

	setWindowIcon(QIcon(":/res/ico/main.png"));
	setWindowTitle(tr("Sniffer -- 测试版"));
}

MainWindow::~MainWindow()		
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (isToContinue()) {
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}

void MainWindow::newFile()
{
	if (isToContinue()) {
		mainTreeView->rebuildInfo();
		sniffer->snifferDataVector.clear();
		setCurrentFile("");
	}
}

void MainWindow::open()
{
	if (isToContinue()) {
		QString fileName = QFileDialog::getOpenFileName(this,
							tr("打开"), ".",
							tr("Sniffer捕获数据文件 (*.sni)"));
		
		if (!fileName.isEmpty()) {
			loadFile(fileName);
		}
	}
}

void MainWindow::save()
{
	QString fileName = QFileDialog::getOpenFileName(this,
							tr("打开"), ".",
							tr("Sniffer捕获数据文件 (*.sni)"));
		
		if (!fileName.isEmpty()) {
			saveFile(fileName);
		}
}

void MainWindow::print()
{
	QPrintDialog printDialog;
	if (printDialog.exec() == QDialog::Accepted) {
		// To Do ...
	}
}

void MainWindow::chooseDev()
{
	ChooseDevDialog chooseDevDialog;

	chooseDevDialog.sniffer = sniffer;
	chooseDevDialog.addNetDevInfo();

	if (chooseDevDialog.exec() == QDialog::Accepted) {
		settingInfo->iOpenDevNum  = chooseDevDialog.iOpenDevNum;
		settingInfo->bPromiscuous = chooseDevDialog.bPromiscuous;
		settingInfo->bAutoBegin   = chooseDevDialog.bAutoBegin;

		if (settingInfo->iOpenDevNum > 0 && settingInfo->bAutoBegin == true) {
			begin();
		} else if (settingInfo->iOpenDevNum > 0) {
			beginAction->setEnabled(true);
		}
	}
}

void MainWindow::begin()
{
	if (mainTreeView->isChanged()) {
		int result = QMessageBox::warning(this, tr("Sniffer"),
						tr("<H3>看起来我们似乎已经捕获到了一些数据。</H3>"
							"您确定放弃这些数据开始新的捕获吗？"),
						QMessageBox::Yes | QMessageBox::No);
		
		if (result == QMessageBox::No) {
			return;
		} else {
			mainTreeView->rebuildInfo();
			sniffer->snifferDataVector.clear();
		}
	} 

	chooseDevAction->setEnabled(false);
	beginAction->setEnabled(false);
	filterAction->setEnabled(false);
	findQQAction->setEnabled(false);
	endAction->setEnabled(true);

	if (captureThread != NULL) {
		delete captureThread;
	}

	captureThread = new CaptureThread;
	captureThread->sniffer = sniffer;
	captureThread->mainTree = mainTreeView;

	bool bOpenSucceed = false;
	if (settingInfo->bPromiscuous == true) {
		bOpenSucceed = sniffer->openNetDev(settingInfo->iOpenDevNum);
	} else {
		bOpenSucceed = sniffer->openNetDev(settingInfo->iOpenDevNum, PCAP_OPENFLAG_NOCAPTURE_LOCAL);
	}

	if (bOpenSucceed == true) {
		sniffer->setDevsFilter("ip");
		captureThread->start();
	} else {
		QMessageBox::warning(this, tr("Sniffer"),
						tr("<H3>无法在您的机器上打开网络适配器接口。</H3>"
							"<p>很遗憾出现这样的结果，可能出现的原因有：\n"
							"<p>1. 不支持您的网卡，请到 <a href=\"http://winpcap.org\">"
							"http://winpcap.org</a> 查阅支持的硬件列表\n"
							"<p>2. 您的杀毒软件或者HIPS程序阻止本程序运行"), QMessageBox::Ok);
	}
}

void MainWindow::end()
{
	chooseDevAction->setEnabled(true);
	beginAction->setEnabled(true);
	filterAction->setEnabled(true);
	findQQAction->setEnabled(true);
	
	endAction->setEnabled(false);

	captureThread->stop();
}

void MainWindow::findQQ()
{
	FindQQDialog findQQDialog;

	findQQDialog.exec();
}

void MainWindow::filter()
{

}

void MainWindow::set()
{

}

void MainWindow::readSettings()
{
	QSettings settings("Sniffer.ini", QSettings::IniFormat);

	settingInfo->iOpenDevNum  = settings.value("iOpenDevNum", true).toInt();
	settingInfo->bPromiscuous = settings.value("bPromiscuous", true).toBool();
	settingInfo->bAutoBegin   = settings.value("bAutoBegin", true).toBool();
}

void MainWindow::writeSettings()
{
	QSettings settings("Sniffer.ini", QSettings::IniFormat);

	settings.setValue("iOpenDevNum", settingInfo->iOpenDevNum);
	settings.setValue("bPromiscuous", settingInfo->bPromiscuous);
	settings.setValue("bAutoBegin", settingInfo->bAutoBegin);
}

bool MainWindow::loadFile(const QString &fileName)
{
	/*if (!mainTreeView->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading canceled"), 2000);
        return false;
	}*/

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
	return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
	/*if (!mainTreeView->writeFile(fileName)) {
		this->statusBar()->showMessage(tr("Saving canceled"), 2000);
		return false;
	}*/

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);

	return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	//setWindowTitle();
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("关于 Sniffer"),
			tr("<h2>Sniffer 测试版</h2>"
				"<p>Copyright (C) 2013 Class 1107 of Computer Science and Technology"
				"<p>一个跨平台的网络数据嗅探&抓包程序，基于Qt 4.x 以及 libpcap 库"
				"（Linux下）和 Winpcap库（Windows 下）。"));
}

bool MainWindow::isToContinue()
{
	if (mainTreeView->isChanged()) {
		int result = QMessageBox::question(this, tr("Sniffer"),
						tr("<H3>看起来我们似乎已经捕获到了一些数据。</H3>"
							"您需要保存这些捕获的数据供以后分析使用吗？"),
						QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		
		if (result == QMessageBox::Yes) {
			return true;
		} else if (result == QMessageBox::Cancel) {
			return false;
		}
	}

	return true;
}

void MainWindow::sleep(unsigned int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);

	while( QTime::currentTime() < dieTime ) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

void MainWindow::createMainWeiget()
{
	mainTreeView    = new ListTreeView;
	explainTreeView = new QTreeView;
	originalEdit    = new QTextEdit;

	originalEdit->setReadOnly(true);

	rightSplitter = new QSplitter(Qt::Vertical);

	rightSplitter->addWidget(explainTreeView);
	rightSplitter->addWidget(originalEdit);

	mainSplitter = new QSplitter(Qt::Horizontal);

	mainSplitter->addWidget(mainTreeView);
	mainSplitter->addWidget(rightSplitter);

	this->setCentralWidget(mainSplitter);
}

void MainWindow::createActions()
{
	newAction = new QAction(tr("新建"), this);
	newAction->setIcon(QIcon(":/res/images/new.png"));
	newAction->setShortcut(QKeySequence::New);
	newAction->setStatusTip(tr("创建一个新的捕获（清空历史）"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openAction = new QAction(tr("打开"), this);
	openAction->setIcon(QIcon(":/res/images/open.png"));
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(tr("打开历史的一个捕获记录"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(tr("保存"), this);
	saveAction->setIcon(QIcon(":/res/images/save.png"));
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("保存本次捕获信息到文件"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	printAction = new QAction(tr("打印"), this);
	printAction->setIcon(QIcon(":/res/images/print.png"));
	printAction->setShortcut(tr("Ctrl+P"));
	printAction->setStatusTip(tr("打印当前捕获的数据"));
	connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

	exitAction = new QAction(tr("退出"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("退出程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	chooseDevAction = new QAction(tr("选择捕获网卡"), this);
	chooseDevAction->setIcon(QIcon(":/res/images/corporation.png"));
	chooseDevAction->setShortcut(tr("Ctrl+N"));
	chooseDevAction->setStatusTip(tr("选择要捕获的网卡"));
	connect(chooseDevAction, SIGNAL(triggered()), this, SLOT(chooseDev()));

	beginAction = new QAction(tr("开始捕获"), this);
	beginAction->setIcon(QIcon(":/res/images/begin.png"));
	beginAction->setShortcut(tr("Ctrl+B"));
	beginAction->setStatusTip(tr("开始捕获数据包"));
	beginAction->setEnabled(false);
	connect(beginAction, SIGNAL(triggered()), this, SLOT(begin()));

	endAction = new QAction(tr("停止捕获"), this);
	endAction->setIcon(QIcon(":/res/images/end.png"));
	endAction->setShortcut(tr("Ctrl+E"));
	endAction->setStatusTip(tr("停止捕获数据包"));
	endAction->setEnabled(false);
	connect(endAction, SIGNAL(triggered()), this, SLOT(end()));

	filterAction = new QAction(tr("过滤器设置"), this);
	filterAction->setIcon(QIcon(":/res/images/filter.png"));
	filterAction->setStatusTip(tr("使用过滤器来过滤捕获到的数据"));
	connect(filterAction, SIGNAL(triggered()), this, SLOT(filter()));

	findQQAction = new QAction(tr("捕获QQ号码"), this);
	findQQAction->setIcon(QIcon(":/res/images/findqq.png"));
	findQQAction->setStatusTip(tr("捕获数据包包含的QQ号码"));
	connect(findQQAction, SIGNAL(triggered()), this, SLOT(findQQ()));

	aboutAction = new QAction(tr("关于"), this);
	aboutAction->setIcon(QIcon(":/res/images/about.png"));
	aboutAction->setStatusTip(tr("关于信息"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("关于Qt"), this);
	aboutQtAction->setIcon(QIcon(":/res/images/aboutqt.png"));
	aboutQtAction->setStatusTip(tr("关于Qt信息"));
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	fileMenu = this->menuBar()->addMenu(tr("文件"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);

	fileMenu->addSeparator();
	fileMenu->addAction(printAction);

	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	toolsMenu = this->menuBar()->addMenu(tr("捕获"));
	toolsMenu->addAction(chooseDevAction);
	toolsMenu->addSeparator();
	toolsMenu->addAction(beginAction);
	toolsMenu->addAction(endAction);
	toolsMenu->addSeparator();
	toolsMenu->addAction(findQQAction);

	optionsMenu = this->menuBar()->addMenu(tr("选项"));
	optionsMenu->addAction(filterAction);

	this->menuBar()->addSeparator();

	helpMenu = this->menuBar()->addMenu(tr("帮助"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("文件"));
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(printAction);

	workToolBar = addToolBar(tr("捕获"));
	workToolBar->addAction(chooseDevAction);
	workToolBar->addSeparator();
	workToolBar->addAction(beginAction);
	workToolBar->addAction(endAction);
	workToolBar->addSeparator();
	workToolBar->addAction(findQQAction);

	setToolBar = addToolBar(tr("选项"));
	setToolBar->addAction(filterAction);

	infoToolBar = addToolBar(tr("帮助"));
	infoToolBar->addAction(aboutAction);
	infoToolBar->addAction(aboutQtAction);
}

void MainWindow::createStatusBar()
{
	statusLabel = new QLabel("Sniffer 测试版");
	statusLabel->setAlignment(Qt::AlignHCenter);
	statusLabel->setMinimumSize(statusLabel->sizeHint());

	this->statusBar()->addWidget(statusLabel);
}
