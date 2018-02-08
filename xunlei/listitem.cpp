#include "listitem.h"

listitem::listitem(QListWidget *parent)
	: QListWidgetItem(parent)
{

}

listitem::~listitem()
{

}
void listitem::mouseMoveEvent(QMouseEvent * event)
{
	event->pos();
	if (event->type() == QEvent::Enter)
	{
		printf("hffffffff");
	}
	else if (event->type() == QEvent::Leave)
	{
		printf("hffffffff");
	}
}
