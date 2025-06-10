#ifndef WRAPPER_H
#define WRAPPER_H
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif

    void *UnixStamp_create();
    void UnixStamp_destroy(void *instance);
    uint32_t UnixStamp_convertToUnix(void *instance, char *txt);
    char *UnixStamp_convertToDay(void *instance, uint32_t epoch);
    char *UnixStamp_convertToDate(void *instance, char *txt, uint32_t *hours);

    void *CalculateValue_create();
    void CalculateValue_destroy(void *instance);
    float CalculateValue_findValue(void *instance, uint16_t *ledBefore, uint16_t *ledAfter, uint8_t reagent, uint8_t drop, float *coef, float *inter, uint8_t *leds, uint8_t amount);
    bool CalculateValue_checkErr(void *instance, uint16_t *ledBefore, uint16_t *ledAfter, uint8_t type);

#ifdef __cplusplus
}
#endif

#endif