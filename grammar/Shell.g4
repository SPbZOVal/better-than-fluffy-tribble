grammar Shell;

line
  : EOF
  | stmt EOF
  ;

stmt
  : assignment+ command?
  | command
  ;

assignment
  : NAME '=' value
  ;

command
  : word+
  ;

value
  : word
  ;

word
  : WORD
  // otherwise it matches word as NAME and expects `=`
  | NAME
  | SQ_STRING
  | DQ_STRING
  ;

NAME
  : [a-zA-Z_][a-zA-Z0-9_]*
  ;

SQ_STRING
  : '\'' (~['\r\n])* '\''
  ;

DQ_STRING
  : '"' (~["\r\n])* '"'
  ;

WORD
  : WORD_CHAR+
  ;

fragment WORD_CHAR
  : ~[ \t\f\r\n'"]
  ;

WS
  : [ \t\f]+ -> skip
  ;
