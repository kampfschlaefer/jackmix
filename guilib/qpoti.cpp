/***************************************************************************
                          kpoti.cpp  -  potentiometer widget
                             -------------------
    begin                : Wed Apr 28 23:05:05 MEST 1999

    copyright            : (C) 1999 by Martin Lorenz
    email                : lorenz@ch.tum.de
                           (C) 2002-2003 Matthias Kretz <kretz@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qpoti.h"
#include "qpoti.moc"

#include <qbitmap.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qdrawutil.h>
#include <qtimer.h>
#include <qkeycode.h>
#include <qpen.h>
#include <qstring.h>
#include <qstyle.h>

#include <math.h>

#include <qpixmap.h>
#include <qglobal.h>
#include <qapplication.h>

#include "qpixmapeffect.h"

#define PI 3.1415926
static const int thresholdTime = 500;
static const int repeatTime    = 100;
static const float maxAngle = PI*135/180; // 140 degrees to both sides
static const float tickLength = 3;

struct QPoti::QPotiPrivate
{
	QPotiPrivate()
		: bgDirty( false )
		, potiDirty( false )
	{}

	bool bgDirty;
	QPixmap bgdb;
	QPixmap bgPixmap( const QColorGroup & colorGroup )
	{
		if( bgDirty || bgdb.isNull() )
		{
			bgdb.resize( buttonRect.size() );
			QPainter dbp( &bgdb );
			dbp.setPen( Qt::NoPen );
			QRect drawRect = bgdb.rect();

			// create mask
			QBitmap mask( bgdb.size(), true );
			QPainter maskpainter( &mask );
			maskpainter.setPen( Qt::NoPen );
			maskpainter.setBrush( Qt::color1 );
			maskpainter.drawEllipse( drawRect );
			maskpainter.end();
			bgdb.setMask( mask );

			// inset shadow
			QPixmap gradient( bgdb.size() );
			gradient.fill( colorGroup.light() );
			QPixmapEffect::gradient( gradient, colorGroup.light(), colorGroup.dark(), QPixmapEffect::DiagonalGradient );
			dbp.setBrush( QBrush( colorGroup.button(), gradient ) );
			dbp.drawEllipse( drawRect );

			potiRect.setSize( drawRect.size() * 0.9 );
			if( potiRect.width() + 6 > drawRect.width() )
			{
				potiRect.setWidth( drawRect.width() - 6 );
				potiRect.setHeight( drawRect.height() - 6 );
			}
			potiRect.moveCenter( center );

			bgDirty = false;
		}
		return bgdb;
	}

	QColor potiColor;
	bool potiDirty;
	QPixmap potidb;
	QPixmap potiPixmap()
	{
		if( ( potiDirty || potidb.isNull() ) && ! potiRect.size().isEmpty() )
		{
			potidb.resize( potiRect.size() );
			QPainter dbp( &potidb );
			dbp.setPen( Qt::NoPen );
			QRect drawRect( potidb.rect() );

			// create mask
			QBitmap mask( potidb.size(), true );
			QPainter maskpainter( &mask );
			maskpainter.setPen( Qt::NoPen );
			maskpainter.setBrush( Qt::color1 );
			maskpainter.drawEllipse( drawRect );
			maskpainter.end();
			potidb.setMask( mask );

			QPixmap gradient( potidb.size() );
			gradient.fill( potiColor );
			QPixmapEffect::gradient( gradient, potiColor.dark( 130 ), potiColor.light( 130 ), QPixmapEffect::DiagonalGradient );
			dbp.setBrush( QBrush( potiColor, gradient ) );
			dbp.drawEllipse( drawRect );

			potiDirty = false;
		}
		return potidb;
	}

	QRect buttonRect;
	QRect potiRect;
	QRect labelRect;
	QString label;
	QPoint center;
};

QSizePolicy QPoti::sizePolicy() const
{
	return QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
}

QSize QPoti::sizeHint() const
{
	return minimumSizeHint();
}

QSize QPoti::minimumSizeHint() const
{
	int width = 30;
	int height = 30;
	if( m_bLabel )
	{
		QFontMetrics metrics( font() );
		d->labelRect = metrics.boundingRect( d->label );
		d->labelRect.setHeight( metrics.lineSpacing() );
		width = QMAX( width, d->labelRect.width() + frameRect().width() - contentsRect().width() );
		height += metrics.lineSpacing();
	}
	//kdDebug() << k_funcinfo << "return " <<  width << "x" << height << endl;
	return QSize( width, height );
}

QString QPoti::text() const
{
	return d->label;
}

void QPoti::setText( const QString & text )
{
	d->label = text;
	setMinimumSize( minimumSizeHint() );
	updateGeometry();
}

/**
  Constructs a poti.

  The \e parent and \e name arguments are sent to the QWidget constructor.
*/
QPoti::QPoti( QWidget *parent, const char *name )
    : QFrame( parent, name, WResizeNoErase | WRepaintNoErase )
	, d( 0 )
{
    init();
}



