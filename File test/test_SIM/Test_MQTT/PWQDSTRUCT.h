#ifndef PWQDSTRUCT_h
#define PWQDSTRUCT_h
#include <Arduino.h>
#define  s_amountReagent  9
#define  s_amountPond  5
const char  s_listreagent[s_amountReagent][10]  = {"PH", "NH3", "NO2", "KH",  "MG", "CA", "DO", "ACIDITY", "SALINITY"};
typedef struct s_reagent {
  uint8_t active = 0;
  uint16_t* timeMea;
  uint32_t lastMea;
  uint16_t intervalMea;
} s_reagent;

typedef struct s_pond {
  uint8_t gw;
  uint8_t active = 0;
  char* pondID = NULL;
  s_reagent reagentPond[s_amountReagent];
} s_pond;


#endif
