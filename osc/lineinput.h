
#ifndef LINEINPUT_H
#define LINEINPUT_H

#include <qlineedit.h>

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

