#ifndef EVENT_FILTER_H
#define EVENT_FILTER_H

#include <QObject>
#include <qevent.h>

class event_filter : public QObject
{
	Q_OBJECT

public:
	event_filter(QObject *parent);
	~event_filter();
	bool eventFilter(QObject *obj, QEvent *ev);
private:
	
};

#endif // EVENT_FILTER_H
