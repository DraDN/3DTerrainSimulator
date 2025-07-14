#include <FastNoise/FastNoise.h>
#include <vector>
#include <iostream>
// #include <immintrin.h>
#include <noiselib/aligned_allocator.hpp>

int main() {
	auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
	auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

	fnFractal->SetSource(fnSimplex);
	fnFractal->SetOctaveCount(5);

	// std::vector<__m256i> noiseOutput(16*16*16);
	AlignedVector<float> noiseOutput(16*16*16);
	std::vector<float> noiseOutputNormal(16*16*16);

	// fnFractal->GenUniformGrid3D(reinterpret_cast<float*>(noiseOutput.data()), 0, 0, 0, 16, 16, 16, 0.2f, 1337);
	fnFractal->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, 16, 16, 16, 1337);
	fnFractal->GenUniformGrid3D(noiseOutputNormal.data(), 0, 0, 0, 16, 16, 16, 1337);

	// auto output = reinterpret_cast<float*>(noiseOutput.data());

	int index = 0;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++)
			std::cout << noiseOutput.at(index) << " (" << noiseOutputNormal.at(index++) << "), ";
			// std::cout << output[index++] << " ";
		std::cout << std::endl;
	}

	return 0;
}
