/***************************************************************************
                          kpoti.h  -  Potentiometer Widget
                             -------------------
    begin                : Wed Apr 28 23:05:05 MEST 1999

    copyright            : (C) 1999 by Martin Lorenz
    email                : lorenz@ch.tum.de
                           (C) 2002 Matthias Kretz <kretz@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _QPOTI_H
#define _QPOTI_H

#include <QtGui/QFrame>
#include <Qt3Support/q3rangecontrol.h>


class QTimer;
struct QPotiData;


class QPoti : public QFrame, public Q3RangeControl
{
    Q_OBJECT
public:

    QPoti( QWidget *parent=0, const char *name=0 );
    QPoti( int minValue, int maxValue, int step, int value,
	     QWidget *parent=0, const char *name=0 );

    ~QPoti();

    void	setTracking( bool enable );
    bool	tracking() const;

    void 	setColor( const QColor & );

    virtual void setTickmarks( bool );
    virtual void setLabel( bool );
    bool        tickmarks() const { return ticks; }

    virtual void setTickInterval( int );
    int 	tickInterval() const { return tickInt; }

    virtual QSizePolicy sizePolicy() const;
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    QString text() const;

public slots:
    void	setValue( int );
    void	addStep();
    void	subtractStep();
    void	setText( const QString & );

signals:
    void	valueChanged( int value );
    void	potiPressed();
    void	potiMoved( int value );
    void	potiReleased();
    void  mouseEntered(int value);

protected:
    void	resizeEvent( QResizeEvent * );
    void	drawContents( QPainter * );

    void	keyPressEvent( QKeyEvent * );

    void	mousePressEvent( QMouseEvent * );
    void	mouseReleaseEvent( QMouseEvent * );
    void	mouseMoveEvent( QMouseEvent * );
    void  enterEvent( QEvent *);

    void	focusInEvent( QFocusEvent *e );
    void	focusOutEvent( QFocusEvent *e );

    void	valueChange();
    void	rangeChange();

    virtual void paletteChange( const QPalette & );

    virtual void paintPoti( QPainter * );
    void	drawButton( QPainter *);
    void	drawTicks( QPainter *, double, double, int=1 ) const;

    virtual void wheelEvent(QWheelEvent *e);
private slots:
    void	repeatTimeout();

private:
    enum State { Idle, Dragging, TimingUp, TimingDown };

    void	init(int value=0);
    float      	positionFromValue( int ) const;
    int		valueFromPosition( float ) const;
    void	movePoti( float );
    void	reallyMovePoti( float );
    void	resetState();
    int		potiRadius() const;
    void	initTicks();

    QTimer	*timer;
    float	potiPos;
    int		potiVal;
    int 	clickOffset;
    State	state;
    bool	track;
    bool	ticks, m_bLabel;
    int		tickInt, space;
    double buttonRadius;
private:
    struct QPotiPrivate;
    QPotiPrivate * d;
};

inline bool QPoti::tracking() const
{
    return track;
}


#endif // _KPOTI_H