/**
  Constructs a poti.

  \arg \e minValue is the minimum slider value.
  \arg \e maxValue is the maximum slider value.
  \arg \e step is the page step value.
  \arg \e value is the initial value.

  The \e parent and \e name arguments are sent to the QWidget constructor.
*/

QPoti::QPoti( int minValue, int maxValue, int step,
		  int value, QWidget *parent, const char *name )
    : QFrame( parent, name, WResizeNoErase | WRepaintNoErase )
	, QRangeControl( minValue, maxValue, 1, step, value )
	, d( 0 )
{
    init(value);
}

QPoti::~QPoti()
{
	delete d;
	d = 0;
}

void QPoti::init(int value)
{
	d = new QPotiPrivate;
	font().setPointSize( 8 );
	d->potiColor.setNamedColor( "red" );

  timer = 0;
  potiVal = value;
  potiPos = positionFromValue(value);
  clickOffset = 0;
  state = Idle;
  track = TRUE;
  ticks = TRUE;
  m_bLabel = true;
  tickInt = 0;

  setFocusPolicy( TabFocus );
  initTicks();
}


/**
  Does what's needed when someone changes the tickmark status
*/

void QPoti::initTicks()
{
	QRect available = contentsRect();
	if( m_bLabel )
		available.rTop() += d->labelRect.height();
	d->center = available.center();
	// make the width and height equal
	if( available.width() > available.height() )
		available.setWidth( available.height() );
	else if( available.height() > available.width() )
		available.setHeight( available.width() );
	available.moveCenter( d->center );
	d->buttonRect = available;

	buttonRadius = available.width() / 2.0;
	if( ticks )
	{
		buttonRadius -= tickLength;
		int tickSpace = static_cast<int>( tickLength );
		d->buttonRect.rTop() += tickSpace;
		d->buttonRect.rLeft() += tickSpace;
		d->buttonRect.rRight() -= tickSpace;
		d->buttonRect.rBottom() -= tickSpace;
	}

	d->potiDirty = true;
	d->bgDirty = true;
}


/**
  Enables slider tracking if \e enable is TRUE, or disables tracking
  if \e enable is FALSE.

  If tracking is enabled (default), the slider emits the
  valueChanged() signal whenever the slider is being dragged.  If
  tracking is disabled, the slider emits the valueChanged() signal
  when the user releases the mouse button (unless the value happens to
  be the same as before).

  \sa tracking()
*/

void QPoti::setTracking( bool enable )
{
    track = enable;
}


/**
  \fn bool QPoti::tracking() const
  Returns TRUE if tracking is enabled, or FALSE if tracking is disabled.

  Tracking is initially enabled.

  \sa setTracking()
*/


/**
  \fn void QPoti::valueChanged( int value )
  This signal is emitted when the slider value is changed, with the
  new slider value as an argument.
*/

/**
  \fn void QPoti::sliderPressed()
  This signal is emitted when the user presses the slider with the mouse.
*/

/**
  \fn void QPoti::sliderMoved( int value )
  This signal is emitted when the slider is dragged, with the
  new slider value as an argument.
*/

/**
  \fn void QPoti::sliderReleased()
  This signal is emitted when the user releases the slider with the mouse.
*/

/**
  Calculates slider position corresponding to value \a v. Does not perform
  rounding.
*/

float QPoti::positionFromValue( int v ) const
{

    int range = maxValue() - minValue();
    return ( (v - minValue() ) *2* maxAngle) / range - maxAngle;
}


/**
  Calculates value corresponding to poti position \a p. Performs rounding.
*/

int QPoti::valueFromPosition( float p ) const
{
    int range = maxValue() - minValue();
    return (int) (minValue() + ((p+maxAngle)*range)/(2*maxAngle));
}

/*!
  Implements the virtual QRangeControl function.
*/

void QPoti::rangeChange()
{
	float newPos = positionFromValue( value() );
	if ( newPos != potiPos ) {
		reallyMovePoti( newPos );
	}
}

void QPoti::paletteChange( const QPalette & )
{
	d->bgDirty = true;
	d->potiDirty = true;
}

/*!
  Changes the value (slot)
*/

void QPoti::valueChange()
{
    if ( potiVal != value() ) {
	float newPos = positionFromValue( value() );
	potiVal = value();
	reallyMovePoti( newPos );
    }
    emit valueChanged(value());
}


