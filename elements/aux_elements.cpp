
#include "aux_elements.h"
#include "aux_elements.moc"

#include "qfloatpoti.h"
#include "qfloatslider.h"
#include "slider.h"
#include "jack_backend.h"

#include <qlayout.h>
#include <qpushbutton.h>

using namespace JackMix;
using namespace MixerElements;
using namespace MixingMatrix;

/// The Factory for creating this things...
class AuxFactory : public ElementFactory
{
public:
	AuxFactory() : ElementFactory() { globaldebug(); }
	~AuxFactory() {}

	QStringList canCreate() const {
		return QStringList()<<"AuxElement"<<"Mono2StereoElement"<<"Stereo2StereoElement";
	}
	QStringList canCreate( int in, int out ) const {
		if ( in==1 && out==1 ) return QStringList()<<"AuxElement";
		if ( in==1 && out==2 ) return QStringList()<<"Mono2StereoElement";
		if ( in==2 && out==2 ) return QStringList()<<"Stereo2StereoElement";
		return QStringList();
	}

	Element* create( QString type , QStringList ins, QStringList outs, Widget* p, const char* n=0 ) {
		if ( type=="AuxElement" )
			return new AuxElement( ins, outs, p, n );
		if ( type=="Mono2StereoElement" )
			return new Mono2StereoElement( ins, outs, p, n );
		if ( type=="Stereo2StereoElement" )
			return new Stereo2StereoElement( ins, outs, p, n );
		return 0;
	}
};
AuxFactory* _factory = new AuxFactory();

AuxElement::AuxElement( QStringList inchannel, QStringList outchannel, Widget* p, const char* n )
	: Element( inchannel, outchannel, p, n )
	, dB2VolCalc( -36, 6 )
	, _inchannel( inchannel[ 0 ] )
	, _outchannel( outchannel[ 0 ] )
{
	QGridLayout* _layout = new QGridLayout( this, 2,2, 3 );
	QPushButton* _btn_select = new QPushButton( "S", this );
	layout()->add( _btn_select );
	connect( _btn_select, SIGNAL( clicked() ), this, SLOT( slot_toggle() ) );
	QPushButton* _btn_replace = new QPushButton( "R", this );
	layout()->add( _btn_replace );
	connect( _btn_replace, SIGNAL( clicked() ), this, SLOT( slot_replace() ) );

	QFloatPoti* poti = new QFloatPoti( amptodb( BACKEND->getVolume( _inchannel, _outchannel ) ), -36, 6, 10, QColor( 255,0,0 ), this, _inchannel );
	_layout->addMultiCellWidget( poti, 1,1, 0,1 );
	connect( poti, SIGNAL( valueChanged( float ) ), this, SLOT( emitvalue( float ) ) );
}
AuxElement::~AuxElement() {
}

void AuxElement::slot_toggle() {
	select( !isSelected() );
}
void AuxElement::slot_replace() {
	emit replace( this );
}

void AuxElement::emitvalue( float n ) {
	//qDebug( "AuxElement::emitvalue( float %f dB ) that means ampfactor %f", n, dbtoamp( n ) );
	BACKEND->setVolume( _inchannel, _outchannel, dbtoamp( n ) );
}

Mono2StereoElement::Mono2StereoElement( QStringList inchannel, QStringList outchannels, Widget* p, const char* n )
	: Element( inchannel, outchannels, p, n )
	, dB2VolCalc( -36, 6 )
	, _inchannel( inchannel[ 0 ] )
	, _outchannel1( outchannels[ 0 ] )
	, _outchannel2( outchannels[ 1 ] )
	, _balance_value( 0 )
	, _volume_value( 0 )
{
	float left = BACKEND->getVolume( _inchannel, _outchannel1 );
	float right = BACKEND->getVolume( _inchannel, _outchannel2 );
	if ( left>right ) {
		_volume_value = left;
		_balance_value = left-right;
	} else {
		_volume_value = right;
		_balance_value = left-right;
	}
	QGridLayout* _layout = new QGridLayout( this, 3,1, 3 );
	QPushButton* _btn_select = new QPushButton( "S", this );
	_layout->addWidget( _btn_select, 0,0 );
	connect( _btn_select, SIGNAL( clicked() ), this, SLOT( slot_toggle() ) );
	QFloatPoti* _balance = new QFloatPoti( _balance_value, -1, 1, 100, QColor( 0,0,255 ), this );
	_layout->addWidget( _balance, 1,0 );
	connect( _balance, SIGNAL( valueChanged( float ) ), this, SLOT( balance( float ) ) );
	JackMix::GUI::Slider* _volume = new JackMix::GUI::Slider( amptodb( _volume_value ), dbmin, dbmax, 1, 3, this );
	_layout->addWidget( _volume, 2,0 );
	connect( _volume, SIGNAL( valueChanged( float ) ), this, SLOT( volume( float ) ) );
}
Mono2StereoElement::~Mono2StereoElement() {
}

