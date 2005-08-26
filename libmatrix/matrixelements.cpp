/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#include "matrixelements.h"
#include "matrixelements.moc"

#include <qlayout.h>

using namespace JackMix;

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

