#ifndef _FRONTEND_H_
#define _FRONTEND_H_

//std
#include <string>
#include <stdint.h>
#include <vector>

//aufgabe
#include "BackEnd.hpp"
#include "QueryResult.hpp"
#include "User.hpp"

//boost
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

//sql
#include <cppconn/connection.h>

void lda(std::ifstream& in, std::ofstream& out);

class IFrontEnd
{
public:
  IFrontEnd() {}
  virtual ~IFrontEnd() {}
  
  // ermittelt das am besten passende Ad und erhöht die
  // Impressionsanzahl

  // XXX will never work this way. IQueryResult is pure virtual, return shared_ptr???

  virtual std::vector<std::string> matchAd(const std::string& query,   
					   const  IUser* user = NULL, 
					   bool* foundAd = NULL) = 0;
  
  // ermittelt die Landing Page des Ads adID und erhöht seine
  // Klickanzahl
  virtual std::string getAdURL(uint32_t adID) = 0;

  // Verwende des Log in file um den Klick-Graphen zu bauen und
  // ähnliche Queries zu finden
  virtual bool analyzeClickGraph(const std::string& file) = 0;

  // Berechne Wahrscheinlichkeiten demographischer Merkmale von
  // Webseiten
  virtual bool analyzeDemographicFeatures(const std::string& userFile, 
					  const std::string& visitFile) = 0;

  virtual void setBackend(IBackEnd* backend) = 0;
};



class FrontEnd : public IFrontEnd
{
public:
  FrontEnd(boost::shared_ptr<sql::Connection> con_) : con(con_), backEnd_(NULL) { 
  }

  virtual ~FrontEnd() {}

  // ermittelt das am besten passende Ad und erhöht die
  // Impressionsanzahl

  virtual std::vector<std::string> matchAd(const std::string& query,   
					   const  IUser* user = NULL, 
					   bool* foundAd = NULL);

  // ermittelt die Landing Page des Ads adID und erhöht seine
  // Klickanzahl
  virtual std::string getAdURL(uint32_t adID);

  // Verwende des Log in file um den Klick-Graphen zu bauen und
  // ähnliche Queries zu finden
  virtual bool analyzeClickGraph(const std::string& file);
  // Berechne Wahrscheinlichkeiten demographischer Merkmale von
  // Webseiten
  virtual bool analyzeDemographicFeatures(const std::string& userFile, 
					  const std::string& visitFile);

  virtual void setBackend(IBackEnd* backend) {
    this->backEnd_ = backend;
  }
private:
  struct EDGE {
    std::string query;
    std::string ad;
    int numClicks;
  };

  boost::shared_ptr<sql::Connection> con;
  boost::numeric::ublas::vector<int> getVec(int i, int size);
  IBackEnd* backEnd_;

  bool tablesExist();
  double spread(boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<double> > row);
  double weight(boost::numeric::ublas::matrix_column<boost::numeric::ublas::matrix<double> > col, int j);
  int countSameNeighbors(boost::numeric::ublas::matrix<double> a, int i, int j);
};


// devilish hackery 

// we just keep all the analyzeClickGraph junk
// getAdURL junk and only overwrite matchAd
// eat this liskov substitution
class LDAFrontEnd : public FrontEnd {
  virtual ~LDAFrontEnd() {}

  // ermittelt das am besten passende Ad und erhöht die
  // Impressionsanzahl
  virtual std::vector<std::string> matchAd(const std::string& query,   
					   const  IUser* user = NULL, 
					   bool* foundAd = NULL);
};

#endif /* _FRONTEND_H_ */