void Mono2StereoElement::slot_toggle() { select( !isSelected() ); }

void Mono2StereoElement::balance( float n ) {
	//qDebug( "Mono2StereoElement::balance( float %f )", n );
	_balance_value = n;
	calculateVolumes();
}
void Mono2StereoElement::volume( float n ) {
	//qDebug( "Mono2StereoElement::volume( float %f )", n );
	_volume_value = n;
	calculateVolumes();
}

void Mono2StereoElement::calculateVolumes() {
	float left, right;
		left = dbtoamp( _volume_value );
		right = dbtoamp( _volume_value );
	if ( _balance_value > 0 )
		left = dbtoamp( _volume_value )*( 1-_balance_value );
	if ( _balance_value < 0 )
		right = dbtoamp( _volume_value )*( 1+_balance_value );
	BACKEND->setVolume( _inchannel, _outchannel1, left );
	BACKEND->setVolume( _inchannel, _outchannel2, right );
}

Stereo2StereoElement::Stereo2StereoElement( QStringList inchannels, QStringList outchannels, Widget* p, const char* n )
	: Element( inchannels, outchannels, p, n )
	, dB2VolCalc( -42, 6 )
	, _inchannel1( inchannels[ 0 ] )
	, _inchannel2( inchannels[ 1 ] )
	, _outchannel1( outchannels[ 0 ] )
	, _outchannel2( outchannels[ 1 ] )
	, _balance_value( 0 )
	, _volume_value( 0 )
{
	float left = BACKEND->getVolume( _inchannel1, _outchannel1 );
	float right = BACKEND->getVolume( _inchannel2, _outchannel2 );
	if ( left>right ) {
		_volume_value = left;
		_balance_value = left-right;
	} else {
		_volume_value = right;
		_balance_value = left-right;
	}
	QGridLayout* _layout = new QGridLayout( this, 3,2, 3 );
	QPushButton* _btn_select = new QPushButton( "S", this );
	_layout->addWidget( _btn_select, 0,0 );
	connect( _btn_select, SIGNAL( clicked() ), this, SLOT( slot_toggle() ) );
	QPushButton* _btn_replace = new QPushButton( "R", this );
	_layout->addWidget( _btn_replace, 0,1 );
	connect( _btn_replace, SIGNAL( clicked() ), this, SLOT( slot_replace() ) );
	_layout->setRowStretch( 0, -100 );
	JackMix::GUI::Slider* _balance_widget = new JackMix::GUI::Slider( _balance_value, -1, 1, 2, 0.1, this, "%1" );
	_layout->addMultiCellWidget( _balance_widget, 1,1, 0,1 );
	_layout->setRowStretch( 1, -100 );
	connect( _balance_widget, SIGNAL( valueChanged( float ) ), this, SLOT( balance( float ) ) );
	JackMix::GUI::Slider* _volume_widget = new JackMix::GUI::Slider( amptodb( _volume_value ), dbmin, dbmax, 1, 3, this );
	_layout->addMultiCellWidget( _volume_widget, 2,2, 0,1 );
	_layout->setRowStretch( 2, 1000 );
	connect( _volume_widget, SIGNAL( valueChanged( float ) ), this, SLOT( volume( float ) ) );
	BACKEND->setVolume( _inchannel1, _outchannel2, 0 );
	BACKEND->setVolume( _inchannel2, _outchannel1, 0);
}
Stereo2StereoElement::~Stereo2StereoElement() {
}

void Stereo2StereoElement::slot_toggle() { select( !isSelected() ); }

void Stereo2StereoElement::balance( float n ) {
	//qDebug( "Mono2StereoElement::balance( float %f )", n );
	_balance_value = n;
	calculateVolumes();
}
void Stereo2StereoElement::volume( float n ) {
	//qDebug( "Mono2StereoElement::volume( float %f )", n );
	_volume_value = n;
	calculateVolumes();
}

void Stereo2StereoElement::calculateVolumes() {
	float left, right;
		left = dbtoamp( _volume_value );
		right = dbtoamp( _volume_value );
	if ( _balance_value > 0 )
		left = dbtoamp( _volume_value )*( 1-_balance_value );
	if ( _balance_value < 0 )
		right = dbtoamp( _volume_value )*( 1+_balance_value );
	BACKEND->setVolume( _inchannel1, _outchannel1, left );
	BACKEND->setVolume( _inchannel2, _outchannel2, right );
}
