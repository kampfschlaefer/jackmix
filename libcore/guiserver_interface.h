#ifndef GUISERVER_INTERFACE_H
#define GUISERVER_INTERFACE_H

#include <QtCore/QString>
#include <QtCore/QStringList>

namespace JackMix {

/**
 * @brief Abstract interface for a GUI server
 *
 */
class GuiServer_Interface
{
public:
	virtual void message( const QString& title, const QString& text ) const =0;
	virtual bool messageYesNo( const QString& title, const QString& text ) const =0;
	virtual bool messageOkCancel( const QString& title, const QString& text ) const =0;

	virtual double getDouble( const QString& title, const QString& label, double initvalue, double minvalue, double maxvalue ) const =0;
	virtual int getInt( const QString& title, const QString& label, int initvalue, int minvalue, int maxvalue ) const =0;
	virtual QString getString( const QString& title, const QString& label, const QString& initvalue ) const =0;
	virtual QString getItem( const QString& title, const QString& label, const QStringList& list ) const =0;
};

};

#endif // GUISERVER_INTERFACE_H

