#ifndef LEDLINEEDIT_H
#define LEDLINEEDIT_H

#include <QLineEdit>
#include <stdint.h>

class ledLineEdit : public QLineEdit
{
private:
    uint32_t number;
    uint8_t colorCode;

public:
    ledLineEdit(uint32_t number, uint8_t colorCode);

    uint32_t getNumber() const;
    void setNumber(const uint32_t &value);
    uint8_t getColorCode() const;
    void setColorCode(const uint8_t &value);
};

#endif // LEDLINEEDIT_H
