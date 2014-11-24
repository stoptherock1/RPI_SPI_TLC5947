#include "ledLineEdit.h"

uint32_t ledLineEdit::getLedNumber() const
{
    return ledData.ledNumber;
}

void ledLineEdit::setLedNumber(const uint32_t &value)
{
    ledData.ledNumber = value;
}

uint8_t ledLineEdit::getColorCode() const
{
    return ledData.ledColorCode;
}

void ledLineEdit::setColorCode(const uint8_t &value)
{
    ledData.ledColorCode = value;
}

ledLineEdit::ledLineEdit(uint32_t ledNumber_, uint8_t color_)
{
    ledData.ledNumber = ledNumber_;
    ledData.ledColorCode = color_;
}
