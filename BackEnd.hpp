#ifndef _BACKEND_H_
#define _BACKEND_H_

#include "QueryResult.hpp"
#include "User.hpp"


//std
#include <stdint.h>



class IBackEnd
{
public:
  virtual ~IBackEnd() {}

  // siehe: IFrontEnd::matchAd

  // XXX see why this wont fucking work

  virtual QueryResult matchAdRewrites(std::list<std::string> rewriteList, 
   				       const IUser* user = NULL, 
   				       bool* foundAd = NULL) = 0;

  // siehe: IFrontEnd::getAdURL
  virtual std::string getAdURL(uint32_t adID) = 0;

  // Datenbank mit Ads und Bid Phrases initialisieren
  virtual bool initDatabase(const std::string& adFile, const std::string& bidPhraseFile) = 0;
};

class BackEnd : public IBackEnd
{
public:
  virtual ~BackEnd() {}
  virtual QueryResult matchAdRewrites(std::list<std::string> rewriteList, 
				      const IUser* user = NULL, 
				      bool* foundAd = NULL);

  // siehe: IFrontEnd::getAdURL
  virtual std::string getAdURL(uint32_t adID);

  // Datenbank mit Ads und Bid Phrases initialisieren
  virtual bool initDatabase(const std::string& adFile, const std::string& bidPhraseFile);

};

#endif /* _BACKEND_H_ */
