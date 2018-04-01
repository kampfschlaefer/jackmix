/*
    Copyright 2018 Nick Bailey <nick@n-ism.org>

    Modified to permit control via MIDI by Nick Bailey <nick@n-ism.org>
    Released as version 0.5.0 (et seq.) copyright 2013.
    
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

#ifndef PEAK_TRACKER_H
#define PEAK_TRACKER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QTime>

namespace JackMix {
        
/*
 * Notify other objects of the signal level at selected nodes in the backend
 * 
 * Signal levels are categorised into bands labeled with the Level enumeration.
 * These are none (virutally no signal detected), nominal (normal working level),
 * high (signal level on the high side) and too_high (very close or at clipping).
 * 
 * The backend passes the maximum value of its input and output channels
 * on a per-buffer basis. This class filters the data from each channel
 * and issues a signal containing the state of the channels where this has changed.
 * 
 * Additionally, the too_high level is made to persist; it isn't cancelled
 * for a minumum period after the high signal level causing it is removed.
 * The intention is that the corresponding GUI action, such as illuminating a
 * "clipping" indicator, is obvious even if the high signal level is very brief.
 */
class PeakTracker : public QObject {
Q_OBJECT
public:
        enum Level {none, nominal, high, too_high};
        Q_ENUM(Level)
        typedef QMap<QString,Level> levels_t;

        PeakTracker(QObject* parent = 0);
        void newInputLevel(QString which, float maxSignal);
        void newOutputLevel(QString which, float maxSignal);
        Level signalToLevel(float sig);
        void report();
        
signals:
        void inputLevelsChanged(JackMix::PeakTracker::levels_t);
        void outputLevelsChanged(JackMix::PeakTracker::levels_t);
        void sendsize(int);

private:
        typedef struct {
                Level level;
                float max;
                bool  changed;
                QTime timeout;
        } Stats;

        QMap<QString, Stats> stats[2];

        void newLevel(Stats& s, float maxSignal); 
        void report_group(int which, levels_t& result);
        
        static const float threshold[];
        
private slots:
        void testSlot(JackMix::PeakTracker::levels_t);

};
        
}

Q_DECLARE_METATYPE(JackMix::PeakTracker::levels_t);

#endif
