#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LedSimulator");

    mmThread = new MemoryMonitor(&simulationOn, &systemLinux, LED_NUMBER);

    diameter = 100;

    window = new QWidget();
    hBoxLayout = new QHBoxLayout();
    window->setLayout(hBoxLayout);
    int border = 1;
    statusBarText = new QLabel("Mode: color calibration");
    this->statusBar()->addWidget(statusBarText);

    int lineEditsCounter = 0;
    for(int i=0; i<LED_NUMBER; ++i)
    {
        vBoxLayouts[i] = new QVBoxLayout();

        //ellipses
        pixmaps[i] = new QPixmap(diameter+border, diameter+border);
        pixmaps[i]->fill(Qt::transparent);

        painters[i] = new QPainter(pixmaps[i]);
//        QBrush brush(Qt::transparent);
        QBrush brush(Qt::black);

        painters[i]->setBrush(brush);
        painters[i]->drawEllipse(0, 0, diameter, diameter);

        labelsPixmaps[i] = new QLabel();
        ledNames[i] = new QLabel("LED " + QString::number(i));
        ledNames[i]->setAlignment(Qt::AlignCenter);

        labelsPixmaps[i]->setPixmap(*pixmaps[i]);
        vBoxLayouts[i]->addWidget( ledNames[i] );
        vBoxLayouts[i]->addWidget( labelsPixmaps[i] );

        QRegExp reNumbers("^([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$");


        for(int j=0; j<LINE_EDITS_NUMBER; ++j)
        {
            hBoxLines[lineEditsCounter] = new QHBoxLayout();

            hexPrefixes[lineEditsCounter] = new QLabel("0x");
            hexPrefixes[lineEditsCounter]->setAlignment(Qt::AlignRight);

            lineEdits[lineEditsCounter] = new ledLineEdit(i, j);
//            hBoxLines[lineEditsCounter]->addWidget( hexPrefixes[lineEditsCounter] );
            hBoxLines[lineEditsCounter]->addWidget( lineEdits[lineEditsCounter] );
            vBoxLayouts[i]->addLayout( hBoxLines[lineEditsCounter] );

            if(0==j)
                lineEdits[lineEditsCounter]->setPlaceholderText("red");
            else if(1==j)
                lineEdits[lineEditsCounter]->setPlaceholderText("green");
            else
                lineEdits[lineEditsCounter]->setPlaceholderText("blue");

            lineEdits[lineEditsCounter]->setValidator( new QRegExpValidator(reNumbers) );

            connect(lineEdits[lineEditsCounter], SIGNAL(textChanged(QString)),
                    this, SLOT(ledColorChanged(QString)) );
            ++lineEditsCounter;
        }

        memset(leds, 0, sizeof(leds));

        hBoxLayout->addLayout( vBoxLayouts[i] );
    }

    simulationOn = false;


    char bash_cmd[256] = "uname";
    FILE *pipe;

    pipe = popen(bash_cmd, "r");
    char system[10];

    if (NULL != pipe)
    {
        fscanf(pipe, "%s", &system);
        pclose(pipe);

        if(strcmp("Linux", system) == 0)
        {
            systemLinux = true;
            qDebug() << "Linux";
        }
    }
    else
        systemLinux = false;

    connect(mmThread, SIGNAL(updateStatusBarText(QString) ), statusBarText, SLOT(setText(QString)) );
    connect(mmThread, SIGNAL(memoryDataUpdated(int)) , this, SLOT(setMemoryLoadLedColors(int)) );


    this->setCentralWidget(window);


    //move current window to the center of the screen
    this->move( QApplication::desktop()->screen()->rect().center() - window->rect().center());
}


void MainWindow::ledColorChanged(QString text)
{
    ledLineEdit *lineEdit = (ledLineEdit *) QObject::sender();
    uint32_t ledNumber = lineEdit->getNumber();
    uint8_t colorCode = lineEdit->getColorCode();
    uint8_t color255 = text.toInt();
    setLedColor(ledNumber, colorCode, color255);
}

void MainWindow::setLedColor(uint32_t ledNumber, uint8_t colorCode, uint8_t color255)
{
    if(0==colorCode)
        leds[ledNumber].red = color255;
    else if(1==colorCode)
        leds[ledNumber].green = color255;
    else
        leds[ledNumber].blue = color255;

    QColor color(leds[ledNumber].red,
                 leds[ledNumber].green,
                 leds[ledNumber].blue);


    QBrush brush(color);
    painters[ledNumber]->setBrush(brush);
    painters[ledNumber]->drawEllipse(0, 0, diameter, diameter);
    labelsPixmaps[ledNumber]->setPixmap(*pixmaps[ledNumber]);
}

MainWindow::~MainWindow()
{
    int lineEditsCounter = 0;
    for(int i=0; i<LED_NUMBER; ++i)
    {
        for(int j=0; j<LINE_EDITS_NUMBER; ++j)
        {
            delete lineEdits[lineEditsCounter];
            delete hexPrefixes[lineEditsCounter];
            delete hBoxLines[lineEditsCounter];
            ++lineEditsCounter;
        }

        delete labelsPixmaps[i];
        delete painters[i];
        delete pixmaps[i];
        delete vBoxLayouts[i];
    }
    delete hBoxLayout;
    delete ui;
}


void MainWindow::runLedScenario()
{
    mmThread->start();

    QString statusBarTextStr = QString("Error! This function is available anly on Linux system");
    statusBarText->setText(statusBarTextStr);
}


void MainWindow::setMemoryLoadLedColors(int limit)
{
    for(int i=0; i<LED_NUMBER; ++i)
    {
        setLedColor(i, 0, 0);
        setLedColor(i, 1, 0);
        setLedColor(i, 2, 0);
    }

    int chunk = LED_NUMBER / 3;

    if(LED_NUMBER%2 == 0)
        ++chunk;

    if(limit==0)
        ++limit;

    for(int i=0; i<chunk && i<limit; ++i)
    {
        setLedColor(i, 0, 140);
        setLedColor(i, 1, 255);
        setLedColor(i, 2, 70);
    }

    for(int i=chunk; i<chunk*2 && i<limit; ++i)
    {
        setLedColor(i, 0, 255);
        setLedColor(i, 1, 255);
        setLedColor(i, 2, 70);
    }

    for(int i=chunk*2; i<LED_NUMBER && i<limit; ++i)
    {
        setLedColor(i, 0, 255);
        setLedColor(i, 1, 50);
        setLedColor(i, 2, 50);
    }
}

void MainWindow::on_actionColor_calibrator_triggered()
{
    if(simulationOn)
    {
        for(int i=0; i<LED_NUMBER*LINE_EDITS_NUMBER; ++i)
        {
            lineEdits[i]->setReadOnly(false);
            lineEdits[i]->clear();
        }

        for(int i=0; i<LED_NUMBER; ++i)
        {
            setLedColor(i, 0, 0);
            setLedColor(i, 1, 0);
            setLedColor(i, 2, 0);
        }

        simulationOn = false;

        statusBarText->setText("Mode: color calibration");
    }
}

void MainWindow::on_actionTLC5947_Simulator_triggered()
{
    for(int i=0; i<LED_NUMBER*LINE_EDITS_NUMBER; ++i)
    {
        lineEdits[i]->setReadOnly(true);
        lineEdits[i]->clear();
    }

    simulationOn = true;

    statusBarText->setText("Mode: tlc5947 simulation");
    runLedScenario();
}
