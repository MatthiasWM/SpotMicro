
#include <Arduino.h>


#if 0
#include "HardwareSerial.h"
#endif


void setupScreen()
{

}

#if 0
void swrite(const char *txt)
{
  int n = strlen(txt)+2;
  //while (Serial.availableForWrite()<n) { delay(1); }
  delay(n*8);
  Serial1.write(txt);
  Serial1.write("\n");
}

void ComposeTrimScreen()
{
  swrite("*.kwl");
  swrite("clear_panel();");
  swrite("set_grid_size(16, 8);");
  swrite("set_panel_notes(\"Robowerk Spot\", \"Trim\", , );");

  /*
  // -- go back to the main panel
  Serial1.write("add_button(14, 0, 4, \"back;\", );\n");
  // -- reload this panel
  Serial1.write("add_button(15, 0, 7, \"reload;\", );\n");
  */
  // -- motor power for all motors
  swrite("add_switch(14, 6, 4, \"pwr1;\", \"pwr0;\", 0, 0);");

/*
  // -- motors leg front right
  Serial1.write("add_button(0, 5,  7, \"fr0a;\", );\n");
  Serial1.write("add_button(0, 6, 12, \"fr1a;\", );\n");
  Serial1.write("add_button(0, 7, 12, \"fr2a;\", );\n");
  delay(100);
  */

  swrite("run();\n");
  swrite("*\n");
}

#endif
