#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H
#include <QThread>
#include <QProcess>
#include <QDebug>



class MemoryMonitor: public QThread
{
    Q_OBJECT

private:
    bool* simulationOn;
    bool* systemLinux;
    int ledNumber;

public:
    MemoryMonitor(bool* simulationOn_, bool* systemLinux_, int ledNumber_);
    ~MemoryMonitor();

    void run();

signals:
    void memoryDataUpdated(int);
    void updateStatusBarText(QString);

};

#endif // MEMORYMONITOR_H
