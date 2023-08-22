
#include <iostream>
#include <cstring>
#include <vector>
#include <memory>

/**
 * \author Uğur Can HOŞSÖZ
 * \author Sema SERVİ
*/

// First order derivative operations.

// Differantial Transformation Method

namespace DifferantialOperations
{

  const std::vector<std::string> SPECIAL_KEYS = {"f", "d", "p", "e", "cos", "sin", "cosh", "sinh", "frac", "i"};

  const std::vector<char> OPERATORS = {'+', '-', '*'};

  const std::vector<char> INVALID_OPERANDS = {'*', '|', '{', '}', '&', '%', '\'', '!', '"', '$', '[', ']', '\\', '_', '@', '~', ',', ';', '.', ':', '<', '>', '='};

  struct specialKey                                 // E.g. "/d{/f{x,u}, }"
  {
    std::string key_function;                           
    std::string nested_expression;  
    std::pair<int,int> scope_of_special_key;       
    std::vector<std::shared_ptr<specialKey>> nested_special_key;
  };

  std::string removeSpaces(const std::string& str) {
    std::string result;
    result.reserve(str.size()); // Reserve space for efficiency

    for (char c : str) {
        if (c != ' ') {
            result.push_back(c);
        }
    }

    return result;
  }

  std::string extractSubstring(const std::string& input) {
    size_t start = input.find('{');
    if (start == std::string::npos) {
        return "";  // No opening '{' found
    }

    size_t end = input.rfind('}');
    if (end == std::string::npos || end <= start) {
        return "";  // No closing '}' found or in wrong position
    }

    int count = 0;
    for (size_t i = start + 1; i < input.length(); ++i) {
        if (input[i] == '{') {
            count++;
        } else if (input[i] == '}') {
            if (count == 0) {
                return input.substr(start + 1, i - start - 1);
            } else {
                count--;
            }
        }
    }

    return "";  // No valid inner substring found
  }

  std::vector<int> strFind(std::string string_data, char character)
  {
    //The function continues by iterating over the characters of the input string and counting the occurrences of the given substring.
    std::vector<int> index_holder;
    for (size_t i = 0; i < string_data.length(); i++)
    {
      if (string_data[i] == character)
      {
        index_holder.push_back(i);
      }
    }
    return index_holder;
  }

  std::vector<std::string> strSplit(std::string string_data, std::string splitter, bool ignoreBracket = false)
  {
      std::vector<std::string> string_array;

      if (ignoreBracket)
      {
          std::string segment;
          int depth = 0;
          for (size_t i = 0, start = 0; i < string_data.length(); ++i) {
              if (string_data[i] == '{') {
                  ++depth;
              }
              else if (string_data[i] == '}') {
                  --depth;
              }
              else if (depth == 0 && string_data.substr(i, splitter.length()) == splitter) {
                  string_array.push_back(string_data.substr(start, i - start));
                  i += splitter.length() - 1;
                  start = i + 1;
              }
              else if (i == string_data.length() - 1) {
                  string_array.push_back(string_data.substr(start, i - start + 1));
              }
          }
      }
      else
      {
          char* char_array = new char[string_data.length() + 1];
          std::strcpy(char_array, string_data.c_str());

          char* tmp_array = std::strtok(char_array, splitter.c_str());
          while (tmp_array)
          {
              string_array.push_back(tmp_array);
              tmp_array = std::strtok(nullptr, splitter.c_str());
          }
          delete[] char_array;
      }

      return string_array;
  }

  class DifferantialTransformation
  {
    
  };

  class DifferantialExpression
  {
  protected:
    std::shared_ptr<specialKey> expression;

  public:
    DifferantialExpression(std::shared_ptr<specialKey> expression_) : expression(expression_) {
    
    }
    virtual std::string getType() const = 0;  // Pure virtual function
    virtual ~DifferantialExpression() {}      // Virtual destructor
  };

  class Scalar : public DifferantialExpression{
  public:
    
    Scalar(std::shared_ptr<specialKey> expression_) : DifferantialExpression(expression_){
      scalar_value = expression_->key_function;
    }
    std::string getValue(){
      return scalar_value;
    }

