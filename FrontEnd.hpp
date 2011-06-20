#ifndef _FRONTEND_H_
#define _FRONTEND_H_

//std
#include <string>
#include <stdint.h>

//aufgabe
#include "BackEnd.hpp"
#include "QueryResult.hpp"
#include "User.hpp"

//boost
#include <boost/scoped_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

//sql
#include <cppconn/connection.h>

class IFrontEnd
{
public:
  IFrontEnd() {}
  virtual ~IFrontEnd() {}
  
  // ermittelt das am besten passende Ad und erhöht die
  // Impressionsanzahl

  // XXX will never work this way. IQueryResult is pure virtual, return shared_ptr???

  virtual QueryResult matchAd(std::string   query,   
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

  // setze das zu verwendende Backend
  virtual void setBackend(IBackEnd* backend) = 0;
};



class FrontEnd : public IFrontEnd
{


public:
  FrontEnd(std::ifstream& in);
  virtual ~FrontEnd() {}

  // ermittelt das am besten passende Ad und erhöht die
  // Impressionsanzahl

  virtual QueryResult matchAd(std::string   query,   
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

  // setze das zu verwendende Backend
  virtual void setBackend(IBackEnd* backend);

protected:
	boost::numeric::ublas::vector<int> getVec(int i, int size);



private:
  IBackEnd* backEnd_;
struct	EDGE{
 	std::string query;
	std::string ad;
	int numClicks;
};
	 void parseConfig(std::ifstream& in);
 	 boost::scoped_ptr<sql::Connection> con;
	 bool tablesExist();
	double spread(boost::numeric::ublas::matrix_row<boost::numeric::ublas::matrix<int> > row);
	double weight(boost::numeric::ublas::matrix_column<boost::numeric::ublas::matrix<int> > col, int j);
	int countSameNeighbors(boost::numeric::ublas::matrix<int> a, int i, int j);
};


#endif /* _FRONTEND_H_ */
