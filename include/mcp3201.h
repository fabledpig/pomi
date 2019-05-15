#ifndef MCP3201_H
#define MCP3201_H

#include "spidevice.hpp"
#include "sensordata.hpp"

#include <iostream>

namespace Pomi {
	class MCP3201 : SPIDevice {
		static constexpr unsigned int speed = 976000;
	public:
		inline explicit MCP3201(Channel ch);

		template<class Callback>
		void read(Callback &&cb);
	};

	MCP3201::MCP3201(Channel ch)
		: SPIDevice{ch, 976000}
	{
	}

	template<class Callback>
	void MCP3201::read(Callback &&cb)
	{
		auto data = SPIDevice::readWrite<2>({0, 0});

		cb(SensorData<unsigned int>{"moisture",
				static_cast<unsigned int>(((data[0] & 0x1F) << 7) | (data[1] >> 1))});
	}
}

#endif
