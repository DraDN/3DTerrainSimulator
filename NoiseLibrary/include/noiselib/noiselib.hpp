#pragma once

#include "noiselib/aligned_allocator.hpp"
#include "noiselib/log.hpp"

namespace noiselib {
	class NoiseGenerator;
	template<typename T, size_t Alignment = 32>
	using AlignedAllocator = AlignedAllocator<T, Alignment>;
	template<typename T, size_t Alignment = 32>
	using AlignedVector = AlignedVector<T, Alignment>;
};
