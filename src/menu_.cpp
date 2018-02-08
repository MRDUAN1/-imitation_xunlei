#include "menu_.h"
#include "tool_widget.h"
#include "global.h"
#include "geturll.h"
#include <qevent.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qclipboard.h>
#include <qstring.h>

# pragma execution_character_set("utf-8")
static QSettings setting("config.ini", QSettings::IniFormat);

menu_::menu_(QFrame *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
	ui.label_9->setText("ͬʱ�����������");
	ui.label->setText("����");
	QPixmap pix("./image/file_folder");
	ui.pushButton->setIcon(pix);
	ui.pushButton->setStyleSheet("border:none");
	ui.pushButton->setCursor(QCursor(Qt::PointingHandCursor));
	ui.comboBox->setEditable(true);
	ui.label_11->setText("����");
	tool_widget *tool = new tool_widget(this);
	tool->setGeometry(this->width() - tool->width(), 0, tool->width(), tool->height());
	tool->show();
	this->setObjectName("qframe");
	this->setStyleSheet("QFrame#qframe{border: 1px solid rgb(97, 192, 199);};");
	QPalette pale;
	pale.setBrush(this->backgroundRole(), QBrush(QColor(240, 248, 255)));
	this->setPalette(pale);
	this->setAttribute(Qt::WA_ShowModal, true);
	this->setFixedSize(QSize(this->width(), this->height()));
	set_name();
	set_connect();
	ui.spinBox->setValue(5);

	read_setting();
}
void menu_::closeEvent(QCloseEvent *event)
{
	event->ignore();
	this->hide();
}
void menu_::paintEvent(QPaintEvent *event)
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
void menu_::mousePressEvent(QMouseEvent *event)
{
	this->windowPos = this->pos();                // ��ò�����ǰλ��
	this->mousePos = event->globalPos();     // ������λ��
	this->dPos = mousePos - windowPos;       // �ƶ��󲿼����ڵ�λ��
}

