#ifndef _CONFIGPARSER_H_
#define _CONFIGPARSER_H_

#include <map>
#include <boost/tokenizer.hpp>

class Config
{
public:
  Config(std::ifstream& in) {
    if (!in.is_open()) throw std::runtime_error("File not found");

    typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
    boost::char_separator<char> sep("=");
    std::vector< std::string > vec;
    std::string line;
    std::size_t commentId;

  
    while (getline(in,line)){
      commentId=line.find("##");
      if(commentId == std::string::npos) {
	Tokenizer tok(line,sep);
	//unsafe - wrong syntax like 'newlines' will crash
	Tokenizer::iterator it=tok.begin();
	dbConfig.insert(std::pair<std::string, std::string>(*(it++), *(it)));
      }
    }
  }
  
  const std::string& operator[](const std::string& x) const {
    CfgMap::const_iterator it = dbConfig.find(x);
    if(it == dbConfig.end())
      throw std::runtime_error("Unknown config value requested");
    
    return it->second;
  }
private:
  typedef std::map<std::string, std::string> CfgMap;
  CfgMap dbConfig;
};

#endif /* _CONFIGPARSER_H_ */
