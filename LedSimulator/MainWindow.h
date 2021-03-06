#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "renderarea.h"
#include "ledLineEdit.h"
#include "../tlc5947_lib/RGB.h"
#include "../tlc5947_lib/tlc5947_controller.h"
#include <unistd.h>
#include <QProcess>
#include "MemoryMonitor.h"


#define LED_NUMBER 8        //only even numbers are allowed
#define LINE_EDITS_NUMBER 3


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    uint8_t txdata[LED_NUMBER/2*9];
    Ui::MainWindow *ui;
    RenderArea *renderArea;
    bool simulationOn;
    bool systemLinux;
    MemoryMonitor *mmThread;

    uint32_t diameter;
    QHBoxLayout *hBoxLayout;
    QWidget *window;
    QVBoxLayout *vBoxLayouts[LED_NUMBER];
    QHBoxLayout *hBoxLines[LED_NUMBER*LINE_EDITS_NUMBER];
    QPixmap *pixmaps[LED_NUMBER];
    QPainter *painters[LED_NUMBER];
    QLabel *labelsPixmaps[LED_NUMBER];
    QLabel *ledNames[LED_NUMBER];
    QLabel *hexPrefixes[LED_NUMBER*LINE_EDITS_NUMBER];
    QLabel *statusBarText;
    ledLineEdit *lineEdits[LED_NUMBER*LINE_EDITS_NUMBER];
    RGB leds[LED_NUMBER];

    void setLedColor(uint32_t ledNumber, uint8_t ledColorCode, uint8_t ledColor);
    void runLedScenario();


public slots:
    void ledColorChanged(QString text);
private slots:
    void on_actionColor_calibrator_triggered();
    void on_actionTLC5947_Simulator_triggered();
    void setMemoryLoadLedColors(int limit);
};

#endif // MAINWINDOW_H
