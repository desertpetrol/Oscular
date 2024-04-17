#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <bitset>

void draw_rectangle(std::vector<uint32_t> &img, 
	const size_t img_w, const size_t img_h,
	const size_t rect_x, const size_t rect_y,
	const size_t rect_w, const size_t rect_h, 
	const uint32_t color) 
{
	assert(img.size() == img_w * img_h);
	size_t cx = 0;
	size_t cy = 0;
	for (size_t i = 0; i < rect_w; i++) {
		for (size_t j = 0; j < rect_h; j++) {
			cx = rect_x + i;
			cy = rect_y + j;
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
	uint32_t red   = pack_color(255, 0, 0);
	uint32_t green = pack_color(0, 255, 0);
	uint32_t blue  = pack_color(0, 0, 255);
	uint32_t black = pack_color(0, 0, 0);
	uint32_t white = pack_color(255, 255, 255);

	const size_t img_h = 512;
	const size_t img_w = 512;
	const size_t img_s = img_w * img_h;
	std::vector<uint32_t> frameBuffer(img_s, 255);

	const size_t map_w = 16;
	const size_t map_h = 16;
	const char map[] =	"0000222222220000"\
				"1              0"\
				"1      11111   0"\
				"1     0        0"\
				"0     0  1110000"\
				"0     3        0"\
				"0   10000      0"\
				"0   0   11100  0"\
				"0   0   0      0"\
				"0   0   1  00000"\
				"0       1      0"\
				"2       1      0"\
				"0       0      0"\
				"0 0000000      0"\
				"0              0"\
				"0002222222200000";

	assert(sizeof(map) == map_w * map_h + 1); // +1 for the null terminated string ( '\0' )


	for (int j = 0; j < img_h; j++) {
		for (int i = 0; i < img_w; i++) {
			uint8_t r, g, b;
			r = 32;
			g = 32;
			b = 32;
			frameBuffer[i+j*img_w] = pack_color(r, g, b);
		}
	}



	//Drawing map inside image
	const size_t rect_w = img_w / map_w;
	const size_t rect_h = img_h / map_h;

	for (size_t j = 0;  j < map_h; j++) {
		for (size_t i = 0; i < map_w; i++)
		{
			if (map[i + j * map_w] == ' ') continue;
			size_t rect_x = i * rect_w;
			size_t rect_y = j * rect_h;

			draw_rectangle(frameBuffer, img_w, img_h, rect_x, rect_y, rect_w, rect_h, red);
		}
	}	


	float player_posx = 3.14;
	float player_posy = 2.34;
	float player_va = 1.5;

	//raycasting
	float c = 0;

	for (float t = 0; t < 20; t += .05) {
		float cx = player_posx + t * cos(player_va);
		float cy = player_posy + t * sin(player_va);
		if (map[int(cx) + int(cy) * map_w] != ' ') break;

		size_t pix_x = cx * rect_w;
		size_t pix_y = cy * rect_h;
		frameBuffer[pix_x + pix_y * img_w] = green;
	}

	draw_rectangle(frameBuffer, img_w, img_h, player_posx * rect_w, player_posy * rect_h, 5, 5, blue);

	drop_ppm_image("./out.ppm", frameBuffer, img_w, img_h);

	return 0;
}