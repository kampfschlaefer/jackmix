
#include "lineinput.h"
#include "lineinput.moc"

LineInput::LineInput( QWidget* p ) : QLineEdit( p ) {
	connect( this, SIGNAL( returnPressed() ), SLOT( enterPressed() ) );
}
LineInput::~LineInput() {
}

void LineInput::enterPressed() {
//	qDebug( "LineInput::enterPressed()" );
	emit textChanged( text() );
	clear();
}

