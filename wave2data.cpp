#include "wave2data.h"

void showHelp() {
    std::cout << "Arguments:" << std::endl;
    std::cout << "\t-h           \t" << "show help" << std::endl;
    std::cout << "\t-v           \t" << "show version" << std::endl;
    std::cout << "\t-f <filename>\t" << "set input filename" << std::endl;
    std::cout << "\t-o <filename>\t" << "set output filename" << std::endl;
}

bool setopts(int argc, char* argv[], std::string &fn, std::string &output) {
    int result;
    auto odef = "output.dat";

    fn = "";
    output = odef;
    auto version = "wave2data 0.1.1";

    if (argc < 2) {
        return false;
    }

    while ((result=getopt(argc, argv, "hvf:o:"))!=-1) {
        switch (result) {
        case 'h':
            // help
            showHelp();
            break;
        case 'v':
            // version
            std::cout << version << std::endl;
            break;
        case 'f':
            fn = std::string(optarg);
            if (output == odef) {
                output = fn + ".dat";
            }
            break;
        case 'o':
            output = std::string(optarg);
            break;
        default:
            break;
        }
    }

    return true;
}


std::string file2string(std::string fn) {
    std::ifstream ifs(fn, std::ios::in | std::ios::binary);

    if (!ifs) {
        std::perror(fn.c_str());
        return std::string();
    }

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    return str;
}

bool dataCheck(std::string data, std::string fn) {
    if (data.empty()) {
        std::cout << fn << ": Unknown Error" << std::endl;
        return false;
    }

    // RIFF Check
    if (data.substr(0,4) != "RIFF") {
        std::cout << fn << ": not RIFF File." << std::endl;
        return false;
    }

    // Wave Check
    if (data.substr(8, 4) != "WAVE") {
        std::cout << fn << ": not Wave File." << std::endl;
        return false;
    }

    return true;
}

int getDataSize(std::string s_hex) {
    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += static_cast<unsigned char>(s_hex.at(i)) << (8*i);
    }

    return sum;
}

bool createData(std::string fn, std::string data) {
    std::ofstream ofs(fn, std::ios::out | std::ios::binary);

    if (!ofs) {
        std::perror(fn.c_str());
        return false;
    }

    ofs << data;
    ofs.close();

    return true;
}
