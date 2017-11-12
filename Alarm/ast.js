// This code snippet is part of a boilerplate code generator for Verum Dezyne
// Copyright (C) 2016   Jan Nieuwenhuizen, Rob Wieringa, Rutger van Beusekom

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

// The authors can be contacted via email: lowercase dot separated fullname @verum.com,

if (!Array.prototype.each) {
  Array.prototype.each = Array.prototype.forEach;
};

if (!Array.prototype.partition) {
  Array.prototype.partition = function (predicate, context) {
    var satisfy = [];
    var rest = [];

    this.each (function (i) {
      if (predicate.call (context, i)) {
        satisfy.push (i);
      } else {
        rest.push (i);
      }
    });
    return [satisfy, rest];
  };
};

function Location (source, start, end) {
  this.class = this.constructor.name;
  this.source = source;
  this.start = start;
  this.end = end;
}
function Position (line, column) {
  this.class = this.constructor.name;
  this.line = line;
  this.column = column;
}

function Root (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}

//Models
function Import (name, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.loc = loc || '';
}
function Interface (name, types, events, behaviour, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.types = types || new Types ([]);
  this.events = events || new Events ([]);
  var events_types = this.events.elements.partition (function (i) {return i.class == 'Event';});
  this.types.elements = this.types.elements.concat (events_types[1]);
  this.events.elements = events_types[0];
  this.behaviour = behaviour;
  this.loc = loc || '';
}
function Component (name, ports, behaviour, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.ports = ports;
  this.behaviour = behaviour;
  this.loc = loc || '';
}
function System (name, ports, instances, bindings, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.ports = ports;
  this.instances = instances || new Instances ([]);
  this.bindings = bindings || new Bindings ([]);
  var instances_bindings = (this.instances.elements || []).partition (function (i) {return i.class == 'Instance';});
  this.instances = new Instances (instances_bindings[0]);
  this.bindings.elements = this.bindings.elements.concat (instances_bindings[1]);
  this.loc = loc || '';
}

