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

#ifndef MIXINGMATRIX_H
#define MIXINGMATRIX_H

#include <qframe.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <qpopupmenu.h>
#include <qaction.h>

namespace JackMix {
namespace MixingMatrix {

class Element;
class ConnectionLister;

/*struct Property {
	Property( QString _name=QString::null, QString _signal=QString::null, QString _slot=QString::null, QString _type=QString::null )
		: name( _name )
		, signal( _signal )
		, slot( _slot )
		, type( _type )
		{ }
	QString name;
	QString signal;
	QString slot;
	QString type;
};

typedef QMap<QString,Property> Properties;*/

struct ElementSlotSignalPair {
	ElementSlotSignalPair( Element* n=0, QString s=0 ) : element( n ), slot( s ) {}
	Element* element;
	QString slot;
	bool operator< ( const ElementSlotSignalPair n ) const {
		return ( ( element < n.element ) || ( slot < n.slot ) );
	}
	bool operator== ( const ElementSlotSignalPair n ) const {
		if ( ( element == n.element ) && ( n.slot == 0 ) )
			return true;
		if ( ( n.element == 0 ) && ( slot == n.slot ) )
			return true;
		return ( ( element == n.element ) && ( slot == n.slot ) );
	}
	QString debug() {
		return QString( "(%1,%2)" ).arg( int( element ) ).arg( slot );
	}
};


class Widget : public QFrame
{
	friend class ConnectionLister;
Q_OBJECT
Q_ENUMS( Mode )
Q_ENUMS( Direction )
Q_PROPERTY( Mode mode READ mode WRITE mode )
Q_PROPERTY( Direction direction READ direction WRITE direction )
public:
	// \param inchannels, outchannels, parent, name=0
	Widget( QStringList, QStringList, QWidget*, const char* =0 );
	~Widget();

	QStringList inchannels() const { return _inchannels; }
	QStringList outchannels() const { return _outchannels; }
	QString nextIn( QString ) const;
	QString nextOut( QString ) const;

	/// New input/output channels
	void addinchannel( QString );
	void addoutchannel( QString );

	/// Create Controls
	// Create controls. return true on success
	bool createControl( QStringList inchannels, QStringList outchannels );

	/// Layout
	QSize smallestElement() const;
	QSize minimumSizeHint() const;
	QSize sizeHint() const { return minimumSizeHint(); }
	void resizeEvent( QResizeEvent* );

	/// Mode
	enum Mode { Normal, Select };
	Mode mode() const { return _mode; }
	void mode( Mode n ) { _mode=n; }

	/// 2D-Direction
	enum Direction { None, Horizontal, Vertical };
	Direction direction() const { return _direction; }
	void direction( Direction n ) { _direction = n; }

	void addElement( Element* );
	void removeElement( Element* );
	Element* getResponsible( QString in, QString out ) const;
	int elements() const { return _elements.size(); }

	// Show/Hide the ConnectionLister
	void toggleConnectionLister( bool );

public slots:
	void replace( Element* );

	// Fills the empty nodes with 1to1-controls
	void autoFill();

	// Connect a given slave to a master
	void connectSlave( Element*, QString );
	// Connect a given master to a slave
	void connectMaster( Element*, QString );
	// Connect a given master to a given slave
	void connectMasterSlave( Element*, QString, Element*, QString );

	// Disconnect a given slave from its master
	void disconnectSlave( Element*, QString );
	// Disconnect a given master from all its slaves
	void disconnectMaster( Element*, QString );

	// Toggle the ConnectionLister
	void toggleConnectionLister();

	// For testing of the properties
	void debugPrint();

private slots:
	void valueChanged( Element*, QString );
private:
	enum Mode _mode;
	Direction _direction;
	QValueList <Element*> _elements;
	QStringList _inchannels, _outchannels;
	ConnectionLister* _connectionlister;
	QMap <ElementSlotSignalPair,ElementSlotSignalPair> _connections;
};

class Element : public QFrame
{
Q_OBJECT
Q_PROPERTY( bool selected READ isSelected WRITE select )
Q_PROPERTY( QStringList in READ in )
Q_PROPERTY( QStringList out READ out )
Q_PROPERTY( int ins READ inchannels )
Q_PROPERTY( int outs READ outchannels )
Q_PROPERTY( QString name READ name WRITE name )
public:
	/**
	 * Gets the upper left corner in channelnames.
	 * And lists with the names of the channels to control.
	 */
	Element( QStringList in, QStringList out, Widget*, const char* =0 );
	virtual ~Element();

	// The size of this control in channels.
	virtual int inchannels() const =0;
	virtual int outchannels() const =0;

	// The channels this control is in charge of
	QStringList in() const { return _in; }
	QStringList out() const { return _out; }

	// Returns wether the control is responsible for the named node.
	bool isResponsible( QString in, QString out );

	bool isSelected() const { return _selected; }

	QString name() const { return QString( name() ); }
	void name( QString n ) { setName( n.latin1() ); }
	//void name( QString n );

	//QWidget* getNameWidget( QWidget* );

	// Properties
	QStringList getPropertyList();

	/**
	 * Do the things you need to do in the subclasses when the
	 * selected state changes.
	 */
	virtual void isSelected( bool ) {};

	// returns the number of selected neighbors
	int neighbors() const;
	// The inchannels of the neighbors
	QStringList neighborsList() const;
	// returns the number of following rows with the given number
	// of selected items.
	int followers( int ) const;
	QStringList followersList() const;

public slots:
	void select( bool );
	void showMenu();

signals:
	void replace( Element* );
	void connectSlave( Element*, QString );
	void disconnectSlave( Element*, QString );
	void disconnectMaster( Element*, QString );

	// Informs, that Element* n, Property s has changed.
	void valueChanged( Element* n, QString s );

protected:
	// Internal pointer
	const Widget* parent() { return _parent; }
	// The contextmenu of the Element. Should be filled by the client...
	QPopupMenu* menu() { return _menu; }

protected slots:
	// Use this slot if you don't want to do something before replacement.
	void slot_simple_replace() { emit replace( this ); }
	// Use this slot if you want a simple selection toggle
	void slot_simple_select() { select( !isSelected() ); }

private:
	QStringList _in, _out;
	bool _selected;
	//Properties _properties;
	Widget* _parent;
	QPopupMenu *_menu;
};


class ElementFactory 
{
public:
	ElementFactory();
	virtual ~ElementFactory();

	/// Returns a list of the elements this factory can create
	virtual QStringList canCreate() const =0;

	/**
	 * Returns a list of the elements this factory can create and
	 * which support the named number of in and out channels
	 */
	virtual QStringList canCreate( int in, int out ) const =0;

	/**
	 * Returns an object of the given Elementtype or 0 if this factory can not create it.
	 */
	virtual Element* create( QString element, QStringList ins, QStringList outs, Widget* parent, const char* name =0 ) =0;

	void globaldebug();
};

}; // MixingMatrix
}; // JackMix

#endif // MIXINGMATRIX_H

