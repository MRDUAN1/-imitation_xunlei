#include "event_filter.h"
#include "class_interface.h"
#include <qlabel.h>
#include <qwidget.h>
#include <qpoint.h>
event_filter::event_filter(QObject *parent)
	: QObject(parent)
{

}

event_filter::~event_filter()
{

}
bool event_filter::eventFilter(QObject *obj, QEvent *ev)
{
	int i = ev->type();
	QWidget *wi = dynamic_cast<QWidget*>(obj);
	if (ev->type() == QEvent::HoverEnter)
	{
		QPoint po = wi->mapToGlobal(wi->pos());
		po.setX(po.x() + wi->size().width());
		QLabel *label_filename = wi->findChild<QLabel*>("file_name");
		QString filename = label_filename->text();
		class_interface *cla = NULL;
		bool tru = class_interface::findByfilename(filename, cla);
		if (!tru)
			return false;
		if (!cla->if_group)
			return false;
		cla->wi->show();
		cla->wi->move(po);
		cla->wi->show();
		QObjectList li = cla->wi->children();
		for (int i = 0; i < li.size(); i++)
		{
			QWidget *widegt_grop = (QWidget*)li[i];
			widegt_grop->show();
		}
	}
	else if (ev->type() == QEvent::HoverLeave)
	{
		wi->hide();
		QObjectList li = wi->children();
		for (int i = 0; i < li.size(); i++)
		{
			QWidget *widegt_grop = (QWidget*)li[i];
			widegt_grop->hide();
		}
	}
	return QObject::eventFilter(obj, ev);
}
