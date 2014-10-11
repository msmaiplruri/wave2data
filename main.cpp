#include "wave2data.h"

int main(int argc, char* argv[]) {
    std::string fn, output;
    std::string data, new_data;
    std::string sub("data");

    auto ret = setopts(argc, argv, fn, output);

    if (!ret) {
        showHelp();
        return 0;
    } else if (fn.empty()) {
        return 0;
    }

    // File Data (std::string)
    data = file2string(fn);

    // Data Check
    if (!dataCheck(data, fn)) {
        return 1;
    }

    // Search `data` string Position
    auto subPos = data.find(sub);

    if (subPos == static_cast<std::string::size_type>(std::string::npos)) {
        std::cout << fn << ": Unknown Error" << std::endl;
        return 0;
    }

    // Create New Split Data
    auto new_size = getDataSize(data.substr(subPos+4, 4));

    new_data = data.substr(subPos+8, new_size);
    if (!createData(output, new_data)) {
        return 1;
    }

    // Write Log
    std::cout << "Data Size: " << new_size << std::endl;
    std::cout << "Output: " << output << std::endl;

    return 0;
}
