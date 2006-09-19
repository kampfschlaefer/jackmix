#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui/QTextEdit>
#include <QtCore/QVariant>

class TextEdit : public QTextEdit
{
Q_OBJECT
public:
	TextEdit( QWidget* =0, const char* =0 );
	~TextEdit();
public slots:
	void appendData( QString, QVariant=0 );
};

#endif // TEXTEDIT_H

