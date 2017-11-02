#include "dzn/locator.hh"
#include "dzn/runtime.hh"
#include <iostream>
#include "Alarm.hh"
#include "stringbuilder.h"

// dzn code -l c++ -s Alarm Alarm.dzn

namespace Foo
{
  long thread_id()
  {
#ifdef _WIN32
    return ::Concurrency::details::platform::GetCurrentThreadId();
#else
    return pthread_self();
#endif
  }

  std::string threadid()
  {
    return fusion::stringbuilder() << " (tid: " << thread_id() << ")";
  }

  void main()
  {
    dzn::locator loc;
    dzn::runtime rt;
    loc.set(rt);
    AlarmSystem as(loc);

    as.console.out.detected = [] {
      std::cout << "Detected!" << threadid() << "\n";
    };
    as.console.out.deactivated = [] {
      std::cout << "deactivated!" << threadid() << "\n";
    };

    as.siren.in.turnon = [] {
      std::cout << "siren on!" << threadid() << "\n";
    };
    as.siren.in.turnoff = [] {
      std::cout << "siren off!" << threadid() << "\n";;
    };

    as.sensor.in.enable = [] {
      std::cout << "sensor enabled" << threadid() << "\n";;
    };

    as.sensor.in.disable = [] {
      std::cout << "sensor disabled" << threadid() << "\n";
    };

    as.check_bindings();


    as.alarm.console.in.arm();
    std::thread t([&] {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      as.alarm.sensor.out.triggered();
    });

    t.join();

    std::this_thread::sleep_for(std::chrono::seconds(5));

  }
} // namespace Foo

int main(int, char* [])
{
  try
  {
    Foo::main();
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
  }
  return 0;
}
