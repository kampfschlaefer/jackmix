
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
