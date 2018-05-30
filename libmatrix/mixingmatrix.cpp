/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

    Modified to permit control via MIDI by Nick Bailey <nick@n-ism.org>
    Released as version 0.5.0 (et seq.) copyright 2013.
    
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


#include "aux_elements.h"
#include "controlsender.h"

#include <QtWidgets/QLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtGui/QCursor>
#include <QtCore/QTimer>
#include <QtGui/QContextMenuEvent>
#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtGui/QColor>

namespace JackMix {
namespace MixingMatrix {
        
const QColor Widget::indicatorColors[] { QColor("dimgrey"), QColor("lawngreen"), QColor("orange"), QColor("red") };

Widget::Widget( QStringList ins, QStringList outs, JackMix::BackendInterface* backend, QWidget* p, const char* n )
	: QFrame( p )
	, QMutex()
	, _mode( Normal )
	, _direction( None )
	, _inchannels( ins )
	, _outchannels( outs )
	, _backend( backend )
	, _controls_remaining(0)
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
	// Time to depart this application.
	// Don't go calling me, kids!
	foreach (Element *e, _elements) e->invalidateRegistry();
}


void Widget::addElement( Element* n ) {
	_elements.push_back( n );
	connect( n, SIGNAL( replace( Element* ) ), this, SLOT( replace( Element* ) ) );
	connect( n, SIGNAL( explode( Element* ) ), this, SLOT( explode( Element* ) ) );
	resizeEvent( 0 );
}
void Widget::removeElement( Element* n ) {
	//qDebug("Removing element");
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
				tmp->deleteLater();
		}
	}
	createControl( in, out );
	QTimer::singleShot( 1, this, SLOT( autoFill() ) );
}

void Widget::explode( Element* n )  {
	//qDebug()<<"Widget::explode slot\n";
	
	n->deleteLater();
	QTimer::singleShot( 1, this, SLOT( autoFill() ) );
}

Element* Widget::getResponsible( QString in, QString out ) const {
	//qDebug() << "Widget::getResponsible(" << in << "," << out << ") size =" << _elements.size();
	for ( int i=0; i<_elements.size(); i++ )
		if ( _elements[ i ] && _elements[ i ]->isResponsible( in, out ) )
			return _elements[ i ];
	return 0;
}

bool Widget::createControl( QStringList inchannels, QStringList outchannels ) {
	//qDebug( "Widget::createControl( QStringList '%s', QStringList '%s', %s)", qPrintable( inchannels.join( "," ) ), qPrintable( outchannels.join( "," ) ) );

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
				if ( !getResponsible( *init, *outit ) ) // {
					//qDebug( "...together with (%s|%s)", qPrintable( *init ), qPrintable( *outit ) );
					createControl( QStringList()<<*init, QStringList()<<*outit );
				// }
				//else
					//qDebug( "   (%s|%s) is allready occupied. :(", qPrintable( *init ), qPrintable( *outit ) );
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
			if ( !getResponsible( *init, *init ) ) {
				//qDebug() << " No responsible element found, creating a new one";
				createControl( QStringList()<<*init, QStringList()<<*init );
			}
			
		}
	}
	resizeEvent( 0 );
}

void Widget::update_peak_inidicators(JackMix::BackendInterface::levels_t newLevels) {
        
        //qDebug() << "update_peak_inidicators slot: " << newLevels;
        JackMix::BackendInterface::levels_t::const_iterator it = newLevels.constBegin();

        while (it != newLevels.constEnd()) {
                QString n { it.key() };
                JackMix::BackendInterface::Level l { it.value() };
                
                Element* e { getResponsible(n, n) };
                if (e) {
                        (static_cast<MixerElements::AuxElement*>(e))->setIndicator(indicatorColors[l]);
                 } else {
                        qDebug() << "getResponsible(" << n << ", " << n << "] returned null";
                }
                
                ++it;
        }
}

void Widget::anotherControl() {
	lock();
	_controls_remaining++;
	unlock();
}

