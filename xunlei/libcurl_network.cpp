#include "libcurl_network.h"
#include "xunlei.h"
#include "class_interface.h"
#include "group_downn.h"
#include "global.h"
#include <qdebug.h>
#include <stdio.h>
#include <qhash.h>
#include <Windows.h>  
#include <CommCtrl.h>  
#include <iostream>
#include <qtimer.h>
#include <process.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment( lib, "ws2_32.lib" )
using namespace std;
#include "geturll.h"
struct FtpFile {
	QString filename;
	FILE *stream;
};
static bool is_up;
static QString url_name; //ȫ�ֱ�����ʹ�Ժ󱻸���Ҳû��ϵ
static bool normal_orftp; //�ж��Ƿ���ftp����http
/*****************************/
static QString file_name;  //·����
static QString file_namee; //�ļ���
static libcurl_network *libb;
static bool is_from_settings;
long reay_sizes;
# pragma execution_character_set("utf-8")
//���߳�����ר�ýṹ��
struct int_clas{
	int num; //�ڼ����߳�
	class_interface*in; //�ڼ����̵߳���ϸ��Ϣ
};
void libcurl_network::start_thread_upload(void *parameter)
{
	CURL *curl = (CURL *)parameter;
	ftp_upload(curl, file_name.toLocal8Bit().data(), "null", 3, 3);
}
void libcurl_network::start_thread(void*parameter)
{
	struct parameter_nonce{
		CURL *curl;
		bool is_group; //�ж��Ƿ�Ⱥ��
	};
	parameter_nonce *curl_group = (parameter_nonce*)parameter;
	CURL *curl = curl_group->curl;
	bool is_group = curl_group->is_group;
	if (!normal_orftp) {
		//http����
		http_download(curl);
	}
	else
	{
		//ftp����
		if (!is_group)
		ftp_download(curl, file_name.toLocal8Bit().data(), "null", 1, 3);
		else
		{
			//�Ƕ��߳�����
			curl_easy_cleanup(curl);
			downLoad(4, url_name.toStdString(), file_name.toStdString());
		}
	}
}
//��ȡ�ļ�����
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
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FtpFile *out = (struct FtpFile *)stream;
	if (out && !out->stream) {
		out->stream = fopen(out->filename.toStdString().data(), "wb");//���ļ�����д��
		if (!out->stream)
			return -1;
	}
	return fwrite(buffer, size, nmemb, out->stream);
}
//�ӵ�curl_off_t�Ǵ���Ŀӵ�
int libcurl_network::progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	
	int_clas *cal = (int_clas*)clientp;
	int num = cal->num;
	class_interface * easy_handles = cal->in;
	CURL * easy_handle;
	//���̴߳���
	group_downn *group = NULL;
	long all_data = 0; //�������Ⱥ�����0�������Ⱥ���������������
	/*****************�ж�ʱ��**********************************/
	if (easy_handles->return_state() == 2)
		return 0;
	if (!easy_handles->return_time())
	{
		return 0;
	}
	else
	{
		easy_handles->restart_set();
	}
	//qDebug() << num << "\n";
	/********************�ж�Ⱥ����Ϣ����ֵ**************************************/
	if (cal->num != -1) ////���Ƕ��߳�
	{
		easy_handle = easy_handles->spl[cal->num]->curls;
		group = easy_handles->dwn[cal->num];
		easy_handles->spl[cal->num]->already_sizes = dlnow;
		if (!easy_handles->is_upl())
		{
			group->setvalue(dlnow);
		}
		else
		{
			group->setvalue(ulnow);
		}
		//int����ˣ��Ѹ����̵߳�������������
		for (int i = 0; i < easy_handles->spl.size(); i++)
		{
			int ii = easy_handles->spl[i]->already_sizes;
			all_data += easy_handles->spl[i]->already_sizes;
			easy_handles->set_ready_size(all_data);
		}
	}
	//������Ƕ��߳�
	else
	{
		easy_handle = easy_handles->curl;
	} //������Ⱥ����
	/***********************************************************/
	char  timeFormat[9] = "Unknow";
	//Ĭ��Ϊ�ֽ�/��  
	double speed;
	char by[5] = { '\0' };
	double speeds = 0.0;
	double  leftTime;
	//����ϴ���ȡ�ϴ��ٶ�
	if (!easy_handles->is_upl())
	{
		curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &speed); // curl_get_info������curl_easy_perform֮�����
	}
	else
	{
		curl_easy_getinfo(easy_handle, CURLINFO_SPEED_UPLOAD, &speed);
	}
	//����Ƕ��̵߳�ǰ�������ٶȵ����߳������Ե�ǰ�߳������ٶ�
	if (group)
	{
		speed = speed * easy_handles->thread_num;
	}
	QString h, m, s, left;
	/*************����******************/
	long hh = easy_handles->get_ready_size();
	long hhhh = easy_handles->get_size();
	/*************����*********************/
	if (speed == 0)
		return 0;
	// ʣ��ʱ�� 
	if (easy_handles->is_upl()) //�Ƿ��ϴ�
	{
		if (cal->num = -1)
		{
			leftTime = (easy_handles->size - ulnow - easy_handles->get_ready_size()) / speed;
		}
		else
		{
			leftTime = (easy_handles->size - ulnow - easy_handles->get_ready_size()) / speed;
		}
	}
	//����
	else
	{
		if (cal->num != -1)
		{
			leftTime = (easy_handles->get_size() - all_data/* - easy_handles->get_ready_size()*/) / speed;
		}
		else
		{
			leftTime = (easy_handles->get_size() - dlnow - easy_handles->get_ready_size()) / speed; //get_ready_sizeָ�����ϴ����ػ����ϴ���size
		}
	}
	int  hours = leftTime / 3600;
	h = QString::number(hours);
	int  minutes = (leftTime - hours * 3600) / 60;
	m = QString::number(minutes);
	int  seconds = leftTime - hours * 3600 - minutes * 60;
	s = QString::number(seconds);
	left = "ʣ��ʱ��:" + h + ":" + m + ":" + s;
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
	//get_ready_sizeĬ��Ϊ0�����һ��������0
	speedd += by;
	{
		if (!easy_handles->is_upl()) //����
		{
			if (group)
			{
				emit libb->changes(easy_handles->file_name, left, speedd, all_data/*dlnow + easy_handles->get_ready_size()*/, cal->num);
			}
			else
			{
				emit libb->changes(easy_handles->file_name, left, speedd, dlnow + easy_handles->get_ready_size(), cal->num);
			}
		}
		//�ϴ�
		else
		{
			if (group)
			{
				group->setvalue(dlnow);
			}
			//else
			emit libb->changes(easy_handles->file_name, left, speedd, ulnow + easy_handles->get_ready_size(), cal->num);
		}
	}
	return 0;
}

