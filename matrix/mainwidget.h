#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <qwidget.h>

#include "mixingmatrix.h"

class MatrixElement;
class QGridLayout;

class MatrixWidget : public MixingMatrix::Widget {
Q_OBJECT
public:
	MatrixWidget( QWidget*, const char* =0 );
	~MatrixWidget();
public slots:
	void deleteSelf();

	void replace( MatrixElement* );
private:
	QGridLayout* _layout;
};

#endif // MAINWIDGET_H

