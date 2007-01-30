// qsynthKnob.cpp
//
/****************************************************************************
   Copyright (C) 2005-2006, rncbc aka Rui Nuno Capela. All rights reserved.
   Copyright (C) 2006, Arnold Krille. ( Porting to Qt4 )

   This widget is based on a design by Thorsten Wilms, 
   implemented by Chris Cannam in Rosegarden,
   adapted for QSynth by Pedro Lopez-Cabanillas

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*****************************************************************************/

#include <cmath>
#include <QtCore/QTimer>
#include <QtGui/QToolTip>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QColormap>

#include "qsynthKnob.h"
#include "qsynthKnob.moc"


struct qsynthKnobCacheIndex
{
	// Constructor.
	qsynthKnobCacheIndex(int s = 0, int kc = 0, int mc = 0,
		int a = 0, int n = 0, int c = 0) :
			size(s), knobColor(kc), meterColor(mc),
			angle(a), numTicks(n), centered(c) {}

	bool operator<(const qsynthKnobCacheIndex &i) const {
		// woo!
		if (size < i.size) return true;
		else if (size > i.size) return false;
		else if (knobColor < i.knobColor) return true;
		else if (knobColor > i.knobColor) return false;
		else if (meterColor < i.meterColor) return true;
		else if (meterColor > i.meterColor) return false;
		else if (angle < i.angle) return true;
		else if (angle > i.angle) return false;
		else if (numTicks < i.numTicks) return true;
		else if (numTicks > i.numTicks) return false;
		else if (centered == i.centered) return false;
		else if (!centered) return true;
		return false;
	}

	int          size;
	unsigned int knobColor;
	unsigned int meterColor;
	int          angle;
	int          numTicks;
	bool         centered;
};

typedef QMap<qsynthKnobCacheIndex, QPixmap> qsynthKnobPixmapCache;


//-------------------------------------------------------------------------
// qsynthKnob - Instance knob widget class.
//

#define QSYNTHKNOB_MIN      (0.25 * M_PI)
#define QSYNTHKNOB_MAX      (1.75 * M_PI)
#define QSYNTHKNOB_RANGE    (QSYNTHKNOB_MAX - QSYNTHKNOB_MIN)


// Constructor.
qsynthKnob::qsynthKnob ( QWidget *pParent, const char *pszName )
	: QDial(pParent),
	m_knobColor(Qt::black), m_meterColor(Qt::white),
	m_bMouseDial(false), m_bMousePressed(false),
	m_iDefaultValue(-1)
{
	setObjectName(pszName);
	setMinimumSize( 40,40 );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setAutoFillBackground( false );
}


// Destructor.
qsynthKnob::~qsynthKnob (void)
{
}


