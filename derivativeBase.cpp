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

  std::vector<std::string> strSplit(std::string string_data, std::string splitter)
  {
    std::vector<std::string> string_array;

    const int string_length = string_data.length();
    char* char_array = new char[string_length + 1];
    std::strcpy(char_array, string_data.c_str());

    char* tmp_array = std::strtok(char_array, splitter.c_str());
    while (tmp_array)
    {
      string_array.push_back(tmp_array);
      tmp_array = std::strtok(nullptr, splitter.c_str());
    }
    delete[] char_array;

    return string_array;
  }

  class DifferantialTransformation
  {

  };


  class Derivative
  {
    Derivative(std::string derivative_format)
    {
      // Latex based string format
      // Derivative Format should be:
      // "\func{w,x} = \func{u,x}

    }


  };

  class FunctionParser
  {
    FunctionParser(char key_symbol, std::vector<char> dependent_variable[])
    {
      
    }

  };

  class StringDerivative
  {
    public:
      StringDerivative(std::string string_derivative_)
      {
        std::string string_derivative = removeSpaces(string_derivative_);
        splitOuterOperator(string_derivative);
        std::vector<std::shared_ptr<specialKey>> expression_tree = getSpecialKey(string_derivative);
        for (auto &tree_element : expression_tree)
        {
          std::cout << "Key expression: " << tree_element->key_function << std::endl;
          std::cout << "Value nested: " << tree_element->nested_expression << std::endl;

          if (tree_element->nested_special_key.size() != 0)
          {
            for (auto & nested_element : tree_element->nested_special_key)
            {
              std::cout << "Nested Key expression: " << nested_element->key_function << std::endl;
              std::cout << "Nested Value nested: " << nested_element->nested_expression << std::endl;
            }
          } 

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

        for (auto splitted_string : splitted_by_operator){
          std::cout << splitted_string << std::endl;
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
  DifferantialOperations::StringDerivative str_deriv("/d{/f{w,x},x} + /f{w,x} * 5");

  return 0;
}
