/*
 * HomieLEDCTNode.cpp
 *
 *  Created on: 16.03.2019
 *      Author: ian
 */

#include <HomieLEDCTNode.h>
#include <LoggerNode.h>

const uint16_t /*PROGMEM*/ HomieLEDCTNode::gamma8[] = {
  0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,5,
  6,7,8,10,11,13,15,17,19,21,24,26,29,32,35,39,
  42,46,50,54,59,63,68,73,79,84,90,96,103,109,116,124,
  131,139,147,155,164,173,182,192,202,212,223,234,245,257,269,281,
  293,307,320,334,348,362,377,392,408,424,441,458,475,493,511,529,
  548,568,587,608,628,650,671,693,716,739,762,786,811,836,861,887,
  913,940,968,996,1024 };


HomieLEDCTNode::HomieLEDCTNode(const char* id, HomieSetting<long>& _pinWW, HomieSetting<long>& _pinCW):
		HomieNode(id, "LED mit Farbtemperatur", "led_ct"),
		pinCW(-1),
		pinWW(-1),
		curBrightness(0),
		curColorTemp(0),
		settingPinWW(_pinWW),
		settingPinCW(_pinCW)
{
	Serial.begin(74880);
	Serial.println("\nSetup CTLED");
	Serial.flush();
	advertise("bright").setName("Helligkeit").setDatatype("integer").setUnit("%").settable();
	advertise("ctemp").setName("Farbtemperatur").setDatatype("integer").setUnit("%").settable();
	advertise("coldwhite").setName("").setDatatype("integer").setFormat("0:1024");
	advertise("warmwhite").setName("").setDatatype("integer").setFormat("0:1024");
	analogWriteFreq(8000);
	settingPinCW.setDefaultValue(-1).setValidator([] (long candidate) {
		return (candidate >= 0 || candidate <= 16);});
	settingPinWW.setDefaultValue(-1).setValidator([] (long candidate) {
		return (candidate >= 0 || candidate <= 16);});
}

bool HomieLEDCTNode::handleInput(const HomieRange& range, const String& property, const String& value) {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received input for property %s, value %s", property.c_str(), value.c_str());
	if (property.equalsIgnoreCase("bright")) {
		curBrightness = value.toInt();
		if (curBrightness < 0 ||  curBrightness > 100) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::WARNING, "Received invalid brightness value %d.", curBrightness);
			curBrightness = 0;
		}
		setProperty("bright").setRetained(true).send(String(curBrightness));
		setPins();
		return true;
	} else if (property.equalsIgnoreCase("ctemp")) {
		curColorTemp = value.toInt();
		if (curColorTemp < 0 ||  curColorTemp > 100) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::WARNING, "Received invalid color temp value %d.", curColorTemp);
			curColorTemp = 50;
		}
		setProperty("ctemp").setRetained(true).send(String(curColorTemp));
		setPins();
		return true;
	}
	return false;
}

void HomieLEDCTNode::setup() {
	pinCW = settingPinCW.get();
	pinWW = settingPinWW.get();
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Set output pins to %d (CW) and %d (WW).", pinCW, pinWW);
	pinMode(pinCW, OUTPUT);
	pinMode(pinWW, OUTPUT);
	setPins();
}

void HomieLEDCTNode::onReadyToOperate() {
	setPins();
	setProperty("bright").setRetained(true).send(String(curBrightness));
	setProperty("ctemp").setRetained(true).send(String(curColorTemp));
}

void HomieLEDCTNode::setPins() {
	uint32_t bright=gamma8[curBrightness];
	uint16_t warm = bright * curColorTemp / 100;
	uint16_t cold = bright - warm;
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Set brightness to gamma-corerected value %d, distributed to %d CW and %d WW (Factor %d).", bright, cold, warm, curColorTemp);
	analogWrite(pinCW, cold);
	analogWrite(pinWW, warm);
	setProperty("coldwhite").setRetained(true).send("cold");
	setProperty("warmwhite").setRetained(true).send("warm");
}
