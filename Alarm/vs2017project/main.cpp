#include <dzn/locator.hh>
#include <dzn/runtime.hh>
#include "Alarm.hh"
#include <iostream>

namespace Foo
{
	void main()
	{
		dzn::locator loc;
		dzn::runtime rt;
		loc.set(rt);
		AlarmSystem as(loc);

		as.console.out.detected = [] {
			std::cout << "Detected!\n";
		};
		as.console.out.deactivated = [] {
			std::cout << "deactivated!\n";
		};

		as.siren.in.turnon = [] {
			std::cout << "siren on!\n";
		};
		as.siren.in.turnoff = [] {
			std::cout << "siren off!\n";
		};

		as.sensor.in.enable = [] {
			std::cout << "sensor enabled\n";
		};

		as.sensor.in.disable = [] {
			std::cout << "sensor disabled\n";
		};

		as.check_bindings();

		as.alarm.console.in.arm();
		as.alarm.sensor.out.triggered();
	}
}

int main(int argc, char* argv[])
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