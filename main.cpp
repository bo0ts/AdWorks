//boost
#include <boost/program_options.hpp>

//std
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>

//c99
#include <stdint.h>


int main(int argc, char* argv[]) {
  namespace po = boost::program_options;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("visit", po::value<uint32_t>(), "visit the ad with the given id")
    ("matchad", po::value<std::vector<std::string> >()->multitoken(), "match to a list of queries")
    ("reload", po::value<std::vector<std::string> >()->multitoken(), "reload db with sepcified files")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }

  if (vm.count("visit")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }

  if (vm.count("matchad")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }

  if (vm.count("reload")) {
    std::cout << desc << "\n";
    return EXIT_SUCCESS;
  }
}
