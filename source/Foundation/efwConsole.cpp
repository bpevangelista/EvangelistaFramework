#include "Foundation/efwConsole.h"

using namespace efw;

// Globals
int32_t gConsoleWriteMask = ConsoleMessages::kInfo | ConsoleMessages::kWarning | ConsoleMessages::kError;
FILE* gConsoleFileOutput = NULL;
char gConsoleBuffer[2 * 1024];

const char* Console::kConsoleBreakLine = "\r\n";

void Console::Write(const char* str, ...)
{
	if ((ConsoleMessages::kInfo & gConsoleWriteMask) != 0)
	{
		// TODO: LOCK

		va_list args;
		va_start(args, str);
		vsprintf(gConsoleBuffer, str, args);
		va_end(args);

		printf("%s", gConsoleBuffer);

		// TODO: UNLOCK
	}
}


void Console::WriteLine(const char* str, ...)
{
	if ((ConsoleMessages::kInfo & gConsoleWriteMask) != 0)
	{
		// TODO: LOCK

		va_list args;
		va_start(args, str);
		vsprintf(gConsoleBuffer, str, args);
		va_end(args);

		printf("%s%s", gConsoleBuffer, kConsoleBreakLine);

		// TODO: UNLOCK
	}
}


void Console::Write(int32_t messageType, const char* str, ...)
{
	if ((messageType & gConsoleWriteMask) != 0)
	{
		// TODO: LOCK

		va_list args;
		va_start(args, str);
		vsprintf(gConsoleBuffer, str, args);
		va_end(args);

		printf("%s", gConsoleBuffer);

		// TODO: UNLOCK
	}
}


void Console::WriteLine(int32_t messageType, const char* str, ...)
{
	if ((messageType & gConsoleWriteMask) != 0)
	{
		// TODO: LOCK

		va_list args;
		va_start(args, str);
		vsprintf(gConsoleBuffer, str, args);
		va_end(args);

		printf("%s%s", gConsoleBuffer, kConsoleBreakLine);

		// TODO: UNLOCK
	}
}


void Console::SetWriteMask(int32_t writeMask)
{
	gConsoleWriteMask = writeMask;
}


void Console::SetOutput(ConsoleOutput consoleOutput, const char* filePath)
{
	const char* input = NULL;
	if (gConsoleFileOutput != NULL)
	{
		fflush(gConsoleFileOutput);
		fclose(gConsoleFileOutput);
		gConsoleFileOutput = NULL;
	}

	switch (consoleOutput)
	{
	case ConsoleOutputs::kNull:
		input = "NUL";
		break;

	case ConsoleOutputs::kStdOut:
		input = "CON";
		break;

	case ConsoleOutputs::kFile:
		EFW_ASSERT(filePath != NULL);
		input = filePath;
		break;
	};

	gConsoleFileOutput = freopen(input, "a", stdout);
}