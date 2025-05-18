#pragma once

#include <memory>

class SQLiteConnection; // SQLiteConnection.hpp.
using SQLiteConnectionPtr = std::unique_ptr<SQLiteConnection>;