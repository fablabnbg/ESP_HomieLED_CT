/*
 * HomieLEDCTNode.h
 *
 *  Created on: 16.03.2019
 *      Author: ian
 */

#ifndef SRC_HOMIELEDCTNODE_H_
#define SRC_HOMIELEDCTNODE_H_

#include <HomieNode.hpp>

class HomieLEDCTNode: public HomieNode {

public:
	HomieLEDCTNode(const char* id, HomieSetting<long>& pinWW, HomieSetting<long>& pinCW);

protected:
	virtual void setup() override;
	//virtual void loop() override; // not needed
	virtual void onReadyToOperate() override;
	virtual bool handleInput(const HomieRange& range, const String  &property, const String &value) override;

private:
	static const uint16_t /*PROGMEM*/ gamma8[];
	uint8_t pinCW, pinWW;
	uint8_t curBrightness;
	uint8_t curColorTemp;
	HomieSetting<long>&  settingPinWW;
	HomieSetting<long>&  settingPinCW;

	void setPins();


};

#endif /* SRC_HOMIELEDCTNODE_H_ */
