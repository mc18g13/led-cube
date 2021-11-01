#include "LEDCubeInterface.h"
#include "SPI.h"

#define LAYER_ONE   0b00000001
#define LAYER_TWO   0b00000010
#define LAYER_THREE 0b00000100
#define LAYER_FOUR  0b00001000
#define LAYER_FIVE  0b00010000
#define LAYER_SIX   0b00100000

#define ONE_MHZ 1000000
#define SPI_CLOCK 2000000


namespace {
  const int latchPin = D3;
  const int clockPin = D5;
  const int dataPin = D7;
  const uint8_t TOTAL_SHIFT_REGISTER_COUNT = 6;
  const uint8_t COLUMN_SHIFT_REGISTER_COUNT = TOTAL_SHIFT_REGISTER_COUNT - 1;
}

LEDCubeInterface::LEDCubeInterface() : 
 m_layerByte(0), 
 m_columnData(new uint8_t[COLUMN_SHIFT_REGISTER_COUNT]) {

}

LEDCubeInterface::~LEDCubeInterface() {
  delete m_columnData;
}

void LEDCubeInterface::setup() {
  pinMode(latchPin, OUTPUT);
  // pinMode(dataPin, OUTPUT);  
  // pinMode(clockPin, OUTPUT);

  digitalWrite(latchPin, HIGH);
  SPI.begin();
}

void LEDCubeInterface::setActiveLayerAndColumnIndices(uint8_t layerIndex, uint8_t columnIndex) {
  setBitForColumnInLayer(columnIndex);
  m_layerByte = layerIndexToLayerControlByte(layerIndex);
}

void LEDCubeInterface::sendData() {

  SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));
  digitalWrite(latchPin, LOW);

  SPI.transfer(m_layerByte);
  SPI.transfer(m_columnData[0]);
  SPI.transfer(m_columnData[1]);
  SPI.transfer(m_columnData[2]);
  SPI.transfer(m_columnData[3]);
  SPI.transfer(m_columnData[4]);


  digitalWrite(latchPin, HIGH);
  SPI.endTransaction();
}

void LEDCubeInterface::printColumnData() {
  printByteArray(m_columnData, COLUMN_SHIFT_REGISTER_COUNT);
}

uint8_t LEDCubeInterface::layerIndexToLayerControlByte(int layerIndex) {
  if (layerIndex == 1) {
    return LAYER_ONE;
  } else if (layerIndex == 2) {
    return LAYER_TWO;
  } else if (layerIndex == 3) {
    return LAYER_THREE;
  } else if (layerIndex == 4) {
    return LAYER_FOUR;
  } else if (layerIndex == 5) {
    return LAYER_FIVE;
  } else if (layerIndex == 6) {
    return LAYER_SIX;
  }

  return 0;
}

void LEDCubeInterface::writeByte(uint8_t byteToSend) {
  shiftOut(dataPin, clockPin, MSBFIRST, byteToSend);
}

void LEDCubeInterface::writeBytes(uint8_t* bytesToSend, uint8_t size) {
  for (int i = size - 1; i >= 0; --i) {
    uint8_t data = bytesToSend[i];
    shiftOut(dataPin, clockPin, MSBFIRST, data);
  }
}

void LEDCubeInterface::setBitForColumnInLayer(int index) {
  for (int currentRegisterIndex = 0; currentRegisterIndex < COLUMN_SHIFT_REGISTER_COUNT; ++currentRegisterIndex) {
    int minForThisRegister = (currentRegisterIndex * 8) + 1;
    int maxForThisRegister = (currentRegisterIndex + 1) * 8;
    bool currentBitIsInThisRegister = index >= minForThisRegister && index <= maxForThisRegister;
    if (currentBitIsInThisRegister) {
      m_columnData[COLUMN_SHIFT_REGISTER_COUNT - 1 - currentRegisterIndex] = (1 << (index - minForThisRegister));
    } else {
      m_columnData[COLUMN_SHIFT_REGISTER_COUNT - 1 - currentRegisterIndex] = 0;
    }
  }
}

void LEDCubeInterface::printByteArray(uint8_t* bytesToPrint, uint8_t size) {
  for (int byteIndex = 0; byteIndex < size; ++byteIndex) {
    uint8_t byteToPrint = bytesToPrint[byteIndex];
    Serial.print(byteToPrint, BIN); Serial.print(" "); 
  }
  Serial.println();
}