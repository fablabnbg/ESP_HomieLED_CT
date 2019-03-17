#include <Arduino.h>
#include <Homie.hpp>

#include <LoggerNode.h>


#include <HomieLEDCTNode.h>

HomieSetting<long> settingCT1_WW ("CTLicht1_PIN_WW", "Output Pin Licht 1 warmweiß");
HomieSetting<long> settingCT1_CW ("CTLicht1_PIN_CW", "Output Pin Licht 1 kaltweiß");
HomieSetting<long> settingCT2_WW ("CTLicht2_PIN_WW", "Output Pin Licht 2 warmweiß");
HomieSetting<long> settingCT2_CW ("CTLicht2_PIN_CW", "Output Pin Licht 2 kaltweiß");

LoggerNode LN;
HomieLEDCTNode CTLight1(String("CTLicht1"), settingCT1_CW, settingCT1_WW);

#define FW_NAME "fln-ledct_2x"
#define FW_VERSION "0.0.1"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

void setup() {
	Serial.begin(74880);
	Serial.println("\nSetup");
	Serial.flush();
	Homie_setFirmware(FW_NAME, FW_VERSION);
	Homie.setup();

}

void loop() {
	Homie.loop();

}


