#ifndef BME280_H
#define BME280_H

#include <chrono>
#include <thread>

#include "i2cdevice.hpp"
#include "BME280_driver/bme280.h"
#include "sensordata.hpp"

namespace Pomi {
	class BME280 : I2CDevice {
		static int fd;

		bme280_dev dev;

		inline static int8_t userRead(uint8_t id, uint8_t regAddr, uint8_t *data, uint16_t len);
		inline static int8_t userWrite(uint8_t id, uint8_t regAddr, uint8_t *data, uint16_t len);
		inline static void userWait(uint32_t period);
	public:
		BME280();

		template<class Callback>
		void read(Callback &&cb);
	};

	int8_t BME280::userRead(uint8_t id, uint8_t regAddr, uint8_t *data, uint16_t len)
	{
		I2CDevice::read(fd, regAddr, data, len);
		return 0;
	}

	int8_t BME280::userWrite(uint8_t id, uint8_t regAddr, uint8_t *data, uint16_t len)
	{
		I2CDevice::write(fd, regAddr, data, len);
		return 0;
	}

	void BME280::userWait(uint32_t period)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

	template<class Callback>
	void BME280::read(Callback &&cb)
	{
		fd = getFileDescriptor();

		bme280_data comp_data;

		bme280_set_sensor_mode(BME280_FORCED_MODE, &dev);
		dev.delay_ms(40);
		bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

		cb(SensorData<int>{"temperature", comp_data.temperature});
		cb(SensorData<unsigned int>{"humidity", comp_data.humidity});
		cb(SensorData<unsigned int>{"pressure", comp_data.pressure});
	}
}

#endif
