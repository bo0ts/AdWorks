#ifndef _BACKEND_H_
#define _BACKEND_H_

#include "QueryResult.hpp"

//std
#include <cstdint>


class IBackEnd
{
public:
  // siehe: IFrontEnd::matchAd
  virtual   IQueryResult    matchAdRewrites(std::list<std::string> rewriteList, 
					    const IUser* user = NULL, 
					    bool* foundAd = NULL) = 0;

  // siehe: IFrontEnd::getAdURL
  virtual std::string getAdURL(uint32_t adID) = 0;

  // Datenbank mit Ads und Bid Phrases initialisieren
  virtual bool initDatabase(const string& adFile, const string& bidPhraseFile) = 0;
};



#endif /* _BACKEND_H_ */
