#include <iostream>
#include <tuple>
#include <map>
#include <fstream>
#include <regex>

#include "gy30.h"
#include "mcp3201.h"
#include "rdsoil3.hpp"
#include "bme280.h"

template<class T, class Func, unsigned int... S>
void forEachHelper(T &&t, Func &&f, std::integer_sequence<unsigned int, S...>)
{
	auto l = {(f(std::get<S>(t)), 0)...};
}

template<class... SensorTypes, class Func>
void forEach(std::tuple<SensorTypes...> &sensors, Func &&f)
{
	forEachHelper(sensors, std::forward<Func>(f), std::make_integer_sequence<unsigned int, sizeof...(SensorTypes)>());
}

std::map<std::string, std::string> readCredentialsFromFile(const char *filename)
{
	std::ifstream f{filename};
	if (!f.is_open())
		throw std::runtime_error{"Credential file not found"};

	std::map<std::string, std::string> credentials;

	std::regex baseRegex{"([a-z]+)[ ]*=[ ]*(.+)"};
	std::smatch baseMatch;
	for (std::string line; std::getline(f, line);) {
		std::regex_match(line, baseMatch, baseRegex);
		credentials[baseMatch[1]] = baseMatch[2];
	}

	return credentials;
}

int main()
{
	auto credentials = readCredentialsFromFile("credentials.txt");
	Pomi::SensorDataMYSQLPersister persister{credentials.at("ip").c_str(),
						 static_cast<unsigned int>(std::stoi(credentials.at("port"))),
						 credentials.at("user").c_str(),
						 credentials.at("password").c_str(),
						 credentials.at("db").c_str()};

	Pomi::GY30 gy30(Pomi::GY30::Mode::ONE_TIME, Pomi::GY30::Resolution::HIGH);
	Pomi::MCP3201 mcp3201{Pomi::SPIDevice::Channel::ZERO};
	Pomi::RDSOIL3<Pomi::MCP3201> rdsoil3{mcp3201};
	Pomi::BME280 bme280;

	auto sensors = std::make_tuple(gy30, rdsoil3, bme280);

	forEach(sensors, persister);

	return 0;
}
