#ifndef MATRIXELEMENTS_H
#define MATRIXELEMENTS_H

#include <qobject.h>
#include <qvaluelist.h>
#include <qpushbutton.h>
#include <qframe.h>

#include "mixingmatrix.h"

class MatrixElement;

class MatrixElement : public MixingMatrix::Element
{
Q_OBJECT
public:
	MatrixElement( int, int, MixingMatrix::Widget*, const char* =0 );
	~MatrixElement();

	int inchannels() const { return 1; }
	int outchannels() const { return 1; }
	QStringList inchannels_list() const { return QStringList(); }
	QStringList outchannels_list() const { return QStringList(); }

private slots:
	void toggleselect();
	void slot_replace();

protected:
	void mouseDoubleClickEvent( QMouseEvent* );

private:
	QPushButton *_btn_select, *_btn_replace;
};

class MultiElement : public QFrame
{
Q_OBJECT
public:
	MultiElement( int, int, QWidget*, const char* =0 );
	~MultiElement();

protected:
	void mouseDoubleClickEvent( QMouseEvent* );

private:
	int _width, _height;
};

#endif // MATRIXELEMENTS_H

