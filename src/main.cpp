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
	//ȷ��ֻ����һ��
	QSystemSemaphore sema("ServerKey", 1, QSystemSemaphore::Open);
	//���ٽ������������ڴ�
	sema.acquire();
	QSharedMemory mem("ServerObject");

	// ���ȫ�ֶ����Դ������˳�
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
