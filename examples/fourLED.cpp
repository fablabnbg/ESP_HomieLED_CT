#include <Arduino.h>
#include <Homie.hpp>

#include <LoggerNode.h>


#include <HomieLEDCTNode.h>

#define CT_LIGHT1_PIN_WW 15
#define CT_LIGHT1_PIN_CW 13
#define CT_LIGHT2_PIN_WW 14
#define CT_LIGHT2_PIN_CW 12
#define CT_LIGHT3_PIN_WW 5
#define CT_LIGHT3_PIN_CW 4
#define CT_LIGHT4_PIN_WW 0
#define CT_LIGHT4_PIN_CW 16

HomieSetting<long> settingCT_PWMFREQ ("CTLicht_PWM_FREQ", "PWM Frequency");

LoggerNode LN;
HomieLEDCTNode CTLight1("CTLicht1", CT_LIGHT1_PIN_WW, CT_LIGHT1_PIN_CW);
HomieLEDCTNode CTLight2("CTLicht2", CT_LIGHT2_PIN_WW, CT_LIGHT2_PIN_CW);
HomieLEDCTNode CTLight3("CTLicht3", CT_LIGHT3_PIN_WW, CT_LIGHT3_PIN_CW);
HomieLEDCTNode CTLight4("CTLicht4", CT_LIGHT4_PIN_WW, CT_LIGHT4_PIN_CW);

#define FW_NAME "fln-ledct_4x"
#define FW_VERSION "0.0.8"

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
	//Homie.disableLedFeedback();	--> on D1 Mini, BUILTIN_LED is used to show connection state (SLOW Blinking: No Wifi Connection, Fast Blinking: Wifi connected, but no MQTT)
	Homie.setup();
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Setting PWM frequency to %d.", settingCT_PWMFREQ.get());
	analogWriteFreq(settingCT_PWMFREQ.get());
	analogWriteRange(1024);
}

void loop() {
	Homie.loop();

}


