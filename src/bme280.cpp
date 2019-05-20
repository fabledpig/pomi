#include "bme280.h"
#include "BME280_driver/bme280.h"
#include "BME280_driver/bme280_defs.h"

namespace Pomi {
	int BME280::fd = -1;

	BME280::BME280() :
		I2CDevice(0x76)
	{
		dev.dev_id = BME280_I2C_ADDR_PRIM;
		dev.intf = BME280_I2C_INTF;
		dev.read = userRead;
		dev.write = userWrite;
		dev.delay_ms = userWait;

		fd = getFileDescriptor();
		bme280_init(&dev);
		uint8_t settings_sel;

		dev.settings.osr_h = BME280_OVERSAMPLING_1X;
		dev.settings.osr_p = BME280_OVERSAMPLING_16X;
		dev.settings.osr_t = BME280_OVERSAMPLING_2X;
		dev.settings.filter = BME280_FILTER_COEFF_16;

		settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
		bme280_set_sensor_settings(settings_sel, &dev);
	}
}
