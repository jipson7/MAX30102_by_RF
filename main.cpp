
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h> 

#include "algorithm_by_RF.h"
#include "max30102.h"
#include "algorithm.h"

using namespace std;

const int sample_size = 100;

uint32_t red[sample_size];
uint32_t ir[sample_size];

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

  string s = "split on    whitespace   ";
  vector<string> result;

  istringstream liness(line);

  liness >> red_line;
  liness >> ir_line;

  istringstream redss(red_line), irss(ir_line);

  convert_ss_to_array(&redss, red);
  convert_ss_to_array(&irss, ir);

  for (int i = 0; i < sample_size; i++) {
    cout << red[i] << " " << ir[i] << endl;
  }

  return 0;
}

