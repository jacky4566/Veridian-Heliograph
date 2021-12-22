typedef struct{
    uint8_t msg_class;
    uint8_t msg_id;	
	
	uint32_t iTOW;		//Byte Offset 0
	uint16_t year;		//Byte Offset 4
	uint8_t month;		//Byte Offset 6
	uint8_t day;		//Byte Offset 7
	uint8_t hour;		//Byte Offset 8
	uint8_t min;		//Byte Offset 9
	uint8_t sec;		//Byte Offset 10
	
	uint8_t valid;		//Byte Offset 11
	uint32_t tAcc;		//Byte Offset 12
	int32_t nano;		//Byte Offset 16
	uint8_t fixType;	//Byte Offset 20
	uint8_t flags;		//Byte Offset 21
	uint8_t flags2;		//Byte Offset 22
	uint8_t numSV;		//Byte Offset 23
	
	int32_t lon;		//Byte Offset 24
	int32_t lat;		//Byte Offset 28
	int32_t height;		//Byte Offset 32
	int32_t hMSL;		//Byte Offset 36
	
	uint32_t hAcc;		//Byte Offset 40
	uint32_t vAcc;		//Byte Offset 44
	int32_t velN;		//Byte Offset 48
	int32_t velE;		//Byte Offset 52
	int32_t velD;		//Byte Offset 56
	int32_t gSpeed;		//Byte Offset 60
	int32_t headMot;	//Byte Offset 64
	uint32_t sAcc;		//Byte Offset 68
	uint32_t headAcc;	//Byte Offset 72
	
	uint16_t pDOP;		//Byte Offset 76
	uint8_t reserved1;	//Byte Offset 78
	int32_t headVeh;	//Byte Offset 84
	int16_t magDec;		//Byte Offset 88
	uint16_t magAcc;	//Byte Offset 90

	uint16_t checksum;

}_UBX_NAV_PVT;

struct _UBX_NAV_PVT _tempPacket;
struct _UBX_NAV_PVT _validPacket;