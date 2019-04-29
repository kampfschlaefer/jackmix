/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

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

#include <QtCore/QDebug>

#include "backend_interface.h"
#include "backend_interface.moc"

namespace JackMix {

BackendInterface::BackendInterface( GuiServer_Interface* g ) : gui( g ) {
// Uncomment to test signal-change detection
// 	connect( this, SIGNAL( outputLevelsChanged(JackMix::BackendInterface::levels_t) ),
// 	         this, SLOT( testSlot(JackMix::BackendInterface::levels_t) ) );
}

BackendInterface::~BackendInterface() {
}

// Thresholds are at -50dB, -18dB and -6dB
const float BackendInterface::threshold[] = {0, 0.00316, 0.12589, 0.50119};

BackendInterface::Level BackendInterface::signalToLevel(float sig) const
{
        if (sig >= threshold[too_high]) return too_high;
        if (sig >= threshold[high])     return high;
        if (sig >= threshold[nominal])  return nominal;
        return none;
}

void BackendInterface::newLevel(Stats& s, float maxSignal)
{
        Level levelNow = signalToLevel(maxSignal);
        if (levelNow == too_high) s.timeout.start();
        if (s.level == too_high && s.timeout.elapsed() < 1000) 
                s.changed = (s.level != too_high);
        else {
                s.changed = (s.level != levelNow);
                s.level = levelNow;
        }
}

void BackendInterface::newInputLevel(QString which, float maxSignal)
{
        newLevel(stats[0][which], maxSignal);
}

void BackendInterface::newOutputLevel(QString which, float maxSignal)
{
        newLevel(stats[1][which], maxSignal);
}

void BackendInterface::set_interp_len(float sr) {
	interp_len = 1 + sr*interp_time;
}


void BackendInterface::report_group(int which, levels_t& result)
{
        QMap<QString, Stats>::const_iterator i = stats[which].constBegin();
        while (i != stats[which].constEnd()) {
                const Stats* v = &i.value();
                if (v->changed) {
                        result[i.key()] = v->level;
                }
                ++i;
        }
}

void BackendInterface::report() {
        levels_t updated;
        
        // Report input levels
        report_group(0, updated);
        if (!updated.empty()) {
                emit(inputLevelsChanged(updated));
        }
        
        updated.clear();
        
        // Report output levels
        report_group(1, updated);
        if (!updated.empty()) {
                emit(outputLevelsChanged(updated));
        }

}

void BackendInterface::testSlot(levels_t changed) {
        if (!changed.empty())
                qDebug() << "BackendInterface Audo level changed testSlot " << changed;
}


BackendInterface::FaderState::FaderState(float initial, BackendInterface* parent)
	: target{initial}, current{initial}, cur_step{0}, p{parent}
	{ }


BackendInterface::FaderState& BackendInterface::FaderState::operator=(float volume) {

	if (!qFuzzyCompare(current, target)) {
		// Time to change current in case we're not finished interpolating
		current += cur_step*(target - current)/num_steps;
	}

	target = volume;
	cur_step = 0;
	num_steps = p->interp_len;

	return *this;
}

};