//Interals
function Event (direction, signature, name, loc) {
  this.class = this.constructor.name;
  this.direction = direction;
  this.signature = signature;
  this.name = name;
  this.loc = loc || '';
}
function Signature (type, parameters, loc) {
  this.class = this.constructor.name;
  this.type = type;
  this.parameters = parameters || {elements:[]};
  this.loc = loc || '';
}
function Formal (name, type, direction, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.type = type;
  this.direction = direction;
  this.loc = loc || '';
}
function Parameter (name, signature, direction, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.signature = signature;
  this.direction = direction;
  this.loc = loc || '';
}
function Function (name, signature, statement, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.signature = signature;
  this.statement = statement;
  this.loc = loc || '';
}
function Name (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Port (direction, type, name, external, injected, loc) {
  this.class = this.constructor.name;
  this.direction = direction;
  this.type = type;
  this.name = name;
  this.external = external;
  this.injected = injected;
  this.loc = loc || '';
}
function Type (name, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.loc = loc || '';
}
function Range (from, to, loc) {
  this.class = this.constructor.name;
  this.from = from;
  this.to = to;
  this.loc = loc || '';
}
function Literal (name, field, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.field = field;
  this.loc = loc || '';
}
function Enum (name, fields, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.fields = fields || [];
  this.loc = loc || '';
}
function Extern (name, data, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.data = data;
  this.loc = loc || '';
}
function Int (name, range, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.range = range;
  this.loc = loc || '';
}
function Data (value, loc) {
  this.class = this.constructor.name;
  this.value = value;
  this.loc = loc || '';
}
function Behaviour (name, types, variables, functions, statements, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.types = types || new Types ([]);
  this.variables = variables || new Variables ([]);
  this.functions = functions || new Functions ([]);
  this.statements = statements || new Compound ([]);
  var variables_rest = this.statements.elements.partition (function (i) {return i.class == 'Variable';});
  this.variables.elements = this.variables.elements.concat (variables_rest[0]);
  var functions_statements = variables_rest[1].partition (function (i) {return i.class == 'Function';});
  this.functions.elements = this.functions.elements.concat (functions_statements[0]);
  this.statements.elements = functions_statements[1];
  this.loc = loc || '';
}
function Trigger (port, event, loc) {
  this.class = this.constructor.name;
  this.port = port;
  this.event = event;
  this.loc = loc || '';
}
function Instance (component, name, loc) {
  this.class = this.constructor.name;
  this.component = component;
  this.name = name;
  this.loc = loc || '';
}
function Variable (name, type, expression, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.type = type;
  this.expression = expression;
  this.loc = loc || '';
}
function Value (type, field, loc) {
  this.class = this.constructor.name;
  this.type = type;
  this.field = field;
  this.loc = loc || '';
}
function Var (name, loc) {
  this.class = this.constructor.name;
  this.name = name;
  this.loc = loc || '';
}
// Statements
function Action (trigger, loc) {
  this.class = this.constructor.name;
  this.trigger = trigger;
  this.loc = loc || '';
}
function Assign (identifier, expression, loc) {
  this.class = this.constructor.name;
  this.identifier = identifier;
  this.expression = expression;
  this.loc = loc || '';
}
function Blocking (statement, loc) {
  this.class = this.constructor.name;
  this.statement = statement;
  this.loc = loc || '';
}
function Call (identifier, arguments, loc) {
  this.class = this.constructor.name;
  this.identifier = identifier;
  this.arguments = arguments || '';
  this.loc = loc || '';
}
function Compound (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Guard (expression, statement, loc) {
  this.class = this.constructor.name;
  this.expression = expression;
  this.statement = statement;
  this.loc = loc || '';
}
function If (expression, then, els, loc) {
  this.class = this.constructor.name;
  this.expression = expression;
  this.then = then;
  this.els = els || '';
  this.loc = loc || '';
}
function Illegal (loc) {
  this.class = this.constructor.name;
  this.loc = loc || '';
}
function On (triggers, statement, loc) {
  this.class = this.constructor.name;
  this.triggers = triggers;
  this.statement = statement;
  this.loc = loc || '';
}
function Reply (expression, port, loc) {
  this.class = this.constructor.name;
  this.expression = expression;
  this.port = port;
  this.loc = loc || '';
}
function Return (expression, loc) {
  this.class = this.constructor.name;
  this.expression = expression;
  this.loc = loc || '';
}

//System
function Bind (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Binding (instance, port, loc) {
  this.class = this.constructor.name;
  this.instance = instance;
  this.port = port;
  this.loc = loc || '';
}

// Expressions
function Expression (value, loc) {
  this.class = this.constructor.name;
  this.value = value;
  this.loc = loc || '';
}
function And (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function OutBinding (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Equals (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Greater (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function GreaterEqual (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Less (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function LessEqual (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Minus (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Or (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}
function Plus (left, right, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.right = right;
  this.loc = loc || '';
}

function Group (left, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.loc = loc || '';
}
function Not (left, loc) {
  this.class = this.constructor.name;
  this.left = left;
  this.loc = loc || '';
}

function Otherwise (loc) {
  this.class = this.constructor.name;
  this.loc = loc || '';
}

// lists
function Arguments (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Bindings (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Formals (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Events (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Functions (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Instances (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Parameters (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Ports (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Types (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Triggers (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}
function Variables (elements, loc) {
  this.class = this.constructor.name;
  this.elements = elements || [];
  this.loc = loc || '';
}

function ast2list (ast) {
  if (ast === undefined || ast === null) {
    return ast;
  }
  if (typeof (ast) === 'object' && (ast.class == 'Location')) {
    return '';
  }
  if (typeof (ast) === 'object' && ast.elements) {
    return [ast.class
            .charAt (0).toLowerCase() + ast.class.slice(1)
           ].concat (ast.elements
                     .map (function (i) {return ast2list (i);}));
  }
  if (typeof (ast) === 'object' && ast.class) {
    if (ast.class === 'Event' || ast.class === 'Port') {
      ast.class = ast.direction;
      delete ast['direction'];
    }
    ast.class = {
      OutBinding: '<-',
      Equals: '==',
      Greater: '>',
      GreaterEqual: '>=',
      Less: '<',
      LessEqual: '<=',
      Minus: '-',
      Not: '!',
      NotEquals: '!=',
      Plus: '+',
    }[ast.class] || ast.class;
    return [ast.class
            .charAt (0).toLowerCase() + ast.class.slice(1)
           ].concat (Object.keys (ast)
                     .slice (1)
                     .map (function (key, index) { return ast2list (ast[key]);}));
  }
  if (typeof (ast) === 'string'
      || typeof (ast) === 'number') {
    return ast;
  }
  if (ast.length || ast.length === 0) {
    return ast.map (function (i) {return ast2list (i);});
  }
  return ast;
}

function list2scm (scm) {
  if (scm === null) {
    return '#f';
  }
  if (scm === '') {
    return '';
  }
  if (scm === false) {
    return '#f';
  }
  if (scm === true) {
    return '#t';
  }
  if (scm && typeof (scm) === 'object' && scm.length) {
    return ('('
            + scm.map (list2scm)
            .filter (function (s) {return s !== '';})
            .join (' ')
            + ')');
  }
  return '' + scm;
}

function is_a (o, type) {
  if (o === null) {
    return false;
  }
  if (o && o.class)
    return o.class === type;
  if (typeof (o) === 'object' && (o.length || o.length === 0))
    return type === 'Array';
  return false;
}

function add_scope (scope, o) {
  //o.name.elements = add_scope (scope, o.name.elements)
  //o.name.elements = scope.elements.concat (o.name.elements);
  if (ast.is_a (o, 'Interface')) {
    return new ast.Interface (add_scope (scope, o.name),
                              add_scope (scope, o.types),
                              add_scope (scope, o.events),
                              add_scope (scope, o.behaviour),
                              o.loc);
  }
  if (ast.is_a (o, 'Component')) {
    return new ast.Component (add_scope (scope, o.name),
                              add_scope (scope, o.ports),
                              add_scope (scope, o.behaviour),
                              o.loc);
  }
  if (ast.is_a (o, 'Behaviour')) {
    return new ast.Behaviour (o.name,
                              add_scope (scope, o.types),
                              o.variables,
                              o.functions,
                              o.statements,
                              o.loc);
  }
  if (ast.is_a (o, 'Name') && o.elements[0] === '*global*') {
    return o;
  }
  if (ast.is_a (o, 'Name')) {
    return new ast.Name (scope.elements.concat (o.elements), o.loc);
  }
  if (ast.is_a (o, 'Port')) {
    return new ast.Port (o.direction,
                         add_scope (scope, o.type),
                         o.name,
                         o.external,
                         o.injected,
                         o.loc);
  }
  if (ast.is_a (o, 'Ports')) {
    return new ast.Ports (o.elements.map (function (t) {return add_scope (scope, t);}), o.loc);
  }
  if (ast.is_a (o, 'Types')) {
    return new ast.Types (o.elements.map (function (t) {return add_scope (scope, t);}), o.loc);
  }
  if (ast.is_a (o, 'Enum')) {
    return new ast.Enum (add_scope (scope, o.name), o.fields, o.loc);
  }
  if (ast.is_a (o, 'Extern')) {
    return new ast.Extern (add_scope (scope, o.name), o.data, o.loc);
  }
  if (ast.is_a (o, 'Int')) {
    return new ast.Int (add_scope (scope, o.name), o.range, o.loc);
  }
  return o;
}

var ast = {
  add_scope: add_scope,
  is_a: is_a,

  Location: Location,
  Position: Position,

  Root: Root,

  //Models: Models
  Import: Import,
  Interface: Interface,
  Component: Component,
  System: System,

  //Interals: Interals
  Event: Event,
  Signature: Signature,
  Formal: Formal,
  Parameter: Parameter,
  Function: Function,
  Port: Port,
  Name: Name,
  Type: Type,
  Int: Int,
  Range: Range,
  Literal: Literal,
  Enum: Enum,
  Extern: Extern,
  Data: Data,
  Behaviour: Behaviour,
  Trigger: Trigger,
  Instance: Instance,
  Variable: Variable,
  Value: Value,
  Var: Var,
  // Statements: Statements
  Action: Action,
  Assign: Assign,
  Blocking: Blocking,
  Call: Call,
  Compound: Compound,
  Guard: Guard,
  If: If,
  Illegal: Illegal,
  On: On,
  Reply: Reply,
  Return: Return,

  //System: System
  Bind: Bind,
  Binding: Binding,

  // Expressions: Expressions
  Expression: Expression,
  And: And,
  Equals: Equals,
  OutBinding: OutBinding,
  Greater: Greater,
  GreaterEqual: GreaterEqual,
  Less: Less,
  LessEqual: LessEqual,
  Minus: Minus,
  Or: Or,
  Plus: Plus,

  Group: Group,
  Not: Not,

  Otherwise: Otherwise,

  // lists
  Arguments: Arguments,
  Bindings: Bindings,
  Events: Events,
  Formals: Formals,
  Functions: Functions,
  Instances: Instances,
  Parameters: Parameters,
  Ports: Ports,
  Types: Types,
  Triggers: Triggers,
  Variables: Variables,

  ast2list: ast2list,
  list2scm: list2scm,
}
module.exports = ast;
