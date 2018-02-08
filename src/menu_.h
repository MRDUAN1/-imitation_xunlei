#ifndef MENU__H
#define MENU__H

#include <QWidget>
#include "ui_menu_.h"
#include <tuple>
using std::tuple;
class menu_ : public QFrame
{
	Q_OBJECT

public:
	menu_(QFrame *parent = 0);
	~menu_();

private:
	Ui::menu_ ui;
	//QString folder_path;
	//tuple<int, bool, bool, bool, int, int>globa;
	void closeEvent(QCloseEvent *event);
	void set_connect();
	void set_name();
	//读取配置文件设置
	void read_setting();
	void menu_::paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	QPoint windowPos, mousePos, dPos;
	private slots:
	void currentindex(QString&);
	void ok_pushbutton();
	void cancel_pushbutton();
	void filedialog_pushbutton();
	void if_toggled_2();
	void if_toggled();
	void start_test();
};

#endif // MENU__H
