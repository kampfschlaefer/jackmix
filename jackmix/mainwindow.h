/* Copyright */

#ifndef JACKMIX_MAINWINDOW_H
#define JACKMIX_MAINWINDOW_H

#include <qmainwindow.h>

namespace JackMix {

class MainWindow : public QMainWindow {
Q_OBJECT
public:
	MainWindow( QWidget* =0, const char* =0);
	~MainWindow();
};

};

#endif
