
#include "qfloatcontrol.h"
#include "qfloatcontrol.moc"

#include "qpixmapeffect.h"
#include <qpainter.h>
#include <qpixmap.h>

namespace JackMix { namespace GUI {

QFloatControl::QFloatControl( QWidget* p, const char* n ) : QFrame( p,n ) {
	setBackgroundColor( QColor( 255,200,200 ) );
}
QFloatControl::~QFloatControl() {
}

void QFloatControl::paintEvent( QPaintEvent* ) {
	//qDebug( "QFloatControl::paintEvent( QPaintEvent* )" );
}

}; };
