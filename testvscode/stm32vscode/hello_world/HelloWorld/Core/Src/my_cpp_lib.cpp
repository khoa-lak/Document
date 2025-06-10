#include "my_cpp_lib.h"
#include "usart.h"
MyCppClass::MyCppClass()
{
    // Constructor
}

void MyCppClass::sayHello()
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Hello from C++\n", 15, 1000);
}

// Triển khai giao diện C
extern "C"
{
    void *MyCppClass_create()
    {
        return new MyCppClass();
    }

    void MyCppClass_sayHello(void *obj)
    {
        static_cast<MyCppClass *>(obj)->sayHello();
    }

    void MyCppClass_destroy(void *obj)
    {
        delete static_cast<MyCppClass *>(obj);
    }
}