#ifndef MIXINGWIDGET1_H
#define MIXINGWIDGET1_H

#include <QtGui/QFrame>

class QGridLayout;

namespace JackMix {

namespace MixingMatrix {


class Widget2 : public QWidget
{
	Q_OBJECT
	public:
		Widget2( QWidget* );

	public slots:

		void addInputChannel( QString );
		void addOutPutChannel( QString );
		void removeInputChannel( QString );
		void removeOutputChannel( QString );

		void toggleInputStrip( bool );
		void toggleOutputStrip( bool );

		void increaseChannel( QString );
		void decreaseChannel( QString );

	private:
		QGridLayout* _layout;
		QStringList _inputs;
		QStringList _outputs;
};

class SampleBox : public QFrame
{
	Q_OBJECT
	public:
		SampleBox( Widget2* );
};

class ChannelTools : public QWidget
{
	Q_OBJECT
	public:
		ChannelTools( QString, Widget2* );

	signals:
		void increaseChannel( QString );
		void decreaseChannel( QString );

	private slots:
		void btnBiggerClicked();
		void btnSmallerClicked();

	private:
		QString name;
};

};

};

#endif // MIXINGWIDGET1_H

