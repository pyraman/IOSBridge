#include "System.h"
#include <chrono>

namespace IOSBridge {
	__int64 System::currentMilliseconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}