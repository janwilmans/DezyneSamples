// This code snippet is part of a boilerplate code generator for Verum Dezyne
// Copyright (C) 2016, 2017   Jan Nieuwenhuizen, Rutger van Beusekom

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// The authors can be contacted via email:
// lowercase dot separated fullname @verum.com.

var fs = require('fs');
var parser = require(__dirname + '/parse.js');

if (!Array.prototype.append_map) {
  Array.prototype.append_map = function(lambda) {
    return [].concat.apply([], this.map(lambda));
  }
}

function print(o) {
  console.log(JSON.stringify(o, null, 2));
}

function read(file) {
  var list = [];
  function doimport(file) {
    list = list.concat([file]);
    return fs.readFileSync(file,'ascii')
      .replace(/import\s+([^;]*);/g,
               function(match, file){return list.indexOf(file) == -1 ? doimport(file) : '';});
  }
  return doimport(file);
}

var ast = parser.parse(read(process.argv[2]));

var om = {
  components : function(o) {
    switch(o.class)
    {
      case 'Root': return o.elements.filter(function(o) {return o.class == 'Component';});
      default: return [];
    }
  },
  systems : function(o) {
    switch(o.class)
    {
      case 'Root': return o.elements.filter(function(o) {return o.class == 'System';});
      default: return [];
    }
  },
  interfaces : function(o) {
    switch(o.class)
    {
      case 'Root': return o.elements.filter(function(o) {return o.class == 'Interface';});
      default: return [];
    }
  },
  interface: function(o) {
    return om.interfaces(ast).find(function(e) {
      return om.name(o.type) == om.name(e);
    });
  },
  types: function(o) {
    switch(o.class)
    {
      case 'Interface': return o.types;
      default: return [];
    }
  },
  ports : function(o) {
    switch(o.class)
    {
      case 'System': return o.ports.elements;
      case 'Component': return o.ports.elements;
      default: return [];
    }
  },
  externs: function(o) {
    switch(o.class)
    {
      case 'Root': return o.elements.filter(function(o) {return o.class == 'Extern';});
      default: return [];
    }
  },
  events : function(o){
    switch(o.class)
    {
      case 'Interface': return o.events.elements;
      case 'Port': return om.events(om.interface(o));
      default: return [];
    }
  },
  returntype : function(o) {
    switch(o.class)
    {
      case 'Event': return Array.isArray(o.signature.type.name.elements) ? o.signature.type.name.elements.join('') : o.signature.type.name;
      default: return 'void';
    }
  },
  formals : function(o) {
    switch(o.class)
    {
      case 'Event': return o.signature.parameters.elements;
      default: return [];
    }
  },
  in_p: function(o) {
    switch(o.class)
    {
      case 'Event': return o.direction == 'in';
      default: return false;
    }
  },
  name: function(o, sep) {
    sep = sep || '_';
    if(o.class == 'Name') return o.elements.join(sep);
    if(o.name && o.name.class == 'Name') return om.name (o.name);
    return o.name || '';
  },
  provides_p: function(o) {
    switch(o.class)
    {
      case 'Port': return o.direction == 'provides';
      default: return false;
    }
  },
  stimulus_p: function(p,e) {
    return om.provides_p(p) && om.in_p(e) || !om.provides_p(p) && !om.in_p(e);
  },
  void_p: function(e) {
    switch(e.class)
    {
      case 'Event': return e.signature.type.name == 'void';
      default: return false;
    }
  },
  bool_p: function(e) {
    switch(e.class)
    {
      case 'Event': return e.signature.type.name == 'bool';
      default: return false;
    }
  },
  subint_p: function(e) {
    switch(e.class)
    {
      case 'Event': return e.signature.type.name == 'subint';
      default: return false;
    }
  },
  enum_p: function(e) {
    return !(om.void_p(e) || om.bool_p(e) || om.subint_p(e));
  }
}

function negate(l) {
  return function() {
    var args = Array.prototype.slice.call (arguments);
    return !l.apply (undefined, args);
  }
}

var snippet = {
  interface: '\n\
#ifndef @om.name(interface).toUpperCase()@_WRAPPER\n\
#define @om.name(interface).toUpperCase()@_WRAPPER\n\
\n\
struct @om.name(interface)@Wrapper {\n\
  @types@\n\
  struct {\n\
    @in_events@\
  } in;\n\
  struct {\n\
    @out_events@\
  } out;\n\
  dzn::port::meta meta;\n\
  inline @om.name(interface)@Wrapper(dzn::port::meta &&m) : meta(std::move(m)){}\n\
  void check_bindings()\n\
  {\n\
    @in_check_binding@\
    @out_check_binding@\
  }\n\
};\n\
\n\
#endif\n\
',
  component: '\n\
#include "@name@.hh"\n\
\n\
#include <dzn/runtime.hh>\n\
\n\
@interfaces@\n\
\n\
struct @name@Wrapper {\n\
  dzn::meta dzn_meta;\n\
  @name@ impl;\n\
  @ports@\n\
  @name@Wrapper(const dzn::locator& l)\n\
  : dzn_meta("","@name@Wrapper",0,{&impl.dzn_meta},{@ports_connected@})\n\
  , impl(l)\n\
  , @ports_initializer_list@\
  {\n\
    @stimuli@\
    @responses@\
  }\n\
  void check_bindings()\n\
  {\n\
    dzn::check_bindings(&dzn_meta);\n\
  }\n\
};\n\
',
  void_stimulus: '\
@port@.@direction@.@event@ = [this](@formals@){\n\
  calling_context cc;\n\
  impl.@port@.@direction@.@event@(cc@comma@@formal_names@);\n\
  cc();\n\
};\n\
',
  valued_stimulus: '\
@port@.@direction@.@event@ = [this](@formals@){\n\
  calling_context cc;\n\
  auto&& r = impl.@port@.@direction@.@event@(cc@comma@@formal_names@);\n\
  cc();\n\
  return std::move(r);\n\
};\n\
',
  response: '\
impl.@port@.@direction@.@event@ = [this](const calling_context& cc@comma@@formals@){\n\
  try {\n\
    return @port@.@direction@.@event@(@formal_names@);\n\
  } catch(...) {\n\
    cc.extend(std::current_exception());\n\
    @return_statement@\n\
  }\n\
};\n\
'
};

