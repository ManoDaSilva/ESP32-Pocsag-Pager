/*
User-modifiable configuration

In the near future, bring this to a menu, and load-edit them in the EEPROM

*/

//Default settings

extern float offset = 0.0044;  // device specific, in MHz. VHF: 0.0014 UHF: 0.0044
extern float frequency = 439.98750;

#define RICNUMBER 8 //Maximum number of RIC usable

struct ric{
  long ricvalue; //RIC adress itself
  char* name; //"Nickname"
  int ringtype; //TBD: ring "melody"
  bool placeholder1;
  bool placeholder2;
}

ric[RICNUMBER]={
  {65009, "IND",0,0,0},
  {1040, "EMERGENCY",0,0,0},
  {1080, "APRSWX",0,0,0},
};


