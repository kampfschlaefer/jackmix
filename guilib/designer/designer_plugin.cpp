/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

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

#include <qwidgetplugin.h>

#include "qfloatcontrol.h"

class JackMix_Widget_Plugin : public QWidgetPlugin
{
public:
	JackMix_Widget_Plugin() {}

	QStringList keys() const {
		QStringList tmp;
		tmp << "QFloatControl";
		return tmp;
	}
	QWidget* create( const QString &classname, QWidget* parent = 0, const char* name = 0 ) {
		if ( classname == "QFloatControl" )
			return new JackMix::GUI::QFloatControl( parent, name );
		return 0;
	}
	QString group( const QString& ) const { return "AudioGuiElements"; }
	QIconSet iconSet( const QString& ) const { return QPixmap(); }
	QString includeFile( const QString& ) const { return "qfloatcontrol.h"; }
	QString toolTip( const QString& ) const { return "A generic control for a float value"; }
	QString whatsThis( const QString& ) const { return ""; }
	bool isContainer( const QString& ) const { return false; }
};

Q_EXPORT_PLUGIN( JackMix_Widget_Plugin )