/*!
  Handles resize events for the poti.
*/

void QPoti::resizeEvent( QResizeEvent * )
{
    rangeChange();
    initTicks();
}

void QPoti::setLabel(bool s)
{
	m_bLabel = s;
	initTicks();
}

/**
  Sets the color of the button
  */
void QPoti::setColor( const QColor &c )
{
	d->potiColor = c;
	d->potiDirty = true;
	repaint();
}


void QPoti::paintPoti( QPainter * p )
{
	if( isVisible() )
	{
		QPixmap db = d->potiPixmap();
		if( db.isNull() )
			return;

		QPainter p2( &db );
		p2.translate( db.rect().center().x(), db.rect().center().y() );
		p2.rotate( potiPos * 180.0 / PI );
		QRect pointer( db.width() / -20, db.width() / -2, db.width() / 10, db.width() / 2 );
		QBrush buttonbrush( colorGroup().button() );
		qDrawShadePanel( &p2, pointer, colorGroup(), true, 1, &buttonbrush );
		p2.end();

		p->drawPixmap( d->potiRect, db );
	}
}

/*!
  Performs the actual moving of the slider.
*/

void QPoti::reallyMovePoti( float newPos )
{
  QPainter p;
  p.begin( this );
  p.setPen(NoPen);
  potiPos = newPos;
  paintPoti(&p);
  p.end();
}




/**
  Handles paint events for the slider.
*/

void QPoti::drawContents( QPainter * p )
{
	QPixmap doublebuffer( contentsRect().size() );
	doublebuffer.fill( colorGroup().background() );
	QPainter dbp( &doublebuffer );
	if( m_bLabel )
	{
		dbp.setFont( font() );
		QFontMetrics metrics = dbp.fontMetrics();
		dbp.drawText( contentsRect().x() - metrics.leftBearing( d->label[ 0 ] ) + ( contentsRect().width() - d->labelRect.width() ) / 2, metrics.height(), d->label );
	}

    int interval = tickInt;
    if( interval <= 0 )
		interval = 12;
    if( ticks )
		drawTicks( &dbp, buttonRadius, tickLength, interval );

	dbp.drawPixmap( d->buttonRect, d->bgPixmap( colorGroup() ) );

	if( hasFocus() )
		style().drawPrimitive( QStyle::PE_FocusRect, &dbp, d->buttonRect, colorGroup() );

    paintPoti( &dbp );
	dbp.end();
	p->drawPixmap( contentsRect(), doublebuffer );
}


/*!
  Handles mouse press events for the slider.
*/

void QPoti::mousePressEvent( QMouseEvent *e )
{
  resetState();

  if ( e->button() == MidButton ) {
    double pos = atan2( double(e->pos().x()-d->center.x()),
                       double(- e->pos().y() + d->center.y()) );
    movePoti( pos );
    return;
  }
  if ( e->button() != LeftButton )
    return;


  int dx=e->pos().x()-d->center.x(), dy=e->pos().y()-d->center.y();

  if ( dx*dx+dy*dy < buttonRadius*buttonRadius ) {
    state = Dragging;
    clickOffset =  potiVal + (e->pos().y() ) ;
    emit potiPressed();
  }  else if ( e->pos().x() < width()/2 ) {
    state = TimingDown;
    subtractPage();
    if ( !timer )
      timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), SLOT(repeatTimeout()) );
    timer->start( thresholdTime, TRUE );
  } else  {
    state = TimingUp;
    addPage();
    if ( !timer )
      timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), SLOT(repeatTimeout()) );
    timer->start( thresholdTime, TRUE );
  }
}

/*!
  Handles mouse move events for the slider.
*/
void QPoti::mouseMoveEvent( QMouseEvent *e )
{

    if ( (e->state() & MidButton) ) { 		// middle button wins
      double pos = atan2( double(e->pos().x()-d->center.x()),
                          double(- e->pos().y()+d->center.y()) );
      movePoti( pos );
      return;
    }
    if ( !(e->state() & LeftButton) )
	return;					// left mouse button is up
    if ( state != Dragging )
	return;


    movePoti( positionFromValue(- e->pos().y() + clickOffset ));
}


/*!
  Handles mouse release events for the slider.
*/

void QPoti::mouseReleaseEvent( QMouseEvent *e )
{
    if ( !(e->button() & LeftButton) )
	return;
    resetState();
}

void QPoti::focusInEvent( QFocusEvent * e )
{
	//setFrameStyle( Raised | Box );
	//setLineWidth( 1 );
	QFrame::focusInEvent( e );
}

void QPoti::focusOutEvent( QFocusEvent * e )
{
	//setFrameStyle( NoFrame );
	//setLineWidth( 0 );
	QFrame::focusOutEvent( e );
}


