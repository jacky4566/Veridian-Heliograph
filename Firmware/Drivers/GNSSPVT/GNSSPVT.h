enum FixType {
  NO_FIX = 0,
  DEAD_RECKONING,
  FIX_2D,
  FIX_3D,
  GNSS_AND_DEAD_RECKONING,
  TIME_ONLY
};
enum PowerSaveMode {
  NOT_ACTIVE = 0,
  ENABLED,
  ACQUISITION,
  TRACKING,
  OPTIMIZED_TRACKING,
  INACTIVE
};
enum CarrierPhaseStatus {
  NO_SOL = 0,
  FLOAT_SOL,
  FIXED_SOL
};
	
void PVTbegin();
bool readSensor();
uint32_t getTow_ms();
uint16_t getYear();
uint8_t getMonth();
uint8_t getDay();
uint8_t getHour();
uint8_t getMin();
uint8_t getSec();
int32_t getNanoSec();
uint8_t getNumSatellites();
double getLongitude_deg();
double getLatitude_deg();
double getEllipsoidHeight_ft();
double getMSLHeight_ft();
double getHorizontalAccuracy_ft();
double getVerticalAccuracy_ft();
double getNorthVelocity_fps();
double getEastVelocity_fps();
double getDownVelocity_fps();
double getGroundSpeed_fps();
double getSpeedAccuracy_fps();
double getMotionHeading_deg();
double getVehicleHeading_deg();
double getHeadingAccuracy_deg();
float getMagneticDeclination_deg();
float getMagneticDeclinationAccuracy_deg();
double getLongitude_rad();
double getLatitude_rad();
double getEllipsoidHeight_m();
double getMSLHeight_m();
double getHorizontalAccuracy_m();
double getVerticalAccuracy_m();
double getNorthVelocity_ms();
double getEastVelocity_ms();
double getDownVelocity_ms();
double getGroundSpeed_ms();
double getSpeedAccuracy_ms();
double getMotionHeading_rad();
double getVehicleHeading_rad();
double getHeadingAccuracy_rad();
float getMagneticDeclination_rad();
float getMagneticDeclinationAccuracy_rad();
float getpDOP();
enum FixType getFixType();
enum PowerSaveMode getPowerSaveMode();
enum CarrierPhaseStatus getCarrierPhaseStatus();
bool isGnssFixOk();
bool isDiffCorrApplied();
bool isHeadingValid();
bool isConfirmedDate();
bool isConfirmedTime();
bool isTimeDateConfirmationAvail();
bool isValidDate();
bool isValidTime();
bool isTimeFullyResolved();
bool isMagneticDeclinationValid();