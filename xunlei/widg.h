//多线程下载小进度条方框
#ifndef WIDG_H
#define WIDG_H

#include <QWidget>
#include "class_interface.h"

class widg : public QWidget
{
	Q_OBJECT

public:
	widg(QWidget *parent);
	~widg();
	//void mouseReleaseEvent(QMouseEvent * event);
	//void mousePressEvent(QMouseEvent * event);
	//void mouseMoveEvent(QMouseEvent * event);
	void enterEvent(QEvent * event);
	void leaveEvent(QEvent * event);
private:
	QString file_name;
	bool status;
	class_interface *cla;
	bool is_ok;
};

#endif // WIDG_H
