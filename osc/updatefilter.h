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

#include <qobject.h>

/**
 * Stopps an internal signal being sent outside if there is already a signal
 * relayed from the outside to the inside.
 */
class UpdateFilter : public QObject
{
	Q_OBJECT
	public:
		UpdateFilter( QObject* p, const char* n=0 ) 
			: QObject( p,n )
			, _inupdate( false )
			, c( 0 )
		{ }
		~UpdateFilter() {}
	public slots:
		void dataIn( int );
		void dataInInternal( int );
	signals:
		void dataOut( int );
		void dataOutInternal( int );
	private:
		bool _inupdate;
		int c;
};

#endif // UPDATEFILTER_H

