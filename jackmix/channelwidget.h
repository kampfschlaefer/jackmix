
#ifndef JACK_CHANNELWIDGET_H
#define JACK_CHANNELWIDGET_H

#include <qwidget.h>

namespace JackMix {

class ChannelWidget : public QWidget {
Q_OBJECT
public:
	ChannelWidget( QWidget* =0, const char* =0 );
	~ChannelWidget();
};

};

#endif

