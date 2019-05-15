#include "gy30.h"

namespace Pomi {
	void GY30::send(Operation op)
	{
		I2CDevice::send(static_cast<Byte>(op));
	}

	void GY30::executeMode()
	{
		if (!poweredOn)
			powerOn();

		I2CDevice::send(static_cast<Byte>(mode) | static_cast<Byte>(res));

		uint8_t measurmentTime;
		if (res == Resolution::LOW)
			measurmentTime = 24;
		else
			measurmentTime = 180;

		std::this_thread::sleep_for(std::chrono::milliseconds(measurmentTime));
	}

	void GY30::powerOn()
	{
		poweredOn = true;
		this->send(Operation::POWER_ON);
	}

	void GY30::powerDown()
	{
		poweredOn = false;
		this->send(Operation::POWER_DOWN);
	}

	GY30::GY30(Mode mode, Resolution res, bool addrLow) :
		I2CDevice{static_cast<Byte>(addrLow ? 0x23 : 0x5C)},
		poweredOn{false}
	{
		setMode(mode, res);
	}

	GY30::~GY30()
	{
		if (poweredOn)
			powerDown();
	}

	void GY30::setMode(Mode mode, Resolution res)
	{
		this->mode = mode;
		this->res = res;

		if (mode == Mode::CONT)
			executeMode();
	}
}
