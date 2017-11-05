#include "dzn/locator.hh"
#include "dzn/runtime.hh"
#include <iostream>
#include "Alarm.hh"
#include "stringbuilder.h"
#include "StreamLogger.h"

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

  std::string postfix_threadid()
  {
    return fusion::stringbuilder() << " (tid: " << thread_id() << ")";
  }

  void main()
  {
    auto f = [](const char* message) {
      std::cout << message << postfix_threadid() << " << BLA\n";
    };

    dzn::locator loc;
    StreamLogger sl(f);
    loc.set(sl);

    dzn::runtime rt;
    loc.set(rt);

    AlarmSystem as(loc);

    as.console.out.detected = [&] {
      sl << "Detected!\n";
    };
    as.console.out.deactivated = [&] {
      sl << "deactivated!\n";
    };

    as.siren.in.turnon = [&] {
      sl << "siren on!\n";
    };
    as.siren.in.turnoff = [&] {
      sl << "siren off!\n";
    };

    as.sensor.in.enable = [&] {
      sl << "sensor enabled\n";
    };

    as.sensor.in.disable = [&] {
      sl << "sensor disabled\n";
    };
    as.check_bindings();

    as.console.in.arm();
    std::thread t([&] {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      as.sensor.out.triggered();
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
