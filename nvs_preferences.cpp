#include "nvs_preferences.h"

NvsPreferences::NvsPreferences() {

  RetrieveDataModelVersion();

  // if(data_model_version != kDataModelVersion) {
  //   // save default data in NVS Memory
  //   SaveDefaults();
  // }

  // PrintSavedData();

  Serial.println(F("ESP32 NVS Memory setup successful!"));
}

void NvsPreferences::RetrieveDataModelVersion() {
  //init preference
 	preferences.begin(kNvsDataKey, /*readOnly = */ true);
  data_model_version = preferences.getUInt(kDataModelVersionKey, 0);
  preferences.end();
  Serial.printf("Data Model Version read = %u\n", data_model_version);
}

void NvsPreferences::SaveDataModelVersion() {
  //init preference
 	preferences.begin(kNvsDataKey, /*readOnly = */ false);

  // set default data
  preferences.putUInt(kDataModelVersionKey, kDataModelVersion);
  data_model_version = kDataModelVersion;
  preferences.end();

  Serial.printf("Data Version set = %u\n", kDataModelVersion);
}

void NvsPreferences::SaveDefaults() {
  //init preference
 	preferences.begin(kNvsDataKey, /*readOnly = */ false);

 	// set default data
  preferences.putUInt(kDataModelVersionKey, kDataModelVersion);
  data_model_version = kDataModelVersion;
  Serial.printf("Data Version set = %u\n", kDataModelVersion);

  preferences.putUChar(kAlarmHrKey, kAlarmHr);
  preferences.putUChar(kAlarmMinKey, kAlarmMin);
  preferences.putBool(kAlarmIsAmKey, kAlarmIsAm);
  preferences.putBool(kAlarmOnKey, kAlarmOn);
  String kWiFiSsidString = kWiFiSsid.c_str();
  preferences.putString(kWiFiSsidKey, kWiFiSsidString);
  String kWiFiPasswdString = kWiFiPasswd.c_str();
  preferences.putString(kWiFiPasswdKey, kWiFiPasswdString);
  preferences.putUInt(kWeatherZipCodeKey, kWeatherZipCode);
  String kWeatherCountryCodeString = kWeatherCountryCode.c_str();
  preferences.putString(kWeatherCountryCodeKey, kWeatherCountryCodeString);
  preferences.putBool(kWeatherUnitsMetricNotImperialKey, kWeatherUnitsMetricNotImperial);
  preferences.putUChar(kAlarmLongPressSecondsKey, kAlarmLongPressSeconds);
  String kFirmwareVersionString = kFirmwareVersion.c_str();
  preferences.putString(kFirmwareVersionKey, kFirmwareVersionString);
  preferences.putUInt(kCpuSpeedMhzKey, cpu_speed_mhz);
  preferences.putBool(kScreensaverMotionTypeKey, true);

  preferences.end();

  Serial.printf("Default Data Set in NVS Memory\n");
  Serial.flush();

}

void NvsPreferences::PrintSavedData() {
  uint8_t long_press_seconds;
  RetrieveLongPressSeconds(long_press_seconds);
  uint8_t alarmHr, alarmMin;
  bool alarmIsAm, alarmOn;
  RetrieveAlarmSettings(alarmHr, alarmMin, alarmIsAm, alarmOn);
  std::string wifi_ssid, wifi_password;
  RetrieveWiFiDetails(wifi_ssid, wifi_password);
  uint32_t location_zip_code;
  std::string location_country_code;
  bool weather_units_metric_not_imperial;
  RetrieveWeatherLocationDetails(location_zip_code, location_country_code, weather_units_metric_not_imperial);
  std::string savedFirmwareVersion;
  RetrieveSavedFirmwareVersion(savedFirmwareVersion);
}

void NvsPreferences::RetrieveLongPressSeconds(uint8_t &long_press_seconds) {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  long_press_seconds = preferences.getUChar(kAlarmLongPressSecondsKey, kAlarmLongPressSeconds);
  preferences.end();
}

void NvsPreferences::SaveLongPressSeconds(uint8_t long_press_seconds) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putUChar(kAlarmLongPressSecondsKey, long_press_seconds);
  preferences.end();
  Serial.printf("NVS Memory long_press_seconds: %d sec\n", long_press_seconds);
}

void NvsPreferences::RetrieveAlarmSettings(uint8_t &alarmHr, uint8_t &alarmMin, bool &alarmIsAm, bool &alarmOn) {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  alarmHr = preferences.getUChar(kAlarmHrKey);
  alarmMin = preferences.getUChar(kAlarmMinKey);
  alarmIsAm = preferences.getBool(kAlarmIsAmKey);
  alarmOn = preferences.getBool(kAlarmOnKey);
  preferences.end();
}

void NvsPreferences::SaveAlarm(uint8_t alarmHr, uint8_t alarmMin, bool alarmIsAm, bool alarmOn) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putUChar(kAlarmHrKey, alarmHr);
  preferences.putUChar(kAlarmMinKey, alarmMin);
  preferences.putBool(kAlarmIsAmKey, alarmIsAm);
  preferences.putBool(kAlarmOnKey, alarmOn);
  preferences.end();
  Serial.printf("NVS Memory SaveAlarm %2d:%02d alarmIsAm=%d alarmOn=%d\n", alarmHr, alarmMin, alarmIsAm, alarmOn);
}

void NvsPreferences::RetrieveWiFiDetails(std::string &wifi_ssid, std::string &wifi_password) {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  String kWiFiSsidString = preferences.getString(kWiFiSsidKey);
  String kWiFiPasswdString = preferences.getString(kWiFiPasswdKey);
  preferences.end();
  wifi_ssid = kWiFiSsidString.c_str();
  wifi_password = kWiFiPasswdString.c_str();
  PrintLn("NVS Memory wifi_ssid: ", wifi_ssid.c_str());
  if(debug_mode) PrintLn("NVS Memory wifi_password: ", wifi_password.c_str());
  PrintLn("WiFi details retrieved from NVS Memory.");
}

