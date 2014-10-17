#include "wave2data.h"

// Version: 0.1

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::printf("Arg: <filename>\n");
		return 0;
	}

	std::string fn = argv[1];
	auto output = fn + ".dat";
	auto w2d = Wave2data();

	if (FAILED(w2d.InitMMio(fn.c_str()))) {
		return 1;
	}

	if (FAILED(w2d.Update(output.c_str()))) {
		return 1;
	}

	return 0;
}