
#include "textedit.h"
#include "textedit.moc"

TextEdit::TextEdit( QWidget* p, const char* n ) : QTextEdit( p,n ) {
	setTextFormat( Qt::LogText );
}
TextEdit::~TextEdit() {
}

void TextEdit::appendData( QString data ) {
	qDebug( "TextEdit::append( %s )", data.latin1() );
	this->append( data );
}

