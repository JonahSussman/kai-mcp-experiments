#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex1;
std::mutex mutex2;

void thread1() {
  std::cout << "Thread 1: Attempting to acquire mutex1" << std::endl;
  std::lock_guard<std::mutex> lock1(mutex1);
  std::cout << "Thread 1: Acquired mutex1" << std::endl;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));  // Simulate some work
  std::cout << "Thread 1: Attempting to acquire mutex2" << std::endl;
  std::lock_guard<std::mutex> lock2(mutex2);
  std::cout << "Thread 1: Acquired mutex2" << std::endl;
}

void thread2() {
  std::cout << "Thread 2: Attempting to acquire mutex2" << std::endl;
  std::lock_guard<std::mutex> lock2(mutex2);
  std::cout << "Thread 2: Acquired mutex2" << std::endl;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));  // Simulate some work
  std::cout << "Thread 2: Attempting to acquire mutex1" << std::endl;
  std::lock_guard<std::mutex> lock1(mutex1);
  std::cout << "Thread 2: Acquired mutex1" << std::endl;
}

int main() {
  std::thread t1(thread1);
  std::thread t2(thread2);

  t1.join();
  t2.join();

  std::cout << "Program finished" << std::endl;
  return 0;
}