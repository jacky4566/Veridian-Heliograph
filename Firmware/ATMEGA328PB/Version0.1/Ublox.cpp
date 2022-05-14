#include "Arduino.h"
#include "Ublox.h"
#include <Time.h>

Ublox::Ublox(HardwareSerial *bus)
{
  bus_ = bus;
}
void Ublox::Begin(uint32_t baud)
{
  bus_->begin(baud);
  bus_->write((byte)0xff); //write dummy byte to wake receiver
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_GGA_OFF, sizeof(UBX_NAV_GGA_OFF));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_GLL_OFF, sizeof(UBX_NAV_GLL_OFF));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_GSA_OFF, sizeof(UBX_NAV_GSA_OFF));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_GSV_OFF, sizeof(UBX_NAV_GSV_OFF));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_RMC_OFF, sizeof(UBX_NAV_RMC_OFF));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_VTG_OFF, sizeof(UBX_NAV_VTG_OFF));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_NAV_PVT_ON, sizeof(UBX_NAV_PVT_ON));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_CFG_RXM, sizeof(UBX_CFG_RXM));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_CFG_PM2, sizeof(UBX_CFG_PM2));
  bus_->flush(); // wait for serial buffer
  bus_->write(UBX_CFG_CFG, sizeof(UBX_CFG_CFG));
  bus_->flush(); // wait for serial buffer
  parser_pos_ = 0;
  bitClear(UCSR1B, TXEN1);//disable TX
}
uint32_t Ublox::tow_ms()
{
  return ubx_nav_pvt.itow;
}
uint16_t Ublox::year()
{
  return ubx_nav_pvt.year;
}
uint8_t Ublox::month()
{
  return ubx_nav_pvt.month;
}
uint8_t Ublox::day()
{
  return ubx_nav_pvt.day;
}
uint8_t Ublox::hour()
{
  return ubx_nav_pvt.hour;
}
uint8_t Ublox::minute()
{
  return ubx_nav_pvt.min;
}
uint8_t Ublox::sec()
{
  return ubx_nav_pvt.sec;
}
Ublox::FixType Ublox::fix()
{
  return ubx_nav_pvt.fix;
}
uint8_t Ublox::num_satellites()
{
  return ubx_nav_pvt.numsv;
}
uint8_t Ublox::fixFlags()
{
  return ubx_nav_pvt.flags;
}
uint32_t Ublox::horizontal_accuracy_cm()
{
  return ubx_nav_pvt.hacc / 10;
}
float Ublox::lat_deg()
{
  return (float)ubx_nav_pvt.lat_deg / 10000000.0;
}
float Ublox::long_deg()
{
  return (float)ubx_nav_pvt.lon_deg / 10000000.0;
}
float Ublox::alt_m()
{
  return (float)ubx_nav_pvt.hmsl / 1000.0;
}

time_t Ublox::UTC()
{
  struct tm * timeParts;
  timeParts->tm_year = ubx_nav_pvt.year;
  timeParts->tm_mon = ubx_nav_pvt.month;
  timeParts->tm_mday = ubx_nav_pvt.day;
  timeParts->tm_hour = ubx_nav_pvt.hour;
  timeParts->tm_min = ubx_nav_pvt.min;
  timeParts->tm_sec = ubx_nav_pvt.sec;
  return mktime (timeParts);
}

bool Ublox::Parse()
{
  while (bus_->available())
  {
    uint8_t byte_read = bus_->read();
    /* Identify the packet header */
    if (parser_pos_ < 2)
    {
      if (byte_read == UBX_HEADER_[parser_pos_])
      {
        parser_pos_++;
      }
      else
      {
        parser_pos_ = 0;
      }
      /* Message class */
    }
    else if (parser_pos_ == 2)
    {
      if (byte_read == UBX_NAV_CLASS_)
      {
        rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
        parser_pos_++;
      }
      else
      {
        parser_pos_ = 0;
      }
      /* Message ID */
    }
    else if (parser_pos_ == 3)
    {
      if (byte_read == UBX_NAV_PVT)
      {
        rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
        parser_pos_++;
      }
      else
      {
        parser_pos_ = 0;
      }
      /* Messgae length */
    }
    else if (parser_pos_ == 4)
    {
      msg_len_buffer_[0] = byte_read;
      rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
      parser_pos_++;
      /* Message length */
    }
    else if (parser_pos_ == 5)
    {
      msg_len_buffer_[1] = byte_read;
      msg_len_ = static_cast<uint16_t>(msg_len_buffer_[1]) << 8 | msg_len_buffer_[0];
      rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
      if (msg_len_ == UBX_PVT_LEN_)
      {
        parser_pos_++;
      }
      else
      {
        parser_pos_ = 0;
      }
      break;

      /* Message payload */
    }
    else if (parser_pos_ < (msg_len_ + UBX_HEADER_LEN_))
    {
      rx_buffer_[parser_pos_ - sizeof(UBX_HEADER_)] = byte_read;
      parser_pos_++;
      /* Checksum */
    }
    else if (parser_pos_ == (msg_len_ + UBX_HEADER_LEN_))
    {
      checksum_buffer_[0] = byte_read;
      parser_pos_++;
    }
    else
    {
      checksum_buffer_[1] = byte_read;
      uint16_t received_checksum = static_cast<uint16_t>(checksum_buffer_[1]) << 8 | checksum_buffer_[0];
      uint16_t computed_checksum = Checksum(rx_buffer_, msg_len_ + UBX_HEADER_LEN_);
      if (computed_checksum == received_checksum)
      {
        if (rx_buffer_[UBX_PAYLOAD_OFFSET_ + 21] & 0x01) { //Fix OK
          memcpy(&ubx_nav_pvt, rx_buffer_ + UBX_PAYLOAD_OFFSET_, UBX_PVT_LEN_);
        }
        parser_pos_ = 0;
        return true;
      }
      else
      {
        parser_pos_ = 0;
        return false;
      }
    }
  }
  return false;
}
uint16_t Ublox::Checksum(uint8_t *data, uint16_t len)
{
  if (!data)
  {
    return 0;
  }
  uint8_t checksum_buffer[2] = {0, 0};
  for (unsigned int i = 0; i < len; i++)
  {
    checksum_buffer[0] += data[i];
    checksum_buffer[1] += checksum_buffer[0];
  }
  return static_cast<uint16_t>(checksum_buffer_[1]) << 8 | checksum_buffer_[0];
}
