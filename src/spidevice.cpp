#include "spidevice.hpp"

namespace Pomi {
	constexpr Byte SPIDevice::spiBPW;
	constexpr const char * SPIDevice::devs[];

	SPIDevice::SPIDevice(Channel ch, unsigned int speed) :
		SPIDevice{ch}
	{
		this->speed = speed;
		constexpr Byte mode = 0;

		if (fd < 0)
			throw std::runtime_error("Failed to open SPI device");

		if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
			throw std::runtime_error("Failed to change SPI mode");

		if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
			throw std::runtime_error("Failed to change SPI BPW");

		if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
			throw std::runtime_error("Failed to change SPI speed");
	}
}
