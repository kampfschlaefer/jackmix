/*
    Copyright (  C ) 2003 Arnold Krille <arnold@arnoldarts.de>

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

#ifndef JACKMIX_DB2VOL_CALC_H
#define JACKMIX_DB2VOL_CALC_H

#include <math.h>

class dB2VolCalc {
private:
	float _base;
public:
	dB2VolCalc( float _dbmin, float _dbmax )
	  : _base( 6/log10( double(2) ) ) // Depends on what you measure: intensity(10), pressure(20), old artscontrol(6/lg2 (near 20))
	  , dbmax( _dbmax )
	  , dbmin( _dbmin )
	{}

	float dbmax, dbmin;
	/**
		Logarithmic/decimal valuation:
		p = ampfactor ( linear )
		db = dezibel ( logarithmic )
		p = 10^( d/10 )
		db = 10*lg( p )

		artscontrol was using
			db = 6/ln( 2 ) * ln( p )
		which would be
			db = 6/lg( 2 ) * lg( p )
	*/
	float amptodb( float p ) {
		float db = _base*log10( p );
		if ( db < dbmin ) db = dbmin;
		if ( db > dbmax ) db = dbmax;
		return db;
	}
	float dbtoamp( float db ) {
		float amp = pow( 10, db/_base );
		if ( db <= dbmin ) amp = 0;
		return amp;
	}
	/// With ndb = normalized dB (between 0 and 1)
	float amptondb( float p ) {
		return  dbtondb( amptodb( p ) ); //- dbmin ) / ( dbmax - dbmin );
	}
	float ndbtoamp( float ndb ) {
		return dbtoamp( ndb * ( dbmax - dbmin ) + dbmin );
	}
	/// Normalizes a dezibel value.
	float dbtondb( float db ) {
		return ( db - dbmin )/( dbmax - dbmin );
	}
	/// Normalizes a volume to a logarithmic value between 0 and 1.
	float dbtovol( float db ) {
		return ( db -dbmin )/( 0-dbmin );
	}
	/// Unnormalizes a dezibel value.
	float ndbtodb( float ndb ) {
		return ( ndb * ( dbmax-dbmin ) +dbmin );
	}
};

#endif
// vim: sw=4 ts=4
