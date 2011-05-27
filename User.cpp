#include "User.hpp"
#include <list>
#include <string>


User::~User() {}
User::User(char gender, int age) {    
  switch(gender) {
  case 'f': gender_ = GENDER_FEMALE; break;
  case 'm': gender_ = GENDER_MALE; break;
  case 'n': gender_ = GENDER_NA; break;
  default: gender_ = GENDER_NA; break;
  }
    
  switch(age) {
  case 0: age_ = AGE_TEEN; break;
  case 1: age_ = AGE_YOUNG; break;
  case 2: age_ = AGE_OLD; break;
  case 3: age_ = AGE_NA; break;
  default: age_ = AGE_NA; break;
  }
}

std::list<std::string> User::getBrowsingHistory() const {
  return std::list<std::string>();
}



