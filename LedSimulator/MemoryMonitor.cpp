#include "MemoryMonitor.h"


MemoryMonitor::MemoryMonitor(bool* simulationOn_, bool* systemLinux_, int ledNumber_)
{
    simulationOn = simulationOn_;
    systemLinux = systemLinux_;
    ledNumber = ledNumber_;
}

MemoryMonitor::~MemoryMonitor()
{

}

void MemoryMonitor::run()
{
    QProcess *process = new QProcess;

    while(*simulationOn && *systemLinux)
    {
        process->start("bash", QStringList() << "-c" << "cat /proc/meminfo | sed -n '1p;2p'");
        process->waitForBytesWritten();
        process->waitForFinished();
        QString output(process->readAll());

        int free = 0;
        int total = 0;

        QStringList outputList = output.split(" ", QString::SkipEmptyParts);
        if(outputList.size() < 4)
            break;

        total = outputList[1].toInt();
        free = outputList[3].toInt();

        int used = total - free;
        float load = 0;
        int chunk = 0;
        if(total != 0)
        {
            load = used / (total / 100);
            chunk = total / ledNumber;
        }

//        qDebug() << "Total:" << total;
//        qDebug() << "Free:" << free;
//        qDebug() << "Load:" << load;

        QString statusBarTextStr = QString("Mode: color calibration\t\t[Memory load: %1%]").arg(load);

        emit memoryDataUpdated(used/chunk);
        emit updateStatusBarText(statusBarTextStr);

        sleep(1);
    }
}

