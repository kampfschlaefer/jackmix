#ifndef QLASH_H
#define QLASH_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include <lash/lash.h>

namespace qLash {

/**
 * @brief LASH frontend for Qt4
 *
 * @todo more dox
 */
class qLashClient : public QObject
{
	Q_OBJECT
	public:
		/**
		 * @brief Construct a LASH-connection
		 *
		 * LASH needs a client-name and needs at least the first of the
		 * commandline arguments to determine the executable ( the others will
		 * be used to determine session and co if lashd has to autostart... ).
		 * If argc and argv both are 0, the arguments will be extracted from
		 * QCoreApplication::arguments().
		 *
		 * @param clientname The name of this client. This should always be the
		 * same to restore configs.
		 * @param argc Number of arguments stored in argv
		 * @param argv Commandline arguments.
		 * @param p The parent QObject.
		 */
		qLashClient( QString clientname, int argc =0, char** argv =0, QObject* p=0 );
		/**
		 * @brief destructor
		 */
		virtual ~qLashClient();

		/**
		 * @brief Connected to LASH?
		 */
		bool isConnected();

	protected slots:
		/**
		 * @brief Saving to dir finished
		 *
		 * Save the values stored in this class and tell the server that saving
		 * to directory is finished.
		 *
		 * @param path The path where the data has to be stored.
		 */
		void saveToDirFinalize( QString path );
		/**
		 * @brief Finalize saving to config finished
		 *
		 * Tells the server that saving to configs is finished.
		 */
		void saveToConfigFinalize();
		/**
		 * @brief Restoring from dir finished
		 *
		 * Tell the server that restoring from directory is finished.
		 *
		 * @param path The path where to restore the data from.
		 */
		void restoreFromDirFinalize( QString path );
		/**
		 * @brief Restoring from config finished
		 *
		 * Tell the server that restoring from configs is finished.
		 */
		void restoreFromConfigFinalize();

	public slots:
		// @{
		/**
		 * @brief Store named values inside this object.
		 *
		 * This removes the burden to save and restore little options by
		 * yourself. Just save them on the signal saveValues() and restore your
		 * values either on valueChanged( QString,QVariant ) or on
		 * restoreValues().
		 */
		void setValue( QString, QVariant );
		QVariant getValue( QString ) const;
		// @}

		/**
		 */
		void setJackName( QString );

	signals:
		/**
		 * @brief The server tells us to quit immediatly.
		 *
		 * ...without saving or user interaction ( if possible, otherwise
		 * handling of lash-sessions gets very painful ).
		 */
		void quitApp();
		/**
		 * @brief Save data into directory
		 *
		 * @param dir The path where to store your files in
		 */
		void saveToDir( QString dir );
		/**
		 * @brief Save data into configs
		 */
		void saveToConfig();
		/**
		 * @brief Restore data from directory
		 *
		 * @param dir The path where to restore your files from
		 */
		void restoreFromDir( QString dir );
		/**
		 * @brief Restore data from configs
		 */
		void restoreFromConfig();

		/**
		 * @brief Signal clients to save values with setValue()
		 */
		void saveValues();
		/**
		 * @brief Signal clients to restore values with getValue()
		 */
		void restoreValues();
		/**
		 * @brief A specific value has changed through lash.
		 */
		void valueChanged( QString, QVariant );

	private:
		void timerEvent( QTimerEvent* );
		lash_client_t *_client;
		QMap<QString,QVariant> _values;

}; // qLashClient

}; // qLash

#endif // QLASH_H

