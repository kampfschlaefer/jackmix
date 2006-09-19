/*
    Copyright ( C ) 2005 Arnold Krille <arnold@arnoldarts.de>

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
#ifndef UPDATEFILTER_H
#define UPDATEFILTER_H

#include <QtCore/QObject>

/// Stops an internal signal being sent outside if there is already a signal relayed from the outside to the inside.
/**
 * <dl>
 * <dt>The Setup:<dd>
 *    A slider representing a value of an object but the object being far
 *    away. And there are several Sliders representing the objects value in
 *    different hosts. And all the sliders should get the changes from the
 *    others.
 * <dt>The Solution:<dd>
 *    Connect every slider with the connections from an to the server and let
 *    the server update all the clients.
 * <dt>The Problem:<dd>
 *    A slider being updated by the server sends the valueChanged-signal
 *    himself to the server, which in return notifies all the sliders, an
 *    infinite loop is born.
 * <dt>The <b>REAL</b> Solution:<dd>
 *    Plug the UpdateFilter between the server-connection and the slider.
 *    Everytime the server sends updates the filter will block the signals
 *    from the inside.
 * </dl>
 *
 * <dl>
 * <dt>The ports ( signals/slots ) to use:<dd>
 *    Connect the externals to dataIn() / dataOut() and the internal, to-be-filtered
 *    objects to dataInInternal() / dataOutInternal().
 * </dl>
 */
class UpdateFilter : public QObject
{
	Q_OBJECT
	public:
		/// Simple Qt constructor
		UpdateFilter( QObject* p ) 
			: QObject( p )
			, _inupdate( false )
		{ }
		/// Simple destructor
		~UpdateFilter() {}
	public slots:
		/// External data input for integer
		void dataIn( int );
		/// Internal filtered data input for integer
		void dataInInternal( int );
		/// External data input for float
		void dataIn( float );
		/// Internal filtered data input for float
		void dataInInternal( float );
		/// External data input for QString
		void dataIn( QString );
		/// Internal filtered data input for QString
		void dataInInternal( QString );
	signals:
		/// External filtered data output for integer
		void dataOut( int );
		/// Internal data output for integer
		void dataOutInternal( int );
		/// External filtered data output for float
		void dataOut( float );
		/// Internal data output for float
		void dataOutInternal( float );
		/// External filtered data output for QString
		void dataOut( QString );
		/// Internal data output for QString
		void dataOutInternal( QString );
	private:
		bool _inupdate;
};

#endif // UPDATEFILTER_H

