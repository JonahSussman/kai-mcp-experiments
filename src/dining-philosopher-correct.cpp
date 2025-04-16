#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#define N 5

enum {
  USING_RESOURCES     = 0, 
  IDLE                = 1, 
  READY_FOR_RESOURCES = 2,
};

enum {
  RESOURCE_FREE     = 0,
  RESOURCE_ACQUIRED = 1,
};

int user_state[N];
int resource_state[N];
std::pair<int, int> user_required_resources[N];

std::mutex mutex;
std::condition_variable resource_cvs[N];

void user_thread(int id) {
  for (int i = 0; i < 5; i++) {
    user_state[id] = IDLE;
    std::cout << id << " idle" << std::endl;

    // Simulate idle
    std::this_thread::sleep_for(std::chrono::milliseconds(1 + rand() % 1000));

    user_state[id] = READY_FOR_RESOURCES;
    std::cout << id << " ready for resources" << std::endl;
    
    std::unique_lock<std::mutex> lock(mutex);
    
    auto [x, y] = user_required_resources[id];

    std::cout << id << " waiting on " << x << std::endl;
    resource_cvs[x].wait(lock, [id, x] { return resource_state[x] != RESOURCE_ACQUIRED; });
    resource_state[x] = RESOURCE_ACQUIRED;
    std::cout << id << " acquired " << x << std::endl;

    std::cout << id << " waiting on " << y << std::endl;
    resource_cvs[y].wait(lock, [id, y] { return resource_state[y] != RESOURCE_ACQUIRED; });
    resource_state[y] = RESOURCE_ACQUIRED;
    std::cout << id << " acquired " << y << std::endl;

    user_state[id] = USING_RESOURCES;
    std::cout << id << " using resources" << std::endl;

    // Simulate using resources
    std::this_thread::sleep_for(std::chrono::milliseconds(1 + rand() % 1000));

    resource_state[x] = RESOURCE_FREE;
    std::cout << id << " freed " << x << std::endl;
    resource_cvs[x].notify_all();

    resource_state[y] = RESOURCE_FREE;
    std::cout << id << " freed " << y << std::endl;
    resource_cvs[y].notify_all();
  }
}

int main() {
  std::thread threads[N];

  for (int i = 0; i < N; i++) {
    user_required_resources[i] = {i, (i + 1) % N};
  }

  for (int i = 0; i < N; i++)
    threads[i] = std::thread(user_thread, i);

  for (int i = 0; i < N; i++)
    threads[i].join();

  return 0;
}