    std::string getType()  const override {
      return "Scalar";
    }

  private:
    std::string scalar_value;
  };

  class Operator : public DifferantialExpression
  {
  public:
    Operator(std::shared_ptr<specialKey> expression_) : DifferantialExpression(expression_)
    {
      if (!(expression_->key_function == "+" || expression_->key_function == "-" ||
          expression_->key_function == "*")){
        throw std::invalid_argument("Operator object must have '+', '-' or '*' keyword. (Which handled internally, so if you get this error, there must be a logical error!");
      }
      operator_key = expression_->key_function;
    }

    std::string operatorKey(){
      return operator_key;
    }

    std::string getType() const override{
      return "Operator";
    }

  private:
    std::string operator_key;
  };

  class Function : public DifferantialExpression
  {
  public:

    Function(std::shared_ptr<specialKey> expression_) : DifferantialExpression(expression_)
    {
      if (expression_->key_function != "f"){
        throw std::invalid_argument("Function object must have 'f' keyword!");
      }


      std::string nested_expression = expression_->nested_expression;
      std::vector<std::string> splitted_nested_expression = strSplit(nested_expression, ",");
      function_key = splitted_nested_expression.at(0);
      for (int i = 1; i < splitted_nested_expression.size(); i++){
        derivative_keys.push_back(splitted_nested_expression.at(i));
      }
    }

    std::string functionKey(){
      return function_key;
    }

    std::vector<std::string> derivativeKeys(){
      return derivative_keys;
    }

    std::string getType() const override{
      return "Function";
    }

  private:
    std::string function_key;
    std::vector<std::string> derivative_keys;
  };


  class Derivative : public DifferantialExpression
  {
  public:

    Derivative(std::shared_ptr<specialKey> expression_) : DifferantialExpression(expression_)
    {
      if (expression_->key_function != "d"){
        throw std::invalid_argument("Derivative object must have 'd' keyword!");
      }

      int nested_expression_counter = 0;

      
      std::vector splitted_expression = strSplit(expression_->nested_expression, ",", true);

      if (splitted_expression.at(0).at(0) == '/'){
        // If first expression is an DifferentialExpression
        std::shared_ptr<specialKey> nested_expression = expression_->nested_special_key.at(nested_expression_counter);
        nested_expression_counter++;
        
        if (nested_expression->key_function == "d"){
          derived_expression = std::make_shared<Derivative>(nested_expression);
        }
        else if (nested_expression->key_function == "f"){
          derived_expression = std::make_shared<Function>(nested_expression);
        }
      }

      else{
        // If first expression is scalar
        std::shared_ptr<specialKey> expr = std::make_shared<specialKey>();
        expr->key_function = splitted_expression.at(0);
        derived_expression = std::make_shared<Scalar>(expr);  
      }

      for (int i = 1; i < splitted_expression.size(); i++){
        if (splitted_expression.at(i).at(0) == '/'){
          std::shared_ptr<specialKey> nstd_expr = expression_->nested_special_key.at(nested_expression_counter);
          nested_expression_counter++;
          
          if (nstd_expr->key_function == "d"){
            throw std::invalid_argument("Derivative key cannot be another derivative!");
          }
          else if (nstd_expr->key_function == "f"){
            std::shared_ptr<DifferantialExpression> function = std::make_shared<Function>(nstd_expr);
            derivative_keys.push_back(function);
          }
        }
        else{
          std::shared_ptr<specialKey> expr = std::make_shared<specialKey>();
          expr->key_function = splitted_expression.at(i);
          std::shared_ptr<DifferantialExpression> scalar = std::make_shared<Scalar>(expr);
          derivative_keys.push_back(scalar);
        }
      }
    }

    std::shared_ptr<DifferantialExpression> derivedExpression(){
      return derived_expression;
    }

    std::vector<std::shared_ptr<DifferantialExpression>> derivativeKeys(){
      return derivative_keys;
    }

    std::string getType() const override{
      return "Derivative";
    }

  private:
    std::shared_ptr<DifferantialExpression> derived_expression;
    std::vector<std::shared_ptr<DifferantialExpression>> derivative_keys;

  };


