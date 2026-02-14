grammar Shell;

line
  : command? EOF
  ;

command
  : word (word)*
  ;

word
  : WORD
  | SQ_STRING
  | DQ_STRING
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
  : ~[ \t\f\v\r\n'"]
  ;

WS
  : [ \t\f\v]+ -> skip
  ;
