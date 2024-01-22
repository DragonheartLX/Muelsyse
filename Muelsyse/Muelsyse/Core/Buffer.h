#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <cstring>

namespace mul
{
	// Non-owning raw buffer class
	struct Buffer
	{
		uint8_t* Data = nullptr;
		uint64_t Size = 0;

		Buffer() = default;
		Buffer(uint64_t size)
		{
			allocate(size);
		}
		Buffer(const Buffer&) = default;

		static Buffer copy(Buffer other)
		{
			Buffer result(other.Size);
			memcpy(result.Data, other.Data, other.Size);
			return result;
		}

        Buffer(const void* data, uint64_t size): 
            Data((uint8_t*)data), Size(size)
		{
		}

		void allocate(uint64_t size)
		{
			release();

			Data = (uint8_t*)malloc(size);
			Size = size;
		}

		void release()
		{
			free(Data);
			Data = nullptr;
			Size = 0;
		}

		template<typename T>
		T* as()
		{
			return (T*)Data;
		}

		operator bool() const
		{
			return (bool)Data;
		}
	};

	struct ScopedBuffer
	{
		ScopedBuffer(Buffer buffer): 
            m_Buffer(buffer)
		{
		}

		ScopedBuffer(uint64_t size): 
            m_Buffer(size)
		{
		}

		~ScopedBuffer()
		{
			m_Buffer.release();
		}

		uint8_t* data() { return m_Buffer.Data; }
		uint64_t size() { return m_Buffer.Size; }

		template<typename T>
		T* as()
		{
			return m_Buffer.as<T>();
		}

		operator bool() const { return m_Buffer; }
	private:
		Buffer m_Buffer;
	};


}
