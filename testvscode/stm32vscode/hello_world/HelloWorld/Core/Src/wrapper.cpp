#include "wrapper.h"
#include "unix_time.h"
#include "calculate_value.h"

void *UnixStamp_create()
{
    return new UnixStamp();
}

void UnixStamp_destroy(void *instance)
{
    UnixStamp *unixStamp = static_cast<UnixStamp *>(instance);
    delete unixStamp;
}

uint32_t UnixStamp_convertToUnix(void *instance, char *txt)
{
    UnixStamp *unixStamp = static_cast<UnixStamp *>(instance);
    return unixStamp->convertToUnix(txt);
}

char *UnixStamp_convertToDay(void *instance, uint32_t epoch)
{
    UnixStamp *unixStamp = static_cast<UnixStamp *>(instance);
    return unixStamp->convertToDay(epoch);
}

char *UnixStamp_convertToDate(void *instance, char *txt, uint32_t *hours)
{
    UnixStamp *unixStamp = static_cast<UnixStamp *>(instance);
    return unixStamp->convertToDate(txt, *hours);
}

void *CalculateValue_create()
{
    return new CalculateValue();
}

void CalculateValue_destroy(void *instance)
{
    CalculateValue *calcValue = static_cast<CalculateValue *>(instance);
    delete calcValue;
}

float CalculateValue_findValue(void *instance, uint16_t *ledBefore, uint16_t *ledAfter, uint8_t reagent, uint8_t drop, float *coef, float *inter, uint8_t *leds, uint8_t amount)
{
    CalculateValue *calcValue = static_cast<CalculateValue *>(instance);
    return calcValue->findValue(ledBefore, ledAfter, reagent, drop, coef, *inter, leds, amount);
}

bool CalculateValue_checkErr(void *instance, uint16_t *ledBefore, uint16_t *ledAfter, uint8_t type)
{
    CalculateValue *calcValue = static_cast<CalculateValue *>(instance);
    return calcValue->checkErr(ledBefore, ledAfter, type);
}