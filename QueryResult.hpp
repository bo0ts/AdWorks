#ifndef _QUERYRESULT_H_
#define _QUERYRESULT_H_

//std
#include <string>
#include <stdint.h>

class QueryResult
{
public:
  QueryResult(const std::string& title, const std::string& creative, uint32_t adID)
    : title_(title), creative_(creative), adID_(adID) {}
  const std::string& getTitle() const { return title_; }
  const std::string& getCreative() const { return creative_; }
  uint32_t getAdID() const { return adID_; }
private:
  std::string title_;
  std::string creative_;
  uint32_t adID_;
};

#endif /* _QUERYRESULT_H_ */