void Widget::placeFilled() {
	lock();
	if (--_controls_remaining == 0) {
		//qDebug() << "Signalling AutoFill completion...";
		emit autoFillComplete(this);
	} else {
		//qDebug() << _controls_remaining << " controls left to initialise";
	}
	unlock();
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
void Widget::renamechannels(QString old_name, QString new_name) {

	if (_direction == Vertical || _direction == Horizontal) {
		// Input and output sliders will have same string in and out
		Element *tmp = getResponsible(old_name, old_name);
		if (tmp) {
			tmp->renamechannels(old_name, new_name);
			//tmp->repaint();
		}
		
	} else {
		if (_outchannels.indexOf(old_name) >= 0) {  // it's an output we're changing
			for (QStringList::Iterator it = _inchannels.begin();
			it != _inchannels.end();
			it++) {
				//qDebug() << "Matrix mixer: renaming (" << *it << ", " << old_name << ")";
				Element *tmp = getResponsible( *it, old_name );
				if ( tmp )
					tmp->renamechannels(old_name, new_name);
			}
		} else { // an input name is changing: iterate over all outputs
			for (QStringList::Iterator it = _outchannels.begin();
			it != _outchannels.end();
			it++) {
				//qDebug() << "Matrix mixer: renaming (" << old_name << ", " << *it << ")";
				Element *tmp = getResponsible( old_name, *it );
				if ( tmp )
					tmp->renamechannels(old_name, new_name);
			}
		}
	}
	
	// Now maintain the private channel lists
	
	int pos;
	if ( (pos = _inchannels.indexOf(old_name)) >= 0 ) _inchannels[pos] = new_name;
	if ( (pos = _outchannels.indexOf(old_name)) >= 0 ) _outchannels[pos] = new_name;
	
}

void Widget::debugPrint() {
        qDebug( "\nWidget::debugPrint()" );
}


Element::Element( QStringList in, QStringList out, Widget* p, const char* n )
	: QFrame( p )
	, _menu( new QMenu( this ) )
	, _cca( 0 )
	, _in( in )
	, _out( out )
	, _selected( false )
	, _parent( p )
{
	//qDebug( "MixingMatrix::Element::Element( QStringList '%s', QStringList '%s' )", qPrintable( in.join(",") ), qPrintable( out.join(",") ) );
	disp_name = 0; // Assumme no label for this element for now
	setFrameStyle( QFrame::Raised|QFrame::Panel );
	setLineWidth( 1 );
	if (p) p->anotherControl();
	QTimer::singleShot( 1, this, SLOT( lazyInit() ) );
	setAutoFillBackground( true );
}
Element::~Element() {
	//qDebug() << "MixingMatrix::Element::~Element() for " << _in << _out;
	if (_parent) _parent->removeElement( this );
	if (_cca) delete _cca;
}
void Element::lazyInit() {
	//qDebug()<<"lazy, innit?";
	_parent->addElement( this );
	// Decrement the number of elements left to initialise in the parent widget
	_parent->placeFilled();
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
				// The dark() method above returns the selected colour, but doesn't
				// change the palette. To return to normal, we just use the old colour again.
				pal.setColor( QPalette::Window, pal.color( QPalette::Window ) );
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

const QList< int >& Element::midiParameters() const { return midi_params; }

void Element::update_midi_parameters(QList< int > pv) {
	// Update MIDI control parameters for all delegates
	if (pv.size() != midi_params.size())
		qWarning()<< "The update dialogue sent " << pv.size()
		          << " MIDI parameters but I was expecting "
			  << midi_params.size();
	for (int i = 0; i < pv.size() && i < midi_params.size() ; i++) {
		//qDebug()<<"New MIDI parameter no "<<i<<": "<<pv[i]<<" (was "<<midi_params[i]<<")";
		JackMix::MidiControl::ControlSender::unsubscribe(this, midi_params[i]);
		midi_params[i] = pv[i];
		JackMix::MidiControl::ControlSender::subscribe(this, midi_params[i]);
	}
}

void Element::controlEvent(int p, int v) {
	//qDebug() << midi_delegates.size() << " delegates.";
	for (int i = 0; i < midi_delegates.size(); i++) {
		//qDebug() << "Considering delegate " << i;
		// Check this delegate isn't null for safety,
		// and send it the parameter if it is subscribed
		if (midi_delegates[i] && p == midi_params[i]) {
			//qDebug() << "Sending " << p << "," << v << " to delegate " << i;
			midi_delegates[i]->setMidiValue(v);
		}
	}
}

void Element::renamechannels(QString old_name, QString new_name)
{
	int pos;
	if ((pos = _in.indexOf(old_name)) >= 0)  _in[pos]  = new_name;
	if ((pos = _out.indexOf(old_name)) >= 0) _out[pos] = new_name;
	// Update the displayed name if necessary
	if (disp_name) disp_name->setText( QString("<qt><center>%1</center></qt>").arg(new_name) );
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
	//qDebug( "Global::create( type = %s, ins = [%s], outs = [%s], parent = %p, name = %s )",
        //        qPrintable( type ), qPrintable( ins.join( "," ) ), qPrintable( outs.join( "," ) ), parent, name );
	// Only AuxElements can be non-selectable (input and output pots).
	Element* elem=0;
        int i;
	for ( i=0; i<_factories.size() && elem==0; i++ ) {
                elem = _factories[ i ]->create( type, ins, outs, parent, name );
                // If the element now exists, make it ready to display peaks
                // All input mixers are AuxElements
                if (elem && parent->direction() != Widget::None) {
                        static_cast<JackMix::MixerElements::AuxElement*>(elem)->
                          setIndicator(Widget::indicatorColors[JackMix::BackendInterface::Level::none]);
                }
        }
        //qDebug("Used factory %d to create elem %p", i-1, elem);
	//qDebug( "Will show and return %p", elem );
	//if ( elem )
	//	elem->show();
	return elem;
}

void Global::debug() {
	for ( int i=0; i<_factories.size(); i++ )
		qDebug("The factory %p can create '%s'",_factories[ i ],qPrintable(_factories[ i ]->canCreate().join(" ")));
}


}; // MixingMatrix
}; //JackMix

