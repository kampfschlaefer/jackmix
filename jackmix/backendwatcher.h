
#ifndef JACKMIX_BACKENDWATCHER_H
#define JACKMIX_BACKENDWATCHER_H

#include <qobject.h>
#include <qstring.h>

namespace JackMix {

class BackendWatcher : public QObject {
Q_OBJECT
public:
	BackendWatcher( QObject* =0, const char* =0 );
	~BackendWatcher();

	void addInput( QString name );
	void addOutput( QString name );
signals:
	void sNewInput( QString name );
	void sNewOutput( QString name );
};

};

#endif

