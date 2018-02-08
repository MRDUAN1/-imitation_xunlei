#include "is_delete.h"
#include "xunlei.h"
#include "global.h"
#include "class_interface.h"
#include "tool_widget.h"
#include <qpainter.h>
#include <qbitmap.h>
#include <qevent.h>
#include <qfileinfo.h>
#include <process.h>
#include <qmessagebox.h>
# pragma execution_character_set("utf-8")

is_delete::is_delete(QString file_name, QFrame *parent)
: QFrame(parent)
{
	ui.setupUi(this);
	tool_widget *tool = new tool_widget(this);
	ui.label_2->setText("确定删除该任务");
	ui.checkBox->setText("同时删除下载文件");
	QPixmap pix("./image/gantanhao2.png");
	pix = pix.scaled(55, 55);
	ui.label->setPixmap(pix);
	this->setObjectName("qframe");
	this->setStyleSheet("QFrame#qframe{border: 1px solid rgb(97, 192, 199);background-color:rgb(240, 248,255)};");
	tool->setGeometry(this->width() - tool->width(), 0, tool->width(), tool->height());
	tool->show();
	this->setAttribute(Qt::WA_ShowModal, true);
	this->setFixedSize(QSize(this->width(), this->height()));
	delete_name = file_name;
	this->setFixedSize(this->width(), this->height());
	connect(ui.delete_button, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(delete_execute()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}
void is_delete::paintEvent(QPaintEvent *event)
{
	QBitmap bitmap(this->size());
	QPainter painter(&bitmap);
	painter.fillRect(bitmap.rect(), Qt::white);
	painter.setBrush(QColor(0, 0, 0));
	painter.setRenderHint(QPainter::Antialiasing);//平滑
	painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	painter.drawRoundedRect(this->rect(), 10, 10);
	this->setMask(bitmap);//可设置不同的形	
}
void is_delete::set_linegradit()
{
	QLinearGradient *line = new QLinearGradient(0, 0, 360, 40);
	line->setColorAt(0, QColor(qRgb(214, 232, 249)));
	line->setColorAt(1, QColor(qRgb(155, 202, 246)));
}
void is_delete::mousePressEvent(QMouseEvent *event)
{
	this->windowPos = this->pos();                // 获得部件当前位置
	this->mousePos = event->globalPos();     // 获得鼠标位置
	this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}
void is_delete::mouseMoveEvent(QMouseEvent *event)
{
	this->move(event->globalPos() - this->dPos);
}
void is_delete::delete_execute()
{
	//此处传递的是文件名
	QFileInfo fi(delete_name);
	HANDLE han;
	QString filename = fi.fileName();
	class_interface *inter;
	class_interface::findByPathname(delete_name, inter);
	if (inter->return_state() == 3 || inter->return_state() == 4)
	{
		if (inter->is_upl())
		{
			if (global::return_upload_map().find(filename) == global::return_upload_map().end())
			{
				QMessageBox::warning(NULL, "错误", "错误");
				return;
			}
			han = global::return_upload_map()[filename];
		}
		else
		{
			if (global::return_download_map().find(filename) == global::return_download_map().end())
			{
				QMessageBox::warning(NULL, "错误", "错误");
				return;
			}
			han = global::return_download_map()[filename];
		}
		QMessageBox::StandardButton bu = QMessageBox::question(NULL, "询问", "当前正在下载，是否停止并删除");
		if (bu == QMessageBox::Yes)
		{
			TerminateThread(han, 0);
			Sleep(500);
			global::return_xunli_extern()->delete_timer(filename);
			QFile::remove(delete_name);
		}
		else
		{
			this->close();
			return;
		}
	}
	int h = global::return_listitem().size();
	global::return_listitem().remove(global::return_listitem().indexOf(inter));
	h = global::return_listitem().size();
	global::return_xunli_extern()->delete_list_widget(filename);
	if (ui.checkBox->isChecked())
	{
		bool right = remove(delete_name.toLocal8Bit().data());
	}
	this->close();
}
is_delete::~is_delete()
{

}
