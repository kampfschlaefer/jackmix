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

#include "mixingmatrix.h"
#include "mixingmatrix_privat.h"
#include "mixingmatrix.moc"

namespace JackMix {
namespace MixingMatrix {


Widget::Widget( QStringList ins, QStringList outs, QWidget* p, const char* n )
	: QFrame( p,n )
	, _mode( Normal )
	, _direction( None )
	, _inchannels( ins )
	, _outchannels( outs )
{
	if ( _inchannels.size()==0 ) {
		_direction = Vertical;
		_inchannels = _outchannels;
	}
	if ( _outchannels.size()==0 ) {
		_direction = Horizontal;
		_outchannels = _inchannels;
	}
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}
Widget::~Widget() {
}


void Widget::addElement( Element* n ) {
	_elements.push_back( n );
	connect( n, SIGNAL( replace( Element* ) ), this, SLOT( replace( Element* ) ) );
}
void Widget::removeElement( Element* n ) { _elements.remove( n ); }

void Widget::replace( Element* n ) {
qDebug( "Widget::replace( Element* %p )", n );
qDebug( "This Element has %i selected neighbors.", n->neighbors() );
qDebug( " and %i selected followers.", n->followers( n->neighbors() ) );
	QStringList in, out;
	in = n->neighborsList();
	qDebug( "Selected ins = %s", in.join( "," ).ascii() );
	out = n->followersList();
	qDebug( "Selected outs = %s", out.join( "," ).ascii() );
	for ( QStringList::ConstIterator it=out.begin(); it!=out.end(); ++it ) {
		for ( QStringList::ConstIterator jt=in.begin(); jt!=in.end(); ++jt ) {
			Element* tmp = getResponsible( ( *jt ),( *it ) );
			qDebug( "About to delete %p", tmp );
			if ( tmp ) {
				removeElement( tmp );
				delete tmp;
			}
		}
	}
	createControl( in, out );
	autoFill();
}

Element* Widget::getResponsible( QString in, QString out ) const {
	for ( uint i=0; i<_elements.size(); i++ )
		if ( _elements[ i ]->isResponsible( in, out ) )
			return _elements[ i ];
	return 0;
}

bool Widget::createControl( QStringList inchannels, QStringList outchannels ) {
qDebug( "Widget::createControl( QStringList '%s', QStringList '%s')", inchannels.join( "," ).latin1(), outchannels.join( "," ).latin1() );

	QString control = Global::the()->canCreate( inchannels.size(), outchannels.size() )[ 0 ];
	//qDebug( "Found %s to control [%i,%i] channels", control.latin1(), inchannels.size(), outchannels.size() );
	return Global::the()->create( control, inchannels, outchannels, this );

	return false;
}

void Widget::autoFill() {
qDebug( "\nWidget::autoFill()" );
//	qDebug( "_direction = %i", _direction );
	if ( _direction == None ) {
//		qDebug( "Doing the Autofill-boogie..." );
		for ( QStringList::Iterator init=_inchannels.begin(); init!=_inchannels.end(); ++init )
			for ( QStringList::Iterator outit=_outchannels.begin(); outit!=_outchannels.end(); ++outit ) {
				if ( !getResponsible( *init, *outit ) ) {
//					qDebug( "...together with (%s|%s)", ( *init ).latin1(), ( *outit ).latin1() );
					createControl( QStringList()<<*init, QStringList()<<*outit );
				}
//				else qDebug( "   (%s|%s) is allready occupied. :(", ( *init ).latin1(), ( *outit ).latin1() );
			}
	} else if ( _direction == Vertical ) {
		for ( QStringList::Iterator outit=_outchannels.begin(); outit!=_outchannels.end(); ++outit )
			if ( !getResponsible( *outit, *outit ) )
				createControl( QStringList()<<*outit, QStringList()<<*outit );
	} else if ( _direction == Horizontal ) {
		for ( QStringList::Iterator init=_inchannels.begin(); init!=_inchannels.end(); ++init )
			if ( !getResponsible( *init, *init ) )
				createControl( QStringList()<<*init, QStringList()<<*init );
	}
	resizeEvent( 0 );
	qDebug( "\n" );
}

void Widget::resizeEvent( QResizeEvent* ) {
	setMinimumSize( sizeHint() );
	//qDebug( "MinimumSize = (%i,%i)", sizeHint().width(), sizeHint().height() );
	int ins = _inchannels.size(), outs = _outchannels.size();
	//qDebug( "%i InChannels and %i OutChannels", ins, outs );

	int w=1, h=1;
	if ( ins && outs ) {
		w = width()/ins;
		h = height()/outs;
	}

	if ( smallestElement().width()>w )
		w = smallestElement().width();
	if ( smallestElement().height()>h )
		h = smallestElement().height();

	if ( _direction == Horizontal )
//		h=0;
		for ( uint i=0; i<_elements.size(); i++ ) {
			_elements[ i ]->setGeometry( _inchannels.findIndex( _elements[ i ]->in()[ 0 ] )*w, 0, w*_elements[ i ]->inchannels(), h );
			_elements[ i ]->show();
		}
	else if ( _direction == Vertical )
//		w=0;
		for ( uint i=0; i<_elements.size(); i++ ) {
			_elements[ i ]->setGeometry( 0, _outchannels.findIndex( _elements[ i ]->out()[ 0 ] )*h, w, h*_elements[ i ]->outchannels() );
			_elements[ i ]->show();
		}
	else
		for ( uint i=0; i<_elements.size(); i++ ) {
			_elements[ i ]->setGeometry( _inchannels.findIndex( _elements[ i ]->in()[ 0 ] )*w, _outchannels.findIndex( _elements[ i ]->out()[ 0 ] )*h, w*_elements[ i ]->inchannels(), h*_elements[ i ]->outchannels() );
			_elements[ i ]->show();
		}
}

QSize Widget::minimumSizeHint() const {
	//qDebug( "Widget::minimumSizeHint()" );
	QSize smallest = smallestElement();
	if ( _direction == Horizontal )
		return QSize(  smallest.width()*_inchannels.size(),smallest.height() );
	if ( _direction == Vertical )
		return QSize(  smallest.width(),smallest.height()*_outchannels.size() );
	return QSize( smallest.width()*_inchannels.size(),smallest.height()*_outchannels.size() );
}

QSize Widget::smallestElement() const {
	//qDebug( "Widget::smallestElement()" );
	int w = 1, h = 1;
	for ( int i=0; i<elements(); i++ ) {
		if ( _elements[ i ]->minimumSizeHint().width() > w )
			w = _elements[ i ]->minimumSizeHint().width();
		if ( _elements[ i ]->minimumSizeHint().height() > h )
			h = _elements[ i ]->minimumSizeHint().height();
	}
	return QSize( w,h );
}

QString Widget::nextIn( QString n ) const {
	for ( QStringList::ConstIterator it = _inchannels.begin(); it != _inchannels.end(); ++it )
		if ( ( *it ) == n ) { ++it; return ( *it ); }
	return 0;
}
QString Widget::nextOut( QString n ) const {
	for ( QStringList::ConstIterator it = _outchannels.begin(); it != _outchannels.end(); ++it )
		if ( ( *it ) == n ) { ++it; return ( *it ); }
	return 0;
}

void Widget::addinchannel( QString name ) {
	_inchannels.push_back( name );
	this->updateGeometry();
}
void Widget::addoutchannel( QString name ) {
	_outchannels.push_back( name );
	this->updateGeometry();
}

Element::Element( QStringList in, QStringList out, Widget* p, const char* n )
	: QFrame( p,n )
	, _in( in )
	, _out( out )
	, _selected( false )
	, _parent( p )
{
	//qDebug( "MixingMatrix::Element::Element( QStringList '%s', QStringList '%s' )", in.join(",").latin1(), out.join(",").latin1() );
	_parent->addElement( this );
	setMargin( 2 );
	setFrameStyle( QFrame::Raised|QFrame::StyledPanel );
	setLineWidth( 2 );
}
Element::~Element() {
	//qDebug( "MixingMatrix::Element::~Element()" );
	_parent->removeElement( this );
}

bool Element::isResponsible( QString in, QString out ) {
	if ( _in.findIndex( in )>-1 && _out.findIndex( out )>-1 )
		return true;
	return false;
}

void Element::select( bool n ) {
qDebug( "MixingMatrix::Element::select( bool %i )", n );
	if ( n != _selected ) {
		if ( _parent->mode() == Widget::Select ) {
			_selected = n;
			if ( _selected ) {
				setFrameShadow( QFrame::Sunken );
				setBackgroundColor( colorGroup().background().dark() );
			} else {
				setFrameShadow( QFrame::Raised );
				setBackgroundColor( colorGroup().background() );
			}
			isSelected( n );
		}
	}
}

int Element::neighbors() const {
//	qDebug( "neighbor: %s", _parent->nextIn( _in[ 0 ] ).latin1() );
	Element* neighbor = _parent->getResponsible( _parent->nextIn( _in[ _in.size()-1 ] ), _out[ 0 ] );
	if ( neighbor && neighbor->isSelected() )
		return neighbor->neighbors()+1;
	return 0;
}
QStringList Element::neighborsList() const {
//	qDebug( "self = [%s]", _in.join( "|" ).latin1() );
//	qDebug( "neighbor = %s", _parent->nextIn( _in[ _in.size()-1 ] ).latin1() );
	Element* neighbor = _parent->getResponsible( _parent->nextIn( _in[ _in.size()-1 ] ), _out[ 0 ] );
	QStringList tmp;
	if ( neighbor && neighbor->isSelected() )
		tmp = neighbor->neighborsList();
	tmp += _in;
	tmp.sort();
	return tmp;
}
int Element::followers( int n ) const {
//qDebug( "Element::followers( int %i )", n );
	Element* follower = _parent->getResponsible( _in[ 0 ], _parent->nextOut( _out[ _out.size()-1 ] ) );
/*	if ( follower )
		qDebug( "follower of %p is %p which has selected=%i", this, follower, follower->isSelected() );
	else
		qDebug( "follower of %p is %p", this, follower );*/
	if ( follower && follower->isSelected() && follower->neighbors() >= n )
		return follower->followers( n )+1;
	return 0;
}
QStringList Element::followersList() const {
//	qDebug( "self = [%s]", _out.join( "|" ).latin1() );
//	qDebug( "follower = %s", _parent->nextOut( _out[ _out.size()-1 ] ).latin1() );
	Element* follower = _parent->getResponsible( _in[ 0 ], _parent->nextOut(  _out[  _out.size()-1 ] ) );
	QStringList tmp;
	if ( follower && follower->isSelected() )
		tmp = follower->followersList();
	tmp += _out;
	tmp.sort();
	return tmp;
}


ElementFactory::ElementFactory() {
	Global::the()->registerFactory( this );
}
ElementFactory::~ElementFactory() {
	Global::the()->unregisterFactory( this );
}

void ElementFactory::globaldebug() {
	Global::the()->debug();
}

Global::Global() {}
Global::~Global() {}

Global* Global::the() {
	static Global* tmp = new Global();
	return tmp;
}

void Global::registerFactory( ElementFactory* n ) {
	//qDebug( "Global::registerFactory( ElementFactory* %p )", n );
	_factories.push_back( n );
}
void Global::unregisterFactory( ElementFactory* n ) {
	//qDebug( "Global::unregisterFactory( ElementFactory* %p )", n );
	_factories.remove( n );
}

QStringList Global::canCreate( int in, int out ) {
	QStringList tmp;
	for ( uint i=0; i<_factories.size(); i++ ) {
		tmp += _factories[ i ]->canCreate( in, out );
	}
	return tmp;
}

bool Global::create( QString type, QStringList ins, QStringList outs, Widget* parent, const char* name ) {
	//qDebug( "Global::create( QString %s, QStringList '%s', QStringList '%s', Widget* %p, const char* %s )", type.latin1(), ins.join( "," ).latin1(), outs.join( "," ).latin1(), parent, name );
	Element* elem=0;
	for ( uint i=0; i<_factories.size() && elem==0; i++ ) {
		elem = _factories[ i ]->create( type, ins, outs, parent, name );
	}
	qDebug( "Will show and return %p", elem );
	if ( elem ) {
		elem->show();
	}
	return elem;
}

void Global::debug() {
	for ( uint i=0; i<_factories.size(); i++ )
		qDebug( "The factory %p can create '%s'", _factories[ i ], _factories[ i ]->canCreate().join( " " ).latin1() );
}


}; // MixingMatrix
}; //JackMix

