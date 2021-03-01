#include <iostream>
#include <fstream>
#include <filesystem>
#include <fmt/format.h>
#include <argparse.hpp>
#include "shared/compressor.h"

namespace {

bool checkFileExists(const std::string& path) {
  return std::filesystem::exists(path);
}


std::vector<char> readFileToMemory(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if (file.read(buffer.data(), size))
  {
      return buffer;
  }

  throw new std::runtime_error(fmt::format("Couldn't read file '{}'", path));
}


void writeMemoryToFile(const std::vector<char>& data, const std::string& path) {
   std::ofstream fout(path, std::ios::binary | std::ios::out);
   fout.write(data.data(), data.size());

   if (fout.bad()) {
     throw new std::runtime_error(fmt::format("Couldn't save to file '{}'", path));
   }
}

void runSomeCryption(const std::string& cmd, const std::string& in, const std::string& out) {
    if (!checkFileExists(in)) {
      throw std::runtime_error(fmt::format("Cannot access input file '{}'", in));
    }
    if (checkFileExists(out)) {
      throw std::runtime_error(fmt::format("Output file '{}' already exists. Exitting cowardly...", out));
    }

    Compressor cr;
    const std::vector<char> inData = readFileToMemory(in);

    if (cmd == "compress") {
      std::cout << fmt::format("Compressing '{}' into '{}'", in, out) << std::endl;

      std::vector<char> outData(cr.maxCompressedSize(inData.size()), 0);
      auto compressedBytes = cr.compress(inData.data(), inData.size(),
                                               outData.data(), outData.size());
      if (0 == compressedBytes) {
        throw std::runtime_error("Something went wrong during compression");
      }

      outData.resize(compressedBytes);
      std::cout << "Success!";
      writeMemoryToFile(outData, out);
    }
    else if (cmd == "uncompress") {
      std::cout << fmt::format("Uncompressing '{}' into '{}'", in, out) << std::endl;

      std::vector<char> outData(inData.size() * 2, 0);//TODO: we can't guarantee it's gonna be only 2x larger
      const auto uncompressedBytes = cr.decompress(inData.data(), inData.size(),
                                                 outData.data(), outData.size());
      if (0 == uncompressedBytes) {
        throw std::runtime_error("Something went wrong during decompression");
      }

      outData.resize(uncompressedBytes);
      std::cout << "Success!";
      writeMemoryToFile(outData, out);
    }
    else {
      throw std::runtime_error("Unrecognized command " + cmd);
    }

}

}

int main(int argc, char* argv[]) {
    
    argparse::ArgumentParser program;

    program.add_argument("-c", "--command")
      .required()
      .help("Command to invoke: ['compress', 'uncompress']");

    program.add_argument("--in")
      .required()
      .help("Input file");

    program.add_argument("--out")
      .required()
      .help("Output file");

    try {
      program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << program;

      return 1;
    }

    const auto cmd = program.get("--command");
    const auto in = program.get<std::string>("--in");
    const auto out = program.get<std::string>("--out");

    try {
      runSomeCryption(cmd, in, out);
    }
    catch (const std::runtime_error& err) {
      std::cerr << "Error while executing: " << err.what() << std::endl;
      return 2;
    }
}
