
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>

#include <QtCore/QDebug>

#include "peak_tracker.h"
#include "peak_tracker.moc"

namespace JackMix {


const float PeakTracker::threshold[] = {0, 0.01, 0.25, 0.5};
        
PeakTracker::PeakTracker(QObject* parent)
{
//         connect( this, SIGNAL( inputLevelsChanged(JackMix::PeakTracker::levels_t) ),
//                  this, SLOT( testSlot(JackMix::PeakTracker::levels_t) ), Qt::DirectConnection );
}
        
PeakTracker::Level PeakTracker::signalToLevel(float sig)
{
        if (sig >= threshold[too_high]) return too_high;
        if (sig >= threshold[high])     return high;
        if (sig >= threshold[nominal])  return nominal;
        return none;
}

void PeakTracker::newLevel(Stats& s, float maxSignal)
{
        Level levelNow = signalToLevel(maxSignal);
        s.changed = (s.level != levelNow);
        s.level = levelNow;
}

void PeakTracker::newInputLevel(QString which, float maxSignal)
{
        //if (which == "in_1") qDebug() << maxSignal;
        newLevel(stats[0][which], maxSignal);
}

void PeakTracker::report_group(int which, levels_t& result)
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

void PeakTracker::report() {
        levels_t updated;
        
        // Report input levels
        report_group(0, updated);
        
        emit(inputLevelsChanged(updated));
}

void PeakTracker::testSlot(levels_t changed) {
        if (!changed.empty())
                qDebug() << "PeakTracker Test Slot " << changed;
}

}
