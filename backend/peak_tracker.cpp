
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
//         connect( this, SIGNAL( outputLevelsChanged(JackMix::PeakTracker::levels_t) ),
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
        if (levelNow == too_high) s.timeout.start();
        if (s.level == too_high && s.timeout.elapsed() < 1000) 
                s.changed = (s.level != too_high);
        else {
                s.changed = (s.level != levelNow);
                s.level = levelNow;
        }
}

void PeakTracker::newInputLevel(QString which, float maxSignal)
{
        newLevel(stats[0][which], maxSignal);
}

void PeakTracker::newOutputLevel(QString which, float maxSignal)
{
        newLevel(stats[1][which], maxSignal);
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

void PeakTracker::testSlot(levels_t changed) {
        if (!changed.empty())
                qDebug() << "PeakTracker Test Slot " << changed;
}

}
