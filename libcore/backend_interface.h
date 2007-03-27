/*
    Copyright 2005 - 2007 Arnold Krille <arnold@arnoldarts.de>

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
#include <guiserver_interface.h>

class QDomElement;
class QDomDocument;

namespace JackMix {

	/**
	 * @brief Abstract interface for backends
	 *
	 * A backend has to implement this functions...
	 */
	class BackendInterface
	{
	public:
		BackendInterface( GuiServer_Interface* );
		virtual ~BackendInterface();

		/**
		 * @brief Return the current list of output channels.
		 */
		virtual QStringList outchannels() =0;
		/**
		 * @brief Return the current list of input channels.
		 */
		virtual QStringList inchannels() =0;

		/**
		 * @brief Set the volume of the named node.
		 */
		virtual void setVolume( QString, QString, float ) =0;
		/**
		 * @brief Get the volume of the named node.
		 */
		virtual float getVolume( QString, QString ) =0;

		/**
		 * @brief Add a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool addOutput( QString ) =0;
		/**
		 * @brief Add a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool addInput( QString ) =0;
		/**
		 * @brief Remove a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool removeOutput( QString ) =0;
		/**
		 * @brief Remove a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool removeInput( QString ) =0;

	protected:
		GuiServer_Interface* gui;

	};
};

#endif // BACKEND_INTERFACE_H

