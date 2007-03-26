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

#ifndef MIXINGMATRIX_H
#define MIXINGMATRIX_H

#include <QtGui/QFrame>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtGui/QMenu>
#include <QtGui/QAction>

#include "backend_interface.h"

#if ( QT_POINTER_SIZE == 8 )
#define pint qint64
#else
#define pint qint32
#endif

namespace JackMix {

namespace MixingMatrix {

class Element;

class Widget : public QFrame
{
Q_OBJECT
Q_ENUMS( Mode )
Q_ENUMS( Direction )
Q_PROPERTY( Mode mode READ mode WRITE mode )
Q_PROPERTY( Direction direction READ direction WRITE direction )
public:
	// \param inchannels, outchannels, backend, parent, name=0
	Widget( QStringList, QStringList, JackMix::BackendInterface*, QWidget*, const char* =0 );
	~Widget();

	JackMix::BackendInterface* backend() const { return _backend; }

	QStringList inchannels() const { return _inchannels; }
	QStringList outchannels() const { return _outchannels; }
	QString nextIn( QString ) const;
	QString nextOut( QString ) const;

	/// New input/output channels
	void addinchannel( QString );
	void addoutchannel( QString );
	/// Remove input/output channels
	void removeinchannel( QString );
	void removeoutchannel( QString );

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


public slots:
	void replace( Element* );

	// Fills the empty nodes with 1to1-controls
	void autoFill();

	// For testing of the properties
	void debugPrint();

private:
	enum Mode _mode;
	Direction _direction;
	QList <Element*> _elements;
	QStringList _inchannels, _outchannels;
	JackMix::BackendInterface* _backend;
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

	JackMix::BackendInterface* backend() const { return _parent->backend(); }

	// The size of this control in channels.
	virtual int inchannels() const =0;
	virtual int outchannels() const =0;

	// The channels this control is in charge of
	QStringList in() const { return _in; }
	QStringList out() const { return _out; }

	// Returns wether the control is responsible for the named node.
	bool isResponsible( QString in, QString out );

	bool isSelected() const { return _selected; }

	QString name() const { return QObject::objectName(); }
	void name( QString n ) { setObjectName( n ); }

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

	// Informs, that Element* n, Property s has changed.
	void valueChanged( Element* n, QString s );

protected:
	// Internal pointer
	const Widget* parent() { return _parent; }
	// The contextmenu of the Element. Should be filled by the client...
	QMenu* menu() { return _menu; }
	/**
	 * @todo the overall layout is needed for hide/show buttons per channel/element...
	 */
	QLayout* layout();

protected slots:
	// Use this slot if you don't want to do something before replacement.
	void slot_simple_replace() { emit replace( this ); }
	// Use this slot if you want a simple selection toggle
	void slot_simple_select() { select( !isSelected() ); }

	void contextMenuEvent( QContextMenuEvent* );
private slots:
	void lazyInit();
private:
	QStringList _in, _out;
	bool _selected;
	Widget* _parent;
	QMenu *_menu;
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

