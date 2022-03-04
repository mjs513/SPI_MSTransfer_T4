#if !defined(_SPI_MSTransfer_MASTER_H_)
#define _SPI_MSTransfer_MASTER_H_

#include "Arduino.h"
#include "circular_buffer.h"
#include <functional>
#include <SPI.h>

#define SPI_MST_QUEUE_SLOTS 8
#define SPI_MST_DATA_BUFFER_MAX 250

struct AsyncMST {
  uint16_t packetID = 0;
  uint16_t slaveID = 0;
};

typedef void (*_slave_handler_ptr)(uint16_t* buffer, uint16_t length, AsyncMST info);
typedef std::function<void(AsyncMST info)> _detectPtr;

typedef void (*_SPI_ptr)();

#define SPI_MSTransfer_MASTER_CLASS template<SPIClass* port, uint8_t cs_pin, uint32_t slave_ID>
#define SPI_MSTransfer_MASTER_FUNC template<SPIClass* port, uint8_t cs_pin, uint32_t slave_ID>
#define SPI_MSTransfer_MASTER_OPT SPI_MSTransfer_MASTER<port, cs_pin, slave_ID>

extern SPIClass SPI;

class SPI_MSTransfer_MASTER_Base {
  public:

};

Circular_Buffer<uint16_t, (uint32_t)pow(2, ceil(log(SPI_MST_QUEUE_SLOTS) / log(2))), SPI_MST_DATA_BUFFER_MAX> mstqueue;

SPI_MSTransfer_MASTER_CLASS class SPI_MSTransfer_MASTER : public SPI_MSTransfer_MASTER_Base {
  public:
    SPI_MSTransfer_MASTER();
    void begin();
    void onTransfer(_slave_handler_ptr handler) { _slave_handler = handler; }
    uint16_t transfer16(uint16_t *buffer, uint16_t length, uint16_t packetID);
    void detectSlaves();
    uint32_t events();
    void pinMode(uint8_t pin, uint8_t state);
    void digitalWrite(uint8_t pin, bool state);
    void digitalWriteFast(uint8_t pin, bool state) { return digitalWrite(pin, state); }
    int digitalRead(uint8_t pin);
    int digitalReadFast(uint8_t pin) { return digitalRead(pin); }

  private:
    void spi_assert();
    void spi_deassert();
    int _portnum = 0;
    uint32_t nvic_irq = 0;
    _slave_handler_ptr _slave_handler;
};

#include "SPI_MSTransfer_MASTER.tpp"
#endif