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

namespace po = boost::program_options;

// TODO queryresult operator<<
//      setup frontEnd and backEnd somewhere in main and pass around
//      simplify


//delicious globals
boost::scoped_ptr<IBackEnd> backEnd;
boost::scoped_ptr<IFrontEnd> frontEnd;

//help message
bool help(int argc, char* argv[]) { 
  (void)argc; (void)argv;
  std::cout << "helpful message" << std::endl;
  return true;
}

//visit
bool visit(int argc, char* argv[]) { 
  uint32_t id;

  po::options_description desc("Allowed options for visit");
  desc.add_options()
    ("help", "produce help message")
    ("id", po::value<uint32_t>(&id)->required(), "visit ad with this id\nanonymous option also possible")
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

  std::cout << " visiting id: " << id << " ";
  std::cout << "with landing page: " << frontEnd->getAdURL(id) << std::endl;
  return EXIT_SUCCESS;
}

bool matchad(int argc, char* argv[]) { 
  int age;
  int gender;
  typedef std::vector< std::string > str_vec;
  str_vec queries;

  po::options_description desc("Allowed options for matchad");
  desc.add_options()
    ("help", "produce help message")
    ("q", po::value<str_vec>(&queries)->multitoken()->required(), "match to a list of queries,"
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

  for(str_vec::const_iterator it = queries.begin(); it != queries.end(); ++it) {
    QueryResult query = frontEnd->matchAd(*it);
    std::cout << "Query: " << *it << " returns ad with\n"
	      << "title: " << query.getTitle() << "\n"
	      << "creative: " << query.getCreative() << "\n"
	      << "id: " << query.getAdID() << std::endl;
  }

  return EXIT_SUCCESS;
}

bool reload(int argc, char* argv[]) { 
  std::string adFile, queryFile;

  po::options_description desc("Allowed options for reload");
  desc.add_options()
    ("help", "produce help message")
    ("adfile", po::value<std::string>(&adFile)->required(), 
     "reload with this adfile \nanonymous option also possible")
    ("queryfile", po::value<std::string>(&queryFile)->required(), 
     "reload with this queryfile \nanonymous option also possible")
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
 
  std::cout << "reloading with \n"
	    << "adfile " << adFile << "\n"
	    << "queryfile " << queryFile  << std::endl;
  
  return backEnd->initDatabase(adFile, queryFile);
}


int main(int argc, char* argv[]) {
  //map options into functions from arguments to bool
  
  //so we still can read the beautiful whatS of exceptions
  //try {
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

    frontEnd.reset(new FrontEnd());
    backEnd.reset(new BackEnd());
    frontEnd->setBackend(backEnd.get());

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
  // } catch(...) {
  //   std::cerr << "aborting..." << std::endl;
  //   return EXIT_FAILURE;
  // }
}
