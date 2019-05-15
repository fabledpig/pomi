#ifndef RDSOIL3_H
#define RDSOIL3_H

#include <iostream>
#include "spidevice.hpp"
#include "sensordata.hpp"

namespace Pomi {
	template<class ADC>
	class RDSOIL3 {
		static constexpr double MAX_VALUE = 2640.0;

		ADC &adc;
	public:
		explicit RDSOIL3(ADC &adc);

		template<class Callback>
		void read(Callback &&cb);
	};

	template<class ADC>
	RDSOIL3<ADC>::RDSOIL3(ADC &adc) :
		adc{adc}
	{}

	template<class ADC>
	template<class Callback>
	void RDSOIL3<ADC>::read(Callback &&cb)
	{
		adc.read([&cb](auto &&data)
			 {
				 cb(SensorData<unsigned int>("moisture",
							     static_cast<unsigned int>(100 * data.getValue() / MAX_VALUE)));
			 });
	}
}

#endif
