#ifndef IS_DELETE_H
#define IS_DELETE_H

#include <QWidget>
#include "ui_is_delete.h"

class is_delete : public QFrame
{
	Q_OBJECT

public:
	is_delete(QString file_name, QFrame *parent = 0);
	~is_delete();

private:
	void is_delete::paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	QPoint windowPos, mousePos, dPos;
	QString delete_name; //path_name
	Ui::is_delete ui;
	void set_linegradit();
	private slots:
	void delete_execute();
};

#endif // IS_DELETE_H
