#ifndef _USER_H_
#define _USER_H_

#include <list>
#include <string>

#include <iostream>

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
  User(char gender, int age);
  virtual Gender getGender() const { std::cout << "get gender" << std::endl; return gender_; }
  virtual Age getAge() const { std::cout << "get age" << std::endl; return age_; }
  std::list<std::string> getBrowsingHistory() const;
private:
  Gender gender_;
  Age age_;
};


#endif /* _USER_H_ */
