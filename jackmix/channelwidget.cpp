
#include "channelwidget.h"
#include "channelwidget.moc"

#include "jack_backend.h"
#include "volumegroup.h"
#include <qstringlist.h>
#include <qlayout.h>
#include <qlabel.h>

using namespace JackMix;

ChannelWidget::ChannelWidget( QString name, QWidget* p, const char* n ) : QFrame( p,n ), _name( name ) {
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	setLineWidth( 1 );
	setMargin( 1 );
	QBoxLayout *_layout = new QBoxLayout( this, QBoxLayout::TopToBottom );
	_layout->setMargin( 1 );
	{
		QLabel *tmp = new QLabel( _name, this );
		tmp->setFrameStyle( QFrame::Panel|QFrame::Raised );
		tmp->setLineWidth( 1 );
		tmp->setMargin( 2 );
		tmp->setAlignment( Qt::AlignCenter );
		tmp->setFont( QFont( "", 14, QFont::Bold ) );;
		_layout->addWidget( tmp, -10 );
	}
	BACKEND->addInput( _name );
	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ ) {
		_layout->addWidget( VolumeGroupFactory::the()->group( i )->channelWidget( _name, this ) );
	}
}
ChannelWidget::~ChannelWidget() {
}

void ChannelWidget::valueChanged( QString out, float n ) {
	BACKEND->setVolume( _name, out, n );
}