void qsynthKnob::paintEvent ( QPaintEvent* )
{
	static qsynthKnobPixmapCache s_pixmaps;

	QPainter paint;

	double angle = QSYNTHKNOB_MIN // offset
		+ (QSYNTHKNOB_RANGE *
			(double(value() - minimum()) /
			(double(maximum() - minimum()))));
	int degrees = int(angle * 180.0 / M_PI);

	int ns = notchSize();
	int numTicks = 1 + (maximum() + ns - minimum()) / ns;
	
	QColor knobColor(m_knobColor);
	if (knobColor == Qt::black)
		knobColor = palette().mid().color();

	QColor meterColor(m_meterColor);
	if (!isEnabled())
		meterColor = palette().mid().color();
	else if (m_meterColor == Qt::white)
		meterColor = palette().highlight().color();

	int m_size = width() < height() ? width() : height();
	qsynthKnobCacheIndex index(m_size,
		QColormap::instance().pixel( knobColor ),
		QColormap::instance().pixel( meterColor ),
		degrees, numTicks, false);

	if (s_pixmaps.find(index) != s_pixmaps.end()) {
		paint.begin(this);
		paint.drawPixmap(0, 0, s_pixmaps[index]);
		paint.end();
		return;
	}

	int scale = 4;
	int width = m_size * scale;
	QPixmap map(width, width);
	map.fill(palette().color( QPalette::Window ));
	paint.begin(&map);

	QPen pen;
	QColor c;

	// Knob body and face...

	c = knobColor;
	pen.setColor(knobColor);
	pen.setWidth(scale);
	
	paint.setPen(pen);
	paint.setBrush(c);

	int indent = (int)(width * 0.15 + 1);

	paint.drawEllipse(indent, indent, width-2*indent, width-2*indent);

	pen.setWidth(2 * scale);
	int pos = indent + (width-2*indent) / 20;
	int darkWidth = (width-2*indent) * 3 / 4;
	while (darkWidth) {
		c = c.light(101);
		pen.setColor(c);
		paint.setPen(pen);
		paint.drawEllipse(pos, pos, darkWidth, darkWidth);
		if (!--darkWidth) break;
		paint.drawEllipse(pos, pos, darkWidth, darkWidth);
		if (!--darkWidth) break;
		paint.drawEllipse(pos, pos, darkWidth, darkWidth);
		++pos; --darkWidth;
	}

	// The bright metering bit...

	c = meterColor;
	pen.setColor(c);
	pen.setWidth(indent);
	paint.setPen(pen);

	paint.drawArc(indent/2, indent/2,
		width-indent, width-indent, (180 + 45) * 16, -(degrees - 45) * 16);

	// Tick notches...

	paint.setBrush(Qt::NoBrush);

	if ( notchesVisible() ) {
		pen.setColor(palette().dark().color());
		pen.setWidth(scale);
		paint.setPen(pen);
		for (int i = 0; i < numTicks; ++i) {
			int div = numTicks;
			if (div > 1) --div;
			drawTick(paint, QSYNTHKNOB_MIN +
				(QSYNTHKNOB_MAX - QSYNTHKNOB_MIN) * i / div,
				width, i != 0 && i != numTicks-1 );
		}
	}

	// Shadowing...

	pen.setWidth(scale);
	paint.setPen(pen);

	// Knob shadow...

	int shadowAngle = -720;
	c = knobColor.dark();
	for (int arc = 120; arc < 2880; arc += 240) {
		pen.setColor(c);
		paint.setPen(pen);
		paint.drawArc(indent, indent,
			width-2*indent, width-2*indent, shadowAngle + arc, 240);
		paint.drawArc(indent, indent,
			width-2*indent, width-2*indent, shadowAngle - arc, 240);
		c = c.light(110);
	}

	// Scale shadow...

	shadowAngle = 2160;
	c = palette().dark().color();
	for (int arc = 120; arc < 2880; arc += 240) {
		pen.setColor(c);
		paint.setPen(pen);
		paint.drawArc(scale/2, scale/2,
			width-scale, width-scale, shadowAngle + arc, 240);
		paint.drawArc(scale/2, scale/2,
			width-scale, width-scale, shadowAngle - arc, 240);
		c = c.light(108);
	}

	// Undraw the bottom part...

	pen.setColor(palette().color( QPalette::Background ));
	paint.setPen(pen);
	paint.drawArc(scale/2, scale/2,
		width-scale, width-scale, -45 * 16, -90 * 16);

	// Pointer notch...

	double hyp = double(width) / 2.0;
	double len = hyp - indent;
	--len;

	double x0 = hyp;
	double y0 = hyp;

	double x = hyp - len * sin(angle);
	double y = hyp + len * cos(angle);

	c = palette().dark().color();
	pen.setColor(isEnabled() ? c.dark(130) : c);
	pen.setWidth(scale * 2);
	paint.setPen(pen);
	paint.drawLine(int(x0), int(y0), int(x), int(y));
	paint.end();

	// Image rendering...

	/// The step with QImage allows smooth scaling. But I don't know how to do that
	/// with qt4...
	//QImage img = map.convertToImage().smoothScale(m_size, m_size);
	//s_pixmaps[index] = QPixmap(img);
	s_pixmaps[ index ] = map.scaled( m_size, m_size );
	paint.begin(this);
	paint.drawPixmap(0, 0, s_pixmaps[index]);
	paint.end();
}


