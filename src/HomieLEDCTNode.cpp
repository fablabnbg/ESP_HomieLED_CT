/*
 * HomieLEDCTNode.cpp
 *
 *  Created on: 16.03.2019
 *      Author: ian
 */

#include <HomieLEDCTNode.h>
#include <LoggerNode.h>

/* gamma 2.8 
const uint16_t PROGMEM HomieLEDCTNode::gamma8[] = {
  0,0,0,0,0,0,0,1,1,1,2,2,3,3,4,5,
  6,7,8,10,11,13,15,17,19,21,24,26,29,32,35,39,
  42,46,50,54,59,63,68,73,79,84,90,96,103,109,116,124,
  131,139,147,155,164,173,182,192,202,212,223,234,245,257,269,281,
  293,307,320,334,348,362,377,392,408,424,441,458,475,493,511,529,
  548,568,587,608,628,650,671,693,716,739,762,786,811,836,861,887,
  913,940,968,996,1023 };
*/


/* Gamma-table with gamma value of 2.2 and linear scaled increments for low percentage values. (18% -> 21/1024 is first point of real gamma curve) */
const uint16_t /*PROGMEM*/ HomieLEDCTNode::gamma8[] = {
  0,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
  19,21,24,27,30,33,37,40,44,49,53,57,62,67,72,78,
  83,89,95,102,108,115,122,129,136,144,152,160,168,177,186,194,
  204,213,223,233,243,253,264,275,286,297,309,321,333,345,358,371,
  384,397,410,424,438,453,467,482,497,512,528,544,560,576,593,610,
  627,644,662,680,698,716,735,754,773,792,812,832,852,873,894,915,
  936,958,979,1002,1024 };


HomieLEDCTNode::HomieLEDCTNode(const char* id, HomieSetting<long>& _pinWW, HomieSetting<long>& _pinCW):
		HomieNode(id, "LED mit Farbtemperatur", "led_ct"),
		pinCW(-1),
		pinWW(-1),
		curBrightness(0),
		curColorTemp(0),
		settingPinWW(_pinWW),
		settingPinCW(_pinCW)
{
	advertise("bright").setName("Helligkeit").setDatatype("integer").setUnit("%").settable();
	advertise("ctemp").setName("Farbtemperatur").setDatatype("integer").setUnit("%").settable();
	advertise("state").setName("Zustand").setDatatype("boolean").settable();
	advertise("coldwhite").setName("").setDatatype("integer").setFormat("0:1024");
	advertise("warmwhite").setName("").setDatatype("integer").setFormat("0:1024");
	settingPinCW.setDefaultValue(-1).setValidator([] (long candidate) {
		return (candidate >= 0 || candidate <= 16);});
	settingPinWW.setDefaultValue(-1).setValidator([] (long candidate) {
		return (candidate >= 0 || candidate <= 16);});
}

bool HomieLEDCTNode::handleInput(const HomieRange& range, const String& property, const String& value) {
	LN.logf(__PRETTY_FUNCTION__, LoggerNode::DEBUG, "Received input for property %s, value %s", property.c_str(), value.c_str());
	uint8_t newB = curBrightness;
	uint8_t newT = curColorTemp;
	if (property.equalsIgnoreCase("bright")) {
		newB = value.toInt();
		if (curBrightness < 0 ||  curBrightness > 100) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::WARNING, "Received invalid brightness value %d.", curBrightness);
			newB = curBrightness;
		}
		setPins(newB, newT);
		setProperty("bright").setRetained(true).send(String(curBrightness));
		return true;
	} else if (property.equalsIgnoreCase("ctemp")) {
		curColorTemp = value.toInt();
		if (curColorTemp < 0 ||  curColorTemp > 100) {
			LN.logf(__PRETTY_FUNCTION__, LoggerNode::WARNING, "Received invalid color temp value %d.", curColorTemp);
			newT = curColorTemp;
		}
		setPins(newB, newT);
		setProperty("ctemp").setRetained(true).send(String(curColorTemp));
		return true;
	} else if (property.equalsIgnoreCase("state")) {
		bool on = value.equalsIgnoreCase("ON");
		if (on && curBrightness == 0) {
			newB = 80;
			curBrightness = 0; //hack to make setPins sending out the new "state" state
		} else if (!on && curBrightness > 0) {
			newB = 0;
			curBrightness = 100;  //hack to make setPins sending out the new "state" state
		}
		setPins(newB, newT);
		return true;
	}
	return false;
}

void HomieLEDCTNode::setup() {
	pinCW = settingPinCW.get();
	pinWW = settingPinWW.get();
	pinMode(pinCW, OUTPUT);
	pinMode(pinWW, OUTPUT);
	setPins(curBrightness, curColorTemp);
}

void HomieLEDCTNode::onReadyToOperate() {
	setProperty("bright").setRetained(true).send(String(curBrightness));
	setProperty("ctemp").setRetained(true).send(String(curColorTemp));
	setPins(curBrightness, curColorTemp);
}

void HomieLEDCTNode::setPins(uint8_t newBrightness, uint8_t newColorTemp) {
	if (newBrightness != curBrightness) {
		if (curBrightness == 0) setProperty("state").setRetained(true).send(String("ON"));
		if (newBrightness == 0) setProperty("state").setRetained(true).send(String("OFF"));
		curBrightness = newBrightness;
		setProperty("bright").setRetained(true).send(String(curBrightness));
	}
	uint32_t bright=gamma8[curBrightness];
	uint16_t warm = bright * curColorTemp / 100;
	uint16_t cold = bright - warm;
	//LN.logf(__PRETTY_FUNCTION__, LoggerNode::INFO, "Set brightness to gamma-corrected value %d, distributed to %d CW and %d WW (Factor %d).", bright, cold, warm, curColorTemp);
	analogWrite(pinCW, cold);
	analogWrite(pinWW, warm);
	setProperty("coldwhite").setRetained(true).send(String(cold));
	setProperty("warmwhite").setRetained(true).send(String(warm));
}
