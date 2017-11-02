// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.



#include "Alarm.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>



Alarm::Alarm(const dzn::locator& dzn_locator)
: dzn_meta{"","Alarm",0,0,{&sensor.meta,&siren.meta},{/*FIXME*/},{[this]{console.check_bindings();},[this]{sensor.check_bindings();},[this]{siren.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)
, state(::Alarm::State::Disarmed)
, sounding(false)


, console({{"console",this,&dzn_meta},{"",0,0}})

, sensor({{"",0,0},{"sensor",this,&dzn_meta}})
, siren({{"",0,0},{"siren",this,&dzn_meta}})


{
  dzn_rt.performs_flush(this) = true;

  console.in.arm = [&](){return dzn::call_in(this,[=]{ return console_arm();}, this->console.meta, "arm");};
  console.in.disarm = [&](){return dzn::call_in(this,[=]{ return console_disarm();}, this->console.meta, "disarm");};
  sensor.out.triggered = [&](){return dzn::call_out(this,[=]{ return sensor_triggered();}, this->sensor.meta, "triggered");};
  sensor.out.disabled = [&](){return dzn::call_out(this,[=]{ return sensor_disabled();}, this->sensor.meta, "disabled");};




}

void Alarm::console_arm ()
{
  if (state == ::Alarm::State::Disarmed) 
  {
    this->sensor.in.enable();
    state = ::Alarm::State::Armed;
  }
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;
}
void Alarm::console_disarm ()
{
  if (state == ::Alarm::State::Armed) 
  {
    this->sensor.in.disable();
    state = ::Alarm::State::Disarming;
  }
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;
}
void Alarm::sensor_triggered ()
{
  if (state == ::Alarm::State::Armed) 
  {
    Detected();
    this->siren.in.turnon();
    sounding = true;
  }
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;
}
void Alarm::sensor_disabled ()
{
  if ((state == ::Alarm::State::Disarming && sounding)) 
  {
    this->console.out.deactivated();
    this->siren.in.turnoff();
    sounding = false;
    state = ::Alarm::State::Disarmed;
  }
  else if ((state == ::Alarm::State::Disarming && !(sounding))) 
  {
    this->console.out.deactivated();
    state = ::Alarm::State::Disarmed;
  }
  else dzn_locator.get<dzn::illegal_handler>().illegal();

  return;
}

void Alarm::Detected () 
{
  this->console.out.detected();
  return ;
}

void Alarm::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void Alarm::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////

#include "Alarm.hh"


AlarmSystem::AlarmSystem(const dzn::locator& locator)
: dzn_meta{"","AlarmSystem",0,0,{&sensor.meta,&siren.meta},{&alarm.dzn_meta},{[this]{console.check_bindings();},[this]{sensor.check_bindings();},[this]{siren.check_bindings();}}}
, dzn_locator(locator.clone().set(dzn_rt).set(dzn_pump))


, alarm(dzn_locator)

, console(alarm.console)
, sensor(alarm.sensor), siren(alarm.siren)
, dzn_pump()
{
  alarm.console.meta.requires.port = "console";

  alarm.sensor.meta.provides.port = "sensor";
  alarm.siren.meta.provides.port = "siren";


  console.in.arm = [&] () {
    return dzn::shell(dzn_pump, [&] {return alarm.console.in.arm();});
  };
  console.in.disarm = [&] () {
    return dzn::shell(dzn_pump, [&] {return alarm.console.in.disarm();});
  };

  sensor.out.triggered = [&] () {
    return dzn_pump([&] {return alarm.sensor.out.triggered();});
  };
  sensor.out.disabled = [&] () {
    return dzn_pump([&] {return alarm.sensor.out.disabled();});
  };

  alarm.console.out.detected = std::ref(console.out.detected);
  alarm.console.out.deactivated = std::ref(console.out.deactivated);

  alarm.sensor.in.enable = std::ref(sensor.in.enable);
  alarm.sensor.in.disable = std::ref(sensor.in.disable);
  alarm.siren.in.turnon = std::ref(siren.in.turnon);
  alarm.siren.in.turnoff = std::ref(siren.in.turnoff);


  alarm.dzn_meta.parent = &dzn_meta;
  alarm.dzn_meta.name = "alarm";


}

void AlarmSystem::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void AlarmSystem::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}
