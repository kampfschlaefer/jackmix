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

#include "updatefilter.h"
#include "updatefilter.moc"

void UpdateFilter::dataIn( int d )
{
	if ( !_inupdate ) {
		_inupdate = true;
		emit dataOutInternal( d );
		_inupdate = false;
	}
}
void UpdateFilter::dataInInternal( int d ) {
	if ( !_inupdate )
		emit dataOut( d );
}

void UpdateFilter::dataIn( float d )
{
	if ( !_inupdate ) {
		_inupdate = true;
		emit dataOutInternal( d );
		_inupdate = false;
	}
}
void UpdateFilter::dataInInternal( float d ) {
	if ( !_inupdate )
		emit dataOut( d );
}

void UpdateFilter::dataIn( QString d )
{
	if ( !_inupdate ) {
		_inupdate = true;
		emit dataOutInternal( d );
		_inupdate = false;
	}
}
void UpdateFilter::dataInInternal( QString d ) {
	if ( !_inupdate )
		emit dataOut( d );
}

