#include "i2cdevice.hpp"

#include <vector>

namespace Pomi {
	I2CDevice::I2CDevice(Address addr) :
		I2CDevice{}
	{
		if (fd < 0)
			throw std::runtime_error("Failed to open I2C bus");

		if (ioctl(fd, I2C_SLAVE, addr) < 0)
			throw std::runtime_error("Failed to get bus access");
	}

	void I2CDevice::write(int fd, Address regAddr, Byte *data, size_t len)
	{
		size_t size = len + 1;
		std::vector<Byte> dataVec(size);
		std::copy(data, data + len, dataVec.begin() + 1);
		dataVec[0] = regAddr;

		::write(fd, dataVec.data(), size);
	}

	void I2CDevice::read(int fd, Address regAddr, Byte *data, size_t len)
	{
		::write(fd, &regAddr, 1);
		::read(fd, data, len);
	}
}
