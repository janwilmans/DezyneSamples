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

#ifndef Alarm_HH
#define Alarm_HH


#include <dzn/meta.hh>

#include <map>
#include <cassert>



struct IConsole
{
#ifndef ENUM_IConsole_State
#define ENUM_IConsole_State 1
  struct State
  {
    enum type
    {
      Disarmed,Armed,Triggered,Disarming
    };
  };
#endif // ENUM_IConsole_State

  struct
  {
    std::function<void()> arm;
    std::function<void()> disarm;
  } in;

  struct
  {
    std::function<void()> detected;
    std::function<void()> deactivated;
  } out;

  dzn::port::meta meta;
  inline IConsole(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.arm) throw dzn::binding_error(meta, "in.arm");
    if (! in.disarm) throw dzn::binding_error(meta, "in.disarm");

    if (! out.detected) throw dzn::binding_error(meta, "out.detected");
    if (! out.deactivated) throw dzn::binding_error(meta, "out.deactivated");

  }
};

inline void connect (IConsole& provided, IConsole& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}



#ifndef ENUM_TO_STRING_IConsole_State
#define ENUM_TO_STRING_IConsole_State 1
inline std::string to_string(::IConsole::State::type v)
{
  switch(v)
  {
    case ::IConsole::State::Disarmed: return "State_Disarmed";
    case ::IConsole::State::Armed: return "State_Armed";
    case ::IConsole::State::Triggered: return "State_Triggered";
    case ::IConsole::State::Disarming: return "State_Disarming";

  }
  return "";
}
#endif // ENUM_TO_STRING_IConsole_State

