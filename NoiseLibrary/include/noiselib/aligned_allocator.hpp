#pragma once

#include <cstddef>
#include <new>
#include <memory>
#include <vector>
#include <limits>

template<typename T, std::size_t Alignment = 32>
struct AlignedAllocator {
	using value_type = T; // needed to be known by c++ containers

	AlignedAllocator() noexcept = default; // use the default constructor (needed to be explicit for std c++)

	template<typename U>
	AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}; // don't copy when using copy construct

	T* allocate(std::size_t n) {
		if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_array_new_length();
		// stop stack overflows

		void* ptr = ::operator new(n * sizeof(T), std::align_val_t(Alignment));
		return static_cast<T*>(ptr);
	}

	void deallocate(T* p, std::size_t) noexcept {
		::operator delete(p, std::align_val_t(Alignment));
	}
	// last two funcs needed internally for c++ containers etc

	template<typename U>
	struct rebind {
		using other = AlignedAllocator<U, Alignment>;
	};
	// for c++ backend
	// allows containers to create an allocator for a different type
};

template<typename T1, typename T2, std::size_t Alignment>
bool operator==(const AlignedAllocator<T1, Alignment>&, const AlignedAllocator<T2, Alignment>&) noexcept {
	return true;
}

template<typename T1, typename T2, std::size_t Alignment>
bool operator!=(const AlignedAllocator<T1, Alignment>&, const AlignedAllocator<T2, Alignment>&) noexcept {
	return false;
}
// operators needed internally

template<typename T, std::size_t Alignment = 32>
using AlignedVector = std::vector<T, AlignedAllocator<T, Alignment>>;