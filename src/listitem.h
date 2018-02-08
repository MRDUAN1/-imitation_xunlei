#ifndef LISTITEM_H
#define LISTITEM_H


#include <qlistwidget.h>
#include <qevent.h>
#include <qobject.h>
class listitem : public QObject,  public QListWidgetItem
{
	Q_OBJECT

public:
	listitem(QListWidget *parent);
	~listitem();
	
	void mouseMoveEvent(QMouseEvent * event);
private:
	
};

#endif // LISTITEM_H
