//œµÕ≥Õ–≈Ã
#ifndef SYSTEM_TRAY_H
#define SYSTEM_TRAY_H

#include <QObject>
#include <QWidget>
#include <qaction.h>
#include <qsystemtrayicon.h>
#include <qmenu.h>
#include "public_toolbutton.h"
# pragma execution_character_set("utf-8")
class system_tray : public_toolbutton
{
	Q_OBJECT

public:
	system_tray(QWidget *parent);
	~system_tray();
	public slots:
	void trayiconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	QSystemTrayIcon *trayIcon;
	public_toolbutton *pb_tb;
	void system_tray::show_normal();
};

#endif // SYSTEM_TRAY_H
