
#ifndef JACKMIX_BACKENDWATCHER_H
#define JACKMIX_BACKENDWATCHER_H

#include <qobject.h>

namespace JackMix {

class BackendWatcher : public QObject {
Q_OBJECT
public:
	BackendWatcher( QObject* =0, const char* =0 );
	~BackendWatcher();
};

};

#endif

