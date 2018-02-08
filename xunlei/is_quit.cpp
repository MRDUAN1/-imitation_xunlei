#include "is_quit.h"
#include "class_interface.h"
#include "xunlei.h"
#include "tool_widget.h"
#include "global.h"
#include <qbitmap.h>
#include <qpainter.h>
#include <qevent.h>
#include <algorithm>
#include <qmessagebox.h>
#include <tuple>
static bool qmessage; //退出时是否需要警告
# pragma execution_character_set("utf-8")
//0失败,1成功完成,2暂停,3未开始,4正常
void setting(class_interface *cs)
{
	//判断此下载状态
	if (cs->return_state() == 4 || cs->return_state() == 3)
	{
		qmessage = true;
	}
}
void is_quit::paintEvent(QPaintEvent *event)
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
is_quit::is_quit(QDialog *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	qmessage = false;
	is_twice = false;
	this->setObjectName("qframe");
	this->setStyleSheet("QFrame#qframe{border: 1px solid rgb(97, 192, 199);};");
	ui.label->setText("关闭提示");
	tool_widget *tool = new tool_widget(this);
	tool->setGeometry(this->width() - tool->width(), 0, tool->width(), tool->height());
	this->setAttribute(Qt::WA_ShowModal, true);
	ui.label_2->setText("您点击了退出按钮，您是想");
	ui.radioButton->setText("最小到系统盘，不要退出");
	ui.radioButton_2->setText("退出程序");
	ui.checkBox->setText("不再提醒");
	ui.pushButton->setText("确认");
	ui.pushButton_2->setText("取消");
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
	ui.radioButton->setChecked(true);
}
void is_quit::mouseMoveEvent(QMouseEvent *event)
{
	this->move(event->globalPos() - this->dPos);
}
void is_quit::mousePressEvent(QMouseEvent *event)
{
	this->windowPos = this->pos();                // 获得部件当前位置
	this->mousePos = event->globalPos();     // 获得鼠标位置
	this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}
void is_quit::findNowDownloading()
{
	std::for_each(global::return_listitem().begin(), global::return_listitem().end(), setting);
}
void is_quit::ok()
{
	//是否第二次确认
	if (is_twice)
	{
		goto end;
	}
	else
	{
		//最小化到系统盘
		if (ui.radioButton->isChecked())
		{
			global::return_xunli_extern()->hide();
			global::return_xunli_extern()->hide();
			this->hide();
			//最小化到系统盘
		}
		else if (ui.radioButton_2->isChecked())
		{
			//判断是否设置直接退出
			if (std::get<2>(globa))
			{
				goto end;
			}
			//处理有关退出的事情，保存下载数据已经下载得数据
			findNowDownloading();
			if (qmessage)
			{
				ui.radioButton->hide();
				ui.radioButton_2->hide();
				ui.checkBox->move(90, 110);
				ui.label_2->setText("当前有正在下载的任务，\n是否确认退出");
				is_twice = true;
			}
			else
			{
				goto end;
			}
		}
	}
	return;
end:
	global::return_xunli_extern()->quit_into_list();
	global::return_xunli_extern()->quit_save();
	exit(0);
}
is_quit::~is_quit()
{

}
