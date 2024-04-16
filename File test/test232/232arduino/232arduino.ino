

String chuoinhan = "";         
bool chuoiok = false;  

void setup() {

  Serial.begin(9600);

  chuoinhan.reserve(200);
}

void loop() {

  if (chuoiok) {
    Serial.println(chuoinhan);

    chuoinhan = "";
    chuoiok = false;
  }
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      chuoinhan += inChar;
    }   
    else {
      chuoiok = true;
    }
  }
}
