#include "xunlei.h"
#include "listitem.h"
#include "widg.h"
#include "is_quit.h"
#include "geturll.h"
#include "menu_.h"
#include "global.h"
#include "group_downn.h"
#include "tool_widget.h"
#include "public_toolbutton.h"
#include "system_tray.h"
#include "is_delete.h"
#include "libcurl_network.h"
#include <qmenu.h>
#include <qaction.h>
#include <qprogressbar.h>
#include <qfile.h>
#include <qlabel.h>
#include <QBitmap>
#include <qfile.h>
#include <QPainter>
#include <qtimer.h>
#include <qprocess.h>
#include <qdir.h>
#include <qevent.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qclipboard.h>
#include <string>
#include <qtextstream.h>
#include <algorithm>
#include <tuple>
using std::string;
using std::for_each;
# pragma execution_character_set("utf-8")
static xunlei *xunli_extern;
/******************************************/
static QMap<QPushButton*, QString>button_string;
static QMap<QMenu*, QString>meun_pathname; //qstring为path_name不论上传还是下载都是pathname
static is_quit *quit;
//0失败,1成功完成,2暂停,3未开始,4正常
xunlei::xunlei(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	xunli_extern = this;
	global::set_xunli_extern(xunli_extern);
	quit = new is_quit();
	meu = new menu_;
	meu->hide();
	make_left_menu();
	system_tray *sys = new system_tray(0);
	settings = new QSettings("listwidgetitem.ini", QSettings::IniFormat);
	setting = new QFile("listwidgetitem_setting.ini");
	setting->open(QIODevice::ReadWrite);
	read_setting();
	//set_setting(str, str1, hu);
	QDir *temp = new QDir;
	ui.frame_2->setObjectName("qframe");
	ui.frame_2->setStyleSheet("QFrame#qframe{border:1px solid rgb(97,192,199);};");
	bool exist = temp->exists("E:/downloads");
	if (!exist)
		bool ok = temp->mkdir("E:/downloads");
	QPixmap pixmap("./addd");
	ui.pushButton_2->setIcon(pixmap);
	QPixmap pixmapp("./filee");
	ui.pushButton_3->setIcon(pixmapp);
	set_attribute();
	
	set_toolbutton();
	global::return_if_ok_shutdown() = false;
	setconnect();
}
void xunlei::set_attribute()
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setWindowModality(Qt::WindowModal);
	tool_widget *tool = new tool_widget(this);
	tool->setGeometry(this->width() - tool->width(), 0, tool->width(), tool->height());
	tool->show();
	this->setFixedSize(QSize(this->width(), this->height()));
	setFixedSize(this->width(), this->height());
	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
	ui.pushButton->setCursor(QCursor(Qt::PointingHandCursor));
	ui.pushButton_2->setCursor(QCursor(Qt::PointingHandCursor));
	ui.pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));
	ui.listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}
