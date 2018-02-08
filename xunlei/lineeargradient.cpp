#include "lineeargradient.h"

lineeargradient::lineeargradient(QWidget *parent)
	: QWidget(parent)
{

}

void lineeargradient::set_scale(int x, int y)
{
	this->setFixedSize(x, y);
}
lineeargradient::~lineeargradient()
{

}
