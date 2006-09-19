
#ifndef LINEINPUT_H
#define LINEINPUT_H

#include <QtGui/QLineEdit>

class LineInput : public QLineEdit
{
Q_OBJECT
public:
	LineInput( QWidget* =0 );
	~LineInput();
public slots:
	void enterPressed();
signals:
	void textChanged( QString );
};

#endif

