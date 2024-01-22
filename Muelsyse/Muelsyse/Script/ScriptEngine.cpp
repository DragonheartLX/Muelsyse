#include "mulpch.h"
#include "Muelsyse/Script/ScriptEngine.h"

namespace mul
{
	struct ScriptEngineData
	{

	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::init()
	{
		s_Data = new ScriptEngineData();

		initMono();
	}

	void ScriptEngine::shutdown()
	{
		shutdownMono();
		delete s_Data;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	void ScriptEngine::initMono()
	{
		
	}

	void ScriptEngine::shutdownMono()
	{
		
	}

}
