//MSG Config
static const uint8_t UBX_CFG_PM_OPERATEMODE_FULL[] = { 0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0xD0, 0x20, 0x00, 0x8C, 0xEF };

//Disable NMEA, Enable NAV_PVT
static const uint8_t UBX_CFG_MSGOUT_UART1[] = { 0xB5, 0x62, 0x06, 0x8A, 0x27, 0x00, 0x01, 0x01, 0x00, 0x00, 0xBB, 0x00, 0x91, 0x20, 0x00, 0xCA, 0x00, 0x91, 0x20, 0x00, 0xC0, 0x00, 0x91, 0x20, 0x00,
		0xC5, 0x00, 0x91, 0x20, 0x00, 0xAC, 0x00, 0x91, 0x20, 0x00, 0xB1, 0x00, 0x91, 0x20, 0x00, 0x07, 0x00, 0x91, 0x20, 0x01, 0xFF, 0xF3, 0xB5, 0x62, 0x06, 0x8A, 0x27, 0x00, 0x01, 0x02, 0x00, 0x00,
		0xBB, 0x00, 0x91, 0x20, 0x00, 0xCA, 0x00, 0x91, 0x20, 0x00, 0xC0, 0x00, 0x91, 0x20, 0x00, 0xC5, 0x00, 0x91, 0x20, 0x00, 0xAC, 0x00, 0x91, 0x20, 0x00, 0xB1, 0x00, 0x91, 0x20, 0x00, 0x07, 0x00,
		0x91, 0x20, 0x01, 0x00, 0x19 };
