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
static bool qmessage; //�˳�ʱ�Ƿ���Ҫ����
# pragma execution_character_set("utf-8")
//0ʧ��,1�ɹ����,2��ͣ,3δ��ʼ,4����
void setting(class_interface *cs)
{
	//�жϴ�����״̬
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
	painter.setRenderHint(QPainter::Antialiasing);//ƽ��
	painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	painter.drawRoundedRect(this->rect(), 10, 10);
	this->setMask(bitmap);//�����ò�ͬ����	
}
is_quit::is_quit(QDialog *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	qmessage = false;
	is_twice = false;
	this->setObjectName("qframe");
	this->setStyleSheet("QFrame#qframe{border: 1px solid rgb(97, 192, 199);};");
	ui.label->setText("�ر���ʾ");
	tool_widget *tool = new tool_widget(this);
	tool->setGeometry(this->width() - tool->width(), 0, tool->width(), tool->height());
	this->setAttribute(Qt::WA_ShowModal, true);
	ui.label_2->setText("��������˳���ť��������");
	ui.radioButton->setText("��С��ϵͳ�̣���Ҫ�˳�");
	ui.radioButton_2->setText("�˳�����");
	ui.checkBox->setText("��������");
	ui.pushButton->setText("ȷ��");
	ui.pushButton_2->setText("ȡ��");
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
	this->windowPos = this->pos();                // ��ò�����ǰλ��
	this->mousePos = event->globalPos();     // ������λ��
	this->dPos = mousePos - windowPos;       // �ƶ��󲿼����ڵ�λ��
}
void is_quit::findNowDownloading()
{
	std::for_each(global::return_listitem().begin(), global::return_listitem().end(), setting);
}
void is_quit::ok()
{
	//�Ƿ�ڶ���ȷ��
	if (is_twice)
	{
		goto end;
	}
	else
	{
		//��С����ϵͳ��
		if (ui.radioButton->isChecked())
		{
			global::return_xunli_extern()->hide();
			global::return_xunli_extern()->hide();
			this->hide();
			//��С����ϵͳ��
		}
		else if (ui.radioButton_2->isChecked())
		{
			//�ж��Ƿ�����ֱ���˳�
			if (std::get<2>(globa))
			{
				goto end;
			}
			//�����й��˳������飬�������������Ѿ����ص�����
			findNowDownloading();
			if (qmessage)
			{
				ui.radioButton->hide();
				ui.radioButton_2->hide();
				ui.checkBox->move(90, 110);
				ui.label_2->setText("��ǰ���������ص�����\n�Ƿ�ȷ���˳�");
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
