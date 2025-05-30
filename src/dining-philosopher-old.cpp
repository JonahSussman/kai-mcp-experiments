#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int user_state[N];

std::mutex mutex;
std::condition_variable S[N];

void test(int phnum) {
  if (user_state[phnum] == HUNGRY && user_state[LEFT] != EATING &&
      user_state[RIGHT] != EATING) {
    // state that eating
    user_state[phnum] = EATING;

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::cout << "Philosopher " << phnum + 1 << " takes fork " << LEFT + 1
              << " and " << phnum + 1 << std::endl;
    std::cout << "Philosopher " << phnum + 1 << " is Eating" << std::endl;

    // notify the philosopher that he can start eating
    S[phnum].notify_all();
  }
}

// take up chopsticks
void take_fork(int phnum) {
  std::unique_lock<std::mutex> lock(mutex);

  // state that hungry
  user_state[phnum] = HUNGRY;

  std::cout << "Philosopher " << phnum + 1 << " is Hungry" << std::endl;

  // eat if neighbours are not eating
  test(phnum);

  // if unable to eat wait to be signalled
  S[phnum].wait(lock);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

// put down chopsticks
void put_fork(int phnum) {
  std::unique_lock<std::mutex> lock(mutex);

  // state that thinking
  user_state[phnum] = THINKING;

  std::cout << "Philosopher " << phnum + 1 << " putting fork " << LEFT + 1
            << " and " << phnum + 1 << " down" << std::endl;
  std::cout << "Philosopher " << phnum + 1 << " is thinking" << std::endl;

  test(LEFT);
  test(RIGHT);
}

void philosopher(int num) {
  while (true) {
    take_fork(num);
    put_fork(num);
  }
}

int main() {
  std::thread threads[N];

  for (int i = 0; i < N; i++) {
    threads[i] = std::thread(philosopher, i);
    std::cout << "Philosopher " << i + 1 << " is thinking" << std::endl;
  }

  for (int i = 0; i < N; i++) threads[i].join();

  return 0;
}