void qsynthKnob::drawTick ( QPainter& paint,
	double angle, int size, bool internal )
{
	double hyp = double(size) / 2.0;
	double x0 = hyp - (hyp - 1) * sin(angle);
	double y0 = hyp + (hyp - 1) * cos(angle);

	if (internal) {

		double len = hyp / 4;
		double x1 = hyp - (hyp - len) * sin(angle);
		double y1 = hyp + (hyp - len) * cos(angle);
		
		paint.drawLine(int(x0), int(y0), int(x1), int(y1));

	} else {

		double len = hyp / 4;
		double x1 = hyp - (hyp + len) * sin(angle);
		double y1 = hyp + (hyp + len) * cos(angle);

		paint.drawLine(int(x0), int(y0), int(x1), int(y1));
	}
}


void qsynthKnob::setKnobColor ( const QColor& color )
{
	m_knobColor = color;
	repaint();
}


void qsynthKnob::setMeterColor ( const QColor& color )
{
	m_meterColor = color;
	repaint();
}


void qsynthKnob::setMouseDial ( bool bMouseDial )
{
	m_bMouseDial = bMouseDial;
}


void qsynthKnob::setDefaultValue ( int iDefaultValue )
{
	m_iDefaultValue = iDefaultValue;
}


// Mouse angle determination.
double qsynthKnob::mouseAngle ( const QPoint& pos )
{
	int dx = pos.x() - (QDial::width() / 2);
	int dy = (QDial::height() / 2) - pos.y();
	return (dx ? atan((double) dy / (double) dx) : 0.0);
}


// Alternate mouse behavior event handlers.
void qsynthKnob::mousePressEvent ( QMouseEvent *pMouseEvent )
{
	if (m_bMouseDial) {
		QDial::mousePressEvent(pMouseEvent);
	} else if (pMouseEvent->button() == Qt::LeftButton) {
		m_bMousePressed = true;
		m_posMouse = pMouseEvent->pos();
		emit sliderPressed();
	} else if (pMouseEvent->button() == Qt::MidButton) {
		// Reset to default value...
		if (m_iDefaultValue < minimum() || m_iDefaultValue > maximum())
			m_iDefaultValue = (maximum() + minimum()) / 2;
		setValue(m_iDefaultValue);
	}
}


void qsynthKnob::mouseMoveEvent ( QMouseEvent *pMouseEvent )
{
	if (m_bMouseDial) {
		QDial::mouseMoveEvent(pMouseEvent);
	} else if (m_bMousePressed) {
		// Dragging by x or y axis when clicked modifies value.
		const QPoint& posMouse = pMouseEvent->pos();
		int iValue = value()
			+ (mouseAngle(m_posMouse) < mouseAngle(posMouse) ? -1 : +1)
			* singleStep();
		m_posMouse = posMouse;
		if (iValue > maximum())
			iValue = maximum();
		else
		if (iValue < minimum())
			iValue = minimum();
		setValue(iValue);
		emit sliderMoved(value());
	}
}

void qsynthKnob::mouseReleaseEvent ( QMouseEvent *pMouseEvent )
{
	if (m_bMouseDial) {
		QDial::mouseReleaseEvent(pMouseEvent);
	} else if (m_bMousePressed) {
		m_bMousePressed = false;
	}
}


void qsynthKnob::wheelEvent ( QWheelEvent *pWheelEvent )
{
	if (m_bMouseDial) {
		QDial::wheelEvent(pWheelEvent);
	} else {
		int iValue = value();
		if (pWheelEvent->delta() > 0)
			iValue -= pageStep();
		else
			iValue += pageStep();
		if (iValue > maximum())
			iValue = maximum();
		else
		if (iValue < minimum())
			iValue = minimum();
		setValue(iValue);
	}
}

void qsynthKnob::valueChange (void)
{
	update();
	emit valueChanged(value());
#if defined(QT_ACCESSIBILITY_SUPPORT)
	QAccessible::updateAccessibility(this, 0, QAccessible::ValueChanged);
#endif
}

QSize qsynthKnob::minimumSizeHint() const {
	int s = size().width();
	if ( s > size().height() )
		s = size().height();
	return QSize( s, s );
}

void qsynthKnob::resizeEvent( QResizeEvent* ) {
/*	if ( size().height() > size().width() )
		resize( size().width(), size().width() );
	else if ( size().width() > size().height() )
		resize( size().height(), size().height() );*/
}

// end of qsynthKnob.cpp
