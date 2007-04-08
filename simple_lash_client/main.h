#ifndef MAIN_H
#define MAIN_H

#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtGui/QPushButton>

class MyButton : public QPushButton
{
	Q_OBJECT
	public:
		MyButton( QString text, QWidget* w=0 ) : QPushButton( text,w ) {
			setObjectName( text );
		}
		~MyButton() {}

	public slots:
		void signalState() {
			qDebug() << "MyButton::signalState() saving" << isChecked();
			emit state( objectName(), QVariant( isChecked() ) );
		}

		void setState( QString n, QVariant v ) {
			qDebug() << "MyButton::setState(" << n << "," << v << ")";
			if ( n == objectName() )
				setChecked( v.toBool() );
		}

	signals:
		void state( QString, QVariant );
};

#endif // MAIN_H

