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

#include "volumegroup.h"
#include "volumegroup.moc"

#include "jack_backend.h"

#include <iostream>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

using namespace JackMix;

VolumeGroupFactory::VolumeGroupFactory() {
}
VolumeGroupFactory::~VolumeGroupFactory() {
}
VolumeGroupFactory* VolumeGroupFactory::the() {
	static VolumeGroupFactory* factory = new VolumeGroupFactory();
	return factory;
}
void VolumeGroupFactory::registerGroup( VolumeGroup* n ) {
std::cerr << "VolumeGroupFactory::registerGroup( " << n << " )" << std::endl;
	_groups.append( n );
	emit sNewVG( n );
}
void VolumeGroupFactory::unregisterGroup( VolumeGroup* n ) {
std::cerr << "VolumeGroupFactory::unregisterGroup( " << n << " )" << std::endl;
	_groups.remove( n );
	emit sRemoveVG( n );
	delete n;
std::cerr << "VolumeGroupFactory::unregisterGroup() finished." << std::endl;
}

VolumeGroup::VolumeGroup( QString name, int channels, QObject* p, const char* n )
 : QObject( p,n )
 , _masterwidget( 0 )
 , _name( name )
 , _channels( channels )
{
std::cerr << "VolumeGroup::VolumeGroup( " << name << ", " << channels << ", " << p << ", n )" << std::endl;
	//VolumeGroupFactory::the()->registerGroup( this );
}
VolumeGroup::~VolumeGroup() {
std::cerr << "VolumeGroup::~VolumeGroup()" << std::endl;
	delete _masterwidget;
}

void VolumeGroup::remove() {
std::cerr << "VolumeGroup::remove()" << std::endl;
	VolumeGroupFactory::the()->unregisterGroup( this );
}


VolumeGroupMasterWidget::VolumeGroupMasterWidget( VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _group( group )
{
	new QVBoxLayout( this );
	QPushButton *tmp = new QPushButton( "remove", this );
	layout()->add( tmp );
	connect( tmp, SIGNAL( clicked() ), _group, SLOT( remove() ) );
}
VolumeGroupMasterWidget::~VolumeGroupMasterWidget() {
}

/*#include <qpixmap.h>
#include <qpixmapeffect.h>
#include <qpainter.h>*/

VolumeGroupChannelWidget::VolumeGroupChannelWidget( QString in, VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _in( in )
 , _group( group )
{
	setMargin( 10 );
	setLineWidth( 1 );
	setFrameStyle( QFrame::Panel|QFrame::Raised );
}
VolumeGroupChannelWidget::~VolumeGroupChannelWidget() {
}

void VolumeGroupChannelWidget::paintEvent( QPaintEvent* ) {
/*std::cerr << "VolumeGroupChannelWidget::paintEvent()" << std::endl;
	QPainter p( this );
	QPixmap bg( this->size() );
	QPixmapEffect::gradient( bg, colorGroup().light(), colorGroup().dark(), QPixmapEffect::DiagonalGradient );
	p.drawPixmap( this->rect(), bg );*/
}

