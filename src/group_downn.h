//多线程下载的每个下载的进度条界面
#ifndef GROUP_DOWNN_H
#define GROUP_DOWNN_H

#include <QWidget>
#include "ui_group_downn.h"

class group_downn : public QWidget
{
	Q_OBJECT

public:
	group_downn(QWidget *parent = 0);
	~group_downn();

	int already;
	int all;
	int get_max();
	public slots:
	void setvalue(int num);
	void set_all(int total);
private:
	Ui::group_downn ui;
signals:
	void emit_widget_num(QProgressBar*, int);
	void emit_widget_all(QProgressBar*, int);
};

#endif // GROUP_DOWNN_H