#ifndef STRING_TO_ENUM_IConsole_State
#define STRING_TO_ENUM_IConsole_State 1
inline ::IConsole::State::type to_IConsole_State(std::string s)
{
  static std::map<std::string, ::IConsole::State::type> m = {
    {"State_Disarmed", ::IConsole::State::Disarmed},
    {"State_Armed", ::IConsole::State::Armed},
    {"State_Triggered", ::IConsole::State::Triggered},
    {"State_Disarming", ::IConsole::State::Disarming},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IConsole_State




#include <dzn/meta.hh>

#include <map>



struct ISensor
{
#ifndef ENUM_ISensor_State
#define ENUM_ISensor_State 1
  struct State
  {
    enum type
    {
      Disabled,Enabled,Disabling,Triggered
    };
  };
#endif // ENUM_ISensor_State

  struct
  {
    std::function<void()> enable;
    std::function<void()> disable;
  } in;

  struct
  {
    std::function<void()> triggered;
    std::function<void()> disabled;
  } out;

  dzn::port::meta meta;
  inline ISensor(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.enable) throw dzn::binding_error(meta, "in.enable");
    if (! in.disable) throw dzn::binding_error(meta, "in.disable");

    if (! out.triggered) throw dzn::binding_error(meta, "out.triggered");
    if (! out.disabled) throw dzn::binding_error(meta, "out.disabled");

  }
};

inline void connect (ISensor& provided, ISensor& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}



#ifndef ENUM_TO_STRING_ISensor_State
#define ENUM_TO_STRING_ISensor_State 1
inline std::string to_string(::ISensor::State::type v)
{
  switch(v)
  {
    case ::ISensor::State::Disabled: return "State_Disabled";
    case ::ISensor::State::Enabled: return "State_Enabled";
    case ::ISensor::State::Disabling: return "State_Disabling";
    case ::ISensor::State::Triggered: return "State_Triggered";

  }
  return "";
}
#endif // ENUM_TO_STRING_ISensor_State

#ifndef STRING_TO_ENUM_ISensor_State
#define STRING_TO_ENUM_ISensor_State 1
inline ::ISensor::State::type to_ISensor_State(std::string s)
{
  static std::map<std::string, ::ISensor::State::type> m = {
    {"State_Disabled", ::ISensor::State::Disabled},
    {"State_Enabled", ::ISensor::State::Enabled},
    {"State_Disabling", ::ISensor::State::Disabling},
    {"State_Triggered", ::ISensor::State::Triggered},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ISensor_State




#include <dzn/meta.hh>

#include <map>



struct ISiren
{
#ifndef ENUM_ISiren_State
#define ENUM_ISiren_State 1
  struct State
  {
    enum type
    {
      Off,On
    };
  };
#endif // ENUM_ISiren_State

  struct
  {
    std::function<void()> turnon;
    std::function<void()> turnoff;
  } in;

  struct
  {
  } out;

  dzn::port::meta meta;
  inline ISiren(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.turnon) throw dzn::binding_error(meta, "in.turnon");
    if (! in.turnoff) throw dzn::binding_error(meta, "in.turnoff");


  }
};

inline void connect (ISiren& provided, ISiren& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}



#ifndef ENUM_TO_STRING_ISiren_State
#define ENUM_TO_STRING_ISiren_State 1
inline std::string to_string(::ISiren::State::type v)
{
  switch(v)
  {
    case ::ISiren::State::Off: return "State_Off";
    case ::ISiren::State::On: return "State_On";

  }
  return "";
}
#endif // ENUM_TO_STRING_ISiren_State

#ifndef STRING_TO_ENUM_ISiren_State
#define STRING_TO_ENUM_ISiren_State 1
inline ::ISiren::State::type to_ISiren_State(std::string s)
{
  static std::map<std::string, ::ISiren::State::type> m = {
    {"State_Off", ::ISiren::State::Off},
    {"State_On", ::ISiren::State::On},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_ISiren_State



#ifndef INTERFACE_ONLY
////////////////////////////////////////////////////////////////////////////////
// #ifndef ALARM_HH
// #define ALARM_HH

#include <iostream>

#include "Alarm.hh"
#include "Alarm.hh"
#include "Alarm.hh"


namespace dzn {
  struct locator;
  struct runtime;
}


struct Alarm
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_Alarm_State
#define ENUM_Alarm_State 1
  struct State
  {
    enum type
    {
      Disarmed,Armed,Disarming
    };
  };
#endif // ENUM_Alarm_State

  ::Alarm::State::type state;
  bool sounding;


  std::function<void ()> out_console;

  ::IConsole console;

  ::ISensor sensor;
  ::ISiren siren;


  Alarm(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Alarm& m)  {
    (void)m;
    return os << "[" << m.state <<", " << m.sounding <<"]" ;
  }

  std::string state_to_string(::Alarm::State::type s)
  {
    switch (s)
    {
    case ::Alarm::State::Disarmed: return "Disarmed";
    case ::Alarm::State::Armed: return "Armed";
    case ::Alarm::State::Disarming:  return "Disarming";
    default: assert(false && "illegal value for ::Alarm::State::type");
    }
    return "<undefined>";
  }

  std::string sounding_to_string(bool s)
  {
    return s ? "true" : "false";
  }

  void dump_state(std::ostream& os)
  {
    os << "[" << state_to_string(state) << ", " << sounding_to_string(sounding) << "]";
  }

  private:
  void console_arm ();
  void console_disarm ();
  void sensor_triggered ();
  void sensor_disabled ();

  void Detected ();
};

// #endif // ALARM_HH
////////////////////////////////////////////////////////////////////////////////
// #ifndef ALARMSYSTEM_HH
// #define ALARMSYSTEM_HH

//interface includes
#include "Alarm.hh"
#include "Alarm.hh"
#include "Alarm.hh"


//component includes


#include <iostream>
#include <dzn/locator.hh>

namespace dzn {
  struct locator;
  struct runtime;
}


struct AlarmSystem
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Alarm alarm;

  ::IConsole& console;

  ::ISensor& sensor;
  ::ISiren& siren;

  AlarmSystem(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os=std::clog) const;
};

// #endif // ALARMSYSTEM_HH
////////////////////////////////////////////////////////////////////////////////

#endif // INTERFACE_ONLY
#endif // Alarm_HH
