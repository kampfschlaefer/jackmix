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
#include <iostream>
#include <qpushbutton.h>

using namespace JackMix;

ChannelWidget::ChannelWidget( QString name, QWidget* p, const char* n ) : QFrame( p,n ), _name( name ) {
	std::cerr << "ChannelWidget::ChannelWidget( " << name << ", " << p << ", n )" << std::endl;
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	setLineWidth( 1 );
	setMargin( 1 );
	QBoxLayout *_layout = new QBoxLayout( this, QBoxLayout::TopToBottom );
	_layout->setMargin( 1 );
	/// Label
	{
		QLabel *tmp = new QLabel( _name, this );
		tmp->setFrameStyle( QFrame::Panel|QFrame::Raised );
		tmp->setLineWidth( 1 );
		tmp->setMargin( 2 );
		tmp->setAlignment( Qt::AlignCenter );
		tmp->setFont( QFont( "", 14, QFont::Bold ) );;
		_layout->addWidget( tmp, 0 );
	}
	/// RemoveButton
	{
std::cerr << "Create PushButton" << std::endl;
		QPushButton* tmp = new QPushButton( "Remove", this );
		_layout->addWidget( tmp );
		connect( tmp, SIGNAL( clicked() ), this, SLOT( remove() ) );
		tmp->show();
	}

	BACKEND->addInput( _name );

	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ )
		newVG( VolumeGroupFactory::the()->group( i ) );

	connect( VolumeGroupFactory::the(), SIGNAL( sNewVG( VolumeGroup* ) ), this, SLOT( newVG( VolumeGroup* ) ) );
	connect( VolumeGroupFactory::the(), SIGNAL( sRemoveVG( VolumeGroup* ) ), this, SLOT( removeVG( VolumeGroup* ) ) );
	this->show();
}
ChannelWidget::~ChannelWidget() {
	std::cerr << "ChannelWidget::~ChannelWidget()" << std::endl;
	BACKEND->removeInput( _name );
}

void ChannelWidget::valueChanged( QString out, float n ) {
	std::cerr << "ChannelWidget::valueChanged( " << out << ", " << n << " )" << std::endl;
	BACKEND->setVolume( _name, out, n );
}

void ChannelWidget::newVG( VolumeGroup* n ) {
	std::cerr << "ChannelWidget::newVG( " << n << " )" << std::endl;
	VolumeGroupChannelWidget* tmp = n->channelWidget( _name, this );
	layout()->add( tmp );
	tmp->show();
	_groupwidgets.append( tmp );
}

void ChannelWidget::removeVG( VolumeGroup* n ) {
	std::cerr << "ChannelWidget::removeVG( " << n << " )" << std::endl;
	for ( uint i=0; i<_groupwidgets.count(); i++ ) {
		if ( _groupwidgets.at( i )->group() == n ) {
			delete _groupwidgets.at( i );
			_groupwidgets.remove( i );
		}
	}
}

void ChannelWidget::remove() {
	std::cerr << "ChannelWidget::remove()" << std::endl;
	_groupwidgets.setAutoDelete( true );
	_groupwidgets.clear();
	delete this;
}

