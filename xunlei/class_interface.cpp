#include "class_interface.h"
#include "global.h"
#include <algorithm>
#include <qfileinfo.h>

using std::for_each;


class_interface::class_interface(QString url_name_, QString file_name_, int state_, QObject *parent)
	: QObject(parent)
{
	//global::return_listitem(global::return_listitem());
	url_name = url_name_;
	file_name = file_name_;
	sta = state_;
	is_time = true;
	time = 100;
	timer = new QTimer();
	timer->start(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(timer_out()));
}

class_interface::class_interface(QObject *parent)
: QObject(parent)
{

}
bool class_interface::return_time()
{
	return is_time;
}
void class_interface::restart_set()
{
	is_time = false;
	
}
void class_interface::stop_pre()
{
	timer->stop();
}
void class_interface::start_pre()
{
	timer->start();
}
void class_interface::time_out_stop()
{
	timer->stop();
}
void class_interface::timer_out()
{
	is_time = true;
	time += 100;
	if (time == 1000)
		time = 100;
	timer->start(time); //进度条更新时间，
	//connect(timer, SIGNAL(timeout()), this, SLOT(timer_out()));
}
//通过url查找classinterface
bool class_interface::findByUrl(QString url_name, class_interface *&urll)
{
	urll = NULL;
	for_each(global::return_listitem().begin(), global::return_listitem().end(), [&urll, url_name](class_interface *my)mutable{if (my->url_name == url_name){
		urll = my;
	}
	});
	if (urll == NULL)
		return false;
	else
		return true;
}
bool class_interface::findByPathname(QString file_name, class_interface *&urll)
{
	urll = NULL;
	class_interface *data_datil;
	QString str_filename;
	QFileInfo fi(file_name);
	str_filename = fi.fileName();
	QString filename;
	foreach(data_datil, global::return_listitem())
	{
		if (data_datil->is_upl())
		{
			//如果是上传，class_interface的filename是path名
			QFileInfo fi(data_datil->file_name);
			filename = fi.fileName();
		}
		else
		{
			filename = data_datil->file_name;
		}
		if (filename == str_filename)
		{
			urll = data_datil;
		}
	}
	if (urll == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool class_interface::findByfilename(QString file_name, class_interface *&urll)
{
	urll = NULL;

	for_each(global::return_listitem().begin(), global::return_listitem().end(), [&urll, file_name](class_interface *my)mutable{if (my->file_name == file_name){
		urll = my;
	}
	});
	if (urll == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}
QString class_interface::get_url()
{
	return url_name;
}
void class_interface::set_change(QString url_name_, QString file_name_, int state, qint64 size_, qint64 already_size_, bool isUpload)
{
	url_name = url_name_;
	file_name = file_name_;
	sta = state;

	if (size_ == -1)
	{
		size_ = 1;
	}
	size = size_;
	ready_size = already_size_;
	is_upload = isUpload;
	if (is_upload)
	{
		update_length = size;
	}
}
//0失败,1成功完成,2暂停,3未开始,4正常
int class_interface::return_state()
{
	return sta;
}
int class_interface::get_size()
{
	return size;
}
QString class_interface::return_fileName()
{
	return file_name;
}
bool class_interface::is_upl()
{
	return is_upload;
}
void class_interface::set_isup()
{
	is_upload = true;
}
qint64 class_interface::get_ready_size()
{ 
	return ready_size;
}
void class_interface::set_ready_size(int in)
{
	ready_size = in;
}
//0失败,1成功完成,2暂停,3未开始,4正常
void class_interface::set_state(int state_)
{
	sta = state_;
}
void class_interface::get_change(QString& url_name_, QString& file_name_, int &state_, qint64 &size_, qint64 &already_size_)
{
	url_name_ = url_name;
	file_name_ = file_name;
	state_ = sta;
	size_ = size;
	already_size_ = ready_size;
}
class_interface::~class_interface()
{

}
