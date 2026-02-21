/// \brief hg memory management
/// \file Memory.h
/// \details recover from https://github.com/cyf-gh/stLib/blob/fdc497dfaeac5de1c2aabf872478b6147e2ac936/src/core/Def.h
#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <mutex>
#include <functional>

#define HG_SAFE_DEL( P ) if ( P != nullptr ) { delete P; P = nullptr; }
#define HG_SAFE_DEL_ARR( ARRAY ) if ( ARRAY != nullptr ) { delete []ARRAY; ARRAY = nullptr; }

/// \brief zero memory
/// \details len is character length, which len = 2 means 16 bits
#define st_zero_memory( p, len )memset( p, 0, len )
/// \sa st_zero_memory
#define HG_ZERO_MEMORY st_zero_memory

namespace HG {
namespace Memory {
	/// \brief ���󿽱� <br>object non copyable
	class NonCopyable {
	protected:
		NonCopyable() = default;
		virtual ~NonCopyable() = default;

	public:
		NonCopyable(const NonCopyable &) = delete;
		NonCopyable(NonCopyable &&) noexcept = default;
		NonCopyable &operator=(const NonCopyable &) = delete;
		NonCopyable &operator=(NonCopyable &&) noexcept = default;
	};

	/// \brief Thread-safe object pool
	/// \tparam T Object type to pool
	template<typename T>
	class ObjectPool : public NonCopyable {
	private:
		std::stack<T*> m_Pool;
		std::mutex m_Mutex;
		std::function<T*()> m_CreateFunc;
		std::function<void(T*)> m_ResetFunc;

	public:
		/// \brief Constructor with default create/reset
		ObjectPool() = default;

		/// \brief Constructor with custom create function
		/// \param createFunc Custom function to create objects
		/// \param resetFunc Custom function to reset objects before reuse
		ObjectPool(std::function<T*()> createFunc, std::function<void(T*)> resetFunc)
			: m_CreateFunc(createFunc), m_ResetFunc(resetFunc) {}

		/// \brief Pre-allocate objects
		/// \param count Number of objects to pre-allocate
		void PreAllocates(size_t count) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			for (size_t i = 0; i < count; ++i) {
				m_Pool.push(new T());
			}
		}

		/// \brief Acquire an object from pool
		/// \return Pointer to acquired object
		T* Acquire() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			if (!m_Pool.empty()) {
				T* obj = m_Pool.top();
				m_Pool.pop();
				if (m_ResetFunc) {
					m_ResetFunc(obj);
				}
				return obj;
			}
			if (m_CreateFunc) {
				return m_CreateFunc();
			}
			return new T();
		}

		/// \brief Release an object back to pool
		/// \param obj Object to release
		void Release(T* obj) {
			if (obj == nullptr) return;
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Pool.push(obj);
		}

		/// \brief Clear all pooled objects
		void Clear() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			while (!m_Pool.empty()) {
				delete m_Pool.top();
				m_Pool.pop();
			}
		}

		/// \brief Get current pool size
		size_t Size() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_Pool.size();
		}

		~ObjectPool() {
			Clear();
		}
	};

	/// \brief Unique pointer wrapper for HGEngine objects
	/// \tparam T Object type
	template<typename T>
	class UniquePtr : public std::unique_ptr<T> {
	public:
		UniquePtr() : std::unique_ptr<T>(nullptr) {}
		explicit UniquePtr(T* ptr) : std::unique_ptr<T>(ptr) {}

		T* Release() { return std::unique_ptr<T>::release(); }
		void Reset(T* ptr = nullptr) { std::unique_ptr<T>::reset(ptr); }
	};

	/// \brief Shared pointer wrapper for HGEngine objects
	/// \tparam T Object type
	template<typename T>
	class SharedPtr : public std::shared_ptr<T> {
	public:
		SharedPtr() : std::shared_ptr<T>(nullptr) {}
		explicit SharedPtr(T* ptr) : std::shared_ptr<T>(ptr) {}
		SharedPtr(const std::shared_ptr<T>& other) : std::shared_ptr<T>(other) {}
	};

}
}

#define HG_EMPTY_STRING ""
#define HG_EMPTY_STRINGW _T("")

/// \brief ban not sure binary copy.\
/// \note 
/// * �Ƽ�ʹ��HG::Memory::NonCopyable <br>HG::Memory::NonCopyable is recommended
#define HG_MEM_NO_BINARY_COPY( CLASSNAME ) \
public: \
CLASSNAME&operator=( const CLASSNAME& rhs ) = delete; \
CLASSNAME&operator=( CLASSNAME&& rhs ) = delete;
