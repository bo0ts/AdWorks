#ifndef _USER_H_
#define _USER_H_

#include <list>
#include <string>

//delicious enums outside a namespace
enum Gender {GENDER_NA, GENDER_MALE, GENDER_FEMALE};
enum Age {AGE_NA, AGE_TEEN, AGE_YOUNG, AGE_OLD};

class IUser
{
public:
  virtual ~IUser() {}
  virtual Gender getGender() const = 0;
  virtual Age getAge() const = 0;
  virtual std::list<std::string> getBrowsingHistory() const = 0;
};

class User : public IUser
{
public:
  virtual ~User();
  virtual Gender getGender() const;
  virtual Age getAge() const;
  std::list<std::string> getBrowsingHistory() const;
};


#endif /* _USER_H_ */
