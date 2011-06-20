#include "BackEnd.hpp"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

#include <boost/lexical_cast.hpp>


#include <stdexcept>


using namespace std;
using namespace boost;

BackEnd::BackEnd(std::ifstream& in) {
  parseConfig(in);
}

QueryResult BackEnd::matchAdRewrites(std::list<std::string> rewriteList, 
			    const IUser* user, 
			    bool* foundAd) {
  (void)foundAd;
  boost::scoped_ptr<sql::PreparedStatement> pstmt;
  boost::scoped_ptr<sql::ResultSet> rs;

  //precond
  if(rewriteList.empty()) throw std::runtime_error("Empty rewrite list");
  
  try {
    if(user == NULL) {
      //no user, just query
      pstmt.reset(con->prepareStatement("SELECT Titel, Slogan, a.AdID FROM (Ads as a JOIN Queries as q ON a.AdID = q.AdID) WHERE Bid_Phrase LIKE ? OR Bid_Phrase LIKE ? OR Bid_Phrase LIKE ? OR Bid_Phrase LIKE ? OR Bid_Phrase LIKE ? OR Bid_Phrase LIKE ? ORDER BY (Anzahl_Klicks / Anzahl_Impressions)*Gebot DESC"));
      pstmt->setString(1, rewriteList.front());
      int count=2;
      std::list<std::string>::iterator it;
      for(it=++(rewriteList.begin());it!=rewriteList.end();it++){
      		pstmt->setString(count, *it);
		count++;
	}
	for(count=count;count<7;count++){
		pstmt->setString(count, *it);
	}
		
		

    } else {
      //gender, dance the user dance
      //determine the gender of the user
      string gender("na");
      switch(user->getGender()) {
      case GENDER_MALE: gender = "male"; break;
      case GENDER_FEMALE: gender = "female" ; break;
      default: break;
      }

      string age("na");
      switch(user->getAge()) {
      case AGE_TEEN: age = "teen"; break;
      case AGE_YOUNG: age = "young"; break;
      case AGE_OLD: age = "old"; break; 
      default: break;
      }
    
      //you don't see this
      age = "%" + age + "%";

      pstmt.reset(con->prepareStatement("SELECT Titel, Slogan, a.AdID FROM (Ads as a JOIN Queries as q ON a.AdID = q.AdID) WHERE Bid_Phrase LIKE ? AND Gender = ? AND Age LIKE ? ORDER BY (Anzahl_Klicks / Anzahl_Impressions)*Gebot DESC"));
      pstmt->setString(1, rewriteList.front());
      pstmt->setString(2, gender);
      pstmt->setString(3, age);
    }
    rs.reset(pstmt->executeQuery());

    //we only care about the first result, if there is no first result, return the dummy
    if(!(rs->next()))
      return QueryResult("dummy", "dummy", 23);

    //good to go, we increment the anzahl_impressions where the adid
    //equals the delivered id 
    //just in case the reader is braindead...
    pstmt.reset(con->prepareStatement("UPDATE Ads SET Anzahl_Impressions=Anzahl_Impressions+1 WHERE AdID = ?"));
    pstmt->setInt(1, rs->getInt(3));
    pstmt->executeUpdate();

    return QueryResult(rs->getString(1) ,rs->getString(2) ,rs->getInt(3));

  } catch(sql::SQLException &e) {
    cout << "in BackEnd::matchAdRewrites: " << e.getErrorCode() << endl;
    throw e;
  } 


}

void BackEnd::parseConfig(std::ifstream& in) {
  if (!in.is_open()) throw std::runtime_error("File not found");

  typedef tokenizer< char_separator<char> > Tokenizer;
  char_separator<char> sep("=");
  vector< string > vec;
  string line;
  size_t commentId;
  map<string,string> dbConfig;
  
  while (getline(in,line)){
    commentId=line.find("##");
    if (commentId==string::npos){       
      Tokenizer tok(line,sep);
      //unsafe - wrong syntax like 'newlines' will crash
      Tokenizer::iterator it=tok.begin();
      dbConfig.insert(std::pair<string, string>(*(it++), *(it)));
    }
  }

  sql::Driver* driver = get_driver_instance();
  con.reset(driver->connect(dbConfig["Server"]+":"+dbConfig["Port"], dbConfig["User"], dbConfig["Password"]));
  con->setSchema("AdWorks");
}


  // siehe: IFrontEnd::getAdURL
std::string BackEnd::getAdURL(uint32_t adID) {
  boost::scoped_ptr<sql::PreparedStatement> pstmt;
  boost::scoped_ptr<sql::ResultSet> rs;
  try {
    pstmt.reset(con->prepareStatement("SELECT URL FROM Ads WHERE AdID = ?"));
    pstmt->setInt(1, adID);
    rs.reset(pstmt->executeQuery());

    //adID is a primary key, this works
    if(!(rs->next())) return string("AdID NOT FOUND");

    pstmt.reset(con->prepareStatement("UPDATE Ads SET Anzahl_Klicks=Anzahl_Klicks+1 WHERE AdID = ?"));
    pstmt->setInt(1, adID);
    pstmt->executeUpdate();

    return rs->getString(1);
  } catch(sql::SQLException &e) {
    cout << "in BackEnd::getAdURL: " << e.getErrorCode() << endl;
    throw e;
  } 
}