void xunlei::setconnect()
{
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(clear_already()));
	connect(ui.listWidget, SIGNAL(customContextMenuRequested(const QPoint)), this, SLOT(show_menu(const QPoint)));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(new_task()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(menu__()));
}
void xunlei::itemEnter(QListWidgetItem *wi)
{
	QString str = listitem_filename_map.key(wi);
	QFileInfo fi(str);
	if (fileName_urlName.find(str) == fileName_urlName.end())
	{
		QMessageBox::warning(0, "警告", "在copy_address_solt\n中file_name_urlname错误");
		return;
	}
	QString url__name = fileName_urlName[str];
	class_interface::findByUrl(url__name, interfaces); //此处upload不会发生冲突
	if (!interfaces->if_group) return;
	QWidget *wid = wi->listWidget();
	QPoint po = wid->mapToGlobal(wid->pos());
	po.setX(po.x() + wid->size().width());
	wid->show();
	QObjectList li = interfaces->wi->children();
	for (int i = 0; i < li.size(); i++)
	{
		QWidget *widegt_grop = (QWidget*)li[i];
		widegt_grop->show();
	}
}
int isDigitStr(QString src)
{
	QByteArray ba = src.toLatin1();//QString 转换为 char*  
	const char *s = ba.data();
	while (*s && *s >= '0' && *s <= '9') s++;

	if (*s)
	{ //不是纯数字  
		return -1;
	}
	else
	{ //纯数字  
		return 0;
	}
}
//逻辑很强势
void xunlei::read_setting()
{
	setting->open(QIODevice::ReadWrite);
	QTextStream in(setting);
	QString ba;
	string str;
	while (1)
	{
		if ((ba = in.readLine()).isNull())
		{
			return;
		}
		str = ba.toStdString();
		int first_d = str.find(",");
		string str_url_name = str.substr(0, first_d);
		int second_d = str.find(",", first_d + 2);
		int len = second_d - first_d - 1;
		string file_namee = str.substr(first_d + 1, len);
		int third_d = str.find(",", second_d + 2);
		int len_2 = third_d - second_d - 1;
		string statee = str.substr(second_d + 1, len_2);
		int stte = atoi(statee.data());
		int forth_d = str.find(",", third_d + 2);
		int len_3 = forth_d - third_d - 1;
		string sizee = str.substr(third_d + 1, len_3);
		int sizzz = atoi(sizee.data());
		int fifth_d = str.find(",", forth_d + 2);
		int len_4 = fifth_d - forth_d - 1;
		string already_sizee = str.substr(forth_d + 1, len_4);
		int already_si = atoi(already_sizee.data());
		int sixth = str.find(",", fifth_d + 2);
		int len_5 = sixth - fifth_d - 1;
		string is_uploadd = str.substr(fifth_d + 1, len_5);
		int is_uupload = atoi(is_uploadd.data());
		class_interface *neww = new class_interface(QString::fromStdString(str_url_name), QString::fromStdString(file_namee), stte);
		neww->set_change(QString::fromStdString(str_url_name), QString::fromStdString(file_namee), stte, sizzz, already_si, is_uupload);
		set_list_widget(QString::fromStdString(file_namee), sizzz, QString::fromStdString(str_url_name), stte, already_si, is_uupload, true);
	}
}
void xunlei::quit_into_list()
{
	class_interface *clas;
	int num = ui.listWidget->count();
	for (int i = 0; i < num; i++)
	{
		QListWidgetItem *list_item = ui.listWidget->item(i);
		QWidget *wid = ui.listWidget->itemWidget(list_item);
		QProgressBar *barr = wid->findChild<QProgressBar*>("progressbar");
		int val = barr->value();
		QLabel *label_filename = wid->findChild<QLabel*>("file_name");
		QString filename = label_filename->text();
		class_interface::findByfilename(filename, clas);
		clas->set_ready_size(val);
		if (clas->return_state() == 4 || clas->return_state() == 3)
		{
			clas->set_state(2); //2代表暂停
		}
	}
}
//如果是多线程下载退出就不能用多线程下载
void xunlei::quit_save()
{
	setting->remove();
	setting = new QFile("listwidgetitem_setting.ini");
	for (int i = 0; i < global::return_listitem().size(); i++)
	{
		set_setting(global::return_listitem()[i]->get_url(), global::return_listitem()[i]->return_fileName(), global::return_listitem()[i]->return_state(), global::return_listitem()[i]->get_size(), global::return_listitem()[i]->get_ready_size(), global::return_listitem()[i]->is_upl(), true);
	}
}
void xunlei::set_setting(QString url_name, QString file_name, int state, qint64 size_, qint64 already, bool is_upload, bool is_quit)
{
	setting->close();
	setting->open(QIODevice::Append);
	setting->write(url_name.toLocal8Bit());
	setting->write(",");
	setting->write(file_name.toLocal8Bit());
	setting->write(",");
	setting->write(QString::number(state).toLocal8Bit());
	setting->write(",");
	setting->write(QString::number(size_).toLocal8Bit());
	setting->write(",");
	setting->write(QString::number(already).toLocal8Bit());
	setting->write(",");
	setting->write(QString::number(is_upload).toLocal8Bit());
	setting->write("\n");
	/**************************/
	/*********************/
	setting->flush();
	setting->close();
}
void xunlei::show_menu(const QPoint pos)
{//0失败,1成功完成,2暂停,3未开始,4正常
	QListWidgetItem *item = ui.listWidget->itemAt(pos);
	QString str = listitem_filename_map.key(item);
	QFileInfo fi(str);
	if (fileName_urlName.find(str) == fileName_urlName.end())
	{
		QMessageBox::warning(0, "警告", "在copy_address_solt\n中file_name_urlname错误");
		return;
	}
	QString url__name = fileName_urlName[str];
	class_interface::findByUrl(url__name, interfaces); //此处upload不会发生冲突
	int state = interfaces->return_state();
	if (state == 0)
	{
		open_file->setEnabled(false);
		start_->setEnabled(false);
		stop_mission->setEnabled(false);
		restart->setEnabled(true);
	}
	else if (state == 1)
	{
		if (interfaces->is_upl())
		{
			open_file_folder->setEnabled(false);
			open_file->setEnabled(false);
		}
		else
		{
			open_file->setEnabled(true);
			open_file_folder->setEnabled(true);
		}
		start_->setEnabled(false);
		stop_mission->setEnabled(false);
		restart->setEnabled(true);
	}
	else if (state == 2)
	{
		open_file->setEnabled(false);
		start_->setEnabled(true);
		stop_mission->setEnabled(false);
		restart->setEnabled(false);
	}
	else if (state == 4 || state == 3)
	{
		open_file->setEnabled(false);
		start_->setEnabled(false);
		stop_mission->setEnabled(true);
		restart->setEnabled(false);
	}
	if (str.isEmpty() || str.isNull())
	{
		QMessageBox::warning(0, "警告", "数据错误,在菜单listwidgetitem上");
		return;
	}
	meun_pathname.clear();
	meun_pathname[menu_2] = str;
	menu_2->move(cursor().pos());
	menu_2->exec();
}
void xunlei::make_left_menu()
{
	menu_2 = new QMenu(ui.listWidget);
	open_file = new QAction("打开文件", ui.listWidget);
	connect(open_file, SIGNAL(triggered()), this, SLOT(open_file_slot()));
	open_file_folder = new QAction("打开文件夹", ui.listWidget);
	connect(open_file_folder, SIGNAL(triggered()), this, SLOT(open_file_folder_slot()));
	start_ = new QAction("开始", ui.listWidget);
	connect(start_, SIGNAL(triggered()), this, SLOT(start_slot()));
	delete_mission = new QAction("删除任务", ui.listWidget);
	connect(delete_mission, SIGNAL(triggered()), this, SLOT(delete_mission_slot()));
	stop_mission = new QAction("停止任务", ui.listWidget);
	connect(stop_mission, SIGNAL(triggered()), this, SLOT(stop_mission_slot()));
	select_all = new QAction("选中全部", ui.listWidget);
	connect(select_all, SIGNAL(triggered()), this, SLOT(select_all_slot()));
	restart = new QAction("重新开始", ui.listWidget);
	connect(restart, SIGNAL(triggered()), this, SLOT(restart_slot()));
	copy_address = new QAction("拷贝地址", ui.listWidget);
	connect(copy_address, SIGNAL(triggered()), this, SLOT(copy_address_slot()));
	attribute = new QAction("属性", ui.listWidget);
	connect(attribute, SIGNAL(triggered()), this, SLOT(attribute_slot()));
	menu_2->addAction(open_file);
	menu_2->addAction(open_file_folder);
	menu_2->addAction(start_);
	menu_2->addAction(stop_mission);
	menu_2->addAction(delete_mission);
	menu_2->addAction(select_all);
	menu_2->addAction(restart);
	menu_2->addAction(copy_address);
	menu_2->addAction(attribute);
}
void xunlei::open_file_slot()
{
	QAction *obj = (QAction*)sender();
	QString strrr = meun_pathname[menu_2];
	strrr.replace("/", "\\");
	QProcess::startDetached("explorer " + strrr);
}
//开始下载
void xunlei::start_slot()
{
	QAction *obj = (QAction*)sender();
	QList<QTimer*> li;
	QString strrr = meun_pathname[menu_2]; //路径名
	class_interface::findByPathname(strrr, interfaces);
	interfaces->set_state(4);
	QFileInfo fi(strrr);
	QString str = fi.fileName();
	bool is_upload = interfaces->is_upl();
	if (interfaces->is_from == true)
	{
		goto if_from;
	}
	if (is_upload)
	{
		//interfaces->set_ready_size(0);
		if (global::return_upload_map().find(str) == global::return_upload_map().end())
		{
			QMessageBox::warning(NULL, "警告", "数据错误\n，upload_map查找不到表");
			return;
		}
		HANDLE upload_handle = global::return_upload_map()[str];
		ResumeThread(upload_handle);
	}
	else
	{
		//interfaces->set_ready_size(0);
		if (global::return_download_map().find(str) == global::return_download_map().end())
		{
			QMessageBox::warning(NULL, "警告", "数据错误\n，upload_map查找不到表");
			return;
		}
		HANDLE download_handle = global::return_download_map()[str];
		ResumeThread(download_handle);
	}
	li = time_file_map.keys();
	QTimer *time = li[0];
	time->start(10000);
	return;
if_from:
	interfaces->is_from = false;
	libcurl_network *lib = new libcurl_network(0);
	QString strr = fileName_urlName[strrr]; //获取url名
	lib->set_urls(strr); //设置url名 
	lib->test_if_ftp();
	is_upload = interfaces->is_upl();
	//interfaces->set_ready_size(0);
	int siz = interfaces->get_size();
	interfaces->set_state(4);
	if (!is_upload) //如果是下载
	{
		lib->start_download(str, strrr, siz, true, interfaces->get_ready_size(), 0);
	}
	else //如果是上传
	{
		lib->start_upload(strrr, interfaces->update_length, true, true, interfaces->get_ready_size());
	}
}
void xunlei::select_all_slot()
{

}
//此处内存未释放,class_interface内存泄漏
void xunlei::restart_slot()
{
	QAction *obj = (QAction*)sender();
	QString strrr = meun_pathname[menu_2]; //路径名
	class_interface::findByPathname(strrr, interfaces);
	libcurl_network *lib = new libcurl_network(0);
	QFileInfo fii(strrr);
	QListWidgetItem *item;
	if (interfaces->is_upl()) //如果是upload什么事都不做否则取文件
	{
		item = interface_map.value(strrr);
	}
	else
	{
		item = interface_map.value(fii.fileName());
	}
	ui.listWidget->takeItem(ui.listWidget->row(item));
	if (fileName_urlName.find(strrr) == fileName_urlName.end())
	{
		QMessageBox::warning(0, "警告", "在copy_address_solt\n中file_name_urlname错误");
		return;
	}
	QString str = fileName_urlName[strrr]; //获取url名
	lib->set_urls(str); //设置url名 
	lib->test_if_ftp();
	bool is_upload = interfaces->is_upl();
	interfaces->set_ready_size(0);
	int siz = interfaces->get_size();
	interfaces->set_state(4);
	if (!interfaces->is_from) //如果是从read_setting里
	{
		QTimer *time = time_file_map.key(fii.fileName());
		time->stop();
		auto ii = time_file_map.find(time);
		time_file_map.erase(ii);
	}
	if (!is_upload) //下载
	{
		lib->start_download(fii.fileName(), strrr, siz);
	}
	else
	{
		lib->start_upload(strrr, interfaces->update_length, true);
	}
}
void xunlei::attribute_slot()
{
	geturll *get = new geturll;
	QString strrr = meun_pathname[menu_2];
	if (fileName_urlName.find(strrr) == fileName_urlName.end())
	{
		QMessageBox::warning(0, "警告", "在copy_address_solt\n中file_name_urlname错误");
		return;
	}
	class_interface *intt;
	class_interface::findByUrl(fileName_urlName[strrr], intt);
	get->set_detail_context(fileName_urlName[strrr], strrr, intt->get_size());
	get->show();
}
void xunlei::copy_address_slot()
{
	QClipboard *clip = QApplication::clipboard();
	QString strrr = meun_pathname[menu_2];
	//QFileInfo fi(strrr);
	//QString file_name = fi.fileName();
	if (fileName_urlName.find(strrr) == fileName_urlName.end())
	{
		QMessageBox::warning(0, "警告", "在copy_address_solt\n中file_name_urlname错误");
		return;
	}
	QString url_name = fileName_urlName[strrr];
	clip->setText(url_name);
}
void xunlei::delete_mission_slot()
{
	QString pathName = meun_pathname[menu_2];
	is_delete *delete_choice = new is_delete(pathName, 0); //此处为pathname
	delete_choice->show();
}
void xunlei::stop_mission_slot()
{
	QAction *obj = (QAction*)sender();
	QString strrr = meun_pathname[menu_2];
	class_interface *inte;
	class_interface::findByPathname(strrr, inte);
	suspend_download(inte, strrr);
}
void xunlei::suspend_download(class_interface *&inte, QString str)
{
	QFileInfo fi(str);
	QString strr;
	if (inte->is_upl())
	{
		strr = str;
	}
	else
	{
		strr = fi.fileName();
	}
	QListWidgetItem *it = interface_map[strr];
	QWidget *wid = ui.listWidget->itemWidget(it);
	QProgressBar *barr = wid->findChild<QProgressBar*>("progressbar");
	int alredy = barr->value();
	inte->set_ready_size(alredy);
	inte->set_state(2);
	bool is_upload = inte->is_upl();
	if (is_upload)
	{
		QFileInfo fi(strr);
		if (global::return_upload_map().find(fi.fileName()) == global::return_upload_map().end())
		{
			QMessageBox::warning(NULL, "警告", "数据错误\n，upload_map查找不到表");
			return;
		}
		HANDLE upload_handle = global::return_upload_map()[fi.fileName()];
		SuspendThread(upload_handle);
	}
	else
	{
		if (global::return_download_map().find(strr) == global::return_download_map().end())
		{
			QMessageBox::warning(NULL, "警告", "数据错误\n，upload_map查找不到表");
			return;
		}
		HANDLE download_handle = global::return_download_map()[strr];
		SuspendThread(download_handle);
	}
	QList<QTimer*> li = time_file_map.keys();
	QTimer *time = li[0];
	time->stop();
	
}
void xunlei::closeEvent(QCloseEvent *event)
{
	if (std::get<3>(global::return_tuple()))
	{
		event->ignore();
		this->hide();
		return;
	}
	quit->exec();
	event->ignore();
	
	
	
}
void xunlei::open_file_folder_slot()
{
	QAction *obj = (QAction*)sender();
	QString strrr = meun_pathname[menu_2];
	QFileInfo fi(strrr);
	QString path = fi.absolutePath();
	path.replace("/", "\\");
	QProcess::startDetached("explorer " + path);
	
}
void xunlei::paintEvent(QPaintEvent *event)
{
	QBitmap bitmap(this->size());
	QPainter painter(&bitmap);
	painter.fillRect(bitmap.rect(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.setRenderHint(QPainter::Antialiasing);//平滑
	painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	painter.drawRoundedRect(this->rect(), 10, 10);
	this->setMask(bitmap);//可设置不同的形	
}
void xunlei::mousePressEvent(QMouseEvent *event)
{
	this->windowPos = this->pos();                // 获得部件当前位置
	this->mousePos = event->globalPos();     // 获得鼠标位置
	this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}

void xunlei::mouseMoveEvent(QMouseEvent *event)
{
	this->move(event->globalPos() - this->dPos);
}
void xunlei::set_change(QString file_name, QString left_time, QString speed, long size_now, int num)
{
	QTimer* timerr = time_file_map.key(file_name);
	class_interface *untt;
	class_interface::findByPathname(file_name, untt);
	
	timerr->start(20000);
	if (file_name.isNull()) {
		QMessageBox::warning(NULL, "警告", "数据错误");
		return;
	}
	QListWidgetItem *it = interface_map[file_name];
	QWidget *wid = ui.listWidget->itemWidget(it);
	QProgressBar *barr = wid->findChild<QProgressBar*>("progressbar");
	double size = barr->maximum();
	double siii = size_now;
	QLabel *state = wid->findChild<QLabel*>("state");
	if (untt->get_size() == 1)
	{
		state->setText("进度条更新失败，正在处理中");
		return;
	}
	if (!state)
	{
		QMessageBox::warning(NULL, "警告", "数据错误");
		return;
	}
	if (untt->return_state() == 1)
	{
		return;
	}
	if (siii / size >= 0.99)
	{
		barr->setValue(size);
		timerr->stop();
		state->setText("传输完成");
		foreach(untt, global::return_listitem())
		{
			if (untt->file_name == file_name)
			{
				
				if (untt->is_upl())
				state->setText("上传完成");
				untt->set_state(1);
				global::return_um_all()--;
				untt->set_ready_size(size);
			}
		}
	}
	else
	{
		barr->setValue(size_now);
		state->setText(left_time);
		//untt->set_ready_size(size_now);
	}
	QLabel *lab_speed = wid->findChild<QLabel*>("size");
	lab_speed->setText(speed);
}
void xunlei::timer_out()
{
	QTimer *timer = (QTimer*)sender();
	if (time_file_map.find(timer) == time_file_map.end())
		return;
	QString file_name = time_file_map[timer];
	if (interface_map.find(file_name) == interface_map.end())
		return;
	QListWidgetItem *list = interface_map[file_name];
	QWidget *wi = ui.listWidget->itemWidget(list);
	QLabel *size = wi->findChild<QLabel*>("size");
	size->setText("未知大小");
	QLabel *state = wi->findChild<QLabel*>("state");
	state->setText("下载失败");
	state->setStyleSheet("font-color:red");
	wi->setStyleSheet("background-color:rgb(255, 252, 240);");
	timer->stop();
}
void xunlei::start_down_upload(QString file_name)
{
	QTimer *timer = new QTimer;
	timer->start(20000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timer_out()));
	time_file_map[timer] = file_name; //如果是upload是path_name, 否则是file_name
	QListWidgetItem *it = interface_map[file_name];
	QWidget *wid = ui.listWidget->itemWidget(it);
	QLabel *lab = new QLabel(wid);
	lab->setObjectName("time");
	lab->setStyleSheet("border:none");
	lab->show();
	lab->setGeometry(280, 40, 150, 20);
	lab->setFrameStyle(0);
}
void xunlei::open_folder()
{
	QPushButton *bu = (QPushButton*)sender();
	if (button_string.find(bu) == button_string.end())
	{
		return;
	}
	QString str = button_string[bu];
	QFileInfo fi(str);
	QString stt = fi.absoluteFilePath();
	int h = QProcess::startDetached("explorer " + stt);
}
void xunlei::is_fishished_(QString file_name, bool is_ok)
{
	bool size_zero = false;
	QTimer* timerr = time_file_map.key(file_name);
	class_interface *untt;
	class_interface::findByPathname(file_name, untt);
	if (is_ok == 0)
	{
		return;
	}
	timerr->stop();
	if (file_name.isNull()) {
		QMessageBox::warning(NULL, "警告", "数据错误");
		return;
	}
	QListWidgetItem *it = interface_map[file_name];
	QWidget *wid = ui.listWidget->itemWidget(it);
	QProgressBar *barr = wid->findChild<QProgressBar*>("progressbar");
	double size = barr->maximum();
	
	if (size == 0.0)
	{
		barr->setMaximum(1);
		size_zero = true;
	}
	QLabel *state = wid->findChild<QLabel*>("state");
	if (!state)
	{
		QMessageBox::warning(NULL, "警告", "数据错误");
		return;
	}
	if (untt->return_state() == 1)
	{
		return;
	}
	if (!size_zero)
	{
		barr->setValue(size);
	}
	else
	{
		barr->setValue(1);
	}
	timerr->stop();
	state->setText("传输完成");
	foreach(untt, global::return_listitem())
	{
		if (untt->file_name == file_name)
		{
			if (untt->is_upl())
				state->setText("上传完成");
				untt->set_state(1);
				global::return_um_all()--;
				untt->set_ready_size(size);
			}
		}
	untt->time_out_stop();
}
//0失败,1成功完成,2暂停,3未开始
//如果是upload str就是路径名
void xunlei::set_list_widget(QString str, long sizez, QString url_name, int statee, qint64 ready_size, bool is_upload, bool is_begin, bool is_mul)
{
	bool should_inert_list_item = true;
	class_interface *inty;
	QString path_url;
	if (is_upload)
	{
		path_url = str;
	}
	else
	{
		path_url = global::return_folder_path() + "/" + str;
	}
	QFile file(str);
	fileName_urlName[path_url] = url_name;
	QListWidgetItem *it = new QListWidgetItem(ui.listWidget);
	foreach(inty, global::return_listitem())
	{
		if (inty->file_name == str)
		{
			should_inert_list_item = false;
		}
	}
	if (should_inert_list_item)
	{
		class_interface *list_item_ = new class_interface(url_name, str, statee);
		list_item_->path_name = path_url;
		list_item_->if_group = false;
		if (is_mul)
		{
			list_item_->wi = new QWidget;
			list_item_->wi->setWindowFlags(Qt::FramelessWindowHint);
			list_item_->wi->setAttribute(Qt::WA_TranslucentBackground);
			list_item_->wi->setFixedSize(100, 130);
			for (int i = 0; i < 4; i++)
			{
				group_downn *dow = new group_downn(list_item_->wi);
				list_item_->dwn.push_back(dow);
			}
		}
		list_item_->is_from = false;
		if (statee == 2)
		{
			list_item_->is_from = true;
		}
		if (is_begin)
			list_item_->is_from = true;
		list_item_->set_change(url_name, str, statee, sizez, ready_size, is_upload);
		global::return_listitem().append(list_item_);
	}
	ui.listWidget->addItem(it);
	it->setSizeHint(QSize(680, 65));
	widg *wi = new widg(0);
	wi->setMouseTracking(true);
	wi->setObjectName("hello");
	wi->setStyleSheet("QWidget#hello{background-color:rgb(240, 248, 255);}");
	wi->setObjectName("listitem_widget");
	wi->setFixedSize(680, 65);
	//当大小获取失败为0
	QProgressBar *bar = new QProgressBar(wi);
	bar->setObjectName("progressbar");
	bar->setGeometry(0, 0, 680, 65);
	bar->setMinimum(0);  // 最小值
	if (sizez == -1)
	{
		sizez = 1;
	}
	bar->setMaximum(sizez);  // 最大值
	bar->setValue(0);
	bar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	bar->setStyleSheet("QProgressBar{background:transparent;border-radius: 5px;} QProgressBar::chunk{background-color:rgb(210,233,247,180);}");
	QLabel *pict = new QLabel(wi);
	pict->setStyleSheet("border:none");
	pict->setObjectName("pict");
	pict->setGeometry(10, 15, 51, 35);
	QLabel *file_name = new QLabel(wi);
	file_name->setStyleSheet("border:none");
	file_name->setObjectName("file_name");
	file_name->setFrameStyle(0);
	//file_name->setStyleSheet()
	file_name->setGeometry(70, 10, 281, 16);
	file_name->setText(file.fileName());
	QLabel *size = new QLabel(wi);
	size->setStyleSheet("border:none"); //第一个未知大小正常下载是速度与总值的比如果已经完成则是已经完成的大小
	size->setFrameStyle(0);
	size->setObjectName("size");
	size->setText("未知大小");
	size->setGeometry(70, 40, 202, 15);
	QLabel *state = new QLabel(wi);
	state->setStyleSheet("border:none");
	state->setFrameStyle(0);
	state->setObjectName("state");
	state->setGeometry(180, 40, 202, 15);
	QPushButton *restart = new QPushButton(wi); //重新开始的控件或暂停或开始的控件，需要有程序来
	restart->setStyleSheet("border:none");
	restart->setGeometry(420, 23, 41, 28);
	pathName_state[path_url] = statee;
	restart_path[restart] = url_name;
	connect(restart, SIGNAL(clicked()), this, SLOT(restart_pause_open()));
	QPixmap pix("./image/disk_icon.png");
	pix = pix.scaled(51, 35);
	pict->setPixmap(pix);
	QPushButton *folder = new QPushButton(wi);
	button_string[folder] = path_url;
	connect(folder, SIGNAL(clicked()), this, SLOT(open_folder()));
	folder->setGeometry(530, 23, 41, 38);
	QPixmap pixx("./image/file_folder");
	pixx = pixx.scaled(41, 38);
	folder->setIcon(pixx);
	folder->setStyleSheet("border:none");
	QPushButton *delete_button = new QPushButton(wi);
	delete_button->setGeometry(605, 23, 41, 28);
	delete_button->setStyleSheet("border:none");
	QLabel *restart_label = new QLabel(wi);
	restart_label->setStyleSheet("border:none");
	restart_label->setGeometry(465, 30, 30, 15);
	QLabel *folder_label = new QLabel(wi);
	folder_label->setStyleSheet("border:none");
	ui.listWidget->setItemWidget(it, wi);
	folder_label->setGeometry(565, 30, 30, 15);
	folder_label->setText("文件目录");
	connect(delete_button, SIGNAL(clicked()), this, SLOT(delete_pushbutton()));
	delete_button_map[delete_button] = path_url;
	interface_map[str] = it;
	listitem_filename_map[path_url] = it;
	/*************************/
	if (statee == 4) return;
	else if (statee == 0)
	{
		restart_label->setText("重新开始");
		state->setText("下载失败");
		state->setStyleSheet("font-color:red");
		wi->setStyleSheet("background-color:rgb(255, 252, 240);");//失败
	}
	else if (statee == 1)
	{
		//已经完成，应该设置已经完成的线,应该设置下载成功的线为绿线
		bar->setValue(sizez);
		restart_label->setText("打开");
		state->setText("下载成功");
	}
	else if (statee == 2)
	{
		bar->setValue(ready_size);
		state->setText("任务暂停");
		//暂停,设置进度条
	}
	
}
//开始->暂停(已经暂停点击后开始)，重新下载->暂停(暂停意味着正在下载)，完成了就打开，，暂停->下载失败，暂停->开始(正常下载后点击暂停)
void xunlei::restart_pause_open()
{
	QPushButton *bu = (QPushButton *)sender();
	if (restart_path.find(bu) == restart_path.end())
	{
		QMessageBox::warning(this, "警告", "数据异常");
		return;
	}
	QString file_name = restart_path[bu];
	class_interface *inter;
	class_interface::findByUrl(file_name, inter);
	QString path_name__ = inter->path_name;
	QListWidgetItem *it = listitem_filename_map[inter->path_name];
	QWidget *wi = ui.listWidget->itemWidget(it);
	QProgressBar *barr = wi->findChild<QProgressBar*>("progressbar");
	QLabel *state = wi->findChild<QLabel*>("state");
	//0失败,1成功完成,2暂停,3未开始
	int state_ = pathName_state[inter->path_name];
	//0失败,1成功完成,2暂停,3未开始,4正常
	if (state_ == 0)
	{
		state->setText("暂停");
		
	}
	else if (state_ == 1)
	{
		state->setText("打开");
		wi->setStyleSheet("background-color:rgb(237, 249, 238)");
		QProcess::startDetached("explorer " + path_name__);
	}
	else if (state_ == 2)
	{
		state->setText("暂停");
		barr->setValue(inter->get_ready_size());
		barr->setStyleSheet("background-color:rgb(200, 241, 246)");
		resume_download(inter, NULL);
	}
	else if (state_ == 3 || state_ == 4)
	{
		state->setText("开始");
		suspend_download(inter, path_name__);
	}
}
void xunlei::resume_download(class_interface *&intr, QString str)
{
	HANDLE rs = global::return_download_map()[intr->file_name];
	ResumeThread(rs);
	intr->set_state(4);
	QListWidgetItem *it = listitem_filename_map[intr->path_name];
	QWidget *wi = ui.listWidget->itemWidget(it);
	QProgressBar *barr = wi->findChild<QProgressBar*>("progressbar");
	QLabel *state = wi->findChild<QLabel*>("state");
	state->setText("暂停");
}
void xunlei::delete_list_widget(QString file_name)
{
	QListWidgetItem *it = interface_map[file_name];
	ui.listWidget->takeItem(ui.listWidget->row(it));
}
void xunlei::delete_pushbutton()
{
	QPushButton *bu = (QPushButton *)sender();
	if (delete_button_map.find(bu) == delete_button_map.end())
	{
		QMessageBox::warning(this, "警告", "数据异常");
		return;
	}
	QString file_name = delete_button_map[bu];
	is_delete *dl = new is_delete(file_name, 0);
	dl->show();
}
void xunlei::set_toolbutton()
{
	menu = new QMenu();
	QAction *batch_task = new QAction(menu);
	batch_task->setText("新建批量任务");
	menu->addAction(batch_task);
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
	ui.toolButton->setMenu(menu);
	ui.toolButton->setPopupMode(QToolButton::MenuButtonPopup);
	ui.toolButton_2->setMenu(menu_1);
	ui.toolButton_2->setPopupMode(QToolButton::MenuButtonPopup);
	connect(setting_tool, SIGNAL(triggered(bool)), this, SLOT(setting_tool_slot()));
	connect(shutdown_after_download, SIGNAL(triggered(bool)), this, SLOT(shutdown_after_download_slot()));
	connect(about, SIGNAL(triggered(bool)), this, SLOT(about_slot()));
	connect(quit_tool, SIGNAL(triggered(bool)), this, SLOT(quit_tool_slot()));
}
void xunlei::setting_tool_slot()
{
	geturll *url_get = new geturll;
	url_get->show();
}
void xunlei::shutdown_after_download_slot()
{
	global::return_if_ok_shutdown() = true;
}
void xunlei::about_slot()
{

}
void xunlei::quit_tool_slot()
{
	exit(0);
}
void xunlei::new_task()
{
	geturll *url_get = new geturll;
	url_get->show();

}
void xunlei::menu__()
{
	meu->show();
}
void xunlei::clear_already()
{
	for (QVector<class_interface*>::iterator it = global::return_listitem().begin(); it != global::return_listitem().end();)
	{
		if ((*it)->return_state() == 1)
		{
			delete_list_widget((*it)->file_name);
			it = global::return_listitem().erase(it);
		}
		else
		{
			it++;
		}
	}
	
}
/*******************群组的设置********************************/
void xunlei::update_progress(QProgressBar*bar, int num)
{
	int i = bar->maximum();
	bar->setValue(num);
}
void xunlei::update_progress_all(QProgressBar* bar, int total)
{
	bar->setMinimum(0);  // 最小值
	bar->setMaximum(total);  // 最大值
	bar->setValue(0);
	bar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
/**********************群组的设置********************************/
void xunlei::delete_timer(QString str)
{
	QTimer *time = time_file_map.key(str);
	time->stop();
	time_file_map.remove(time);
}
xunlei::~xunlei()
{

}

