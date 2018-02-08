#include "system_tray.h"
#include "xunlei.h"
#include "global.h"

system_tray::system_tray(QWidget *parent)
: public_toolbutton(parent)
{
	trayIcon = new QSystemTrayIcon(this);
	//global::return_xunli_extern(xunli_extern);
	QIcon icon = QIcon("xllitelogo.png");
	trayIcon->setIcon(icon);
	//QObject::connect(trayIcon, SIGNAL(triggered()), this, SLOT(show_normal()));
	trayIcon->show();
	QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(trayiconActivated(QSystemTrayIcon::ActivationReason)));
	set_detail();
	trayIcon->setContextMenu(menu_1);
}

system_tray::~system_tray()
{

}
void system_tray::trayiconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:

	case QSystemTrayIcon::DoubleClick:
		//Ë«»÷ÍÐÅÌÍ¼±ê
		global::return_xunli_extern()->showNormal();
		global::return_xunli_extern()->raise();
		break;
	default:
		break;
	}
}
void system_tray::show_normal()
{

	global::return_xunli_extern()->showNormal();
}
