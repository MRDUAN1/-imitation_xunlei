#ifndef LIBCURL_NETWORK_H
#define LIBCURL_NETWORK_H

#include <QObject>
#include "curl/curl.h"
#include <string>
#include <tuple>
using std::tuple;
using std::string;
class libcurl_network : public QObject
{
	Q_OBJECT

public:
	
	libcurl_network(QObject *parent);
	~libcurl_network();
	void set_urls(QString);
	static void start_thread(void*);
	static void start_thread_upload(void *);
	void start_download(QString ,QString, long, bool is_from_setting = 0, long read_size = 0, bool ismul = 0);
	void start_upload(QString, long, bool is_restart = false, bool is_from_setting = 0, long read_size = 0);
	static void test_if_ftp();
	static bool downLoad(int threadNum, string Url, string Path);
private:
	//QString file_name;
//	QString file_namee; //ÎÄ¼þÃû
	//libcurl_network *libb;
	//QMap<QString, HANDLE>download_map;
	//QMap<QString, HANDLE>upload_map;
	//bool is_from_settings;
	static int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	//static QString url_name;
	//static bool normal_orftp;
	static int ftp_download(CURL *curlhandle, const char * remotepath, const char * localpath1, long timeout, long tries);
	static void http_download(CURL *curlhandle);
	static int ftp_upload(CURL *curlhandle, const char * remotepath, const char * localpath, int timeout, int try_ = 3);
signals:
	void start(QString);
	void changes(QString file_name, QString left_time, QString speed, long size_now, int num = -1);
	void download_group(int);
	void download_group_all(int);
	void finished(QString, bool = 1);
};

#endif // LIBCURL_NETWORK_H
