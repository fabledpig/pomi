#ifndef GY30_H
#define GY30_H

#include <chrono>
#include <thread>

#include "i2cdevice.hpp"
#include "sensordata.hpp"

namespace Pomi {
	class GY30 : I2CDevice {
	public:
		enum class Mode : Byte {ONE_TIME = 0x20, CONT = 0x10};
		enum class Resolution : Byte {LOW = 0x3, HIGH = 0x0, HIGH2 = 0x1};
	private:
		Mode mode;
		Resolution res;
		bool poweredOn;

		enum class Operation : Byte {POWER_DOWN = 0x0, POWER_ON = 0x1, RESET = 0x7};

		void send(Operation op);
		void executeMode();
		void powerOn();
		void powerDown();
	public:
		explicit GY30(Mode mode = Mode::ONE_TIME, Resolution res = Resolution::LOW, bool addrLow = true);
		~GY30();

		void setMode(Mode mode, Resolution res);
		template<class Callback>
		void read(Callback &&cb);
	};

	template<class Callback>
	void GY30::read(Callback &&cb)
	{
		if (mode == Mode::ONE_TIME)
			executeMode();

		auto rawData = receive<2>();

		if (mode == Mode::ONE_TIME)
			poweredOn = false;

		double value = ((rawData[0] << 8) | rawData[1]) / 1.2;
		cb(SensorData<double>{"illuminance", value});
	}
}

#endif