  class StringDerivative
  {
    public:
      StringDerivative(std::string string_derivative_)
      {
        std::vector<std::shared_ptr<DifferantialExpression>> differential_expressions;

        
        std::string string_derivative = removeSpaces(string_derivative_);
        std::vector<std::vector<std::shared_ptr<specialKey>>> specialKeyVector;

        for (auto &splitted_expression: splitOuterOperator(string_derivative)) {
          std::cout << "Splitted Exp: " << splitted_expression << std::endl;
          bool is_operator = false;
          for (char valid_operator : OPERATORS){
            if (splitted_expression.length() > 0){
              if (splitted_expression.at(0) == valid_operator){
              is_operator = true;
              }
            }
          }
          if (is_operator){
            std::shared_ptr<specialKey> special_key = std::make_shared<specialKey>(); 
            special_key->key_function = splitted_expression;
            std::shared_ptr<Operator> operatorPtr;
            operatorPtr = std::make_shared<Operator>(special_key);
            differential_expressions.push_back(operatorPtr);
          }
          else{
            std::vector<std::shared_ptr<specialKey>> special_key = getSpecialKey(splitted_expression);
            if(special_key.size() > 0){
              if (special_key.at(0)->key_function == "d"){
                std::shared_ptr<Derivative> derivative;
                derivative = std::make_shared<Derivative>(special_key.at(0));
                differential_expressions.push_back(derivative);
              }
              else if(special_key.at(0)->key_function == "f"){
                std::shared_ptr<Function> function;
                function = std::make_shared<Function>(special_key.at(0));
                differential_expressions.push_back(function);
              }
            }
            else{
              std::shared_ptr<specialKey> special_key = std::make_shared<specialKey>(); 
              special_key->key_function = splitted_expression;
              std::shared_ptr<Scalar> scalar;
              scalar = std::make_shared<Scalar>(special_key);
              differential_expressions.push_back(scalar);
            }
          }
        }

        // Add loop to print here!
        for (auto differential_expression : differential_expressions){
          std::cout << differential_expression->getType() << std::endl;
        }
      }

    private:

      bool isOutOfBrackets(int index, const std::string& expression)
      {
        int negative_direction_right_bracket_counter = 0;
        int negative_direction_left_bracket_counter = 0;
        int positive_direction_right_bracket_counter = 0;
        int positive_direction_left_bracket_counter = 0;

        for (int i = index; i >= 0; i--)

        {
          if (expression.at(i) == '{') negative_direction_left_bracket_counter++;
          else if (expression.at(i) == '}') negative_direction_right_bracket_counter++;
        }

        for (int i = index; i < expression.length(); i++)
        {
          if (expression.at(i) == '{') positive_direction_left_bracket_counter++;
          else if (expression.at(i) == '}') positive_direction_right_bracket_counter++;
        }
        if (negative_direction_left_bracket_counter == negative_direction_right_bracket_counter && 
            positive_direction_left_bracket_counter == positive_direction_right_bracket_counter)
            return true;
        else
            return false;
      }

      std::vector<std::string> splitOuterOperator(const std::string& expression)
      { 
        std::vector<std::pair<char,int>> operator_vector;
        int potential_operator_index = 0;
        for (char potential_operator : expression)
        {
          for (char valid_operator : OPERATORS)
          {
            if (potential_operator == valid_operator)
            {
              if (isOutOfBrackets(potential_operator_index, expression))
              {
                operator_vector.push_back(std::pair<char,int>(potential_operator, potential_operator_index));
              }
            }
          }
          potential_operator_index++;
        }

        for (auto &confirmed_operator : operator_vector)
        {
          if (confirmed_operator.second == expression.length() - 1)
            throw std::invalid_argument("An operand is required after an operator!");

          for (char invalid_operand: INVALID_OPERANDS)
          {
            if (expression.at(confirmed_operator.second + 1) == invalid_operand)
              throw std::invalid_argument("An operand cannot be a special character!");
          }          
        }

        std::vector<std::string> tmp_splitted_by_operator;
        std::string copy_expression = expression;

        for (auto &confirmed_operator : operator_vector)
        {
          copy_expression.at(confirmed_operator.second) = '?';
        }

        tmp_splitted_by_operator = strSplit(copy_expression, "?");

        std::vector<std::string> splitted_by_operator;

        bool is_first_char_operator = false;
        for (char valid_operator : OPERATORS){
          if (expression.at(0) == valid_operator) 
            is_first_char_operator = true;
        }
        
        if (is_first_char_operator) 
          splitted_by_operator.push_back(std::string(1,operator_vector.at(0).first));


        for (int i = 0; i < tmp_splitted_by_operator.size(); i++)
        {
            splitted_by_operator.push_back(tmp_splitted_by_operator.at(i));
            if (is_first_char_operator)
              splitted_by_operator.push_back(std::string(1,operator_vector.at(i+1).first));
            else
            {
              if (operator_vector.size() > i)
                splitted_by_operator.push_back(std::string(1,operator_vector.at(i).first));
            }
        }

        return splitted_by_operator;
      }

