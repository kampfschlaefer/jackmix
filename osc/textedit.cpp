
#include "textedit.h"
#include "textedit.moc"

TextEdit::TextEdit( QWidget* p, const char* n ) : QTextEdit( p,n ) {
	setTextFormat( Qt::LogText );
}
TextEdit::~TextEdit() {
}

void TextEdit::appendData( QString path, QVariant data ) {
//	qDebug( "TextEdit::append( %s, %s (%s) )", path.latin1(), data.toString().latin1(), data.typeName() );
	this->append( QString( path )+" ("+data.toString()+")" );
}

