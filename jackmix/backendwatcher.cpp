
#include "backendwatcher.h"
#include "backendwatcher.moc"

using namespace JackMix;

BackendWatcher::BackendWatcher( QObject* p, const char* n ) : QObject( p,n ) {
}
BackendWatcher::~BackendWatcher() {
}

void BackendWatcher::addInput( QString name ) {
}
void BackendWatcher::addOutput( QString name ) {
}

