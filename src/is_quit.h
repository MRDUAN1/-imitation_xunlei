#ifndef IS_QUIT_H
#define IS_QUIT_H

#include <QWidget>
#include "ui_is_quit.h"
#include <qdialog.h>

class is_quit : public QDialog
{
	Q_OBJECT

public:
	
	is_quit(QDialog *parent = 0);
	~is_quit();
	void findNowDownloading();
	public slots :
	void ok();
private:
	std::tuple<int, bool, bool, bool, int, int>globa;
	void is_quit::paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	bool is_twice;
	QPoint windowPos, mousePos, dPos;
	
	Ui::is_quit ui;
};

#endif // IS_QUIT_H
