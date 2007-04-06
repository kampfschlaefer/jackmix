#ifndef MAIN_H
#define MAIN_H

#include <QtCore/QDebug>
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
			qDebug() << "MyButton::signalState()";
			emit state( isChecked() );
		}

	signals:
		void state( int );
};

#endif // MAIN_H