libcurl_network::libcurl_network(QObject *parent)
	: QObject(parent)
{
	normal_orftp = false;
	libb = this;
	connect(this, SIGNAL(start(QString)), global::return_xunli_extern(), SLOT(start_down_upload(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(changes(QString, QString, QString, long, int)), global::return_xunli_extern(), SLOT(set_change(QString, QString, QString, long)), Qt::QueuedConnection);
	connect(this, SIGNAL(finished(QString, bool)), global::return_xunli_extern(), SLOT(is_fishished_(QString, bool)), Qt::QueuedConnection);
}

libcurl_network::~libcurl_network()
{

}
int libcurl_network::ftp_download(CURL *curlhandle, const char * remotepath, const char * localpath1, long timeout, long tries)
{
	FILE *f;
	class_interface *face;
	class_interface *face1;
	foreach(face, global::return_listitem())
	{
		if (face->url_name == url_name)
		{
			face1 = face;
			break;
		}
	}
	face1->curl = curlhandle;
	//�������ݳ�ʼ��
	emit libb->start(file_namee);
	curl_off_t local_file_len = reay_sizes;
	long filesize = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	struct stat file_info;
	int use_resume = 1;
	//��ȡ�����ļ���С��Ϣ  
	//׷�ӷ�ʽ���ļ���ʵ�ֶϵ��������е�ӵ�
	f = fopen(remotepath, "ab+");
	if (f == NULL) {
		perror(NULL);
		return 0;
	}
	struct curl_slist *head = NULL;
	url_name = GBKToUTF8(url_name.toLocal8Bit().data()).data();
	//url_name = curl_easy_escape(curlhandle, url_name.toStdString().data(), url_name.size() + 2);
	curl_easy_setopt(curlhandle, CURLOPT_URL, url_name.toStdString().data());
	//��ȡ�����ٶ�����
	curl_off_t len = get<4>(global::return_tuple()) * 1024;
	if (get<4>(global::return_tuple()) != 0)
	{
		curl_easy_setopt(curlhandle, CURLOPT_MAX_RECV_SPEED_LARGE, len);
	}
	curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "Anonymous:NULL");
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
	curl_off_t resume_position = is_from_settings ? local_file_len : 0;
	//�Ƿ�ϵ�����
	if (is_from_settings)
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, resume_position);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
	//���Ƕ��̣߳�numΪ-1
	int_clas *str = new int_clas;
	str->num = -1;
	str->in = face1;
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFODATA, (void*)str);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFOFUNCTION, progress_callback);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	r = curl_easy_perform(curlhandle);
	fflush(f);
	fclose(f);
	if (r == CURLE_OK)
	{
		//�������
		emit libb->finished(face1->file_name);
		return 1;
		
	}
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}
void libcurl_network::http_download(CURL *curl)
{
	
	CURLcode res;
	struct FtpFile ftpfile = {
		file_name,
		//�������ص����ص��ļ�λ�ú�·��
		NULL
	};

	emit libb->start(file_namee);
	class_interface *face;
	class_interface *face1;
	foreach(face, global::return_listitem())
	{
		if (face->url_name == url_name)
		{
			face1 = face;
			break;
		}
	}
	face1->curl = curl;
	//downloding_uploading[url_name] = face1;
	  //��ʼ��һ��curlָ��
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_off_t len = get<4>(global::return_tuple()) * 1024;
	if (get<4>(global::return_tuple()) != 0)
	{
		curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, len);
	}
	if (curl) { //curl������ڵ������ִ�еĲ���

		//����Զ�˵�ַ
		curl_easy_setopt(curl, CURLOPT_URL, url_name.toStdString().data());

		//ִ��д���ļ�������
		//curl_easy_setopt(curl, CURLOPT_XFERINFODATA, curl);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);//�������ݱ�д��,�ص�����������,
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile); //���ýṹ���ָ�봫�ݸ��ص�����
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		int_clas *str = new int_clas;
		str->num = -1;
		str->in = face1;
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void*)str);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
		//����ʱ��㱨���е���Ϣ,�����STDERR��ָ����CURLOPT_STDERR��
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		curl_easy_setopt(curl, CURLOPT_USERPWD, "SUREN:SUREN");
		//xunli_extern->start_down_upload(file_namee);
		//д���ļ�
		res = curl_easy_perform(curl);

		//�ͷ�curl����
		curl_easy_cleanup(curl);

		if (res != CURLE_OK)
		{
			cout << stderr << res << endl;
			emit libb->finished(face1->file_name, 0);
		}
		else
		{
			emit libb->finished(face1->file_name);
			//return;
		}
	}

	if (ftpfile.stream)
	{
		//�ر��ļ���
		fclose(ftpfile.stream);
	}

	//�ͷ�ȫ��curl����
	curl_global_cleanup();
}
//����url����
void libcurl_network::set_urls(QString str)
{
	//curl_easy_escape
	url_name = str;
}
void libcurl_network::start_upload(QString up_file_name, long length_, bool is_restart,  bool is_from_setting, long read_size)
{
	is_from_settings = is_from_setting;
	reay_sizes = read_size;

	is_up = true;
	//up_file_name��ȫ���Ĵ�ȡλ��
	QFileInfo fi(up_file_name);
	file_namee = fi.fileName();
	if (!is_restart)
	url_name += file_namee;
	// int state = 3, qint64 ready_size = 0, bool is_upload
	if (!is_from_settings)
	global::return_xunli_extern()->set_list_widget(up_file_name, length_, url_name, 3, 0, true);
	//update_length = length_;
	
	file_name = up_file_name;
	CURL *curl;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	test_if_ftp();
	HANDLE hand = (void*)_beginthread(start_thread_upload, 0, (void *)curl);
	global::return_upload_map()[fi.fileName()] = hand;
}
void libcurl_network::test_if_ftp()
{
	QString proto = url_name.left(3);
	if(!strcmp(proto.toLocal8Bit().data(), "ftp"))
	{
		normal_orftp = true;
	}
	else
	{
		normal_orftp = false;
	}
}
//�ڴ�й©
void libcurl_network::start_download(QString file_namee1, QString stt, long size, bool is_from_setting, long read_size, bool multi_thread)
{
	//in��ָ�Ƿ��Ƕ��߳�����
	if (global::return_um_all() > std::get<0>(global::return_tuple()))
	{
		QMessageBox::warning(NULL, "����", "�Ѿ��ﵽ����/�ϴ����ߣ��޷��½�����");
		return;
	}
	is_up = false;
	global::return_um_all()++;
	//update_length = size;
	is_from_settings = is_from_setting;
	reay_sizes = read_size;
	file_namee = file_namee1;//�ļ���
	if (!is_from_settings)
	{
		if (multi_thread)
			global::return_xunli_extern()->set_list_widget(file_namee1, size, url_name, 3, 0, 0, 0, 1);
		else
			global::return_xunli_extern()->set_list_widget(file_namee1, size, url_name, 3, 0, 0, 0);
	}
	//update_length = size;
	file_name = stt;//·����
	CURL *curl;
	test_if_ftp();
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	//�ڴ�й©
	struct curl_multi{
		CURL *curl;
		bool is_multi;
	};
	curl_multi *curl_multi_ = new curl_multi;
	curl_multi_->curl = curl;
	curl_multi_->is_multi = multi_thread;
	HANDLE handle = (void *)_beginthread(start_thread, 0, curl_multi_);
	global::return_download_map()[file_namee] = handle;
}
/****************��********************/
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE*)stream;
	size_t n;
	if (ferror(f))
		return CURL_READFUNC_ABORT;
	n = fread(ptr, size, nmemb, f) * size;
	return n;
}
int libcurl_network::ftp_upload(CURL *curlhandle, const char * remotepath, const char * localpath, int timeout, int tries)
{
	FILE *f;
	class_interface *face;
	class_interface *face1;
	foreach(face, global::return_listitem())
	{
		if (face->url_name == url_name)
		{
			face1 = face;
			break;
		}
	}
	face1->curl = curlhandle;
	face1->set_isup();
	//downloding_uploading[url_name] = face1;
	emit libb->start(remotepath);
	long uploaded_len = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	int c;
	f = fopen(remotepath, "rb");
	if (f == NULL) {
		perror(NULL);
		return 0;
	}
	if (is_from_settings)
	{
		fseek(f, reay_sizes, 0);
	}
	curl_off_t ddd = 10000000;
	curl_off_t len = get<5>(global::return_tuple()) * 1024;
	if (get<5>(global::return_tuple()) != 0)
	{
		curl_easy_setopt(curlhandle, CURLOPT_MAX_RECV_SPEED_LARGE, len);
	}
	curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_URL, url_name.toLocal8Bit().data());
	curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "Anonymous:NULL");
	if (timeout)
		curl_easy_setopt(curlhandle, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
	if (get<5>(global::return_tuple()) != 0)
	{
		curl_easy_setopt(curlhandle, CURLOPT_MAX_SEND_SPEED_LARGE, (curl_off_t)get<5>(global::return_tuple()) * 1024);
	}
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &uploaded_len);
	//curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, discardfunc);
	curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
	curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_FTPPORT, "-"); /* disable passive mode */
	curl_easy_setopt(curlhandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	//�Ƕ��߳�
	int_clas *str = new int_clas;
	str->num = -1;
	str->in = face1;
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFODATA, (void*)str);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFOFUNCTION, progress_callback);
	//�������ʲô��˼
	//curl_easy_setopt(curlhandle, CURLOPT_INFILESIZE_LARGE, ddd);
	for (c = 0; (r != CURLE_OK) && (c < tries); c++) {
		if (c) { /* yes */
			curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
			curl_easy_setopt(curlhandle, CURLOPT_HEADER, 1L);
			//xunli_extern->start_down_upload(file_namee);
			r = curl_easy_perform(curlhandle);
			if (r != CURLE_OK)
				continue;
			curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 0L);
			curl_easy_setopt(curlhandle, CURLOPT_HEADER, 0L);
			fseek(f, uploaded_len, SEEK_SET);
			curl_easy_setopt(curlhandle, CURLOPT_APPEND, 1L);
		}
		else { /* no */
			curl_easy_setopt(curlhandle, CURLOPT_APPEND, 0L);
		}
		r = curl_easy_perform(curlhandle);
	}
	fclose(f);
	if (r == CURLE_OK)
	{
		emit libb->finished(face1->file_name);
		return 1;
	}
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}
static size_t writeFunc2(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	int_clas *str = (int_clas *)userdata;
	int i = str->num;
	class_interface *interface_temp = str->in;
	split_ *node = interface_temp->spl[i];
	//tNode *node = (tNode *)userdata;
	size_t written = 0;
//	pthread_mutex_lock(&g_mutex);
	char *hgdg = (char*)ptr;
	if (node->startPos + size * nmemb <= node->endPos)
	{
		fseek(node->fp, node->startPos, SEEK_SET);
		written = fwrite(ptr, size, nmemb, node->fp);
		node->startPos += size * nmemb;
	}
	else
	{
		fseek(node->fp, node->startPos, SEEK_SET);
		written = fwrite(ptr, 1, node->endPos - node->startPos + 1, node->fp);
		node->startPos = node->endPos;
	}
//	pthread_mutex_unlock(&g_mutex);
	return written;
	//fclose
}


