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
  : '\'' ( SQ_ESC | ~['\r\n\\] )* '\''
  ;

DQ_STRING
  : '"' ( DQ_ESC | ~["\r\n\\] )* '"'
  ;

fragment SQ_ESC
  : '\\' '\''
  | '\\' '\\'
  ;

fragment DQ_ESC
  : '\\' '"'
  | '\\' '\\'
  ;

WORD
  : WORD_CHAR+
  ;

fragment WORD_CHAR
  : ~[ \t\r\n'"]
  ;

WS
  : [ \t]+ -> skip
  ;
