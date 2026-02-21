#include "Log.hpp"

using namespace HG;

HGLog* HGLog::s_Instance = new HGLog();

std::mutex HGLog::s_Mutex = std::mutex();