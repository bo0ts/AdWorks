//boost
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/assign.hpp>


//std
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <tr1/functional>


//c99
#include <stdint.h>

//lib
#include "BackEnd.hpp"
#include "FrontEnd.hpp"

//help message
bool help(int argc, char* argv[]) { 
  (void)argc; (void)argv;
  std::cout << "helpful message" << std::endl;
  return true;
}

bool visit(int argc, char* argv[]) { 
  namespace po = boost::program_options;

  po::options_description desc("Allowed options for reload");
  desc.add_options()
    ("help", "produce help message")
    ("id", po::value<uint32_t>(), "visit ad with this id\nanonymous option also possible")
    ;

  po::positional_options_description p;
  p.add("id", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
	    options(desc).positional(p).run(), vm);

  po::notify(vm);
  
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  if (vm.count("id")) {
    std::cout << " visiting id: "
	      << vm["id"].as<uint32_t>();

    return EXIT_SUCCESS;
  }

  return true;
}

bool matchad(int argc, char* argv[]) { 
  namespace po = boost::program_options;

  po::options_description desc("Allowed options for reload");

  int age;
  int gender;

  desc.add_options()
    ("help", "produce help message")
    ("q", po::value<std::vector<std::string> >()->multitoken(), "match to a list of queries,"
     "\nseparate queries in \" or whatever pleases your shell")
    ("a", po::value<int>(&age)->default_value(0), "optional age parameter, defaults to 0")
    ("g", po::value<int>(&gender)->default_value(0), "optional gender parameter, defaults to 0");
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  boost::scoped_ptr<FrontEnd> frontEnd(new FrontEnd());
  boost::scoped_ptr<BackEnd> backEnd(new BackEnd());
  frontEnd->setBackend(backEnd.get());
  return true;
}

bool reload(int argc, char* argv[]) { 
  namespace po = boost::program_options;

  po::options_description desc("Allowed options for reload");
  desc.add_options()
    ("help", "produce help message")
    ("adfile", po::value<std::string>(), "reload with this adfile \nanonymous option also possible")
    ("queryfile", po::value<std::string>(), "reload with this queryfile \nanonymous option also possible")
    ;

  po::positional_options_description p;
  p.add("adfile", 1);
  p.add("queryfile", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
	    options(desc).positional(p).run(), vm);
  po::notify(vm);


  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }
 
  if(vm.count("adfile") == 0 || vm.count("queryfile") == 0) {
    std::cout << "reload requires two file args" << std::endl;
    return EXIT_FAILURE;
  } 
  std::cout << "reloading with "
	    << "adfile " << vm["adfile"].as<std::string>() << std::endl
	    << "queryfile " << vm["queryfile"].as<std::string>() << std::endl;

  return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {
  //map options into functions from arguments to bool
  typedef std::tr1::function<bool (int argc, char* argv[])> OptionFunction;
  typedef std::map<std::string, OptionFunction> OptionMap;
  OptionMap options;
  
  boost::assign::insert(options)
    ( "help",    &help )( "visit",  &visit )
    ( "matchad", &matchad )( "reload", &reload );

  if(argc < 2) { 
    std::cout << "no option specified" << std::endl;
    return EXIT_FAILURE;
  }

  std::string option(argv[1]);

  OptionMap::iterator it = options.find(option);
  if(it != options.end()) {
    //execute the matched function with argv reduced by the program
    //name and argc decremmented by one
    return (it->second)(--argc, ++argv);
  } else {
    std::cout << "unknown option: `" << option << "'"  << std::endl;
    return EXIT_FAILURE;
  }
}
