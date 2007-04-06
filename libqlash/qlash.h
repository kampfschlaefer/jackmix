#ifndef QLASH_H
#define QLASH_H

#include <QtCore/QObject>

namespace qLash {

class qLashClient : public QObject
{
	Q_OBJECT
	public:
		qLashClient( QObject* =0 );
		virtual ~qLashClient();

}; // qLashClient

}; // qLash

#endif // QLASH_H

