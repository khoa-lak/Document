# 1 "c:\\Users\\KhoaAnh\\Desktop\\github\\Document\\testvscode\\arduinovscode\\tesst.ino"
void setup()
{
 SerialUSB.begin(9600);
}

void loop()
{
 SerialUSB.println("hello");
    delay(500);
}
