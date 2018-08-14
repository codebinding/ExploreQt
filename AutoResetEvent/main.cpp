#include <QCoreApplication>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

class AutoResetEvent
{
  public:
  explicit AutoResetEvent(bool initial = false);

  ~AutoResetEvent();
  void Set();
  void Reset();

  bool WaitOne();

  private:
  AutoResetEvent(const AutoResetEvent&);
  AutoResetEvent& operator=(const AutoResetEvent&); // non-copyable
  bool flag_;
  std::mutex mutex_;
  std::condition_variable signal_;
};

AutoResetEvent::AutoResetEvent(bool initial)
: flag_(initial)
{
}

void AutoResetEvent::Set()
{
  std::lock_guard<std::mutex> lock(mutex_);
  flag_ = true;
  signal_.notify_one();
}

void AutoResetEvent::Reset()
{
  std::lock_guard<std::mutex> lock(mutex_);
  flag_ = false;
}

bool AutoResetEvent::WaitOne()
{
  std::unique_lock<std::mutex> lock(mutex_);
  while( !flag_ ) // prevent spurious wakeups from doing harm
    signal_.wait(lock);
  flag_ = false; // waiting resets the flag
  return true;
}

AutoResetEvent::~AutoResetEvent()
{
}


AutoResetEvent event(true);

void otherthread()
{
  event.WaitOne();
  printf("Hello from other thread!\n");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::thread t(otherthread);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Hello from main thread ..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    event.Set();

    t.join();

    return a.exec();
}
