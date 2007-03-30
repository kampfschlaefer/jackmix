#ifndef GRAPHICALGUISERVER_H
#define GRAPHICALGUISERVER_H

#include <guiserver_interface.h>

class QWidget;

namespace JackMix {
namespace GUI {

class GraphicalGuiServer : public GuiServer_Interface
{
public:
	GraphicalGuiServer( QWidget* );

	void message( const QString&, const QString& ) const;
	bool messageYesNo( const QString&, const QString& ) const;
	bool messageOkCancel( const QString&, const QString& ) const;

	double getDouble( const QString&, const QString&, double, double, double ) const;
	int getInt( const QString&, const QString&, int, int, int ) const;
	QString getString( const QString&, const QString&, const QString& ) const;
	QString getItem( const QString&, const QString&, const QStringList& ) const;
private:
	QWidget* _parent;
};

};
};
#endif // GRAPHICALGUISERVER_H

