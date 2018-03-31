
#include <QtCore/QObject>

#include "peak_tracker.h"

namespace JackMix {


const float PeakTracker::threshold[] = {0, 0.01, 0.33, 0.5};
        
PeakTracker::PeakTracker(QObject* parent)
{
}
        
PeakTracker::Level PeakTracker::level() const
{
        return PeakTracker::none;
}

void PeakTracker::new_current_level(float max_signal)
{
        if (current_level == too_high)
                current_max = max_signal;
        else {
                if (max_signal > threshold[too_high])
                        current_level = too_high;
                emit (levelChanged(current_level));
        }
}


}