bool BackEnd::initDatabase(const std::string& adFile, const std::string& bidPhraseFile) {
  boost::scoped_ptr<sql::PreparedStatement> pstmt;
  typedef tokenizer< escaped_list_separator<char> > Tokenizer;
  escaped_list_separator<char> sep("\\", " \t", "\"");

  if(tablesExist()) {
    //drop all
    try {
      pstmt.reset(con->prepareStatement("DROP TABLE Queries"));
      pstmt->executeUpdate();
      pstmt.reset(con->prepareStatement("DROP TABLE Ads"));
      pstmt->executeUpdate();
    } catch(sql::SQLException &e) {
      cout<<"in BackEnd::initDatabase whiledropping: " <<e.getErrorCode()<<endl;
      throw e;
    } 
  }
  
  //create all
  try {
    pstmt.reset(con->prepareStatement("CREATE TABLE Queries (AdID INT, Bid_Phrase VARCHAR(60), Gebot FLOAT,  PRIMARY KEY (AdID, Bid_Phrase))"));
    pstmt->executeUpdate();
    
    pstmt.reset(con->prepareStatement("CREATE TABLE Ads\
 (AdID INT PRIMARY KEY, Titel VARCHAR(1024), Slogan VARCHAR(1024), URL VARCHAR(1024),	\
 Anzahl_Impressions INT, Anzahl_Klicks INT, Gender ENUM('male','female','na'), Age SET('old','teen','young','na'))"));
    pstmt->executeUpdate();
  } catch(sql::SQLException &e) {
    cout<<"in BackEnd::initDatabase while creating: " << e.getErrorCode() << endl;
    throw e;
  }

  {
    ifstream in(bidPhraseFile.c_str());
    if (!in.is_open()) throw;
    string line;

    try{
      pstmt.reset(con->prepareStatement("INSERT INTO Queries(AdID, Bid_Phrase,Gebot) VALUES (?, ?, ?)"));
      while (getline(in,line)){
	Tokenizer tok(line,sep);
			
	Tokenizer::iterator tok_iter=tok.begin();

	pstmt->setInt(1, lexical_cast<int>(*(tok_iter++)));
	pstmt->setString(2, *(tok_iter++));
	pstmt->setDouble(3, lexical_cast<double>(*(tok_iter++)));
	pstmt->executeUpdate();	
      }	

    }catch(sql::SQLException &e) {
      cout << "in BackEnd::initDatabase while inserting queries: " << e.getErrorCode() << endl;
      throw e;
    }
  }
  {
    ifstream in(adFile.c_str());
    if (!in.is_open()) throw;
    string line;

    try{
      pstmt.reset(con->prepareStatement("INSERT INTO Ads(AdID, Titel, Slogan, URL, Anzahl_Impressions, Anzahl_Klicks, Gender, Age) VALUES (?, ?, ?, ?, ? ,?, ? ,?)"));
      while (getline(in,line)){
	Tokenizer tok(line,sep);
			
	Tokenizer::iterator tok_iter=tok.begin();

	pstmt->setInt(1, lexical_cast<int>(*(tok_iter++)));
	pstmt->setString(2, *(tok_iter++));
	pstmt->setString(3, *(tok_iter++));	
	pstmt->setString(4, *(tok_iter++));		
	pstmt->setInt(5, lexical_cast<int>(*(tok_iter++)));
	pstmt->setInt(6, lexical_cast<int>(*(tok_iter++)));
	pstmt->setString(7, *(tok_iter++));
	pstmt->setString(8, *(tok_iter));	
	pstmt->executeUpdate();	
      }    		
    }catch(sql::SQLException &e) {
      cout << "in BackEnd::initDatabase while inserting ads: " << e.getErrorCode() << endl;
      throw e;
    }
  }
  
  return true;
}

bool BackEnd::tablesExist() {
  try{
    scoped_ptr<sql::PreparedStatement> pstmt;
    scoped_ptr<sql::ResultSet> rs;

    pstmt.reset(con->prepareStatement("SELECT table_name "
				      "FROM information_schema.tables "
				      "WHERE table_schema = 'AdWorks' "
				      "AND table_name = 'Queries';"));
    //you won't see it
    rs.reset(pstmt->executeQuery());
    return rs->next();
  } catch (sql::SQLException &e) {
    cout<<"in BackEnd::tablesExist: " <<e.getErrorCode()<<endl;
    throw e;
  }
}
