#include <Arduino.h>

class LEDCubeInterface {
private:
  uint8_t m_layerByte;
  uint8_t* m_columnData;
  uint8_t* m_allData;

public:
  LEDCubeInterface();
  ~LEDCubeInterface();
  void setup();
  void setActiveLayerAndColumnIndices(uint8_t layerIndex, uint8_t columnIndex);
  void sendData();
  void printColumnData();

private:
  uint8_t layerIndexToLayerControlByte(int layerIndex);
  void writeByte(uint8_t byteToSend);
  void writeBytes(uint8_t* bytesToSend, uint8_t size);
  void setBitForColumnInLayer(int index);
  void printByteArray(uint8_t* bytesToPrint, uint8_t size);

};