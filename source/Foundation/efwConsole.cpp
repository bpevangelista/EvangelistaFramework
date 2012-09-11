#include "Foundation/efwConsole.h"

#include <stdio.h>
#include <stdarg.h>

using namespace efw;

char gConsoleBuffer[2 * 1024];

void Console::Write(const char* str, ...)
{
	// TODO: LOCK

	va_list args;
	va_start(args, str);
	vsprintf(gConsoleBuffer, str, args);
	va_end(args);

	printf(gConsoleBuffer);

	// TODO: UNLOCK
}


void Console::WriteLine(const char* str, ...)
{
	// TODO: LOCK

	const char* kConsoleBreakLine = "\r\n";

	va_list args;
	va_start(args, str);
	vsprintf(gConsoleBuffer, str, args);
	va_end(args);

	printf("%s%s", gConsoleBuffer, kConsoleBreakLine);

	// TODO: UNLOCK
}


void Console::Write(int32_t messageType, const char* str, ...)
{

}


void Console::WriteLine(int32_t messageType, const char* str, ...)
{

}