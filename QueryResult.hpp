#ifndef _QUERYRESULT_H_
#define _QUERYRESULT_H_

//std
#include <string>
#include <stdint.h>

class IQueryResult
{
public:
  virtual ~IQueryResult() {}
  virtual std::string getTitle() const = 0;
  virtual std::string getCreative() const = 0;
  virtual uint32_t getAdID() const = 0;
};

class QueryResult : public IQueryResult
{
  virtual std::string getTitle() const;
  virtual std::string getCreative() const;
  virtual uint32_t getAdID() const;
};

#endif /* _QUERYRESULT_H_ */

