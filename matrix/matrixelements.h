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

