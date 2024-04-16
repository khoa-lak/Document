const uint8_t SC_NameColor[64][20]   = {"Black", "Navy", "MediumBlue", "Blue", "Green", "Teal", "BondiBlue", "DodgerBlue", "IslamicGreen",
                                    "CaribbeanGreen", "RobinEggBlue", "DodgerBlue", "Lime", "SpringGreen", "MSpringGreen", "Aqua",
                                    "Maroon", "Purple", "DarkViolet", "ElectricIndigo", "Olive", "Grey", "WildBule", "LightSlateBlue",
                                    "Citrus", "DeYork", "Glacier", "MayaBlue", "Chartreuse", "MintGreenL", "Aquamarine", "ElectricBule",
                                    "FreeSpeechRed", "Eggplant", "DeepMagenta", "ElectricPurple", "DarkGoldenrod", "OrientalPink", "Lilac", "Heliotrope",
                                    "LaRioja", "PineGlade", "Silver", "LavenderBlue", "ElectricLime", "MintGreenH", "MintGreenH2", "LightCyan",
                                    "Red", "DeepPink", "HotMagenta", "Magenta", "DarkOrange", "MonaLisa", "NeonPink", "FuchsiaPink", "Amber",
                                    "Macaroni&Cheese", "YourPink", "CottonCandy", "Yellow", "Canary", "LemonChiffon", "White"
                                   };

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint8_t n[15];
  abc(n);
  Serial.println("111111111111");
  Serial.println((char*)n);
  delay(500);
}

void abc(uint8_t p[]) {
  qbz(p);
}

void qbz(uint8_t p[]) {
//  //String j = "1111111111111";
//  uint8_t
//  String j = (char*)SC_NameColor[1];
//  j.toCharArray(SC_NameColor[1], 15);
//  Serial.println(j);
//  uint8_t length1 = j.length();
//  uint8_t k[15];
//  j.toCharArray(k, length1 + 1);
  for (uint8_t i = 0 ; i < 15 ; i++) {
    p[i] = SC_NameColor[8][i];
  }
  
}
