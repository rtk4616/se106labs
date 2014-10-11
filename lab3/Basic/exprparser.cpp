#include "exprparser.h"
#include "utility.h"
#include <stdexcept>
using namespace std;

int exprparser::parse_token(const string &str, int &index)
{
  index = skipblank(str, index);
  
  int num = 0;
  string varname;
  if(parseint(str, index, num))
    return num;
  else if(parsevar(str, index, varname))
  {
    varname = stolower(varname);
    auto it = vartable->find(varname);
    if(it == vartable->end())
       throw logic_error("VARIABLE NOT DEFINED");
    return it->second;
  }
  else if(str[index] == '(')
  {
    index++;
    int res = parse_expr(str, index);
    index = skipblank(str, index);
    if(str[index] != ')')
      throw logic_error("SYNTAX ERROR");
    index++;
    return res;
  }
  else throw logic_error("SYNTAX ERROR");

}

int exprparser::parse_sub(const string &str, int &index)
{
  //first token
  int res = 0;
  index = skipblank(str, index);
  res = parse_token(str, index);
  
  int len = str.size();
  while(true)
  {
    index = skipblank(str, index);
    if(index == len || str[index] == ')')
      return res;
    //oper
    char oper = str[index];
    if(oper == '*')
    {
      //token
      index = skipblank(str, index + 1);
      int tmp = parse_token(str, index);
      res *= tmp;
    }
    else if(oper == '/')
    {
      //token
      index = skipblank(str, index + 1);
      int tmp = parse_token(str, index);
      if(tmp == 0)
        throw logic_error("DIVIDE BY ZERO");
      res /= tmp;
    }
    else return res;
  }

}


int exprparser::parse_expr(const string &str, int &index)
{
  //first sub
  index = skipblank(str, index);
  bool minus = (str[index] == '-');
  if(minus) index = skipblank(str, index + 1);
  int res = parse_sub(str, index);
  if(minus) res = -res;

  int len = str.size();
  while(true)
  {
    index = skipblank(str, index);
    if(index == len || str[index] == ')')
      return res;
    //oper
    char oper = str[index];
    if(oper == '+')
    {
      //sub
      index = skipblank(str, index + 1);
      int tmp = parse_sub(str, index);
      res += tmp;
    }
    else if(oper == '-')
    {
      //sub
      index = skipblank(str, index + 1);
      int tmp = parse_sub(str, index);
      res -= tmp;
    }
    else return res;
  }

}  

int exprparser::parse(const string &str, int &index, bool chktail)
{
  int res = parse_expr(str, index);
  if(chktail)
  {
    index = skipblank(str, index);
    if(index != str.size())
      throw logic_error("SYNTAX ERROR");
  }
  return res;
}
