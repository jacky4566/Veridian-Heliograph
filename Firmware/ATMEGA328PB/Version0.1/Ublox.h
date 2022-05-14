#ifndef Ublox_h
#define Ublox_h

#include "Arduino.h"
#include "UbloxData.h"
#include <Time.h>

class Ublox
{
  public:
    enum FixType
    {
      FIX_NONE = 0,
      FIX_DEAD_RECKONING_ONLY = 1,
      FIX_2D = 2,
      FIX_3D = 3,
      FIX_GNSS_DEAD_RECKONING = 4,
      FIX_TIME_ONLY = 5
    };

    explicit Ublox(HardwareSerial* bus);
    void Begin(uint32_t baud);
    bool Parse();
    uint32_t tow_ms();
    time_t UTC();
    uint16_t year();
    uint8_t month();
    uint8_t day();
    uint8_t hour();
    uint8_t minute();
    uint8_t sec();
    FixType fix();
    uint8_t fixFlags();
    uint8_t num_satellites();
    float lat_deg();
    float long_deg();
    float alt_m();
    uint32_t horizontal_accuracy_cm();

  private:
    /* Communication */
    HardwareSerial *bus_;
    /* Parsing */
    const uint8_t UBX_HEADER_[2] = {0xB5, 0x62};
    static constexpr uint8_t UBX_HEADER_LEN_ = 6;
    static constexpr uint8_t UBX_PAYLOAD_OFFSET_ = 4;
    static constexpr uint8_t UBX_NAV_CLASS_ = 0x01;
    static constexpr uint8_t UBX_NAV_PVT = 0x07;
    static constexpr uint8_t UBX_PVT_LEN_ = 92;
    uint16_t parser_pos_ = 0;
    uint8_t msg_len_buffer_[2];
    uint16_t msg_len_;
    uint8_t checksum_buffer_[2];
    uint8_t rx_buffer_[96];
    /* Data Packets */
    struct
    {
      uint32_t itow = 0;
      uint16_t year = 0;
      uint8_t month = 0;
      uint8_t day = 0;
      uint8_t hour = 0;
      uint8_t min = 0;
      uint8_t sec = 0;
      uint8_t valid = 0;
      uint32_t tacc = 0;
      int32_t nano = 0;
      uint8_t fix = 0;
      uint8_t flags = 0;
      uint8_t flags2 = 0;
      uint8_t numsv = 0;
      int32_t lon_deg = 0;
      int32_t lat_deg = 0;
      int32_t height = 0;
      int32_t hmsl = 0;
      uint32_t hacc = 0;
      uint32_t vacc = 0;
      int32_t veln = 0;
      int32_t vele = 0;
      int32_t veld = 0;
      int32_t gspeed = 0;
      int32_t headmot = 0;
      uint32_t sacc = 0;
      uint32_t headacc = 0;
      uint16_t pdop = 0;
      uint8_t flags3 = 0;
      uint8_t reserved1[5] = {0, 0, 0, 0, 0};
      int32_t headveh = 0;
      int16_t magdec = 0;
      uint16_t magacc = 0;
    } ubx_nav_pvt;
    uint16_t Checksum(uint8_t *data, uint16_t len);
};

#endif
