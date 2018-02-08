#include "geturll.h"
#include "libcurl_network.h"
#include "tool_widget.h"
#include "class_interface.h"
#include "global.h"
#include <memory>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <string>
#include <QFileInfo>
#include <QFileDialog>
#include <qtimer.h>
#include <qpainter.h>
#include <qevent.h>
#include <qbitmap.h>
#include <qpalette.h>
using namespace std;
//在时不支持bt下载
# pragma execution_character_set("utf-8")
/**********************/
void GetFileNameFormUrl(char* fileName, const char* url)
{
	int urlLen = strlen(url);
	char mUrl[512] = { 0 };
	char fName[256] = { 0 };
	strcpy(mUrl, url);
	int cutIndex = 0;
	int i = urlLen - 1, j = 0;
	while (mUrl[--i] != '/');
	i++;
	while (mUrl[i] != '\0' && mUrl[i] != '?' &&mUrl[i] != '&')
	{
		fName[j++] = mUrl[i++];
	}
	fName[j] = '\0';
	strcpy(fileName, fName);
	return;
}
void geturll::set_detail_context(QString url_name, QString path_name, long size)
{
	ui.plainTextEdit->setEnabled(false);
	ui.lineEdit_2->setEnabled(false);
	ui.pushButton_4->setEnabled(false);
	ui.pushButton_3->setEnabled(false);
	ui.checkBox->setEnabled(false);
	ui.pushButton->hide();
	ui.plainTextEdit->setPlainText(url_name);
	
	QFileInfo fi(path_name);
	ui.lineEdit_2->setText(fi.fileName());
	ui.comboBox->setCurrentText(fi.absoluteFilePath());
	ui.label_5->setText(QString::number(size));
}
size_t get_size_struct(void* ptr, size_t size, size_t nmemb, void* data){
	return (size_t)(size * nmemb);
}

double get_download_size(const char* url){
	CURL* curl;
	CURLcode res;
	double size = 0.0;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, get_size_struct);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	res = curl_easy_perform(curl);
	res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
	if (res != CURLE_OK){
		fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);

	return size;
}
string GBKToUTF81(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}
void geturll::paintEvent(QPaintEvent *event)
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
geturll::geturll(QFrame *parent)
	: QFrame(parent)
{
	ui.setupUi(this);
	if_mult = false;

	ui.pushButton_4->hide();
	ui.checkBox_2->setText("多线程下载");
	connect(ui.checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(checkbox_change2(int)));
	//ui.lineEdit_2->setFocusPolicy(Qt::NoFocus);
	ui.label_7->setText("新建任务");
	ui.comboBox->setEditable(true);
	QPixmap pix("./image/file_folder");
	ui.pushButton_3->setIcon(pix);
	ui.pushButton_4->setIcon(pix);
	ui.pushButton_4->setStyleSheet("border:none");
	ui.pushButton_4->setCursor(QCursor(Qt::PointingHandCursor));
	ui.pushButton_3->setStyleSheet("border:none");
	ui.pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));
	this->setObjectName("qframe");
	this->setStyleSheet("QFrame#qframe{border: 1px solid rgb(97, 192, 199);};");
	QPalette pale;
	pale.setBrush(this->backgroundRole(), QBrush(QColor(240, 248, 255)));
	this->setPalette(pale);
	tool_widget *tool = new tool_widget(this);
	tool->setGeometry(this->width() - tool->width(), 0, tool->width(), tool->height());
	tool->show();
	this->setAttribute(Qt::WA_DeleteOnClose);
	is_upload = false;
	
	if_change_item = false;
	this->setAttribute(Qt::WA_ShowModal, true);
	this->setFixedSize(QSize(this->width(), this->height()));
	index_num = 0;
	ui.label->setText("下载地址:");
	ui.checkBox->setText("上传");
	ui.label_2->setText("文件名称:");
	ui.label_3->setText("保存路径:");
	ui.label_4->setText("文件大小");
	ui.label_6->setText("打开BT种子");
	ui.label_5->setText("未知大小");
	QPalette pa;
	pa.setColor(QPalette::WindowText, qRgb(138, 138, 138));
	ui.label_5->setPalette(pa);
	ui.pushButton->setText("立即下载");
	ui.pushButton_2->setText("取消");
	ui.comboBox->addItem(global::return_folder_path());
	ui.comboBox->setCurrentIndex(ui.comboBox->count() - 1);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(start_immedicate()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(file_dia()));
	connect(ui.comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(current_index_change(const QString &)));
	connect(ui.plainTextEdit, SIGNAL(textChanged()), this, SLOT(text_change_slot()));
	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkbox_change(int)));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(get_upload_file_path()));
}
void geturll::checkbox_change2(int stat)
{
	if_mult = stat;
}
void geturll::get_upload_file_path()
{
	QString str = QFileDialog::getOpenFileName();
	ui.lineEdit_2->setText(str);
}
void geturll::checkbox_change(int state)
{
	is_upload = state; //is_upload是true
	if (state)
	{
		ui.label->setText("上传地址:");
		ui.label_2->setText("上传文件名");
		ui.label_2->setEnabled(true);
		ui.lineEdit_2->setEnabled(false);
		ui.lineEdit_2->setFixedSize(255, 25);
		ui.label_3->setEnabled(false);
		ui.label_3->setText("请上传文件");
		ui.comboBox->setEnabled(false); 
		ui.pushButton_3->setEnabled(false); //combox后按钮
		ui.pushButton_4->setEnabled(true);
		//ui.lineEdit_2->setFocusPolicy(Qt::StrongFocus);
		ui.pushButton_4->show();
	}
	else
	{
		ui.label->setText("下载地址:");
		ui.label_2->setText("下载文件名");
		ui.label_2->setEnabled(true);
		ui.label_3->setEnabled(true);
		ui.label_3->setText("保存文件");
		ui.lineEdit_2->setEnabled(true);
		ui.lineEdit_2->setFixedSize(280, 25);
		ui.comboBox->setEnabled(true);
		ui.pushButton_3->setEnabled(true);
		ui.pushButton_4->setEnabled(false);
		//ui.lineEdit_2->setFocusPolicy(Qt::NoFocus);
		ui.pushButton_4->hide();
	}
}
void geturll::mousePressEvent(QMouseEvent *event)
{
	this->windowPos = this->pos();                // 获得部件当前位置
	this->mousePos = event->globalPos();     // 获得鼠标位置
	this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}

