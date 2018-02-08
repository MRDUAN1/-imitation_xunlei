//获取下载对话框
#ifndef GETURLL_H
#define GETURLL_H

#include <QWidget>
#include <qframe.h>
#include "ui_geturll.h"

class geturll : public QFrame
{
	Q_OBJECT

public:
	geturll(QFrame *parent = 0);
	~geturll();
	void set_size_name(QString, qlonglong);
	void set_url(QString);
	void set_detail_context(QString url_name, QString path_name, long size);
private:
	Ui::geturll ui;
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void getsize(const char* ,long&);
	bool if_change_item;
	QString str; //作用是获取file_dialog的文件
	QString path_name; //路径名称
	int index_num;
	QString file_name; //文件名称
	long sizez;
	QPoint windowPos, mousePos, dPos;
	bool is_upload;
	bool if_mult;
	private slots:
	void cancel();
	void checkbox_change2(int);
	void get_upload_file_path();
	void checkbox_change(int);
	void paintEvent(QPaintEvent *event);
	void start_immedicate();
	void file_dia();
	void current_index_change(const QString &);
	void text_change_slot();
	void geturll::get_header_detail(QString strUrl);
};

#endif // GETURLL_H