void NvsPreferences::SaveWiFiDetails(std::string wifi_ssid, std::string wifi_password) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  String kWiFiSsidString = wifi_ssid.c_str();
  preferences.putString(kWiFiSsidKey, kWiFiSsidString);
  String kWiFiPasswdString = wifi_password.c_str();
  preferences.putString(kWiFiPasswdKey, kWiFiPasswdString);
  preferences.end();
  if(debug_mode) {
    PrintLn("NVS Memory wifi_ssid: ", wifi_ssid.c_str());
    PrintLn("NVS Memory wifi_password: ", wifi_password.c_str());
  }
  PrintLn("WiFi ssid and password written to NVS Memory");
}

void NvsPreferences::RetrieveSavedFirmwareVersion(std::string &savedFirmwareVersion) {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  String kFirmwareVersionString = preferences.getString(kFirmwareVersionKey);
  preferences.end();
  savedFirmwareVersion = kFirmwareVersionString.c_str();
  PrintLn("Saved Firmware Version: ", savedFirmwareVersion.c_str());
}

void NvsPreferences::SaveCurrentFirmwareVersion() {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  String kFirmwareVersionString = kFirmwareVersion.c_str();
  preferences.putString(kFirmwareVersionKey, kFirmwareVersionString);
  preferences.end();
  PrintLn("Current Firmware Version written to NVS Memory");
}

void NvsPreferences::CopyFirmwareVersionFromEepromToNvs(std::string firmwareVersion) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  String kFirmwareVersionString = firmwareVersion.c_str();
  preferences.putString(kFirmwareVersionKey, kFirmwareVersionString);
  preferences.end();
  PrintLn("Firmware Version from Eeprom written to NVS Memory");
}

void NvsPreferences::RetrieveWeatherLocationDetails(uint32_t &location_zip_code, std::string &location_country_code, bool &weather_units_metric_not_imperial) {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  location_zip_code = preferences.getUInt(kWeatherZipCodeKey);
  String kWeatherCountryCodeString = preferences.getString(kWeatherCountryCodeKey);
  location_country_code = kWeatherCountryCodeString.c_str();
  weather_units_metric_not_imperial = preferences.getBool(kWeatherUnitsMetricNotImperialKey);
  preferences.end();
  PrintLn("NVS Memory location_zip_code: ", location_zip_code);
  PrintLn("NVS Memory location_country_code: ", location_country_code);
  PrintLn("NVS Memory weather_units_metric_not_imperial: ", weather_units_metric_not_imperial);
  PrintLn("Weather Location details retrieved from NVS Memory.");
}

void NvsPreferences::SaveWeatherLocationDetails(uint32_t location_zip_code, std::string location_country_code, bool weather_units_metric_not_imperial) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putUInt(kWeatherZipCodeKey, location_zip_code);
  String kWeatherCountryCodeString = location_country_code.c_str();
  preferences.putString(kWeatherCountryCodeKey, kWeatherCountryCodeString);
  preferences.putBool(kWeatherUnitsMetricNotImperialKey, weather_units_metric_not_imperial);
  preferences.end();
  PrintLn("Weather Location details written to NVS Memory");
}

void NvsPreferences::SaveWeatherUnits(bool weather_units_metric_not_imperial) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putBool(kWeatherUnitsMetricNotImperialKey, weather_units_metric_not_imperial);
  preferences.end();
  PrintLn("Weather Location details written to NVS Memory");
}

uint32_t NvsPreferences::RetrieveSavedCpuSpeed() {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  uint32_t saved_cpu_speed_mhz = preferences.getUInt(kCpuSpeedMhzKey);
  preferences.end();
  Serial.printf("NVS Memory saved_cpu_speed_mhz: %u MHz\n", saved_cpu_speed_mhz);
  return saved_cpu_speed_mhz;
}

void NvsPreferences::SaveCpuSpeed() {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putUInt(kCpuSpeedMhzKey, cpu_speed_mhz);
  preferences.end();
  Serial.printf("NVS Memory cpu_speed_mhz: %u MHz saved.\n", cpu_speed_mhz);
}

void NvsPreferences::CopyCpuSpeedFromEepromToNvsMemory(uint32_t cpu_speed_mhz_from_eeprom) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putUInt(kCpuSpeedMhzKey, cpu_speed_mhz_from_eeprom);
  preferences.end();
  Serial.printf("NVS Memory cpu_speed_mhz_from_eeprom: %u MHz saved.\n", cpu_speed_mhz_from_eeprom);
}

bool NvsPreferences::RetrieveScreensaverBounceNotFlyHorizontally() {
  preferences.begin(kNvsDataKey, /*readOnly = */ true);
  bool screensaver_bounce_not_fly_horiontally = preferences.getBool(kScreensaverMotionTypeKey);
  preferences.end();
  Serial.printf("NVS Memory screensaver_bounce_not_fly_horiontally: %d retrieved.\n", screensaver_bounce_not_fly_horiontally);
  return screensaver_bounce_not_fly_horiontally;
}

void NvsPreferences::SaveScreensaverBounceNotFlyHorizontally(bool screensaver_bounce_not_fly_horiontally) {
  preferences.begin(kNvsDataKey, /*readOnly = */ false);
  preferences.putBool(kScreensaverMotionTypeKey, screensaver_bounce_not_fly_horiontally);
  preferences.end();
  Serial.printf("NVS Memory screensaver_bounce_not_fly_horiontally: %d saved.\n", screensaver_bounce_not_fly_horiontally);
}