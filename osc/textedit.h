#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <qtextedit.h>

class TextEdit : public QTextEdit
{
Q_OBJECT
public:
	TextEdit( QWidget* =0, const char* =0 );
	~TextEdit();
public slots:
	void appendData( QString );
};

#endif // TEXTEDIT_H

