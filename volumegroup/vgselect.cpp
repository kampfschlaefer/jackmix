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

#include "vgselect.h"
#include "vgselect.moc"

#include <qlayout.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

using namespace JackMix;

VGSelectDialog::VGSelectDialog( QWidget* p, const char* n )
 : QDialog( p,n )
 , _vg( 0 )
{
	/// Basic Layouts
	QVBoxLayout* _layout1 = new QVBoxLayout( this );
	QHBoxLayout* _layout2 = new QHBoxLayout( _layout1 );
	QHBoxLayout* _layout3 = new QHBoxLayout( _layout1 );

	/// The mainpart
	_btngroup = new QButtonGroup( 1, Qt::Horizontal, "New Volumegroup", this );
	_layout2->addWidget( _btngroup );
	QRadioButton* _btnaux = new QRadioButton( "Auxiliary channels", _btngroup );
	QRadioButton* _btnstereo = new QRadioButton( "Stereo channel", _btngroup );

	/// The bottom line
	QPushButton* _ok = new QPushButton( "Ok", this );
	connect( _ok, SIGNAL( clicked() ), this, SLOT( finish() ) );
	QPushButton* _abort = new QPushButton( "Cancel", this );
	connect( _abort, SIGNAL( clicked() ), this, SLOT( reject() ) );
	_layout3->addStretch( 100 );
	_layout3->addWidget( _ok, 0 );
	_layout3->addWidget( _abort, 0 );
}
VGSelectDialog::~VGSelectDialog() {
}

void VGSelectDialog::finish() {
	qDebug( "VGSelectDialog::finish()" );
	qDebug( QString::number( _btngroup->selectedId() ) );
	switch ( _btngroup->selectedId() ) {
		case 0:
			break;
		case 1:
			break;
		default:
			break;
	}
	accept();
}


