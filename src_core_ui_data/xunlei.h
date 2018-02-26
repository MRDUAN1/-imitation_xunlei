//�ܽ���
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
	void set_attribute();//���ÿؼ�����
	void xunlei::paintEvent(QPaintEvent *event);
	class_interface *interfaces; //�˵�����ʹ�õ�class_interfaces
	void resume_download(class_interface *&, QString); //�ָ�����
	void suspend_download(class_interface *&, QString); //��stringΪpathname����ͣ����
	void closeEvent(QCloseEvent *event);
	QSettings *settings;
	QFile *setting;
	QMenu *menu_2, *menu;
	QAction *open_file, *open_file_folder, *delete_mission, *start_, *stop_mission, *select_all, *restart, *copy_address, *attribute;
	void make_left_menu(); //�Ҽ��˵�
	void xunlei::mouseMoveEvent(QMouseEvent *event);
	void xunlei::mousePressEvent(QMouseEvent *event);
	QPoint windowPos, mousePos, dPos;
	Ui::xunleiClass ui;
	QMenu *menu_1;
	menu_ *meu;
	QMap<QPushButton*, QString>restart_path; //restart_path
	QMap<QPushButton*, QString>delete_button_map; //path_name delete_path�Ӱ�ť��ȡ·����
	QMap<QPushButton*, QString>restart_string; //path_name delete_path
	QMap<QString, QListWidgetItem*>interface_map; //Ϊ�˽����ȡlistwidgetitem���ļ�����
	QMap<QTimer*, QString>time_file_map; //�ļ���,��start_down_up�б�new timer���ļ�����ӳ��
	QMap<QString, QListWidgetItem*>listitem_filename_map; //Ϊ�˲˵���ȡ·����,upload��·��������·����
	QMap<QString, QString>fileName_urlName; //url��filenameӳ��
	QMap<QString, int>pathName_state;
	void read_setting();//������
	//�����������ļ���ȡ
	void set_setting(QString url_name, QString file_name, int state, qint64 size, qint64 already, bool is_upload, bool is_quit = false);
	public slots:
	void itemEnter(QListWidgetItem*);
	//����������
	void set_change(QString file_name, QString left_time, QString speed, long size_now, int num = -1);
	void restart_pause_open();
	void start_down_upload(QString file_name);
	void open_file_slot();
	void start_slot();
	void select_all_slot();
	void restart_slot();
	//��ʾ����
	void attribute_slot();
	void copy_address_slot();
	void stop_mission_slot();
	void open_file_folder_slot();
	void show_menu(const QPoint);
	void open_folder();
	void timer_out();
	//ɾ����ť�ۺ���
	void delete_pushbutton();
	//����Ѿ��������
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
	//���¶��߳�С������
	void update_progress(QProgressBar*, int);
	//���¶��߳�С�ܽ�����
	void update_progress_all(QProgressBar*, int);
};

#endif // XUNLEI_H
