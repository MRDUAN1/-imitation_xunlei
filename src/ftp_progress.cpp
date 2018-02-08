#include "ftp_progress.h"
#include "xunlei.h"
#include "include\curl\curl.h"
#include <Windows.h>
extern xunlei *xunli_extern;
using namespace std;
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int r;
	long len = 0;
	/* _snscanf() is Win32 specific */
	//r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);  
	r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *)stream) = len;
	return size * nmemb;
}
void get_length(char *)
{
	long downloadFileLenth = 0; curl_global_init(CURL_GLOBAL_ALL);

	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, "https://img6.bdstatic.com/img/image/public/yuanjihuasy.png");
	//curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //只要求header头  
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
	if (curl_easy_perform(handle) == CURLE_OK)
	{
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
	}
	else {
		downloadFileLenth = -1;
	}
	curl_easy_cleanup(handle);
}
size_t getLocalFileLenth(char *file_name)
{
	FILE * fp = fopen(file_name, "r");
	if (fp == NULL) return 0;
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fclose(fp);
	return size;
}
ftp_progress::~ftp_progress()
{

}
size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, (FILE*)stream);
}
string GBKToUTF8(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	
	CURL * easy_handle = static_cast <CURL *>(clientp);
	char timeFormat[9] = "Unknow";

	//默认为字节/秒  
	double speed;
	char by[5] = { '\0' };
	double speeds = 0.0;
	curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &speed); // curl_get_info必须在curl_easy_perform之后调用  
	QString h, m, s, left;
	if (speed != 0)
	{
		//剩余时间  
		double  leftTime = (dltotal - dlnow) / speed;
		int  hours = leftTime / 3600;
		s = QString::number(hours);
		int  minutes = (leftTime - hours * 3600) / 60;
		m = QString::number(minutes);
		int  seconds = leftTime - hours * 3600 - minutes * 60;
		s = QString::number(seconds);
		left = "剩余时间:" + s + ":" + m + ":" + s;
	}
	if (speed > 1024 * 1024 * 1024)
	{
		speeds = speed / 1024.0 / 1024.0 / 1024.0;
		strcpy(by, "GB/s");
	}
	else if (speed > 1024 * 1024)
	{
		speeds = speed / 1024.0 / 1024.0;
		strcpy(by, "MB/s");
	}
	else if (speed > 1024)
	{
		speeds = speed / 1024.0;
		strcpy(by, "KB/s");
	}
	else
	{
		speeds = speed;
		strcpy(by, "B/s");
	}
	QString speedd = QString::number(speeds, 10, 3);
	speedd += by;
	xunli_extern->set_change()
	set_speed(speedd);
	return 0;
}

int download(CURL *curlhandle, const char * remotepath, const char * localpath1, long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1;
	long filesize = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	struct stat file_info;
	int use_resume = 1;
	//获取本地文件大小信息  

	//追加方式打开文件，实现断点续传  
	f = fopen("./hhhhh.mp3", "wb+");
	if (f == NULL) {
		perror(NULL);
		return 0;
	}
	struct curl_slist *head = NULL;
	//head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
	//curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, head);
	curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
	//curl_easy_setopt(curlhandle,CURLOPT_XFERINFODAT,void * pointer);

	curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "Anonymous:NULL");
	//连接超时设置  
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);
	//设置头处理函数  
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
	// 设置断点续传  
	//必须是curl_off_t有点小坑
	curl_off_t resume_position = 19000;
	int huhudufud = use_resume ? local_file_len : 0;
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, resume_position);
	//int hg = getLocalFileLenth("hhhhh.flv");
	//curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, "10000");
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFODATA, curlhandle);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFOFUNCTION, progress_callback);
	//curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);


	r = curl_easy_perform(curlhandle);
	fclose(f);
	if (r == CURLE_OK)
		return 1;
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}

int main(int c, char **argv)
{
	CURL *curlhandle = NULL;
	CURL *curldwn = NULL;
	curl_global_init(CURL_GLOBAL_ALL);
	curlhandle = curl_easy_init();
	curldwn = curl_easy_init();

	string str = "ftp://10.1.0.88/纪录片/A Bite of China-II/CNTV Live-2014.05.02.Seasons-A Bite of China-II.V3.mp4";
	string gg = GBKToUTF8(str.c_str());
	download(curldwn, gg.data(), "D:/abc1.jpg", 1, 3);

	curl_easy_cleanup(curlhandle);
	curl_easy_cleanup(curldwn);
	curl_global_cleanup();
	return 0;
	get_length("hhh");
}

ftp_progress::ftp_progress(QObject *parent)
	: QObject(parent)
{

}
