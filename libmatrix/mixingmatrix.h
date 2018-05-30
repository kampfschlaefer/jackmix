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

#ifndef MIXINGMATRIX_H
#define MIXINGMATRIX_H

#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QLabel>

#include <QtCore/QDebug>

#include "backend_interface.h"
#include "abstractslider.h"
#include "midicontrolchannelassigner.h"
#include "controlreceiver.h"

#if ( QT_POINTER_SIZE == 8 )
#define pint qint64
#else
#define pint qint32
#endif

namespace JackMix {

namespace MixingMatrix {

class Element;

class Widget : public QFrame, QMutex
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
	/** For an input or output element, allow the user to rename the channel
	 *  It is assumed that the supplied new name is unique 
	 */
	void renamechannels( QString, QString );
	
	/// Create Controls
	// Create controls. return true on success
	bool createControl( QStringList inchannels, QStringList outchannels);

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
	int elements() const { return _elements.size(); };
	/** Called when another (not yet initialised) element is created */
	void anotherControl();
	/** Called when a lazy initialisation completes */
	void placeFilled();
        
        /** These are the colours used for different levels defined in peak_tracker.h */
        static const QColor indicatorColors[];

signals:
	/** AutoFill pass complete: safe to getResponsible() etc now */
	void autoFillComplete(MixingMatrix::Widget *);

public slots:
        // Combine many elements to form a multichannel one
	void replace( Element* );
        
        // Split a multichannel element into its constituents
        void explode( Element* );

	// Fills the empty nodes with 1to1-controls
	void autoFill();

	// For testing of the properties
	void debugPrint();
	
        /** When the peak tracker in the backend wants to change the indicator states
         *  on the widget, it sends a map of the ones it wants to change to this slot
         */ 
        void update_peak_inidicators(JackMix::BackendInterface::levels_t newLevels);

private:
	enum Mode _mode;
	Direction _direction;
	QList <Element*> _elements;
	QStringList _inchannels, _outchannels;
	JackMix::BackendInterface* _backend;
	/** Controls have lazy initialisation: this counts how many are left to go
	 *  before the whole widget is initialised. */
	int _controls_remaining;
                
};

class Element : public QFrame
              , JackMix::MidiControl::ControlReceiver
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

	/** returns the number of selected neighbors */
	int neighbors() const;
	/** The inchannels of the neighbors */
	QStringList neighborsList() const;
	/** returns the number of following rows with the given number
	 * of selected items. */
	int followers( int ) const;
	QStringList followersList() const;
	/** Allow others to see our controlling midi parameters (but not change them) */
	const QList<int>& midiParameters() const;
	
	/** Inform the Element the parent's dead so it doesn't try to deregister itself
	 *  on destruction (otherwise there may be a segfault at closedown)
	 */
	void invalidateRegistry() { _parent = 0; };
	/** Replace all occurances of the old channel name with the new one in both the
	 *  input and output channel lists
	 */
	void renamechannels(QString old_name, QString new_name);

public slots:
	void select( bool );
	void showMenu();
	/** Receive a signal, typically from the MIDI parameter selection dialogue
	 *  to change re-subscribe the sliders in this element to new MIDI
	 *  parameters. Also called directly to update the parameters after loading
	 *  a saved control layout.
	 */
	void update_midi_parameters(QList< int > pv);

signals:
	void replace( Element* );
        void explode( Element* );
	// Informs, that Element* n, Property s has changed.
	void valueChanged( Element* n, QString s );
	/** Indicates completion of lazy initialisation */
	void initComplete();
	
protected:
	// Internal pointer
	const Widget* parent() { return _parent; }
	/** The contextmenu of the Element. Should be filled by the client */
	QMenu *_menu;
	QMenu* menu() { return _menu; }
	/**
	 * @todo the overall layout is needed for hide/show buttons per channel/element...
	 */
	/** The current parameter associated with each delegate. */
	QLayout* layout();
	/** If the derrived class has a label, this is it.
	 *  Used to change text if an input or output channel is renamed
	 */
	QLabel* disp_name;

	QList<int> midi_params;
	/** A list of sliders which have a setMidiValue(int) member
	 *  function, ordered to correspond with the parameter array
	 *  midi_params. Would typically be initialised in a derived
	 *  class's constructor with:
	 *
	 *	midi_delegates << slider1 << slider2 etc.
	 */
	QList<JackMix::GUI::AbstractSlider*> midi_delegates;
	/** Dialogue to allow midi parameters to be associated with each delegate */
	JackMix::GUI::MidiControlChannelAssigner* _cca;
	void contextMenuEvent( QContextMenuEvent* );

	QStringList _in, _out;

protected slots:
	// Use this slot if you don't want to do something before replacement.
	virtual void slot_simple_replace() { replace( this ); }
	// Use this slot if you don't want to do something before explosion.
	virtual void slot_simple_explode() { explode( this ); }
	// Use this slot if you want a simple selection toggle
	virtual void slot_simple_select() { select( !isSelected() ); }
	/** Receive and forward midi events (parameter, value pairs) to
	 *  their associated sliders.
	 */
	void controlEvent(int p, int v);
	/** Show the midi parameter selection menu */
	virtual void slot_assign_midi_parameters() { _cca->updateParameters(midi_params); _cca->show(); };

private slots:
	void lazyInit();
private:
	bool _selected;
	Widget* _parent;
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

