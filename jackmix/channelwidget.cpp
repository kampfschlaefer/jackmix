/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

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
		_layout->addWidget( tmp, 0 );
	}
	BACKEND->addInput( _name );
	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ ) {
		_layout->addWidget( VolumeGroupFactory::the()->group( i )->channelWidget( _name, this ), 100 );
	}
}
ChannelWidget::~ChannelWidget() {
}

void ChannelWidget::valueChanged( QString out, float n ) {
	BACKEND->setVolume( _name, out, n );
}

