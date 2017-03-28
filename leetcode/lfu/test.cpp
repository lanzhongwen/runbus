#include "LFUCache.h"
#include <iostream>

int main() {
  {
  cout << "case 1: \n";
  LFUCache lfu(2);
  lfu.put(1,1);
  lfu.put(2,2);
  cout << "Expected: 1 | Actual: " << lfu.get(1) << endl;
  lfu.put(3,3);
  cout << "Expected: -1 | Actual: " << lfu.get(2) << endl;
  cout << "Expected: 3 | Actual: " << lfu.get(3) << endl;
  lfu.put(4,4);
  cout << "Expected: -1 | Actual: " << lfu.get(1) << endl;
  cout << "Expected: 3 | Actual: " << lfu.get(3) << endl;
  cout << "Expected: 4 | Actual: " << lfu.get(4) << endl;
  }

  cout << "case 2: \n";
  {
  LFUCache lfu(3);
  lfu.put(1,1);
  lfu.put(2,2);
  lfu.put(3,3);
  lfu.put(4,4);
  cout << "Expected: 4 | Actual: " << lfu.get(4) << endl;
  cout << "Expected: 3 | Actual: " << lfu.get(3) << endl;
  cout << "Expected: 2 | Actual: " << lfu.get(2) << endl;
  cout << "Expected: -1 | Actual: " << lfu.get(1) << endl;
  lfu.put(5,5);
  cout << "Expected: -1 | Actual: " << lfu.get(1) << endl;
  cout << "Expected: 2 | Actual: " << lfu.get(2) << endl;
  cout << "Expected: 3 | Actual: " << lfu.get(3) << endl;
  cout << "Expected: -1 | Actual: " << lfu.get(4) << endl;
  cout << "Expected: 5 | Actual: " << lfu.get(5) << endl;
  }
}
