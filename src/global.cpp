#include "global.h"
#include <qstring.h>
#include <Windows.h>
#include <qpushbutton.h>
#include <qmap.h>
#include "xunlei.h"
#include "is_quit.h"
#include "class_interface.h"
static QString folder_path_; //����Ŀ¼
static xunlei *xunli_extern_; //�������ָ��
static bool if_ok_shutdown_; //
static int num_all_; //�������������ͬʱ����
static  QMap<QString, HANDLE>download_map_; //�߳̾�����ļ�����ӳ��
static  QMap<QString, HANDLE>upload_map_;
/******************************************/
static QVector<class_interface*>list_item_; //class_interfaceΪ�������ݽṹ������ÿһ������������������ݣ�list_item_������������������������
static std::tuple<int, bool, bool, bool, int, int>globa_; //����menu�����õ�����
global::global()
{
}


global::~global()
{
}
QString& global::return_folder_path()
{
	return folder_path_;
}
xunlei*& global::return_xunli_extern()
{
	return xunli_extern_;
}
bool& global::return_if_ok_shutdown()
{
	return if_ok_shutdown_;
}
int& global::return_um_all()
{
	return num_all_;
}
QMap<QString, HANDLE>& global::return_download_map()
{
	return download_map_;
}
QMap<QString, HANDLE>& global::return_upload_map()
{
	return upload_map_;
}
QVector<class_interface*>& global::return_listitem()
{
	return list_item_;
}
std::tuple<int, bool, bool, bool, int, int>& global::return_tuple()
{
	return globa_;
}



void global::set_folder_path(QString& str)
{
	folder_path_ = str;
}
void global::set_xunli_extern(xunlei*& xunl)
{
	xunli_extern_ = xunl;
}
void global::set_if_ok_shutdown(bool &glob)
{
	if_ok_shutdown_ = glob;
}
void global::set_um_all(int &i)
{
	num_all_ = i;
}
void global::set_download_map(QMap<QString, HANDLE>& i)
{
	download_map_ = i;
}
void global::set_upload_map(QMap<QString, HANDLE> &glob)
{
	glob = upload_map_;
}
void global::set_listitem(QVector<class_interface*>& i)
{
	list_item_ = i;
}
void global::set_tuple(std::tuple<int, bool, bool, bool, int, int>&i)
{
	globa_ = i;
}
