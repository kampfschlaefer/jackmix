/*
    Copyright 2004 - 2006 Arnold Krille <arnold@arnoldarts.de>

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

#ifndef MIDICONTROLCHANNELASSIGNER_H
#define MIDICONTROLCHANNELASSIGNER_H

#include <QtGui/QDialog>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>

class QSpinBox;
class QPushButton;

namespace JackMix {

namespace GUI {

/**
 * A dialog to obtain control channel numbers which are to be assigned to
 * an element's potentiometers and sliders.
 */
class MidiControlChannelAssigner : public QDialog
{
Q_OBJECT
public:
	/** Contstructor
	 *  Build a dialoge with the given title and label, and with the initial values supplied
	 *  as a list of integers. Midi supports control channels in the range 0-119;
	 *  120 and above are reserved for "Channel Mode Messages"
	 *  (see http://www.midi.org/techspecs/midimessages.php). If the pointer supplied in init
	 *  is not null, it is assumed to reference an array of int large enough to hold one initial
	 *  value for each requested conotrol channel.
	 * 
	 *  The controls parameter is a list of strings which identify to the user the slider within
	 *  the element being controlled, so a Mono-to-Stereo element may request a MIDI channel to
	 *  control "Pan" and "Gain", for example.
	 * 
	 * @param title Dialogue title
	 * @param label String (not a QLabel!) to appear at the top of the dialog.
	 * @param controls A list of strings identifying the channels.
	 * @param init Initial values for the control channel numbers in the range 0-119 inclusive.
	 *             
	 */
	MidiControlChannelAssigner( QString title, QString label, QStringList controls, const QList<int> &init, QWidget*  p = NULL );
	~MidiControlChannelAssigner();
signals:
	void assignParameters( QList<int> );
public slots:
	void updateParameters( QList<int> );
private slots:
	void commit();
	void commitnquit();
private:
	int _num_controls;
	QPushButton *_commit, *_commit_n_quit, *_cancel;
	QSpinBox** _cchans;
};

};
};
#endif // MIDICONTROLCHANNELASSIGNER_H

