#include <iostream>
#include <thread>

int counter = 0;

void increment_counter() {
  for (int i = 0; i < 100000; ++i) {
    counter++;
  }
}

int main() {
  std::thread t1(increment_counter);
  std::thread t2(increment_counter);

  t1.join();
  t2.join();

  std::cout << "Counter value: " << counter << std::endl;
  return 0;
}