function animate(x, l) {
  return snippet[x].replace(/@([^@]*)@/g, l);
}

Object.prototype.values = function(o) {
  return Object.keys(o).map(function(key){return o[key];});
};

function $(o) {
  var l = '(function (' + Object.keys(o).join(',') + ', _, x){return eval(x);}.bind(undefined, ' + Object.values(o).join(',') + '))';
  return l;
}

var code = {
  interface : function(o) {
    return animate('interface', eval($({interface:'o',
                                        types:"code.types(om.types(o),om.name(o)).join('  ')",
                                        in_events:"om.events(o).filter(om.in_p).map(code.event).join('  ')",
                                        out_events:"om.events(o).filter(negate(om.in_p)).map(code.event).join('  ')",
                                        in_check_binding:"om.events(o).filter(om.in_p).map(code.in_check_binding).join('  ')",
                                        out_check_binding:"om.events(o).filter(negate(om.in_p)).map(code.out_check_binding).join('  ')"
                                       })));
  },
  component : function(ast, o) {

    var interfaces = om.interfaces(ast).filter(function(interface) {
      return om.ports(o).map(function(port) {
        return code.type(port);
      }).indexOf(om.name(interface)) != -1;
    }).map(code.interface).join('\n');

    var stimuli = om.ports(o)
          .map(function(p){
          return om.events(p)
              .filter(function(e){return om.stimulus_p(p,e);})
              .map(function(e) {
                return animate((om.void_p(e) ? 'void' : 'valued') + '_stimulus',
                               eval($({port:"om.name(p)",
                                       direction:"e.direction",
                                       event:"om.name(e)",
                                       formals:"om.formals(e).map(code.formal)",
                                       formal_names:"om.formals(e).map(om.name)",
                                       comma:"om.formals(e).length ? ',' : ''"
                                      })));
              }).join('');
          }).join('');

    var responses = om.ports(o)
        .map(function(p){
          return om.events(p)
            .filter(function(e){return !om.stimulus_p(p,e);})
            .map(function(e) {
              return animate('response', eval($({port:"om.name(p)",
                                                 direction:"e.direction",
                                                 event:"om.name(e)",
                                                 return_statement: "code.return_statement(p,e)",
                                                 formals:"om.formals(e).map(code.formal)",
                                                 formal_names:"om.formals(e).map(om.name)",
                                                 comma:"om.formals(e).length ? ',' : ''"
                                                })));
            }).join('');
        }).join('');

    return animate('component', eval($({name: "om.name(o)",
                                        ports: "om.ports(o).map(code.port_decl).join('  ')",
                                        ports_connected:"om.ports(o).map(code.port_connected).join(',')",
                                        ports_initializer_list:"om.ports(o).map(code.port_def).join('  , ')",
                                        interfaces: "interfaces",
                                        stimuli: "stimuli",
                                        responses: "responses"})));
  },
  in_check_binding: function(o) {
    return 'if(!in.' + o.name + ') throw dzn::binding_error(meta, "in.' + o.name +'");\n';
  },
  out_check_binding: function(o) {
    return 'if(!out.' + o.name + ') throw dzn::binding_error(meta, "out.' + o.name +'");\n';
  },
  event: function(o) {
    return 'std::function<' + om.returntype(o) + '(' + om.formals(o).map(code.formal_type).join(',') + ')> ' + o.name + ';\n';
  },
  formal_type: function(o) {
    return code.extern(om.externs(ast).find(function(e) {
      return om.name(o.type) == om.name(e);
    }));
  },
  formal: function(o) {
    return code.formal_type(o) + ' ' + om.name(o);
  },
  extern: function(o) {
    return o.data;
  },
  type: function(o) {
    if(o.type && o.type.class == 'Name') return om.name(o.type);
    return o.type || '';
  },
  port_decl: function(o) {
    return code.type(o) + 'Wrapper ' + om.name(o) + ';\n';
  },
  port_def: function(o) {
    return o.direction == 'provides'
      ? om.name(o) + '({{"' + om.name(o) + '",this,&dzn_meta},{"' + om.name(o) + '",&impl,&impl.dzn_meta}})\n'
      : om.name(o) + '({{"' + om.name(o) + '",&impl,&impl.dzn_meta},{"' + om.name(o) + '",this,&dzn_meta}})\n';
  },
  port_connected: function(o) {
    return '[this]{' + om.name(o) + '.check_bindings();}';
  },
  return_statement: function(p,e) {
    if(om.void_p(e))
      return 'return;';
    if(om.bool_p(e))
      return 'return false;';
    if(om.subint_p(e))
      return 'dunno?!!';
    else
      return 'return ' + code.type(p) + '::' + om.returntype(e)+'::type();';
  },
  types: function(o, i) {
    return o.elements.map(function(o){
      return 'typedef ' + i + '::' + o.name.elements.join('') + '::type ' + o.name.elements.join('')+ ';\n';
    });
  }
}

//generate for the last system or the last component
var model = om.systems(ast);
if(model.length == 0) {
  model = om.components(ast);
}
if(model.length) {
  process.stdout.write(code.component(ast, model[model.length - 1]));
}
