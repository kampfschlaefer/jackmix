
#include "mainwidget.h"
#include "mainwidget.moc"

#include "matrixelements.h"

#include <qlayout.h>
#include <qaction.h>

MatrixWidget::MatrixWidget( QWidget* p, const char* n ) : MixingMatrix::Widget( p,n ) {
	QAction* quit = new QAction( "Quit", CTRL+Key_Q, this );
	connect( quit, SIGNAL( activated() ), this, SLOT( deleteSelf() ) );

//	_layout = new QGridLayout( this,5,5 );
//	_layout->setAutoAdd( true );

	MixingMatrix::Server::the()->mode( MixingMatrix::Server::Select );

	MatrixElement* elem;
	for ( int i=0; i<25; i++ ) {
		elem = new MatrixElement( i%5, i/5, this );
		connect( elem, SIGNAL( replace( MatrixElement* ) ), this, SLOT( replace( MatrixElement* ) ) );
	}

	setBackgroundColor( QColor( 255,125,125 ) );
}

MatrixWidget::~MatrixWidget() {
qDebug( "MatrixWidget::~MatrixWidget()" );
}

void MatrixWidget::deleteSelf() {
	delete this;
}

void MatrixWidget::replace( MatrixElement* n ) {
//	_layout->setAutoAdd( false );
qDebug( "MatrixElement::replace( MatrixElement* %p )", n );
	int w = n->in();
	int h = n->out();
	int dw = n->neighbors();
	int dh = n->followers( dw );
	delete n;
	for ( int j=0; j<dh; j++ ) {
		for ( int i=0; i<dw; i++ ) {
			delete MixingMatrix::Server::the()->getResponsible( h+i, w+j );
		}
	}
	MultiElement* tmp = new MultiElement( dw, dh, this );
	tmp->setBackgroundColor( QColor( 125,255,125 ) );
	tmp->show();
//	_layout->addMultiCellWidget( tmp, h, h+dh, w, w+dw );
}

