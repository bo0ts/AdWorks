#ifndef _FRONTEND_H_
#define _FRONTEND_H_

class IFrontEnd
{
public:
  // ermittelt das am besten passende Ad und erhöht die
  // Impressionsanzahl
  virtual IQueryResult matchAd(std::string   query,   const  IUser*
			       user = NULL, bool* foundAd = NULL) = 0;

  // ermittelt die Landing Page des Ads adID und erhöht seine
  // Klickanzahl
  virtual std::string getAdURL(uint32_t adID) = 0;

  // Verwende des Log in file um den Klick-Graphen zu bauen und
  // ähnliche Queries zu finden
  virtual bool analyzeClickGraph(const std::string& file) = 0;
  // Berechne Wahrscheinlichkeiten demographischer Merkmale von
  // Webseiten
  virtual   bool   analyzeDemographicFeatures(const    std::string& userFile, 
					      const std::string& visitFile) = 0;

  // setze das zu verwendende Backend
  virtual void setBackend(IBackEnd* backend) = 0;
};

class FrontEnd : public IFrontEnd
{
public:
  FrontEnd();
  virtual ~FrontEnd();
};


#endif /* _FRONTEND_H_ */
