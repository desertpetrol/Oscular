#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <bitset>

void draw_rectangle(std::vector<uint32_t> &img, 
	const size_t img_w, const size_t img_h,
	const size_t sqr_x, const size_t sqr_y,
	const size_t sqr_w, const size_t sqr_h, 
	const uint32_t color) 
{
	assert(img.size() == img_w * img_h);
	size_t cx = 0;
	size_t cy = 0;
	for (size_t i = 0; i < sqr_w; i++) {
		for (size_t j = 0; j < sqr_h; j++) {
			cx = sqr_x + i;
			cy = sqr_y + j;
			assert(cx < img_w && cy < img_h);
			img[cx + cy * img_w] = color;
		}
	}
}


uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
{
	return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a)
{
	//apply a mask so we only get a Byte from the bit-shifted value
	r = (color >>  0) & 255;
	g = (color >>  8) & 255;
	b = (color >> 16) & 255;
	a = (color >> 24) & 255;
}

void drop_ppm_image(const std::string filename, const std::vector<uint32_t>& imageBuffer, const size_t w, const size_t h)
{
	const size_t imageB_size = w * h;
	assert(imageBuffer.size() == imageB_size);

	std::ofstream ofs(filename, std::ios::binary);

	//Adding the necessary header for the ppm image: "Magic number" + width, height and maximum rgba value.
	ofs << "P6\n" << w << " " << h << "'\n255'\n";
	for (size_t i = 0; i < imageB_size; i++) {
		uint8_t r, g, b, a;
		unpack_color(imageBuffer[i], r, g, b, a);
		ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
	}

	ofs.close();
}


int main()
{
	uint32_t red = pack_color(255, 0, 0);
	const size_t img_h = 512;
	const size_t img_w = 512;
	const size_t img_s = img_w * img_h;

	std::vector<uint32_t> frameBuffer(img_s, 255);

	int fA = 0, fB=1, fC=0;

	for (int j = 0; j < img_h; j++) {
		for (int i = 0; i < img_w; i++) {
			fC = fA + fB;
			uint8_t r, g, b;
			r = 255 * fC/float(2178309);
			g = 255 * fC/float(2178309);
			b = 255 * fC/float(2178309);

			frameBuffer[i+j*img_w] = pack_color(r, g, b);

			fA = fB;
			fB = fC;
			if (fC >= 2178309) {
				fC = 0;
				fA = 1;
				fB = 1;
			}
		}
	}
	
	draw_rectangle(frameBuffer, img_w, img_h, 255-16, 225-16, 32, 32, red);

	drop_ppm_image("./out.ppm", frameBuffer, img_w, img_h);

	return 0;
}