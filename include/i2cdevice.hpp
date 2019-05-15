#ifndef I2CDEVICE_HPP
#define I2CDEVICE_HPP

#include <cstdint>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <array>
#include <algorithm>

#include "byte.h"

namespace Pomi {
	class I2CDevice {
		static constexpr const char *I2CDevFilePath = "/dev/i2c-1";

		int fd;

		inline I2CDevice();
	public:
		using Address = Byte;
		template <size_t N>
		using Data = std::array<Byte, N>;

		explicit I2CDevice(Address addr);
		inline ~I2CDevice();

		inline void send(Byte operation);
		template <size_t N>
		Data<N> receive();
		template <size_t N>
		void write(Address regAddr, const Data<N> &data);
		template <size_t N>
		Data<N> read(Address regAddr);
	};

	I2CDevice::I2CDevice() :
		fd{open(I2CDevFilePath, O_RDWR)}
	{}

	I2CDevice::~I2CDevice()
	{
		if (fd >= 0)
			close(fd);
	}

	void I2CDevice::send(Byte operation)
	{
		::write(fd, &operation, 1);
	}

	template <size_t N>
	I2CDevice::Data<N> I2CDevice::receive()
	{
		I2CDevice::Data<N> data;
		::read(fd, data.data(), N);

		return data;
	}

	template <size_t N>
	void I2CDevice::write(Address regAddr, const I2CDevice::Data<N> &data)
	{
		constexpr size_t size = N + 1;
		Data<size> addrCompletedData;
		addrCompletedData[0] = regAddr;
		std::copy(data.begin(), data.end(), addrCompletedData.begin() + 1);

		::write(fd, addrCompletedData.data(), size);
	}

	template <size_t N>
	I2CDevice::Data<N> I2CDevice::read(Address regAddr)
	{
		I2CDevice::Data<N> data;
		send(regAddr); // select register
		::read(fd, data.data(), N);

		return data;
	}
}

#endif
