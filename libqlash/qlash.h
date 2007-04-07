#ifndef QLASH_H
#define QLASH_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include <lash/lash.h>

namespace qLash {

class qLashClient : public QObject
{
	Q_OBJECT
	public:
		qLashClient( QString clientname, int argc =0, char** argv =0, QObject* =0 );
		virtual ~qLashClient();

		bool isConnected();

		/**
		 * Only for DEBUGGING!
		 *
		 * @todo Remove me!
		 */
		lash_client_t* client() { return _client; }

	public slots:
		/**
		 * @brief Saving to dir finished
		 *
		 * Tell the server that saving to directory is finished.
		 */
		void saveToDirFinished();
		/**
		 * @brief Finalize saving to config finished
		 *
		 * Saves the values stored inside this object and tells the server that saving to configs is finished.
		 *
		 * If you implement this, you should call this function at the end.
		 */
		virtual void saveToConfigFinalize();
		/**
		 * @brief Restoring from dir finished
		 *
		 * Tell the server that restoring from directory is finished.
		 */
		void restoreFromDirFinished();
		/**
		 * @brief Restoring from config finished
		 *
		 * Tell the server that restoring from configs is finished.
		 */
		virtual void restoreFromConfigFinalize();

		/**
		 * @todo not finished yet. Have to think about saving/restoring to/from configs a while.
		 */
		//void saveToConfig( int );
		//void readFromConfig( int );

		// @{
		/**
		 */
		void setValue( QString, QVariant );
		QVariant getValue( QString ) const;
		// @}

	signals:
		/**
		 * @brief The server tells us to quit immediatly.
		 *
		 * ...without saving.
		 */
		void quitApp();
		/**
		 * @brief Save data into directory
		 *
		 * You should ( have to ) call saveToDirFinished() afterwards.
		 */
		void saveToDir( QString dir );
		/**
		 * @brief Save data into configs
		 *
		 * saveToConfigFinalize() is called after returning from this signal to save the values stored in this object. @see setValue(), getValue()
		 *
		 * You can either subclass from qLashClient and implement saveToConfigFinalize or use this signal to save your settings via setValue or both.
		 */
		void saveToConfig();
		/**
		 * @brief Restore data from directory
		 *
		 * You should ( have to ) call restoreFromDirFinished() afterwards.
		 */
		void restoreFromDir( QString dir );
		/**
		 * @brief Restore data from configs
		 *
		 * You should ( have to ) call restoreFromConfigFinished() afterwards.
		 */
		void restoreFromConfig();

		void valueChanged( QString, QVariant );

	protected:
		void timerEvent( QTimerEvent* );

	private:
		lash_client_t *_client;
		QMap<QString,QVariant> _values;

}; // qLashClient

}; // qLash

#endif // QLASH_H

