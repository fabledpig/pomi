#include "sensordata.hpp"

namespace Pomi {
	SensorDataMYSQLPersister::SensorDataMYSQLPersister(const char *server, unsigned int port, const char *user, const char *password, const char *db) :
		conn{mysql_init(NULL), mysql_close}
	{
		if (!mysql_real_connect(conn.get(), server, user, password, db, port, NULL, 0))
			throw std::runtime_error("Failed to connect to database");
	}
}
