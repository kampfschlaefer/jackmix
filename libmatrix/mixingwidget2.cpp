
#include "mixingwidget2.h"
#include "mixingwidget2.moc"

#include <QtCore/QDebug>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>

namespace JackMix {
namespace MixingMatrix {

Widget2::Widget2( QWidget* ) {
	_layout = new QGridLayout( this );
}

void Widget2::addInputChannel( QString s ) {
	ChannelTools* tmp = new ChannelTools( s, this );
	connect( tmp, SIGNAL( increaseChannel( QString ) ), this, SLOT( increaseChannel( QString ) ) );
	connect( tmp, SIGNAL( decreaseChannel( QString ) ), this, SLOT( decreaseChannel( QString ) ) );

	QList<QWidget*> _widgets;
	for ( int i=1; i<_layout->rowCount(); ++i ) {
		QRect rect = _layout->cellRect( i, _inputs.size() );
		QWidget* tmp = childAt( rect.center() );
		_widgets.push_back( tmp );
		_layout->removeWidget( tmp );
	}

	_layout->addWidget( tmp, 0, _inputs.size() );
	_inputs.push_back( s );

	foreach( QWidget* w, _widgets )
		_layout->addWidget( w, 1+_widgets.indexOf( w ), _inputs.size() );
}
void Widget2::addOutPutChannel( QString s ) {
	ChannelTools* tmp = new ChannelTools( s, this );
	connect( tmp, SIGNAL( increaseChannel( QString ) ), this, SLOT( increaseChannel( QString ) ) );
	connect( tmp, SIGNAL( decreaseChannel( QString ) ), this, SLOT( decreaseChannel( QString ) ) );
	_layout->addWidget( tmp, 1+_outputs.size(), _inputs.size() );
	_outputs.push_back( s );
}
void Widget2::removeInputChannel( QString ) {
}
void Widget2::removeOutputChannel( QString ) {
}

void Widget2::toggleInputStrip( bool ) {
}
void Widget2::toggleOutputStrip( bool ) {
}

void Widget2::increaseChannel( QString ch ) {
	qDebug() << "Widget2::increaseChannel(" << ch << ")";
	int n=-1;
	if ( _inputs.contains( ch ) ) {
		n = _inputs.indexOf( ch );
		_layout->setColumnStretch( n, qMin( 10, _layout->columnStretch( n )+1 ) );
	} else
		if ( _outputs.contains( ch ) ) {
			n = _outputs.indexOf( ch );
			_layout->setRowStretch( n+1, qMin( 10, _layout->rowStretch( n )+1 ) );
		}
}
void Widget2::decreaseChannel( QString ch ) {
	qDebug() << "Widget2::decreaseChannel(" << ch << ")";
	int n=-1;
	if ( _inputs.contains( ch ) ) {
		n = _inputs.indexOf( ch );
		_layout->setColumnStretch( n, qMax( 0, _layout->columnStretch( n )-1 ) );
	} else
		if ( _outputs.contains( ch ) ) {
			n = _outputs.indexOf( ch );
			_layout->setRowStretch( n+1, qMax( 0, _layout->rowStretch( n )-1 ) );
		}
}

SampleBox::SampleBox( Widget2* p ) : QFrame( p ) {
	setFrameStyle( QFrame::Raised|QFrame::Panel );
	setLineWidth( 3 );
	setMinimumSize( 20, 20 );
}

ChannelTools::ChannelTools( QString n, Widget2* p ) : QWidget( p ), name( n ) {
	QGridLayout* _layout = new QGridLayout( this );
	QPushButton* btnBigger = new QPushButton( "+", this );
	_layout->addWidget( btnBigger );
	connect( btnBigger, SIGNAL( clicked() ), this, SLOT( btnBiggerClicked() ) );
	QPushButton* btnSmaller = new QPushButton( "-", this );
	_layout->addWidget( btnSmaller );
	connect( btnSmaller, SIGNAL( clicked() ), this, SLOT( btnSmallerClicked() ) );
}

void ChannelTools::btnBiggerClicked() { emit increaseChannel( name ); }
void ChannelTools::btnSmallerClicked() { emit decreaseChannel( name ); }

};
};
