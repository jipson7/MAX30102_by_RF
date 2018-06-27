
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

void run_maxim_algo() {
  float oxygen;
  int32_t hr;
  int8_t oxygen_valid, hr_valid;

  maxim_heart_rate_and_oxygen_saturation(ir, sample_size, red, &oxygen, &oxygen_valid, &hr, &hr_valid);

  cout << "Maxim" << endl;
  cout << "HR: " << hr << " " << hr_valid << endl;
  cout << "Oxygen: " << oxygen << " " << oxygen_valid << endl;
}

void run_enhanced_algo() {
  float oxygen, ratio, correl;
  int32_t hr;
  int8_t oxygen_valid, hr_valid;

  rf_heart_rate_and_oxygen_saturation(ir, sample_size, red, &oxygen, &oxygen_valid, &hr, &hr_valid, &ratio, &correl);

  cout << "Enhanced" << endl;
  cout << "HR: " << hr << " " << hr_valid << endl;
  cout << "Oxygen: " << oxygen << " " << oxygen_valid << endl;
  cout << "Ratio: " << ratio << ", Correlation: " << correl << endl;
}



void convert_ss_to_array(istringstream *ss, uint32_t *output) {
  uint32_t val;
  int index = 0;


  while (*ss >> val) {
    output[index++] = val;

    if (ss->peek() == ',')
      ss->ignore();
  }
  cout << index << endl;
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

  for (int i = 0; i < sample_size; i++) {
    cout << red[i] << " " << ir[i] << endl;
  }

  run_maxim_algo();
  run_enhanced_algo();

  return 0;
}

