#ifndef ENABLETIMETRACKER_H
#define ENABLETIMETRACKER_H

#include <QWindow>
#include <QByteArray>
#include <QFile>

class EnableTimeTracker
{
public:
    EnableTimeTracker();
    void log30SecsEnabled();
    double getHoursEnabled();

private:
    double m_minutesEnabled = 0.0;
};

#endif // ENABLETIMETRACKER_H
