#include <Arduino.h>
#include "LEDCubeInterface.h"

LEDCubeInterface cube;
static const uint8_t COLUMN_COUNT = 36; 

void setup() {
  
  cube.setup();
  // Serial.begin(9600);

  delay(3000);
}

int j = 1;
int row = 0;
int iteration = 0;
int test = 1;
int increment = 1;
void loop() {

  cube.setActiveLayerAndColumnIndices(row, j);
  cube.sendData();
  delay(100);


  ++j;
  if (j == 37) {
    ++row;
    if (row == 7) 
    {
      row = 1;
    }
    j = 1;
  }

  // ++j;
  // if (j == 37) {
  //   // ++row;
  //   ++iteration;
  //   if (iteration == test) 
  //   {
  //     ++row;
  //     iteration = 0;
  //     if (row == 7) 
  //     {
  //       row = 1;
  //       if (test > 7) increment = -1;
  //       if (test < 2) increment = 1;

  //       test += increment;
  //     }
  //   }
    
  //   j = 1;
  // }
}