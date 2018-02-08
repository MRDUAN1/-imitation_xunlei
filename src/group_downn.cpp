#include "group_downn.h"
#include "xunlei.h"
#include "global.h"
group_downn::group_downn(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	//global::return_xunli_extern(xunli_extern);
	ui.progressBar->setFixedSize(100, 19);
	connect(this, SIGNAL(emit_widget_num(QProgressBar*, int)), global::return_xunli_extern(), SLOT(update_progress(QProgressBar*, int)));
	connect(this, SIGNAL(emit_widget_all(QProgressBar*, int)), global::return_xunli_extern(), SLOT(update_progress_all(QProgressBar*, int)));
}
//设置多线程下载的进度条的数据
int group_downn::get_max()
{
	return ui.progressBar->maximum();
}
//设置多线程下载进度条
void group_downn::setvalue(int number)
{
	emit emit_widget_num(ui.progressBar, number);
}
void group_downn::set_all(int total)
{
	emit emit_widget_all(ui.progressBar, total);
}
group_downn::~group_downn()
{

}

