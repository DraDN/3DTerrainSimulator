// #include <FastNoise/FastNoise.h>
// #include <FastNoise2/include/FastNoise/FastNoise.h>
#include <FastNoise/FastNoise.h>
#include <vector>

int main() {
	auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
	auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

	fnFractal->SetSource(fnSimplex);
	fnFractal->SetOctaveCount(5);

	std::vector<float> noiseOutput(16*16*16);

	fnFractal->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, 16, 16, 16, 0.2f, 1337);

	return 0;
}
