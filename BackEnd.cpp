#include "BackEnd.hpp"

QueryResult BackEnd::matchAdRewrites(std::list<std::string> rewriteList, 
			    const IUser* user, 
			    bool* foundAd) {

  return QueryResult("dummy", "dummy", 23);
}


  // siehe: IFrontEnd::getAdURL
std::string BackEnd::getAdURL(uint32_t adID) {
  return "";
}

bool BackEnd::initDatabase(const std::string& adFile, const std::string& bidPhraseFile) {
  return true;
}
