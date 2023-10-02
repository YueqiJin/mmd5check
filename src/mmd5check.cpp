// Creator: YueqiJin
// Creation Date: 2023-10-02
// Description: mmd5check.cpp, main file of mmd5check

// version information
#define MMD5CHECK_VERSION_MAJOR 1
#define MMD5CHECK_VERSION_MINOR 0
#define MMD5CHECK_VERSION_PATCH 0
#define MMD5CHECK_PROJECT_VERSION "1.0.0"

//#define DEBUG 1

// import STL library
#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
// import cryptopp
#include <cryptopp/cryptlib.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
// import argparse
#include <argparse/argparse.hpp>
// import boost thread_pool
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

std::filesystem::path input_file, output_file;
int thread_num;

void parse_args(int argc, char **argv) {
	argparse::ArgumentParser program("mmd5check", MMD5CHECK_PROJECT_VERSION, argparse::default_arguments::all);
	program.add_description("mmd5check, a tool to check md5 of files");
	program.add_argument("-i", "--input")
		.metavar("FILE")
		.help("file list to check md5")
		.required();
	program.add_argument("-o", "--output")
		.metavar("FILE")
		.help("output file to save md5 check result")
		.required();
	program.add_argument("-t", "--thread")
		.metavar("THREAD")
		.help("number of threads to check md5")
		.default_value(1)
		.scan<'i', int>();
	program.add_argument("-v", "--version")
		.help("show version information")
		.default_value(false)
		.implicit_value(true);
	program.add_argument("-h", "--help")
		.help("show help information")
		.default_value(false)
		.implicit_value(true);
	try {
		program.parse_args(argc, argv);
		if (program["-v"] == true) {
			std::cout << "mmd5check version " << MMD5CHECK_PROJECT_VERSION << std::endl;
			exit(0);
		}
		if (program["-h"] == true) {
			std::cout << program;
			exit(0);
		}
		std::string input = program.get<std::string>("-i");
		std::string output = program.get<std::string>("-o");
		input_file = std::filesystem::path{ input };
		output_file = std::filesystem::path{ output };
		thread_num = program.get<int>("-t");
	} catch (const std::runtime_error &err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		exit(1);
	}
}

std::string md5sum(const std::filesystem::path &file) {
	CryptoPP::Weak::MD5 hash;
	const size_t size = CryptoPP::Weak::MD5::DIGESTSIZE * 2;
	CryptoPP::byte buf[size] = {0};
	CryptoPP::FileSource(file.string().c_str(), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::ArraySink(buf, size))));
	std::string digest = std::string(reinterpret_cast<char *>(buf), size);
	return digest;
}

int main(int argc, char **argv) {
	parse_args(argc, argv);
	std::cout << "********** mmd5check **********" << std::endl;
	std::cout << "input file: " << input_file << std::endl;
	std::cout << "output file: " << output_file << std::endl;
	std::cout << "thread number: " << thread_num << std::endl;
	std::cout << "********** mmd5check **********" << std::endl;
	std::ifstream input(input_file);
	std::ofstream output(output_file);
	boost::asio::thread_pool pool(thread_num);
	std::string line;
	// input format is same as md5sum -c
	// each line contain a md5 and a file path, saparated by space and "*" at the start of file path
	while (std::getline(input, line)) {
		std::string md5 = line.substr(0, 32);
		// transform lower to upper characters
		std::transform(md5.begin(), md5.end(), md5.begin(), ::toupper);
		std::string file = line.substr(34);
		boost::asio::post(pool, [md5, file, &output]() {
			std::string md5_check = md5sum(file);
			if (md5_check == md5) {
				output << file << ": OK" << std::endl;
				#ifdef DEBUG
				std::cout << md5_check << " " << md5 << ": OK" << std::endl;
				#endif
			} else {
				output << file << ": FAILED" << std::endl;
				#ifdef DEBUG
				std::cout << md5_check << " " << md5 << ": FAILED" << std::endl;
				#endif
			}
		});
	}
	pool.join();
	return 0;
}
