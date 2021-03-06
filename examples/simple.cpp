#include <Arduino.h>
#include <Homie.hpp>

#include <LoggerNode.h>


#include <HomieLEDCTNode.h>

HomieSetting<long> settingCT1_WW ("CTLicht1_PIN_WW", "Output Pin Licht 1 warmweiß");
HomieSetting<long> settingCT1_CW ("CTLicht1_PIN_CW", "Output Pin Licht 1 kaltweiß");
HomieSetting<long> settingCT_PWMFREQ ("CTLicht_PWM_FREQ", "PWM Frequency");

LoggerNode LN;
HomieLEDCTNode CTLight1("CTLicht1", settingCT1_WW, settingCT1_CW);

#define FW_NAME "fln-ledct_1x"
#define FW_VERSION "0.0.2"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

void setup() {
	Serial.begin(74880);
	Serial.println("\nSetup");
	Serial.flush();
	Homie_setFirmware(FW_NAME, FW_VERSION);
	Homie.disableResetTrigger();
	//Homie.disableLedFeedback();
	analogWriteFreq(settingCT_PWMFREQ.get());
	Homie.setup();

}

void loop() {
	Homie.loop();

}


