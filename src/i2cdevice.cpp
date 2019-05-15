#include "i2cdevice.hpp"

namespace Pomi {
	I2CDevice::I2CDevice(Address addr) :
		I2CDevice{}
	{
		if (fd < 0)
			throw std::runtime_error("Failed to open I2C bus");

		if (ioctl(fd, I2C_SLAVE, addr) < 0)
			throw std::runtime_error("Failed to get bus access");
	}
}
