#ifndef LEDLINEEDIT_H
#define LEDLINEEDIT_H

#include <QLineEdit>
#include <QSignalMapper>

struct LedData : public QObject
{
    uint32_t ledNumber;
    uint8_t ledColorCode;
};

class ledLineEdit : public QLineEdit
{
private:

    QSignalMapper *signalMapper;
    LedData ledData;

public:
    ledLineEdit(uint32_t ledNumber, uint8_t color);

    uint32_t getLedNumber() const;
    void setLedNumber(const uint32_t &value);
    uint8_t getColorCode() const;
    void setColorCode(const uint8_t &value);
};

#endif // LEDLINEEDIT_H