void geturll::mouseMoveEvent(QMouseEvent *event)
{
	this->move(event->globalPos() - this->dPos);
}
void geturll::text_change_slot()
{
	if (is_upload)
		return;
	QString strUrl = ui.plainTextEdit->toPlainText();
	get_header_detail(strUrl);
}
void geturll::set_size_name(QString str, qlonglong size)
{
	if (str.isNull() || str.isEmpty())
	{
		str = "index";
	}
	ui.lineEdit_2->setText(str);
	if (size <= 0)
	{
		ui.label_5->setText("未知大小");
	}
	QVariant va(size);
	QString st = va.toString();
	ui.label_5->setText(st);
}
void geturll::current_index_change(const QString &)
{
	int i = ui.comboBox->currentIndex();
	if (i == ui.comboBox->count())
	{
		ui.comboBox->clear();
		ui.comboBox->addItem(str);
		ui.comboBox->addItem("清除全部");
	}
}
void geturll::file_dia()
{
	if_change_item = true;
	str = QFileDialog::getExistingDirectory(this, "保存路径", "./");
	ui.comboBox->addItem(str);
}
void geturll::cancel()
{
	this->close();
}
void geturll::start_immedicate()
{
	if (ui.plainTextEdit->toPlainText().isEmpty())
	{
		QMessageBox::warning(NULL, "警告", "地址为空");
		return;
	}
	class_interface *cla = NULL;
	class_interface::findByUrl(ui.plainTextEdit->toPlainText(), cla);
	if (cla != NULL)
	{
		QMessageBox::warning(NULL, "警告", "列表已经存在该下载");
		return;
	}
	libcurl_network *curl_ = new libcurl_network(0);
	curl_->set_urls(ui.plainTextEdit->toPlainText());
	if (!is_upload)
	{
		if (if_mult) //是否多线程
			curl_->start_download(file_name, path_name, sizez, 0, 0, 1);
		else
		{
			curl_->start_download(file_name, path_name, sizez, 0, 0, 0);
		}
	}
	else
	{
		QFileInfo fi(ui.lineEdit_2->text());
		qint64 size_file = fi.size();
		curl_->start_upload(ui.lineEdit_2->text(), size_file);
	}

	this->close();
}
geturll::~geturll()
{

}
void geturll::get_header_detail(QString strUrl)
{
	string str = /*GBKToUTF81(strUrl.toLocal8Bit().data());*/strUrl.toLocal8Bit().data();
	char temporary_char[100] = { '\0' };
	getsize(str.data(), sizez);
	GetFileNameFormUrl(temporary_char, strUrl.toStdString().data());
	file_name = temporary_char;
	set_size_name(temporary_char, sizez);
	path_name = ui.comboBox->currentText() + "/" + file_name;
}
void geturll::getsize(const char*str, long& size)
{
	size = get_download_size(str);
}
void geturll::set_url(QString str)
{
	ui.plainTextEdit->setPlainText(str);
	get_header_detail(str);
}
