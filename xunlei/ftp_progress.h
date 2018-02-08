#ifndef FTP_PROGRESS_H
#define FTP_PROGRESS_H

#include <QObject>
#include <string>
class ftp_progress : public QObject
{
	Q_OBJECT

public:
	ftp_progress(QObject *parent);
	~ftp_progress();
	void set_urls(QString);
	void start(QString, long);
private:
	QString url_name;
	//std::string GBKToUTF8(const char* strGBK);
	//void get_length(char *);
	//size_t getLocalFileLenth(char *file_name);
	//size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream);
};

#endif // FTP_PROGRESS_H