void menu_::mouseMoveEvent(QMouseEvent *event)
{
	this->move(event->globalPos() - this->dPos);
}
void menu_::start_test()
{
	QClipboard *board = QApplication::clipboard();
	QString str = board->text();
	if (str.contains("http://") || str.contains("ftp://") || str.contains("https://") || str.contains("sftp://"))
	{
		geturll *url_get = new geturll;
		url_get->set_url(str);
		url_get->show();
	}
}
void menu_::read_setting()
{
	bool o = setting.value("check_1").toBool();
	if (setting.contains("check_1")) {
		ui.checkBox->setCheckState(Qt::CheckState(setting.value("check_1").toBool()));
		std::get<1>(global::return_tuple()) = o;
	}
	if (o)
	{
		QClipboard *bar = QApplication::clipboard();
		connect(bar, SIGNAL(dataChanged()), this, SLOT(start_test()));
	}
	if (setting.contains("check_2")) {
		ui.checkBox_2->setCheckState(Qt::CheckState(setting.value("check_2").toBool()));
		std::get<2>(global::return_tuple()) = setting.value("check_2").toBool();
	}
	if (setting.contains("check_3")) {
		ui.checkBox_3->setCheckState(Qt::CheckState(setting.value("check_3").toBool()));
		std::get<3>(global::return_tuple()) = setting.value("check_3").toBool();
	}
	if (setting.contains("check_4")) {
		ui.checkBox_4->setCheckState(Qt::CheckState(setting.value("check_4").toBool()));
	}
	if (setting.contains("mission_num")) { //������
		ui.spinBox->setValue(setting.value("mission_num").toInt());
		std::get<0>(global::return_tuple()) = setting.value("mission_num").toInt();
	}
	else
	{
		ui.spinBox->setValue(5);
	}
	if (setting.contains("path_auto")) { //�Ƿ���ѡ���ϴ�Ŀ¼
		ui.radioButton_2->setChecked(setting.value("path_auto").toBool());
	}
	else
	{
		ui.radioButton_2->setChecked(false);
	}
	QString str = setting.value("combox").toString();
	if (setting.contains("path_chose")) {
		ui.radioButton->setChecked(setting.value("path_chose").toBool());
		ui.comboBox->addItem(setting.value("combox").toString());
	}
	else
	{
		ui.radioButton->setChecked(false);
		ui.checkBox->setEnabled(false);
		ui.pushButton->setEnabled(false);
	}
	if (setting.contains("limit_download")) {
		ui.lineEdit->setText(setting.value("limit_download").toString());
		int num = setting.value("limit_download").toString().toInt();
		std::get<4>(global::return_tuple()) = num;
	}
	else
	{
		std::get<4>(global::return_tuple()) = 0;
		ui.lineEdit->setText("0");
	}
	if (setting.contains("limit_upload")) {
		ui.lineEdit_3->setText(setting.value("limit_upload").toString());
		int num = setting.value("limit_upload").toString().toInt();
		std::get<5>(global::return_tuple()) = num;
	}
	else
	{
		ui.lineEdit_3->setText("0");
		std::get<5>(global::return_tuple()) = 0;
	}
	global::return_folder_path() = ui.comboBox->currentText();
}
void menu_::set_name()
{
	ui.label->setText("����");
	ui.label_2->setText("ͬʱ������ص�����");
	ui.checkBox->setText("���Ӽ�����������½�����");
	ui.checkBox_2->setText("�˳�ʱ�������������ʱ����");
	ui.checkBox_3->setText("�ر�ʱ��С����ϵͳ����");
	ui.checkBox_4->setText("Ĭ�����ع���");
	ui.label_6->setText("�洢Ŀ¼");
	ui.radioButton->setText("�Զ�ʹ���ϴ�����Ŀ¼");
	ui.radioButton_2->setText("ʹ��ָ��Ŀ¼");
	ui.label_2->setText("�ٶ�");
	ui.label_3->setText("�����ٶ�����");
	ui.label_4->setText("�ϴ��ٶ�����");
	ui.label_5->setText("0��ʾ������");
	ui.label_7->setText("0��ʾ������");
	QPalette pal2;
	pal2.setColor(QPalette::ButtonText, QColor(150, 153, 196));
	ui.label_3->setPalette(pal2);
	ui.label_4->setPalette(pal2);
	ui.pushButton_2->setText("ȷ��");
	ui.pushButton_3->setText("ȡ��");
}
void menu_::set_connect()
{
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(ok_pushbutton()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(cancel_pushbutton()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(filedialog_pushbutton()));
	connect(ui.radioButton, SIGNAL(clicked(bool)), this, SLOT(if_toggled()));
	connect(ui.radioButton_2, SIGNAL(clicked(bool)), this, SLOT(if_toggled_2()));
	connect(ui.comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(currentindex(QString&)));
}
void menu_::currentindex(QString& str)
{
	global::return_folder_path() = str;
}
void menu_::if_toggled_2()
{
	ui.comboBox->setEnabled(false);
	ui.pushButton->setEnabled(false);
}
void menu_::if_toggled()
{
	ui.comboBox->setEnabled(true);
	ui.pushButton->setEnabled(true);
}
void menu_::filedialog_pushbutton()
{
	int coun = ui.comboBox->count();
	global::return_folder_path() = QFileDialog::getExistingDirectory(this, "����·��", "./");
	ui.comboBox->addItem(global::return_folder_path());
	ui.comboBox->setCurrentIndex(ui.comboBox->count() - 1);
}
void menu_::ok_pushbutton()
{
	setting.setValue(tr("check_1"), ui.checkBox->checkState());
	std::get<1>(global::return_tuple()) = ui.checkBox->checkState();
	setting.setValue(tr("check_2"), ui.checkBox_2->checkState());
	std::get<2>(global::return_tuple()) = ui.checkBox_2->checkState();
	setting.setValue(tr("check_3"), ui.checkBox_3->checkState());
	std::get<3>(global::return_tuple()) = ui.checkBox_3->checkState();
	setting.setValue(tr("check_4"), ui.checkBox_4->checkState());
	setting.setValue(tr("mission_num"), ui.spinBox->value());
	std::get<0>(global::return_tuple()) = ui.spinBox->value();
	setting.setValue("path_auto", ui.radioButton_2->isChecked());
	setting.setValue("path_chose", ui.radioButton->isChecked());
	setting.setValue("limit_download", ui.lineEdit->text());
	std::get<4>(global::return_tuple()) = ui.lineEdit->text().toInt();
	setting.setValue("limit_upload", ui.lineEdit_3->text());
	std::get<5>(global::return_tuple()) = ui.lineEdit_3->text().toInt();
	QString str = ui.comboBox->currentText();
	setting.setValue("combox", str);
	this->close();
}
void menu_::cancel_pushbutton()
{
	this->close();
}
menu_::~menu_()
{

}
