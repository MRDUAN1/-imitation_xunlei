#ifndef PUBLIC_TOOLBUTTON_H
#define PUBLIC_TOOLBUTTON_H

#include <QObject>
#include <QWidget>
#include <qaction.h>
#include <qsystemtrayicon.h>
#include <qmenu.h>
# pragma execution_character_set("utf-8")

class public_toolbutton : public QObject
{
	Q_OBJECT

public:
	public_toolbutton(QObject *parent);
	~public_toolbutton();

protected:
	void set_detail();
	QMenu *menu_1;
	public slots:
	void setting_tool_slot();
	void shutdown_after_download_slot();
	void about_slot();
	void quit_tool_slot();
	
};

#endif // PUBLIC_TOOLBUTTON_H
