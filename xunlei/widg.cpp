#include "widg.h"
#include <qlabel.h>
widg::widg(QWidget *parent)
	: QWidget(parent)
{
	is_ok = false;
}

widg::~widg()
{

}
void widg::enterEvent(QEvent * event)
{
	if (!is_ok) //为了提高效率
	{
		QLabel *label_filename = this->findChild<QLabel*>("file_name");
		QString filename = label_filename->text();
		cla = NULL;
		status = class_interface::findByfilename(filename, cla);
		is_ok = true;
	}
	QPoint point_globa = this->mapToGlobal(QPoint(0, 0));
	point_globa.setX(point_globa.x() + this->size().width());
	if (!status)
		return ;
	if (!cla->if_group)
		return;
	cla->wi->show();
	cla->wi->move(point_globa);
	QObjectList li = cla->wi->children();
	for (int i = 0; i < li.size(); i++)
	{
		QWidget *widegt_grop = (QWidget*)li[i];
		widegt_grop->show();
	}
}
void widg::leaveEvent(QEvent * event)
{
	if (!cla->if_group)
		return;
	QObjectList li = cla->wi->children();
	int h = li.size();
	for (int i = 0; i < li.size(); i++)
	{
		QWidget *widegt_grop = (QWidget*)li[i];
		widegt_grop->hide();
	}
	cla->wi->hide();
}
