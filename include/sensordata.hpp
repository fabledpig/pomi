#ifndef SENSORDATA_HPP
#define SENSORDATA_HPP

#include <chrono>
#include <type_traits>
#include <memory>
#include <mysql/mysql.h>
#include <sstream>

namespace Pomi {
	using Clock = std::chrono::system_clock;
	using TimeStamp = Clock::time_point;

	template<class DataType>
	class SensorData {
		const std::string dataType;
		const TimeStamp timeStamp;
		const DataType value;
	public:
		template<class U>
		inline explicit SensorData(const std::string &dataType, U &&value);
		inline const std::string &getDataType() const;
		inline const TimeStamp &getTimeStamp() const;
		inline const DataType &getValue() const;
	};

	class SensorDataMYSQLPersister {
		std::shared_ptr<MYSQL> conn;
		MYSQL_RES *res;
	public:
		SensorDataMYSQLPersister(const char *server, unsigned int port, const char *user, const char *password, const char *db);

		template<class T>
		void operator()(T &t);
		template<class T>
		void operator()(const Pomi::SensorData<T> &data);
	};

	template<class T>
	std::ostream &operator<<(std::ostream &os, const SensorData<T> &data)
	{
		os << data.getValue();
		return os;
	}

	template<class DataType>
	template<class U>
	SensorData<DataType>::SensorData(const std::string &dataType, U &&value) :
		dataType{dataType},
		timeStamp{Clock::now()},
		value{std::forward<U>(value)}
	{}

	template<class DataType>
	const std::string &SensorData<DataType>::getDataType() const
	{
		return dataType;
	}

	template<class DataType>
	const TimeStamp &SensorData<DataType>::getTimeStamp() const
	{
		return timeStamp;
	}

	template<class DataType>
	const DataType &SensorData<DataType>::getValue() const
	{
		return value;
	}

	template<class T>
	void SensorDataMYSQLPersister::operator()(T &t)
	{
		t.read(*this);
	}

	template<class T>
	void SensorDataMYSQLPersister::operator()(const Pomi::SensorData<T> &data)
	{
		std::stringstream ss;
		ss << "INSERT INTO " << data.getDataType() << " values(NULL, FROM_UNIXTIME(" << data.getTimeStamp().time_since_epoch().count() / 1000000000 << "), " << data.getValue() << ")";

		if (mysql_query(conn.get(), ss.str().c_str()))
			std::runtime_error(mysql_error(conn.get()));
	}
}

#endif