/************************************************************************/
/* ��ȡҪ���ص�Զ���ļ��Ĵ�С                                            */
/************************************************************************/
long getDownloadFileLenth(const char *url)
{
	double downloadFileLenth = 0;
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_HEADER, 1);   //ֻ��Ҫheaderͷ  
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);   //����Ҫbody  
	if (curl_easy_perform(handle) == CURLE_OK)
	{
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
	}
	else
	{
		downloadFileLenth = -1;
	}
	return downloadFileLenth;
}

void workThread(void *pData)
{
	int_clas *class_num = (int_clas*)pData;
	class_interface * pNode = (class_interface *)class_num->in;
	int i = class_num->num;
	int res = curl_easy_perform(pNode->spl[i]->curls);
	char buf[10] = { '\0' };
	itoa(i, buf, 10);
	if (res == 0)
	{
		emit pNode->dwn[i]->setvalue(pNode->spl[i]->all_num);
	}
	return ;
}
bool libcurl_network::downLoad(int threadNum, string Url, string Path)
{
	class_interface *face;
	class_interface *face1;
	const int temp_num = threadNum;
	HANDLE thread_handle[4];
	foreach(face, global::return_listitem())
	{
		if (face->url_name == url_name)
		{
			face1 = face;
			break;
		}
	}
	face1->if_group = true;
	face1->thread_num = threadNum;
	emit libb->start(file_namee);
	long fileLength = getDownloadFileLenth(Url.c_str());
	string file_phath = Path;
	if (fileLength <= 0)
	{
		printf("get the file length error...");
		return false;
	}  
	// Create a file to save package.  
	const string outFileName = Path;
	FILE *fp = fopen(outFileName.c_str(), "wb");
	if (!fp)
	{
		return false;
	}
	long partSize = fileLength / threadNum;
	for (int i = 0; i < threadNum; i++)
	{
		if (i == threadNum - 1)
		{
			partSize = fileLength - i * partSize;
		}
		int_clas *assemble_num_classinterface = new int_clas;
		assemble_num_classinterface->num = i;
		assemble_num_classinterface->in = face1;
		split_ *spi = new split_;
		face1->spl.push_back(spi);
		face1->spl[i]->num = i;
		face1->spl[i]->already_sizes = 0; //�����
		face1->spl[i]->all_num = partSize;
		if (i < threadNum - 1)
		{
			face1->spl[i]->startPos = i * partSize;
			face1->spl[i]->endPos = (i + 1) * partSize - 1;
		}
		else
		{
			face1->spl[i]->startPos = i * partSize;
			face1->spl[i]->endPos = fileLength - 1;
		}
		CURL *curl = curl_easy_init();
		curl_global_init(CURL_GLOBAL_DEFAULT);
		face1->spl[i]->curls = curl;
		face1->spl[i]->fp = fp;
		char range[64] = { 0 };
		_snprintf(range, sizeof (range), "%ld-%ld", face1->spl[i]->startPos, face1->spl[i]->endPos);
		curl_off_t len = get<4>(global::return_tuple()) * 1024;
		if (get<4>(global::return_tuple()) != 0)
		{
			curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, len);
		}
		curl_easy_setopt(curl, CURLOPT_URL, Url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "Anonymous:NULL");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunc2);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)assemble_num_classinterface);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void *)assemble_num_classinterface);
		curl_easy_setopt(curl, CURLOPT_RANGE, range);
		curl_easy_setopt(curl, CURLOPT_USERPWD, "Anonymous:NULL");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		//group_downn����������
		group_downn *dow = face1->dwn[i];
		dow->set_all(partSize);
		switch (i)
		{
		case 0:
			dow->setGeometry(0, 0, 100, 19);
			break;
		case 1:
			dow->setGeometry(0, 20, 100, 19);
			break;
		case 2:
			dow->setGeometry(0, 40, 100, 19);
			break;
		case 3:
			dow->setGeometry(0, 60, 100, 19);
			break;
		}
		dow->all = partSize;
		dow->already = 0;
		HANDLE hand = (HANDLE)_beginthread(workThread, 0, (void*)assemble_num_classinterface);
		thread_handle[i] = hand;
	}
	DWORD dawit = WaitForMultipleObjects(4, thread_handle, TRUE, INFINITE);
	if (dawit != WAIT_OBJECT_0)
	{
		
	}
	Sleep(1000);
	fclose(fp);
	emit libb->finished(face1->file_name);
	return 1;
}
