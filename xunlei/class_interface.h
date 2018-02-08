//核心数据文件掌控下载所有相关数据
#ifndef CLASS_INTERFACE_H
#define CLASS_INTERFACE_H

#include <QObject>
#include <qtimer.h>
#include <qvector.h>
#include "curl/curl.h"
#include "group_downn.h"
#include "标头.h"


class class_interface : public QObject
{
	Q_OBJECT

public:
	void restart_set();//设置是否到时间更新进度条
	bool return_time(); //设置是否到时间更新进度条
	static bool findByUrl(QString url_name, class_interface *&urll);//通过url获取classface
	static bool findByPathname(QString file_name, class_interface *&urll);
	static bool findByfilename(QString file_name, class_interface *&urll);
	void stop_pre();//设置是否到时间更新进度条
	void class_interface::start_pre();//设置是否到时间更新进度条
	class_interface(QString, QString, int,QObject *parent = 0);
	class_interface(QObject *parent = 0);
	~class_interface();
	void set_change(QString = NULL, QString = NULL, int state = 0, qint64 size = 0, qint64 already_size = 0, bool isUpload = false);
	void get_change(QString&, QString&, int &state, qint64 & , qint64 &already_size );
	void findByUrl(QString url_name, QString);
	QString get_url();
	qint64 get_ready_size();
	void set_ready_size(int);
	void set_state(int);
	int get_size();
	CURL *curl;
	QString url_name;
	QString file_name; //upload是路径名
	QString path_name;
	bool is_upl(); //是否上传
	void set_isup(); //设置上传
	int return_state(); //返回状态
	QString return_fileName(); //设置文件名
	int update_length;
	bool is_from; //is_from指的是是否从配置文件读取文件
	QTimer *timer;
	bool is_time;
	/**************************group多线程********************************************/
	bool if_group;
	int thread_num; //线程数目
	//typedef split_ sl;
	//split_ spl[4];
	//每个下载线程下载详情
	QVector<split_*>spl;
	qint64 size; //总大小多少
	QVector<group_downn*>dwn; //多线程下载的每个下载的进度条界面
	QWidget *wi; //group_widget的显示时间
	void time_out_stop();
private:
	
	int time; //多线程多少秒更新一次界面。
	void start_time();
	void time_show_hide();
	int sta; //0失败,1成功完成,2暂停,3未开始,4正常
	qint64 ready_size; //已经完成多少
	bool is_upload;
	public slots:
	void timer_out();
};

#endif // CLASS_INTERFACE_H
