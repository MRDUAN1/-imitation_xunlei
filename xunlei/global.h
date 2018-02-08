//全局变量
#pragma once
class class_interface;
class xunlei;
class is_quit;
#include <Windows.h>
#include <qvector.h>
#include <tuple>
#include "class_interface.h"
using std::tuple;
class global
{
public:
	global();
	static QString& return_folder_path();
	static xunlei*& return_xunli_extern();
	static bool& return_if_ok_shutdown();
	static int& return_um_all();
	static QMap<QString, HANDLE>& return_download_map();
	static QMap<QString, HANDLE>& return_upload_map();
	static QVector<class_interface*> &return_listitem();
	static std::tuple<int, bool, bool, bool, int, int>& return_tuple();
	~global();
	static void set_folder_path(QString&);
	static void set_xunli_extern(xunlei*&);
	static void set_if_ok_shutdown(bool &);
	static void set_um_all(int &);
	static void set_download_map(QMap<QString, HANDLE>&);
	static void set_upload_map(QMap<QString, HANDLE>&);
	static void set_listitem(QVector<class_interface*>&);
	static void set_tuple(std::tuple<int, bool, bool, bool, int, int>&);
};

