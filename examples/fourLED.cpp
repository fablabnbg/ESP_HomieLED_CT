#include <Arduino.h>
#include <Homie.hpp>

#include <LoggerNode.h>


#include <HomieLEDCTNode.h>

HomieSetting<long> settingCT1_WW ("CTLicht1_PIN_WW", "Output Pin Licht 1 warmweiß");
HomieSetting<long> settingCT1_CW ("CTLicht1_PIN_CW", "Output Pin Licht 1 kaltweiß");
HomieSetting<long> settingCT2_WW ("CTLicht2_PIN_WW", "Output Pin Licht 2 warmweiß");
HomieSetting<long> settingCT2_CW ("CTLicht2_PIN_CW", "Output Pin Licht 2 kaltweiß");
HomieSetting<long> settingCT3_WW ("CTLicht3_PIN_WW", "Output Pin Licht 3 warmweiß");
HomieSetting<long> settingCT3_CW ("CTLicht3_PIN_CW", "Output Pin Licht 3 kaltweiß");
HomieSetting<long> settingCT4_WW ("CTLicht4_PIN_WW", "Output Pin Licht 4 warmweiß");
HomieSetting<long> settingCT4_CW ("CTLicht4_PIN_CW", "Output Pin Licht 4 kaltweiß");


HomieSetting<long> settingCT_PWMFREQ ("CTLicht_PWM_FREQ", "PWM Frequency");

LoggerNode LN;
HomieLEDCTNode CTLight1("CTLicht1", settingCT1_WW, settingCT1_CW);
HomieLEDCTNode CTLight2("CTLicht2", settingCT2_WW, settingCT2_CW);
HomieLEDCTNode CTLight3("CTLicht3", settingCT3_WW, settingCT3_CW);
HomieLEDCTNode CTLight4("CTLicht4", settingCT4_WW, settingCT4_CW);

#define FW_NAME "fln-ledct_4x"
#define FW_VERSION "0.0.7"

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
	Homie.setup();
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Setting PWM frequency to %d.", settingCT_PWMFREQ.get());
	analogWriteFreq(settingCT_PWMFREQ.get());


}

void loop() {
	Homie.loop();

}


