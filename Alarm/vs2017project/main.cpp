#include "dzn/locator.hh"
#include "dzn/runtime.hh"
#include <iostream>
#include <thread>
#include "AlarmSystem_exception_forwarding.hh"
#include "stringbuilder.h"
#include "StreamLogger.h"

// dzn code -l c++ -s Alarm Alarm.dzn

namespace Foo
{
  std::string postfix_threadid()
  {
    return fusion::stringbuilder() << " (tid: " << std::this_thread::get_id() << ")\n";
  }

  void main()
  {
    auto f = [](const char* message) {
      std::cout << message << postfix_threadid();
    };

    dzn::locator loc;
    StreamLogger sl(f);
    loc.set(static_cast<std::ostream&>(sl));

    dzn::runtime rt;
    loc.set(rt);

    AlarmSystemWrapper as(loc);

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

    std::cout << "sleep 5 seconds\n";
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
