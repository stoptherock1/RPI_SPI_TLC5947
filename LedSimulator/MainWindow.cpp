#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LedSimulator");

    diameter = 100;

    window = new QWidget();

    hBoxLayout = new QHBoxLayout();
    window->setLayout(hBoxLayout);
    int border = 1;

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
                    this, SLOT(setLedColor(QString)) );
            ++lineEditsCounter;
        }

        memset(leds, 0, sizeof(leds));

        hBoxLayout->addLayout( vBoxLayouts[i] );
    }


    this->setCentralWidget(window);

    //move current window to the center of the screen
    this->move( QApplication::desktop()->screen()->rect().center() - window->rect().center());
}

void MainWindow::setLedColor(QString text)
{
    ledLineEdit *lineEdit = (ledLineEdit *) QObject::sender();
    uint32_t ledNumber = lineEdit->getNumber();
    uint8_t ledColorCode = lineEdit->getColorCode();
    uint8_t ledColor = text.toInt();

    if(0==ledColorCode)
        leds[ledNumber].red = ledColor;
    else if(1==ledColorCode)
        leds[ledNumber].green = ledColor;
    else
        leds[ledNumber].blue = ledColor;

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
