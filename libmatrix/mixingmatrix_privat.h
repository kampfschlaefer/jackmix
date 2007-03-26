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

#ifndef MIXINGMATRIX_PRIVAT_H
#define MIXINGMATRIX_PRIVAT_H

#include <QtGui/QWidget>
#include <QtCore/QList>

class QGridLayout;
class QPushButton;
class QListBox;

namespace JackMix {
namespace MixingMatrix {

class ElementFactory;
class Widget;

class Global
{
private:
	Global();
	~Global();
public:
	static Global* the();

	void registerFactory( ElementFactory* );
	void unregisterFactory( ElementFactory* );

	/**
	 * Returns the names of the Controls for the given number of in
	 * and out channels
	 */
	QStringList canCreate( int in, int out );

	/**
	 * Create a control of the given type and for the given in and
	 * out channels.
	 * Return true on success else false.
	 */
	bool create( QString type, QStringList ins, QStringList outs, Widget* parent, const char* =0 );

	void debug();
private:
	QList <ElementFactory*> _factories;
};

}; // MixingMatrix
}; // JackMix

#endif // MIXINGMATRIX_PRIVAT_H

