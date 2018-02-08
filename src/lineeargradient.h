#ifndef LINEEARGRADIENT_H
#define LINEEARGRADIENT_H

#include <QWidget>

class lineeargradient : public QWidget
{
	Q_OBJECT

public:
	lineeargradient(QWidget *parent);
	~lineeargradient();
	void set_scale(int x, int y);
private:
	
};

#endif // LINEEARGRADIENT_H
