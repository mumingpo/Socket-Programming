#include "strexpeval.h"

int str_exp_eval(char * s){
  int left, right;
  char * pos_of_operand;
  
  pos_of_operand = strchr(s, (int) '*');
  if (pos_of_operand != NULL){
    left = atoi(s);
    right = atoi(pos_of_operand + 1);
    return left * right;
  }
  
  pos_of_operand = strchr(s, (int) '/');
  if (pos_of_operand != NULL){
    left = atoi(s);
    right = atoi(pos_of_operand + 1);
    return left / right;
  }
  
  pos_of_operand = strchr(s, (int) '+');
  if (pos_of_operand != NULL){
    left = atoi(s);
    right = atoi(pos_of_operand + 1);
    return left + right;
  }
    pos_of_operand = strchr(s, (int) '-');
  if (pos_of_operand != NULL){
    left = atoi(s);
    right = atoi(pos_of_operand + 1);
    return left - right;
  }
}