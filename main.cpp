
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h> 

#include "algorithm_by_RF.h"
#include "algorithm.h"

using namespace std;

const int sample_size = 100;

uint32_t red[sample_size];
uint32_t ir[sample_size];

string run_maxim_algo() {
  float oxygen;
  int32_t hr;
  int8_t oxygen_valid;
  int8_t hr_valid;

  maxim_heart_rate_and_oxygen_saturation(ir, sample_size, red, &oxygen, &oxygen_valid, &hr, &hr_valid);

  ostringstream os;

  os << "{\"hr\": " << hr << ", \"hr_valid\":" << to_string(hr_valid) << ", ";
  os << "\"oxygen\": " << oxygen << ", \"oxygen_valid\":" << to_string(oxygen_valid) << "}";
  return os.str();
}

string run_enhanced_algo() {
  // Invalid ratio default param
  float ratio = -999;
  float oxygen, correl;
  int32_t hr;
  int8_t oxygen_valid;
  int8_t hr_valid;
  rf_heart_rate_and_oxygen_saturation(ir, sample_size, red, &oxygen, &oxygen_valid, &hr, &hr_valid, &ratio, &correl);

  ostringstream os;

  os << "{\"hr\": " << hr << ", \"hr_valid\":" << to_string(hr_valid) << ", ";
  os << "\"oxygen\": " << oxygen << ", \"oxygen_valid\":" << to_string(oxygen_valid);
  os << ", \"ratio\": " << ratio << ", \"correlation\":" << correl << "}";
  return os.str();
}



void convert_ss_to_array(istringstream *ss, uint32_t *output) {
  uint32_t val;
  int index = 0;


  while (*ss >> val) {
    output[index++] = val;

    if (ss->peek() == ',')
      ss->ignore();
  }
  assert (index == sample_size);
}

int main() {
  string line, red_line, ir_line;
  getline(cin, line);

  istringstream liness(line);

  liness >> red_line;
  liness >> ir_line;

  istringstream redss(red_line), irss(ir_line);

  convert_ss_to_array(&redss, red);
  convert_ss_to_array(&irss, ir);

  string maxim_result = run_maxim_algo();
  string enhanced_result = run_enhanced_algo();


  ostringstream os;

  os << "{\"maxim\": " << maxim_result << ", ";
  os << "\"enhanced\": " << enhanced_result << "}";

  cout << os.str() << endl;

  return 0;
}

