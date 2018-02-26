//总界面
#ifndef XUNLEI_H
#define XUNLEI_H

#include <QtWidgets/QWidget>
#include <qaction.h>
#include <qvector.h>
#include <qsettings.h>
#include <qfile.h>
#include "ui_xunlei.h"
#include "class_interface.h"
#include "menu_.h"
class xunlei : public QWidget
{
	Q_OBJECT

public:
	xunlei(QWidget *parent = 0);
	void delete_list_widget(QString);
	~xunlei();
	void quit_save();
	void set_list_widget(QString str, long size, QString url_name, int state = 3, qint64 ready_size = 0, bool is_upload = false, bool is_begin = false, bool is_size = 0);
	void quit_into_list();
	void delete_timer(QString);
private:
	void setconnect(); 
	void set_attribute();//设置控件属性
	void xunlei::paintEvent(QPaintEvent *event);
	class_interface *interfaces; //菜单正在使用的class_interfaces
	void resume_download(class_interface *&, QString); //恢复下载
	void suspend_download(class_interface *&, QString); //此string为pathname，暂停下载
	void closeEvent(QCloseEvent *event);
	QSettings *settings;
	QFile *setting;
	QMenu *menu_2, *menu;
	QAction *open_file, *open_file_folder, *delete_mission, *start_, *stop_mission, *select_all, *restart, *copy_address, *attribute;
	void make_left_menu(); //右键菜单
	void xunlei::mouseMoveEvent(QMouseEvent *event);
	void xunlei::mousePressEvent(QMouseEvent *event);
	QPoint windowPos, mousePos, dPos;
	Ui::xunleiClass ui;
	QMenu *menu_1;
	menu_ *meu;
	QMap<QPushButton*, QString>restart_path; //restart_path
	QMap<QPushButton*, QString>delete_button_map; //path_name delete_path从按钮获取路径名
	QMap<QPushButton*, QString>restart_string; //path_name delete_path
	QMap<QString, QListWidgetItem*>interface_map; //为了界面获取listwidgetitem的文件名用
	QMap<QTimer*, QString>time_file_map; //文件名,在start_down_up中被new timer与文件名的映射
	QMap<QString, QListWidgetItem*>listitem_filename_map; //为了菜单获取路径用,upload是路径名都是路径名
	QMap<QString, QString>fileName_urlName; //url到filename映射
	QMap<QString, int>pathName_state;
	void read_setting();//读设置
	//从设置配置文件读取
	void set_setting(QString url_name, QString file_name, int state, qint64 size, qint64 already, bool is_upload, bool is_quit = false);
	public slots:
	void itemEnter(QListWidgetItem*);
	//进度条更新
	void set_change(QString file_name, QString left_time, QString speed, long size_now, int num = -1);
	void restart_pause_open();
	void start_down_upload(QString file_name);
	void open_file_slot();
	void start_slot();
	void select_all_slot();
	void restart_slot();
	//显示属性
	void attribute_slot();
	void copy_address_slot();
	void stop_mission_slot();
	void open_file_folder_slot();
	void show_menu(const QPoint);
	void open_folder();
	void timer_out();
	//删除按钮槽函数
	void delete_pushbutton();
	//清除已经完成下载
	void clear_already();
	void new_task();
	void menu__();
	void set_toolbutton();
	void delete_mission_slot();
	void setting_tool_slot();
	void shutdown_after_download_slot();
	void about_slot();
	void quit_tool_slot();
	void is_fishished_(QString, bool);
	//更新多线程小进度条
	void update_progress(QProgressBar*, int);
	//更新多线程小总进度条
	void update_progress_all(QProgressBar*, int);
};

#endif // XUNLEI_H
