#include "public_toolbutton.h"
#include "global.h"
#include "menu_.h"
public_toolbutton::public_toolbutton(QObject *parent)
: QObject(parent)
{
	
}
void public_toolbutton::set_detail()
{
	menu_1 = new QMenu();
	QAction *setting_tool = new QAction(menu_1);
	setting_tool->setText("设置");
	menu_1->addAction(setting_tool);
	QAction *shutdown_after_download = new QAction(menu_1);
	shutdown_after_download->setText("下载完后关机");
	menu_1->addAction(shutdown_after_download);
	QAction *about = new QAction(menu_1);
	about->setText("关于");
	menu_1->addAction(about);
	QAction *quit_tool = new QAction(menu_1);
	quit_tool->setText("退出");
	menu_1->addAction(quit_tool);
	connect(setting_tool, SIGNAL(triggered(bool)), this, SLOT(setting_tool_slot()));
	connect(shutdown_after_download, SIGNAL(triggered(bool)), this, SLOT(shutdown_after_download_slot()));
	connect(about, SIGNAL(triggered(bool)), this, SLOT(about_slot()));
	connect(quit_tool, SIGNAL(triggered(bool)), this, SLOT(quit_tool_slot()));
}
public_toolbutton::~public_toolbutton()
{

}
void public_toolbutton::setting_tool_slot()
{
	menu_ *url_get = new menu_;
	url_get->show();
}
void public_toolbutton::shutdown_after_download_slot()
{
	global::return_if_ok_shutdown() = true;
}
void public_toolbutton::about_slot()
{

}
void public_toolbutton::quit_tool_slot()
{
	exit(0);
}
