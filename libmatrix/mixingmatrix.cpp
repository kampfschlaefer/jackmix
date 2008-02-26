/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

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
#include "mixingmatrix_privat.moc"

#include <QtGui/QLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QCursor>
#include <QtCore/QTimer>
#include <QtGui/QContextMenuEvent>
#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>

namespace JackMix {
namespace MixingMatrix {

Widget::Widget( QStringList ins, QStringList outs, JackMix::BackendInterface* backend, QWidget* p, const char* n )
	: QFrame( p )
	, _mode( Normal )
	, _direction( None )
	, _inchannels( ins )
	, _outchannels( outs )
	, _backend( backend )
{
	if ( _inchannels.size()==0 ) {
		_direction = Vertical;
		_inchannels = _outchannels;
	}
	if ( _outchannels.size()==0 ) {
		_direction = Horizontal;
		_outchannels = _inchannels;
	}
	setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}
Widget::~Widget() {
}


void Widget::addElement( Element* n ) {
	_elements.push_back( n );
	connect( n, SIGNAL( replace( Element* ) ), this, SLOT( replace( Element* ) ) );
	resizeEvent( 0 );
}
void Widget::removeElement( Element* n ) {
	_elements.removeAll( n );
}

void Widget::replace( Element* n ) {
	//qDebug( "Widget::replace( Element* %p )", n );
	//qDebug( "This Element has %i selected neighbors.", n->neighbors() );
	//qDebug( " and %i selected followers.", n->followers( n->neighbors() ) );
	QStringList in, out;
	in = n->neighborsList();
	//qDebug( "Selected ins = %s", qPrintable( in.join( "," ) ) );
	out = n->followersList();
	//qDebug( "Selected outs = %s", qPrintable( out.join( "," ) ) );
	for ( QStringList::ConstIterator it=out.begin(); it!=out.end(); ++it ) {
		for ( QStringList::ConstIterator jt=in.begin(); jt!=in.end(); ++jt ) {
			Element* tmp = getResponsible( ( *jt ),( *it ) );
			//qDebug( "About to delete %p", tmp );
			if ( tmp )
				delete tmp;
		}
	}
	createControl( in, out );
	//autoFill();
	QTimer::singleShot( 100, this, SLOT( autoFill() ) );
}

Element* Widget::getResponsible( QString in, QString out ) const {
	//qDebug() << "Widget::getResponsible(" << in << "," << out << ") size =" << _elements.size();
	for ( int i=0; i<_elements.size(); i++ )
		if ( _elements[ i ] && _elements[ i ]->isResponsible( in, out ) )
			return _elements[ i ];
	return 0;
}

bool Widget::createControl( QStringList inchannels, QStringList outchannels ) {
	//qDebug( "Widget::createControl( QStringList '%s', QStringList '%s')", qPrintable( inchannels.join( "," ) ), qPrintable( outchannels.join( "," ) ) );

	QStringList controls = Global::the()->canCreate( inchannels.size(), outchannels.size() );
	if ( ! controls.isEmpty() ) {
		//qDebug( "Found %s to control [%i,%i] channels", controls.front().toStdString().c_str(), inchannels.size(), outchannels.size() );
		return Global::the()->create( controls.front(), inchannels, outchannels, this );
	}

	return false;
}

void Widget::autoFill() {
	//qDebug() << "\nWidget::autoFill()";
	//qDebug() << " _direction = " << _direction;
	if ( _direction == None ) {
		//qDebug( "Doing the Autofill-boogie..." );
		for ( QStringList::Iterator init=_inchannels.begin(); init!=_inchannels.end(); ++init )
			for ( QStringList::Iterator outit=_outchannels.begin(); outit!=_outchannels.end(); ++outit ) {
				if ( !getResponsible( *init, *outit ) )// {
					//qDebug( "...together with (%s|%s)", qPrintable( *init ), qPrintable( *outit ) );
					createControl( QStringList()<<*init, QStringList()<<*outit );
				//}
				//else qDebug( "   (%s|%s) is allready occupied. :(", qPrintable( *init ), qPrintable( *outit ) );
			}
	} else if ( _direction == Vertical ) {
		//qDebug() << "Available outputs are" << _outchannels.join( "," );
		for ( QStringList::Iterator outit=_outchannels.begin(); outit!=_outchannels.end(); ++outit ) {
			//qDebug() << "Setting element for" << *outit;
			if ( !getResponsible( *outit, *outit ) )
				createControl( QStringList()<<*outit, QStringList()<<*outit );
		}
	} else if ( _direction == Horizontal ) {
		//qDebug() << "Available inputs are" << _inchannels.join( "," );
		for ( QStringList::Iterator init=_inchannels.begin(); init!=_inchannels.end(); ++init ) {
			//qDebug() << "Setting element for" << *init;
			if ( !getResponsible( *init, *init ) )// {
				//qDebug() << " No responsible element found, creating a new one";
				createControl( QStringList()<<*init, QStringList()<<*init );
			//}
		}
	}
	resizeEvent( 0 );
	//qDebug() << "";
}

void Widget::resizeEvent( QResizeEvent* ) {
	setMinimumSize( sizeHint() );
	//qDebug( "MinimumSize = (%i,%i)", sizeHint().width(), sizeHint().height() );
	int ins = _inchannels.size(), outs = _outchannels.size();
	//qDebug( "%i InChannels and %i OutChannels", ins, outs );
	//qDebug( " in: %s\n out: %s", qPrintable( _inchannels.join( "," ) ), qPrintable( _outchannels.join( "," ) ) );

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
		//h=0;
		for ( int i=0; i<_elements.size(); i++ ) {
			_elements[ i ]->setGeometry( _inchannels.indexOf( _elements[ i ]->in()[ 0 ] )*w, 0, w*_elements[ i ]->inchannels(), h );
			_elements[ i ]->show();
		}
	else if ( _direction == Vertical )
		//w=0;
		for ( int i=0; i<_elements.size(); i++ ) {
			_elements[ i ]->setGeometry( 0, _outchannels.indexOf( _elements[ i ]->out()[ 0 ] )*h, w, h*_elements[ i ]->outchannels() );
			_elements[ i ]->show();
		}
	else
		for ( int i=0; i<_elements.size(); i++ ) {
			_elements[ i ]->setGeometry( _inchannels.indexOf( _elements[ i ]->in()[ 0 ] )*w, _outchannels.indexOf( _elements[ i ]->out()[ 0 ] )*h, w*_elements[ i ]->inchannels(), h*_elements[ i ]->outchannels() );
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
		w = qMax( _elements[ i ]->minimumSizeHint().width(), w );
		h = qMax( _elements[ i ]->minimumSizeHint().height(), h );
	}
	return QSize( w,h );
}

QString Widget::nextIn( QString n ) const {
	//qDebug() << "Widget::nextIn(" << n << ")";
	if ( n.isNull() )
		return 0;
	int i = _inchannels.indexOf( n ) + 1;
	//qDebug() << " i=" << i;
	if ( i < _inchannels.size() )
		return _inchannels.at( i );
	return 0;
}
QString Widget::nextOut( QString n ) const {
	if ( n.isNull() )
		return 0;
	int i = _outchannels.indexOf( n ) + 1;
	if ( i<_outchannels.size() )
		return _outchannels.at( i );
	return 0;
}

void Widget::addinchannel( QString name ) {
	//qDebug() << "Widget::addinchannel(" << name << ")";
	_inchannels.push_back( name );
	if ( _direction == Horizontal )
		_outchannels.push_back( name );
	this->updateGeometry();
}
void Widget::addoutchannel( QString name ) {
	//qDebug() << "Widget::addoutchannel(" << name << ")";
	_outchannels.push_back( name );
	if ( _direction == Vertical )
		_inchannels.push_back( name );
	this->updateGeometry();
}
void Widget::removeinchannel( QString name ) {
	//qDebug() << "Widget::removeinchannel(" << name << ")";
	for ( QStringList::Iterator it = _outchannels.begin(); it != _outchannels.end(); it++ ) {
		Element* tmp = getResponsible( name, *it );
		if ( tmp )
			delete tmp;
	}
	_inchannels.removeAll( name );
}
void Widget::removeoutchannel( QString name ) {
	//qDebug() << "Widget::removeoutchannel(" << name << ")";
	for ( QStringList::Iterator it = _inchannels.begin(); it != _inchannels.end(); it++ ) {
		Element* tmp = getResponsible( *it, name );
		if ( tmp )
			delete tmp;
	}
	_outchannels.removeAll( name );
}



void Widget::debugPrint() {
	qDebug( "\nWidget::debugPrint()" );
}


Element::Element( QStringList in, QStringList out, Widget* p, const char* n )
	: QFrame( p )
	, _in( in )
	, _out( out )
	, _selected( false )
	, _parent( p )
	, _menu( new QMenu( this ) )
{
	//qDebug( "MixingMatrix::Element::Element( QStringList '%s', QStringList '%s' )", qPrintable( in.join(",") ), qPrintable( out.join(",") ) );
	setFrameStyle( QFrame::Raised|QFrame::Panel );
	setLineWidth( 1 );
	QTimer::singleShot( 1, this, SLOT( lazyInit() ) );
	setAutoFillBackground( true );
}
Element::~Element() {
	//qDebug( "MixingMatrix::Element::~Element()" );
	_parent->removeElement( this );
}
void Element::lazyInit() {
	_parent->addElement( this );
}

bool Element::isResponsible( QString in, QString out ) {
	if ( _in.indexOf( in )>-1 && _out.indexOf( out )>-1 )
		return true;
	return false;
}

void Element::select( bool n ) {
	//qDebug( "MixingMatrix::Element::select( bool %i )", n );
	if ( n != _selected ) {
		if ( _parent->mode() == Widget::Select ) {
			_selected = n;
			QPalette pal;
			if ( _selected ) {
				setFrameShadow( QFrame::Sunken );
				pal.setColor( QPalette::Window, pal.color( QPalette::Window ).dark() );
			} else {
				setFrameShadow( QFrame::Raised );
				pal.setColor( QPalette::Window, pal.color( QPalette::Window ).light() );
			}
			setPalette( pal );
			isSelected( n );
		}
	}
}

int Element::neighbors() const {
	Element* neighbor = _parent->getResponsible( _parent->nextIn( _in[ _in.size()-1 ] ), _out[ 0 ] );
	if ( neighbor && neighbor->isSelected() )
		return neighbor->neighbors()+1;
	return 0;
}
QStringList Element::neighborsList() const {
	//qDebug( "self = [%s]", qPrintable( _in.join( "|" ) ) );
	//qDebug( "neighbor = %s", qPrintable( _parent->nextIn( _in[ _in.size()-1 ] ) ) );
	Element* neighbor = _parent->getResponsible( _parent->nextIn( _in[ _in.size()-1 ] ), _out[ 0 ] );
	QStringList tmp;
	if ( neighbor && neighbor->isSelected() )
		tmp = neighbor->neighborsList();
	tmp = _in + tmp;
	return tmp;
}
int Element::followers( int n ) const {
	if ( n==0 )
		return 0;
	Element* follower = _parent->getResponsible( _in[ 0 ], _parent->nextOut( _out[ _out.size()-1 ] ) );
	if ( follower && follower->isSelected() && follower->neighbors() >= n )
		return follower->followers( n )+1;
	return 0;
}
QStringList Element::followersList() const {
	//qDebug( "self = [%s]", qPrintable( _out.join( "|" ) ) );
	//qDebug( "follower = %s", qPrintable( _parent->nextOut( _out[ _out.size()-1 ] ) ) );
	Element* follower = _parent->getResponsible( _in[ 0 ], _parent->nextOut(  _out[  _out.size()-1 ] ) );
	QStringList tmp;
	if ( follower && follower->isSelected() )
		tmp = follower->followersList();
	tmp = _out + tmp;
	return tmp;
}

void Element::showMenu() {
	_menu->exec( QCursor::pos() );
}
void Element::contextMenuEvent( QContextMenuEvent* ev ) {
	//qDebug() << "Element::contextMenuEvent(" << ev << ") is accepted?" <<ev->isAccepted();
	showMenu();
	ev->accept();
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
	_factories.removeAll( n );
}

QStringList Global::canCreate( int in, int out ) {
	//qDebug() << "Global::canCreate(" << in << "," << out << ")";
	QStringList tmp;
	for ( int i=0; i<_factories.size(); i++ )
		tmp += _factories[ i ]->canCreate( in, out );
	//qDebug() << " returning" << tmp;
	return tmp;
}

bool Global::create( QString type, QStringList ins, QStringList outs, Widget* parent, const char* name ) {
	//qDebug( "Global::create( QString %s, QStringList '%s', QStringList '%s', Widget* %p, const char* %s )", qPrintable( type ), qPrintable( ins.join( "," ) ), qPrintable( outs.join( "," ) ), parent, name );
	Element* elem=0;
	for ( int i=0; i<_factories.size() && elem==0; i++ )
		elem = _factories[ i ]->create( type, ins, outs, parent, name );
	//qDebug( "Will show and return %p", elem );
	if ( elem )
		elem->show();
	return elem;
}

void Global::debug() {
	for ( int i=0; i<_factories.size(); i++ )
		qDebug("The factory %p can create '%s'",_factories[ i ],qPrintable(_factories[ i ]->canCreate().join(" ")));
}


}; // MixingMatrix
}; //JackMix

