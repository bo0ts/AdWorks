#ifndef _USER_H_
#define _USER_H_

//delicious enums outside a namespace
enum Gender {GENDER_NA, GENDER_MALE, GENDER_FEMALE};
enum Age {AGE_NA, AGE_TEEN, AGE_YOUNG, AGE_OLD};

class IUser
{
      public:
            Gender getGender() const = 0;
            Age getAge() const = 0;
            std::list<std::string> getBrowsingHistory() const = 0;
};

class User : public IUser
{
public:
  User();
  virtual ~User();
  Gender getGender() const = 0;
  Age getAge() const = 0;
  std::list<std::string> getBrowsingHistory() const = 0;
};


#endif /* _USER_H_ */
