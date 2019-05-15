#ifndef SPIDEVICE_HPP
#define SPIDEVICE_HPP

#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>
#include <array>
#include <unistd.h>

#include "byte.h"

namespace Pomi {
	class SPIDevice {
	public:
		enum Channel {ZERO, ONE};
	private:
		static constexpr Byte spiBPW = 8;
		static constexpr const char * devs[] = {"/dev/spidev0.0", "/dev/spidev0.1"};

		int fd;
		unsigned int speed;

		inline SPIDevice(Channel ch);
	public:
		template<size_t N>
		using Data = std::array<Byte, N>;

		explicit SPIDevice(Channel ch, unsigned int speed);
		inline ~SPIDevice();

		template<size_t N>
		Data<N> readWrite(Data<N> data);
		template<size_t N>
		Data<N> read();
		template<size_t N>
		void write(const Data<N> &data);
	};

	SPIDevice::SPIDevice(Channel ch) :
		fd{open(devs[ch], O_RDWR)}
	{}

	SPIDevice::~SPIDevice()
	{
		close(fd);
	}

	template<size_t N>
	SPIDevice::Data<N> SPIDevice::readWrite(SPIDevice::Data<N> data)
	{
		spi_ioc_transfer spi;

		memset(&spi, 0, sizeof(spi));

		spi.tx_buf = (unsigned long)data.data();
		spi.rx_buf = (unsigned long)data.data();
		spi.len = data.size();
		spi.delay_usecs = 0;
		spi.speed_hz = speed;
		spi.bits_per_word = spiBPW;

		ioctl(fd, SPI_IOC_MESSAGE(1), &spi);

		return data;
	}

	template<size_t N>
	SPIDevice::Data<N> SPIDevice::read()
	{
		SPIDevice::Data<N> data;
		::read(fd, data.data(), N);
		return data;
	}

	template<size_t N>
	void SPIDevice::write(const SPIDevice::Data<N> &data)
	{
		::write(fd, data.data(), N);
	}
}

#endif
