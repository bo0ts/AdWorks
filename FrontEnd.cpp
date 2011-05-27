#include "FrontEnd.hpp"

#include <stdexcept>

QueryResult FrontEnd::matchAd(std::string query,   
			      const  IUser* user, 
			      bool* foundAd) {
  std::list<std::string> rewrites;
  rewrites.push_back(query);

  if(backEnd_ != NULL)
    return backEnd_->matchAdRewrites(rewrites, user, foundAd);
  else
    throw std::runtime_error("backEnd is NULL");
}

std::string FrontEnd::getAdURL(uint32_t adID) {
  if(backEnd_ != NULL)
    return backEnd_->getAdURL(adID);
  else
    throw std::runtime_error("backEnd is NULL");
}

bool FrontEnd::analyzeClickGraph(const std::string& file) { return true; }

bool FrontEnd::analyzeDemographicFeatures(const std::string& userFile, 
				const std::string& visitFile)
{return true;}

void FrontEnd::setBackend(IBackEnd* backend) {
  this->backEnd_ = backend;
}