void QPoti::enterEvent( QEvent * )
{
    emit mouseEntered( potiVal );
}


/*!
  Moves the left (or top) edge of the slider to position
  \a pos. Performs snapping.
*/

void QPoti::movePoti( float pos )
{
    float newPos = QMIN( maxAngle, QMAX( -maxAngle, pos ) );
    int newVal = valueFromPosition( newPos );
    if ( potiVal != newVal ) {
	potiVal = newVal;
	emit potiMoved( potiVal );
    }
    if ( tracking() && potiVal != value() ) {
	directSetValue( potiVal );
	emit valueChanged( potiVal );
    }


    if ( potiPos != newPos )
	reallyMovePoti( newPos );
}


/*!
  Resets all state information and stops my timer.
*/

void QPoti::resetState()
{
    if ( timer ) {
	timer->stop();
	timer->disconnect();
    }
    switch ( state ) {
    case TimingUp:
    case TimingDown:
	break;
    case Dragging: {
	setValue( valueFromPosition( potiPos ) );
	emit potiReleased();
	break;
    }
    case Idle:
	break;
    default:
	//kdWarning() << "QPoti: in wrong state" << endl;
	break;
    }
    state = Idle;
}


/*!
  Handles key press events for the slider.
*/

void QPoti::keyPressEvent( QKeyEvent *e )
{

    switch ( e->key() ) {
    case Key_Left:
      subtractLine();
      break;
    case Key_Right:
      addLine();
      break;
    case Key_Up:
      addLine();
      break;
    case Key_Down:
      subtractLine();
      break;
    case Key_Prior:
      subtractPage();
      break;
    case Key_Next:
      addPage();
      break;
    case Key_Home:
      setValue( minValue() );
      break;
    case Key_End:
      setValue( maxValue() );
      break;
    default:
	e->ignore();
	return;
    }
    e->accept();
}





/*!
  Makes QRangeControl::setValue() available as a slot.
*/

void QPoti::setValue( int value )
{
    QRangeControl::setValue( value );
}


/*!
  Moves the slider one pageStep() upwards.
*/

void QPoti::addStep()
{
    addPage();
}


/*!
  Moves the slider one pageStep() downwards.
*/

void QPoti::subtractStep()
{
    subtractPage();
}


/*!
  Waits for autorepeat.
*/

void QPoti::repeatTimeout()
{
    Q_ASSERT( timer );
    timer->disconnect();
    if ( state == TimingDown )
	connect( timer, SIGNAL(timeout()), SLOT(subtractStep()) );
    else if ( state == TimingUp )
	connect( timer, SIGNAL(timeout()), SLOT(addStep()) );
    timer->start( repeatTime, FALSE );
}




/*!
  Using \a p, draws tickmarks at a distance of \a dist from the edge
  of the widget, using \a w pixels and \a i intervals.
  */

void QPoti::drawTicks( QPainter *p, double dist, double w, int i ) const
{
	p->setPen( colorGroup().foreground() );
	double angle,s,c;
	double x, y;
	for (int v=0; v<=i; v++)
	{
		angle = -maxAngle+2*maxAngle*v/i;
		s = sin( angle );
		c = cos( angle );
		x = d->center.x() - s * dist;
		y = d->center.y() - c * dist;

		p->drawLine( (int)x, (int)y, (int)(x - s * w), (int)(y - c * w) );
	}
}

void QPoti::wheelEvent(QWheelEvent *e)
{
	setValue(value()+e->delta()/120*8);
}


/*!
  Sets the way tickmarks are displayed by the slider. \a s can take
  the following values:
  <ul>
  <li> \c NoMarks
  <li> \c Above
  <li> \c Left
  <li> \c Below
  <li> \c Right
  <li> \c Both
  </ul>
  The initial value is \c NoMarks.
  \sa tickmarks(), setTickInterval()
*/

void QPoti::setTickmarks( bool s )
{
    ticks = s;
    initTicks();
    update();
}



/*!
  Sets the interval between tickmarks to \a i. This is a value interval,
  not a pixel interval. If \a i is 0, the slider
  will choose between lineStep() and pageStep(). The initial value of
  tickInterval() is 0.
  \sa tickInterval(), QRangeControl::lineStep(), QRangeControl::pageStep()
*/

void QPoti::setTickInterval( int i )
{
    tickInt = QMAX( 0, i );
    update();
}


/*!
  \fn int QPoti::tickInterval() const
  Returns the interval between tickmarks. Returns 0 if the slider
  chooses between pageStep() and lineStep().
  \sa setTickInterval()
*/

// vim: sw=4 ts=4
