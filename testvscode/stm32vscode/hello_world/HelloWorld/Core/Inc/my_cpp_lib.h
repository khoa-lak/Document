#ifndef MY_CPP_LIB_H
#define MY_CPP_LIB_H

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Giao diện C để gọi từ main.c
    void *MyCppClass_create();
    void MyCppClass_sayHello(void *obj);
    void MyCppClass_destroy(void *obj);

#ifdef __cplusplus
} // extern "C"

// Định nghĩa class chỉ hiển thị trong C++
class MyCppClass
{
public:
    MyCppClass();
    void sayHello();
};
#endif

#endif