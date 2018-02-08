//���������ļ��ƿ����������������
#ifndef CLASS_INTERFACE_H
#define CLASS_INTERFACE_H

#include <QObject>
#include <qtimer.h>
#include <qvector.h>
#include "curl/curl.h"
#include "group_downn.h"
#include "��ͷ.h"


class class_interface : public QObject
{
	Q_OBJECT

public:
	void restart_set();//�����Ƿ�ʱ����½�����
	bool return_time(); //�����Ƿ�ʱ����½�����
	static bool findByUrl(QString url_name, class_interface *&urll);//ͨ��url��ȡclassface
	static bool findByPathname(QString file_name, class_interface *&urll);
	static bool findByfilename(QString file_name, class_interface *&urll);
	void stop_pre();//�����Ƿ�ʱ����½�����
	void class_interface::start_pre();//�����Ƿ�ʱ����½�����
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
	QString file_name; //upload��·����
	QString path_name;
	bool is_upl(); //�Ƿ��ϴ�
	void set_isup(); //�����ϴ�
	int return_state(); //����״̬
	QString return_fileName(); //�����ļ���
	int update_length;
	bool is_from; //is_fromָ�����Ƿ�������ļ���ȡ�ļ�
	QTimer *timer;
	bool is_time;
	/**************************group���߳�********************************************/
	bool if_group;
	int thread_num; //�߳���Ŀ
	//typedef split_ sl;
	//split_ spl[4];
	//ÿ�������߳���������
	QVector<split_*>spl;
	qint64 size; //�ܴ�С����
	QVector<group_downn*>dwn; //���߳����ص�ÿ�����صĽ���������
	QWidget *wi; //group_widget����ʾʱ��
	void time_out_stop();
private:
	
	int time; //���̶߳��������һ�ν��档
	void start_time();
	void time_show_hide();
	int sta; //0ʧ��,1�ɹ����,2��ͣ,3δ��ʼ,4����
	qint64 ready_size; //�Ѿ���ɶ���
	bool is_upload;
	public slots:
	void timer_out();
};

#endif // CLASS_INTERFACE_H
