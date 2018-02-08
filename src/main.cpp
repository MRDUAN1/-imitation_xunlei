#include "xunlei.h"
#include <qsystemsemaphore.h>
#include <qsharedmemory.h>
#include <qmessagebox.h>
#include <QtWidgets/QApplication>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libcurl.lib")
# pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//确保只运行一次
	QSystemSemaphore sema("ServerKey", 1, QSystemSemaphore::Open);
	//在临界区操作共享内存
	sema.acquire();
	QSharedMemory mem("ServerObject");

	// 如果全局对象以存在则退出
	if (!mem.create(1))
	{
		QMessageBox::warning(NULL, "error",
			"main");
		sema.release();
		return 0;
	}
	sema.release();
	xunlei w;
	w.show();
	return a.exec();
}
