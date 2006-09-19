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

#ifndef BACKEND_INTERFACE_H
#define BACKEND_INTERFACE_H

#include <QtCore/QStringList>

namespace JackMix {

	class BackendInterface
	{
	public:
		BackendInterface() {}
		virtual ~BackendInterface() {}

		// Return the current list of output channels.
		virtual QStringList outchannels() =0;
		// Return the current list of input channels.
		virtual QStringList inchannels() =0;

		// Set the volume of the named node.
		virtual void setVolume( QString, QString, float ) =0;
		// Get the volume of the named node.
		virtual float getVolume( QString, QString ) =0;

		// Add a channel and return true on success.
		virtual bool addOutput( QString ) =0;
		virtual bool addInput( QString ) =0;
		// Remove a channel and return true on success.
		virtual bool removeOutput( QString ) =0;
		virtual bool removeInput( QString ) =0;
	};
};

#endif // BACKEND_INTERFACE_H