      bool isValidKey(const std::string& key_function) {
        for (auto &valid_expression: SPECIAL_KEYS) {
          if (key_function == valid_expression) {
            return true;
          }
        }
        return false;
      }

    std::shared_ptr<specialKey> createSpecialKey(const std::string& expression, int special_key_index) {
      std::shared_ptr<specialKey> special_key_struct = std::make_shared<specialKey>(); 

      std::string tmp_special_key = expression.substr(special_key_index + 1, expression.length());

      if (strFind(tmp_special_key,'{').size() == 0)
        throw std::invalid_argument("All special expressions starting with '/' should encapsulate including data with brackets '{}'!");
      else if (strFind(tmp_special_key,'/').size() != 0 && strFind(tmp_special_key,'/').at(0) < strFind(tmp_special_key,'{').at(0))
        throw std::invalid_argument("All special expressions starting with '/' should encapsulate including data with brackets '{}'!");

      std::string key_function = tmp_special_key.substr(0,strFind(tmp_special_key,'{').at(0));

      if (!isValidKey(key_function))
        throw std::invalid_argument(std::string("The given '"+ key_function + "' key is not a valid key!\n\t\tAcceptable keys are: f, d, i, p, e, cos, sin, cosh, sinh, frac"));

      std::string nested_expression = extractSubstring(tmp_special_key);

      int starting_point = special_key_index + strFind(tmp_special_key, '{').at(0) + 1;
      int end_point = starting_point + 1 + nested_expression.length() + 1;

      special_key_struct->key_function = key_function;
      special_key_struct->nested_expression = nested_expression;
      special_key_struct->scope_of_special_key = std::pair<int,int>(starting_point, end_point);

      return special_key_struct;
    }

    std::vector<std::shared_ptr<specialKey>> getSpecialKey(const std::string &expression){
      std::vector<std::shared_ptr<specialKey>> expression_tree;

      std::vector<int> index_of_special_keys = strFind(expression, '/');
      
      for (auto &special_key_index : index_of_special_keys) {
        std::shared_ptr<specialKey> special_key_struct = createSpecialKey(expression, special_key_index);
          
        if(!treeImplemention(expression_tree, special_key_struct)) {
          expression_tree.push_back(special_key_struct);
        }
      }

      return expression_tree;
    }

    bool treeImplemention(std::vector<std::shared_ptr<specialKey>>& expression_tree, std::shared_ptr<specialKey> current)
    {
      for (auto &special_expression_element : expression_tree)
      {
        if (current->scope_of_special_key.first > special_expression_element->scope_of_special_key.first && current->scope_of_special_key.second < special_expression_element->scope_of_special_key.second)
        {
          bool sub_status = false;
          sub_status = treeImplemention(special_expression_element->nested_special_key, current);
          if (sub_status) 
            return true;
          else
          {
            special_expression_element->nested_special_key.push_back(current);
            return true;
          }
        }
      }
      return false;
    }
  };

} // DifferantialOperations

int main()
{
  std::cout << "/d{/f{w,x},x} + /f{w,x} * 5" << std::endl;
  DifferantialOperations::StringDerivative str_deriv("/d{/f{w,x},x} + /f{w,x} * 5");

  return 0;
}
