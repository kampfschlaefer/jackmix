
#include "matrixelements.h"
#include "matrixelements.moc"

#include <qlayout.h>


MatrixElement::MatrixElement( int col, int row, MixingMatrix::Widget* p, const char* n )
	: MixingMatrix::Element( col, row, p, n )
{
	//qDebug( "MatrixElement::MatrixElement()" );
	new QGridLayout( this, 1,2, 5, 5 );
	layout()->setAutoAdd( true );

	_btn_select = new QPushButton( "Select", this );
	connect( _btn_select, SIGNAL( clicked() ), this, SLOT( toggleselect() ) );
	_btn_replace = new QPushButton( "Replace", this );
	connect( _btn_replace, SIGNAL( clicked() ), this, SLOT( slot_replace() ) );

	setMinimumSize( 30, 30 );
}

MatrixElement::~MatrixElement() {
	//qDebug( "MatrixElement::~MatrixElement()" );
}

void MatrixElement::toggleselect() {
	//qDebug( "MatrixElement::toggleselect()" );
	select( ! isSelected() );
}

void MatrixElement::slot_replace() {
	qDebug( "MatrixElement::slot_replace()" );
	qDebug( "Trying to replace %i rows of %i elements", followers( neighbors() ), neighbors() );
	emit replace( this );
}

void MatrixElement::mouseDoubleClickEvent( QMouseEvent* ) {
qDebug( "MatrixElement::mouseDoubleClickEvent( QMouseEvent * )" );
//qDebug( "Trying to replace %i rows of %i elements", followers( neighbors() ), neighbors() );
//	emit replace( this );
}


MultiElement::MultiElement( int width, int height, QWidget* p, const char* n )
	: QFrame( p,n )
	, _width( width )
	, _height( height )
{
qDebug( "MultiElement::MultiElement( int %i, int %i, QWidget* %p, const char* %s )", width, height, p, n );
	setMargin( 2 );
	setFrameStyle( QFrame::Panel|QFrame::Sunken );
	setLineWidth( 2 );
}

MultiElement::~MultiElement() {
qDebug( "MultiElement::~MultiElement()" );
}

void MultiElement::mouseDoubleClickEvent( QMouseEvent* ) {
qDebug( "MultiElement::mouseDoubleClickEvent()" );
}

