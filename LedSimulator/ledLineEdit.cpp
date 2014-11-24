#include "ledLineEdit.h"

uint32_t ledLineEdit::getNumber() const
{
    return number;
}

void ledLineEdit::setNumber(const uint32_t &value)
{
    number = value;
}

uint8_t ledLineEdit::getColorCode() const
{
    return colorCode;
}

void ledLineEdit::setColorCode(const uint8_t &value)
{
    colorCode = value;
}

ledLineEdit::ledLineEdit(uint32_t ledNumber_, uint8_t colorCode_)
{
    number = ledNumber_;
    colorCode = colorCode_;
}
