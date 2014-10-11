#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <getopt.h>

void showHelp();
bool setopts(int, char*[], std::string&, std::string&);
std::string file2string(std::string);
bool dataCheck(std::string, std::string);
int getDataSize(std::string);
bool createData(std::string, std::string);
