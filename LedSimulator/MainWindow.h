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
#include "renderarea.h"
#include "ledLineEdit.h"
#include "RGB.h"

#define LED_NUMBER 8
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

protected:
//    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    RenderArea *renderArea;

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
    ledLineEdit *lineEdits[LED_NUMBER*LINE_EDITS_NUMBER];
    RGB leds[LED_NUMBER];

public slots:
    void setLedColor(QString text);
};


#endif // MAINWINDOW_H
