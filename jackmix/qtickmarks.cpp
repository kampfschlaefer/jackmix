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

#include "qtickmarks.h"
#include "qtickmarks.moc"

#include <iostream>
#include <qpainter.h>
#include <qfont.h>
#include <qfontmetrics.h>

using namespace JackMix;

QTickmarks::QTickmarks( float _dbmin, float _dbmax, Direction dir, long pos, QWidget* p, const char* n )
 : QFrame( p,n )
 , dB2VolCalc( _dbmin, _dbmax )
 , _pos( pos )
 , _dir( dir )
 , minstep( 1 )
 , substep( 0.5 )
{
	setMinimumSize( 20,20 );
}

void QTickmarks::drawContents( QPainter* p ) {
	//std::cerr << "KTickmarks::drawContents( QPainter* " << p << " )" << std::endl;
	bool left=false, right=false;
	if ( _pos&posLeft ) left=true;
	if ( _pos&posRight ) right=true;
	// Setting the font
	QFont font;
	font.setPixelSize( 8 /*font.pixelSize()/2*/ ); // Maybe this could be adjusted...
	p->setFont( font );
	// Determining the size of the largest text (currently the text at the minimum-scale)
	QFontMetrics fontmetric( font );
	QRect fontrect = fontmetric.boundingRect( QString::number( dbmin ) );
	// Calculating stepsizes
	float _minstepcount = ( dbmax-dbmin )/minstep;
	float _minstep = minstep; // this value gets changed
	float _substepcount = ( dbmax-dbmin )/substep;
	float _substep = substep; // this value gets changed
	// Calculating minimum size of the widget
	int _minsize;
	// Shorcuts
	int w,h;
	QColor colornormal = colorGroup().foreground();
	QColor colordiff = colorGroup().buttonText();

	if ( _dir == BottomToTop || _dir == TopToBottom ) {
		p->translate( contentsRect().left(), contentsRect().bottom() );
		// Calculating stepsizes
		for ( int i=1; _minstepcount*( fontrect.height()+4 ) > contentsRect().height(); i++ ) {
			_minstepcount = ( dbmax-dbmin ) / minstep / i;
			_minstep = minstep*i;
		}
		while ( _substepcount*2 > contentsRect().height() ) { _substepcount/=2; _substep*=2; }
		// Calculating minimum size of the widget
		_minsize=fontrect.width()+4;
		if ( left ) _minsize+=6;
		if ( right ) _minsize+=6;
		setMinimumWidth( _minsize + frameWidth() + 2 );
//		setMaximumWidth( _minsize /*+6*/ );
		w = contentsRect().width(); // Just a shortcut
		h=0;
		// Painting substep marks
		p->setPen( QPen( colordiff, 1 ) );
		for ( float i=dbmax; i>=dbmin; i-=_substep ) {
			h = int( -contentsRect().height() * dbtondb( i ) );
			if ( _dir==TopToBottom ) h = 1 - h;
			if ( left ) p->drawLine( 0, h, 3, h );
			if ( right ) p->drawLine( w-3, h, w, h );
		}
		// Painting step marks and texts
		p->setPen( QPen( colornormal, 1 ) );
		for ( float i=0; i>=dbmin; i-=_minstep ) {
			h = int( -contentsRect().height() * dbtondb( i ) );
			if ( _dir==TopToBottom ) h = 1 - h;
			if ( left ) p->drawLine( 0, h, 6, h );
			p->drawText( ( w - (left)*6 - (right)*6 - fontrect.width() )/2 + (left)*6
			             , h-fontrect.height()/2,
			             fontrect.width(), fontrect.height()+2,
			             Qt::AlignRight|Qt::AlignTop, QString::number( i ) );
			if ( right ) p->drawLine( w-6, h, w, h );
		}
		for ( float i=_minstep; i<=dbmax; i+=_minstep ) {
			h = int( -contentsRect().height() * dbtondb( i ) );
			if ( _dir==TopToBottom ) h = 1 - h;
			if ( left ) p->drawLine( 0, h, 6, h );
			p->drawText( ( w - (left)*6 - (right)*6 - fontrect.width() )/2 + (left)*6
			             , h-fontrect.height()/2,
			             fontrect.width(), fontrect.height()+2,
			             Qt::AlignRight|Qt::AlignTop, QString::number( i ) );
			if ( right ) p->drawLine( w-6, h, w, h );
		}
	} else {
	//if ( _dir == LeftToRight || _dir == RightToLeft ) {
		// Calculating stepsizes
		for ( int i=1; _minstepcount*( fontrect.width()+4 ) > contentsRect().width(); i++ ) {
			_minstepcount = ( dbmax-dbmin ) / minstep / i;
			_minstep = minstep*i;
		}
		while ( _substepcount*2 > contentsRect().width() ) { _substepcount/=2; _substep*=2; }
		// Calculating minimum size of the widget
		_minsize=fontrect.height()+4;
		if ( left ) _minsize+=6;
		if ( right ) _minsize+=6;
		setMinimumHeight( _minsize + frameWidth() + 2 );
//		setMaximumHeight( _minsize /*+6*/ );
		w = 0; // Just a shortcut
		h = frameWidth() + contentsRect().height();
		// Painting substep marks
		p->setPen( QPen( colordiff, 1 ) );
		for ( float i=dbmax; i>=dbmin; i-=_substep ) {
			w = this->frameWidth()+ int( contentsRect().width() * dbtondb( i ) );
			if ( _dir==RightToLeft ) w = 1 - w;
			if ( left ) p->drawLine( w, frameWidth(), w, frameWidth() + 3 );
			if ( right ) p->drawLine( w, h-3, w, h );
		}
		// Painting step marks and texts
		p->setPen( QPen( colornormal, 1 ) );
		for ( float i=0; i>=dbmin; i-=_minstep ) {
			w = int( contentsRect().width() * dbtondb( i ) );
			if ( _dir==RightToLeft ) w = 1 - w;
			if ( left ) p->drawLine( w, 0, w, 6 );
			p->drawText( w - fontrect.width()/2
			             , ( h - (left)*6 - (right)*6 - fontrect.height() )/2 + (left)*6,
			             fontrect.width(), fontrect.height()+2,
			             Qt::AlignRight|Qt::AlignTop, QString::number( i ) );
			if ( right ) p->drawLine( w, h-6, w, h );
		}
		for ( float i=_minstep; i<=dbmax; i+=_minstep ) {
			h = int( -contentsRect().height() * dbtondb( i ) );
			if ( _dir==RightToLeft ) w = 1 - w;
			if ( left ) p->drawLine( w, 0, w, 6 );
			p->drawText( w - fontrect.width()/2
			             , ( h - (left)*6 - (right)*6 - fontrect.height() )/2 + (left)*6,
			             fontrect.width(), fontrect.height()+2,
			             Qt::AlignRight|Qt::AlignTop, QString::number( i ) );
			if ( right ) p->drawLine( w, h-6, w, h );
		}
	}
}

// vim: sw=4 ts=4
