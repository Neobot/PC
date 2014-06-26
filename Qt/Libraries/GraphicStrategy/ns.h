enum SymbolConstants
{
   SYM_EOF                        =   0, // (EOF)
   SYM_ERROR                      =   1, // (Error)
   SYM_COMMENT                    =   2, // Comment
   SYM_WHITESPACE                 =   3, // Whitespace
   SYM_TIMESDIV                   =   4, // '*/'
   SYM_DIVTIMES                   =   5, // '/*'
   SYM_DIVDIV                     =   6, // '//'
   SYM_MINUS                      =   7, // '-'
   SYM_PERCENT                    =   8, // '%'
   SYM_LPAREN                     =   9, // '('
   SYM_RPAREN                     =  10, // ')'
   SYM_COMMA                      =  11, // ','
   SYM_SEMI                       =  12, // ';'
   SYM_LBRACE                     =  13, // '{'
   SYM_RBRACE                     =  14, // '}'
   SYM_EQ                         =  15, // '='
   SYM_DEGREE                     =  16, // '°'
   SYM_ACTION                     =  17, // action
   SYM_ALL                        =  18, // all
   SYM_ALLACTIONSFINISHED         =  19, // AllActionsFinished
   SYM_AS                         =  20, // as
   SYM_ASYNC                      =  21, // async
   SYM_AUTO                       =  22, // auto
   SYM_AX12                       =  23, // 'ax12'
   SYM_BACKWARD                   =  24, // backward
   SYM_BLACK                      =  25, // black
   SYM_BLUE                       =  26, // blue
   SYM_CASE                       =  27, // case
   SYM_CLOSE                      =  28, // close
   SYM_COLOR                      =  29, // color
   SYM_CONCURRENT                 =  30, // concurrent
   SYM_DECLARE                    =  31, // declare
   SYM_DEFAULTS                   =  32, // defaults
   SYM_DETECTED                   =  33, // detected
   SYM_DISABLE                    =  34, // disable
   SYM_DO                         =  35, // do
   SYM_ELSE                       =  36, // else
   SYM_ENABLE                     =  37, // enable
   SYM_EOF2                       =  38, // eof
   SYM_FAR                        =  39, // far
   SYM_FIRSTACTIONFINISHED        =  40, // FirstActionFinished
   SYM_FLOAT                      =  41, // Float
   SYM_FORWARD                    =  42, // forward
   SYM_GO                         =  43, // go
   SYM_GREEN                      =  44, // green
   SYM_IDENTIFIER                 =  45, // Identifier
   SYM_IF                         =  46, // if
   SYM_IMPORT                     =  47, // import
   SYM_IN                         =  48, // in
   SYM_INTEGER                    =  49, // Integer
   SYM_IS                         =  50, // is
   SYM_ISNOT                      =  51, // 'is not'
   SYM_LASTACTIONFINISHED         =  52, // LastActionFinished
   SYM_MAKE                       =  53, // make
   SYM_MICROSWITCH                =  54, // microswitch
   SYM_MOVE                       =  55, // move
   SYM_MOVEMENT                   =  56, // movement
   SYM_MS                         =  57, // ms
   SYM_NEWLINE                    =  58, // NewLine
   SYM_NOTIN                      =  59, // 'not in'
   SYM_OFF                        =  60, // off
   SYM_ON                         =  61, // on
   SYM_ONEACTIONFINISHED          =  62, // OneActionFinished
   SYM_OPPONENT                   =  63, // opponent
   SYM_ORIENTATION                =  64, // orientation
   SYM_PARAMETER                  =  65, // parameter
   SYM_POSITION                   =  66, // position
   SYM_R                          =  67, // r
   SYM_RECT                       =  68, // rect
   SYM_RED                        =  69, // red
   SYM_REVERSED                   =  70, // reversed
   SYM_S                          =  71, // s
   SYM_SENSOR                     =  72, // sensor
   SYM_SET                        =  73, // set
   SYM_SHARP                      =  74, // sharp
   SYM_SPEED                      =  75, // speed
   SYM_STRATEGY                   =  76, // strategy
   SYM_STRING                     =  77, // String
   SYM_SWITCH                     =  78, // switch
   SYM_TELEPORT                   =  79, // teleport
   SYM_TO                         =  80, // to
   SYM_UNKNOWN                    =  81, // unknown
   SYM_WAIT                       =  82, // wait
   SYM_WHILE                      =  83, // while
   SYM_WHITE                      =  84, // white
   SYM_YELLOW                     =  85, // yellow
   SYM_ACTION2                    =  86, // <action>
   SYM_ACTION_OR_VAR              =  87, // <action_or_var>
   SYM_ACTUATORSTATEMENT          =  88, // <ActuatorStatement>
   SYM_ANGLE                      =  89, // <angle>
   SYM_ANGLERANGE                 =  90, // <angleRange>
   SYM_AX12_IDENTIFIER            =  91, // <ax12_identifier>
   SYM_AX12_OR_VAR                =  92, // <ax12_or_var>
   SYM_AX12ASYNCMOVEMENTSTATEMENT =  93, // <Ax12AsyncMovementStatement>
   SYM_AX12MOVEMENTSTATEMENT      =  94, // <Ax12MovementStatement>
   SYM_COLORSENSORVALUE           =  95, // <colorSensorValue>
   SYM_CONCURRENCYCONDITION       =  96, // <concurrencyCondition>
   SYM_CONCURRENTSTATEMENT        =  97, // <ConcurrentStatement>
   SYM_CONDITIONINOPERATOR        =  98, // <ConditionInOperator>
   SYM_CONDITIONISOPERATOR        =  99, // <ConditionIsOperator>
   SYM_CONDITIONSTATEMENT         = 100, // <ConditionStatement>
   SYM_DECLAREACTIONSTATEMENT     = 101, // <DeclareActionStatement>
   SYM_DECLAREAX12STATEMENT       = 102, // <DeclareAx12Statement>
   SYM_DECLAREPARAMSTATEMENT      = 103, // <DeclareParamStatement>
   SYM_DECLAREPOINTSTATEMENT      = 104, // <DeclarePointStatement>
   SYM_DECLARERECTSTATEMENT       = 105, // <DeclareRectStatement>
   SYM_DECLARESENSORSTATEMENT     = 106, // <DeclareSensorStatement>
   SYM_DIRECTION                  = 107, // <direction>
   SYM_DISABLESENSORSTATEMENT     = 108, // <DisableSensorStatement>
   SYM_ENABLESENSORSTATEMENT      = 109, // <EnableSensorStatement>
   SYM_ID                         = 110, // <id>
   SYM_IFCONDITION                = 111, // <IfCondition>
   SYM_IMPORTSTATEMENT            = 112, // <ImportStatement>
   SYM_LISTSTATEMENT              = 113, // <ListStatement>
   SYM_MICROSWITCHVALUE           = 114, // <microSwitchValue>
   SYM_MOVEAX12STATEMENT          = 115, // <MoveAx12Statement>
   SYM_MOVETOSTATEMENT            = 116, // <MoveToStatement>
   SYM_NL                         = 117, // <nl>
   SYM_NLOPT                      = 118, // <nl Opt>
   SYM_NUM                        = 119, // <num>
   SYM_OPPONENTCONDITION          = 120, // <OpponentCondition>
   SYM_OPT_BRACKET                = 121, // <opt_bracket>
   SYM_OPT_DIRECTION              = 122, // <opt_direction>
   SYM_OPT_SPEED                  = 123, // <opt_speed>
   SYM_OPT_TIME                   = 124, // <opt_time>
   SYM_ORIENTATIONCONDITION       = 125, // <OrientationCondition>
   SYM_PARAMETER_IDENTIFIER       = 126, // <parameter_identifier>
   SYM_PARAMETER_OR_VAR           = 127, // <parameter_or_var>
   SYM_POINT                      = 128, // <point>
   SYM_POINT_OR_VAR               = 129, // <point_or_var>
   SYM_POSITIONCONDITION          = 130, // <PositionCondition>
   SYM_PROGRAM                    = 131, // <Program>
   SYM_R_POINT                    = 132, // <r_point>
   SYM_R_RECT                     = 133, // <r_rect>
   SYM_RECT2                      = 134, // <rect>
   SYM_RECT_OR_VAR                = 135, // <rect_or_var>
   SYM_REVERSECONDITION           = 136, // <reverseCondition>
   SYM_SENSOR2                    = 137, // <sensor>
   SYM_SENSOR_IDENTIFIER          = 138, // <sensor_identifier>
   SYM_SENSOR_OR_VAR              = 139, // <sensor_or_var>
   SYM_SENSORCONDITION            = 140, // <SensorCondition>
   SYM_SENSORVALUE                = 141, // <sensorValue>
   SYM_SETPARAMSTATEMENT          = 142, // <SetParamStatement>
   SYM_SHARPVALUE                 = 143, // <sharpValue>
   SYM_SPEED2                     = 144, // <speed>
   SYM_START                      = 145, // <Start>
   SYM_STATEMENT                  = 146, // <Statement>
   SYM_STATEMENT_END              = 147, // <statement_end>
   SYM_STATEMENTS                 = 148, // <Statements>
   SYM_SWITCH_CASE                = 149, // <switch_case>
   SYM_SWITCH_CASES               = 150, // <switch_cases>
   SYM_SWITCH_VALUE               = 151, // <switch_value>
   SYM_SWITCHCONDITION            = 152, // <SwitchCondition>
   SYM_SWITCHSTATEMENT            = 153, // <SwitchStatement>
   SYM_TELEPORTSTATEMENT          = 154, // <TeleportStatement>
   SYM_TIME                       = 155, // <time>
   SYM_TIME_UNIT                  = 156, // <time_unit>
   SYM_VAR                        = 157, // <var>
   SYM_WAITSTATEMENT              = 158, // <WaitStatement>
   SYM_WHILESTATEMENT             = 159  // <WhileStatement>
};

enum ProductionConstants
{
   PROD_NL_NEWLINE                                                 =   0, // <nl> ::= NewLine <nl>
   PROD_NL_NEWLINE2                                                =   1, // <nl> ::= NewLine
   PROD_NLOPT_NEWLINE                                              =   2, // <nl Opt> ::= NewLine <nl Opt>
   PROD_NLOPT                                                      =   3, // <nl Opt> ::= 
   PROD_STATEMENT_END_SEMI                                         =   4, // <statement_end> ::= ';'
   PROD_STATEMENT_END                                              =   5, // <statement_end> ::= <nl>
   PROD_STATEMENT_END_EOF                                          =   6, // <statement_end> ::= eof
   PROD_OPT_BRACKET_LPAREN                                         =   7, // <opt_bracket> ::= '('
   PROD_OPT_BRACKET                                                =   8, // <opt_bracket> ::= 
   PROD_ID_INTEGER                                                 =   9, // <id> ::= Integer
   PROD_NUM_FLOAT                                                  =  10, // <num> ::= Float
   PROD_NUM_INTEGER                                                =  11, // <num> ::= Integer
   PROD_SPEED_SPEED_INTEGER_PERCENT                                =  12, // <speed> ::= speed Integer '%'
   PROD_OPT_SPEED                                                  =  13, // <opt_speed> ::= <speed>
   PROD_OPT_SPEED2                                                 =  14, // <opt_speed> ::= 
   PROD_ANGLE_SYM_DEGREE                                           =  15, // <angle> ::= <num> '°'
   PROD_TIME_UNIT_S                                                =  16, // <time_unit> ::= s
   PROD_TIME_UNIT_MS                                               =  17, // <time_unit> ::= ms
   PROD_TIME_INTEGER                                               =  18, // <time> ::= Integer <time_unit>
   PROD_OPT_TIME                                                   =  19, // <opt_time> ::= <time>
   PROD_OPT_TIME2                                                  =  20, // <opt_time> ::= 
   PROD_VAR_IDENTIFIER                                             =  21, // <var> ::= Identifier
   PROD_ANGLERANGE_MINUS                                           =  22, // <angleRange> ::= <angle> '-' <angle>
   PROD_POINT_COMMA                                                =  23, // <point> ::= <opt_bracket> <num> ',' <num> <opt_bracket>
   PROD_POINT_COMMA_COMMA                                          =  24, // <point> ::= <opt_bracket> <num> ',' <num> ',' <angle> <opt_bracket>
   PROD_POINT                                                      =  25, // <point> ::= <r_point>
   PROD_R_POINT_R_LPAREN_RPAREN                                    =  26, // <r_point> ::= r '(' <point> ')'
   PROD_POINT_OR_VAR                                               =  27, // <point_or_var> ::= <point>
   PROD_POINT_OR_VAR2                                              =  28, // <point_or_var> ::= <var>
   PROD_RECT_RECT_COMMA_COMMA_COMMA                                =  29, // <rect> ::= rect <num> ',' <num> ',' <num> ',' <num>
   PROD_RECT                                                       =  30, // <rect> ::= <r_rect>
   PROD_R_RECT_R_LPAREN_RPAREN                                     =  31, // <r_rect> ::= r '(' <rect> ')'
   PROD_RECT_OR_VAR                                                =  32, // <rect_or_var> ::= <rect>
   PROD_RECT_OR_VAR2                                               =  33, // <rect_or_var> ::= <var>
   PROD_DIRECTION_FORWARD                                          =  34, // <direction> ::= forward
   PROD_DIRECTION_BACKWARD                                         =  35, // <direction> ::= backward
   PROD_DIRECTION_AUTO                                             =  36, // <direction> ::= auto
   PROD_OPT_DIRECTION                                              =  37, // <opt_direction> ::= <direction>
   PROD_OPT_DIRECTION2                                             =  38, // <opt_direction> ::= 
   PROD_SENSOR_COLOR_SENSOR                                        =  39, // <sensor> ::= color sensor
   PROD_SENSOR_MICROSWITCH                                         =  40, // <sensor> ::= microswitch
   PROD_SENSOR_SHARP                                               =  41, // <sensor> ::= sharp
   PROD_SENSOR_IDENTIFIER                                          =  42, // <sensor_identifier> ::= <sensor> <id>
   PROD_SENSOR_OR_VAR                                              =  43, // <sensor_or_var> ::= <sensor_identifier>
   PROD_SENSOR_OR_VAR2                                             =  44, // <sensor_or_var> ::= <var>
   PROD_COLORSENSORVALUE_UNKNOWN                                   =  45, // <colorSensorValue> ::= unknown
   PROD_COLORSENSORVALUE_RED                                       =  46, // <colorSensorValue> ::= red
   PROD_COLORSENSORVALUE_GREEN                                     =  47, // <colorSensorValue> ::= green
   PROD_COLORSENSORVALUE_BLUE                                      =  48, // <colorSensorValue> ::= blue
   PROD_COLORSENSORVALUE_YELLOW                                    =  49, // <colorSensorValue> ::= yellow
   PROD_COLORSENSORVALUE_WHITE                                     =  50, // <colorSensorValue> ::= white
   PROD_COLORSENSORVALUE_BLACK                                     =  51, // <colorSensorValue> ::= black
   PROD_SHARPVALUE_DETECTED                                        =  52, // <sharpValue> ::= detected
   PROD_SHARPVALUE_FAR                                             =  53, // <sharpValue> ::= far
   PROD_SHARPVALUE_CLOSE                                           =  54, // <sharpValue> ::= close
   PROD_MICROSWITCHVALUE_ON                                        =  55, // <microSwitchValue> ::= on
   PROD_MICROSWITCHVALUE_OFF                                       =  56, // <microSwitchValue> ::= off
   PROD_SENSORVALUE                                                =  57, // <sensorValue> ::= <colorSensorValue>
   PROD_SENSORVALUE2                                               =  58, // <sensorValue> ::= <sharpValue>
   PROD_SENSORVALUE3                                               =  59, // <sensorValue> ::= <microSwitchValue>
   PROD_SENSORVALUE4                                               =  60, // <sensorValue> ::= <num>
   PROD_CONCURRENCYCONDITION_ALLACTIONSFINISHED                    =  61, // <concurrencyCondition> ::= AllActionsFinished
   PROD_CONCURRENCYCONDITION_ONEACTIONFINISHED                     =  62, // <concurrencyCondition> ::= OneActionFinished
   PROD_CONCURRENCYCONDITION_FIRSTACTIONFINISHED                   =  63, // <concurrencyCondition> ::= FirstActionFinished
   PROD_CONCURRENCYCONDITION_LASTACTIONFINISHED                    =  64, // <concurrencyCondition> ::= LastActionFinished
   PROD_ACTION_ACTION_INTEGER_COMMA_INTEGER_COMMA                  =  65, // <action> ::= action Integer ',' Integer ',' <time>
   PROD_ACTION_OR_VAR                                              =  66, // <action_or_var> ::= <action>
   PROD_ACTION_OR_VAR2                                             =  67, // <action_or_var> ::= <var>
   PROD_PARAMETER_IDENTIFIER_PARAMETER                             =  68, // <parameter_identifier> ::= parameter <id>
   PROD_PARAMETER_OR_VAR                                           =  69, // <parameter_or_var> ::= <parameter_identifier>
   PROD_PARAMETER_OR_VAR2                                          =  70, // <parameter_or_var> ::= <var>
   PROD_AX12_IDENTIFIER_AX12                                       =  71, // <ax12_identifier> ::= 'ax12' <id>
   PROD_AX12_OR_VAR                                                =  72, // <ax12_or_var> ::= <ax12_identifier>
   PROD_AX12_OR_VAR2                                               =  73, // <ax12_or_var> ::= <var>
   PROD_START                                                      =  74, // <Start> ::= <nl Opt> <Program>
   PROD_PROGRAM                                                    =  75, // <Program> ::= <Statements>
   PROD_STATEMENTS                                                 =  76, // <Statements> ::= <Statement> <Statements>
   PROD_STATEMENTS2                                                =  77, // <Statements> ::= 
   PROD_STATEMENT                                                  =  78, // <Statement> ::= <ImportStatement>
   PROD_STATEMENT2                                                 =  79, // <Statement> ::= <WaitStatement>
   PROD_STATEMENT3                                                 =  80, // <Statement> ::= <DeclareActionStatement>
   PROD_STATEMENT4                                                 =  81, // <Statement> ::= <DeclareSensorStatement>
   PROD_STATEMENT5                                                 =  82, // <Statement> ::= <DeclareParamStatement>
   PROD_STATEMENT6                                                 =  83, // <Statement> ::= <DeclarePointStatement>
   PROD_STATEMENT7                                                 =  84, // <Statement> ::= <DeclareRectStatement>
   PROD_STATEMENT8                                                 =  85, // <Statement> ::= <DeclareAx12Statement>
   PROD_STATEMENT9                                                 =  86, // <Statement> ::= <TeleportStatement>
   PROD_STATEMENT10                                                =  87, // <Statement> ::= <MoveToStatement>
   PROD_STATEMENT11                                                =  88, // <Statement> ::= <ListStatement>
   PROD_STATEMENT12                                                =  89, // <Statement> ::= <ConcurrentStatement>
   PROD_STATEMENT13                                                =  90, // <Statement> ::= <SetParamStatement>
   PROD_STATEMENT14                                                =  91, // <Statement> ::= <EnableSensorStatement>
   PROD_STATEMENT15                                                =  92, // <Statement> ::= <DisableSensorStatement>
   PROD_STATEMENT16                                                =  93, // <Statement> ::= <Ax12MovementStatement>
   PROD_STATEMENT17                                                =  94, // <Statement> ::= <Ax12AsyncMovementStatement>
   PROD_STATEMENT18                                                =  95, // <Statement> ::= <MoveAx12Statement>
   PROD_STATEMENT19                                                =  96, // <Statement> ::= <ActuatorStatement>
   PROD_STATEMENT20                                                =  97, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT21                                                =  98, // <Statement> ::= <WhileStatement>
   PROD_STATEMENT22                                                =  99, // <Statement> ::= <SwitchStatement>
   PROD_IMPORTSTATEMENT_IMPORT_STRING                              = 100, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                         = 101, // <WaitStatement> ::= wait <time> <statement_end>
   PROD_DECLAREACTIONSTATEMENT_DECLARE_AS                          = 102, // <DeclareActionStatement> ::= declare <action> as <var> <statement_end>
   PROD_DECLARESENSORSTATEMENT_DECLARE_AS                          = 103, // <DeclareSensorStatement> ::= declare <sensor_identifier> as <var> <statement_end>
   PROD_DECLAREPARAMSTATEMENT_DECLARE_AS                           = 104, // <DeclareParamStatement> ::= declare <parameter_identifier> as <var> <statement_end>
   PROD_DECLAREPOINTSTATEMENT_DECLARE_AS                           = 105, // <DeclarePointStatement> ::= declare <point> as <var> <statement_end>
   PROD_DECLARERECTSTATEMENT_DECLARE_AS                            = 106, // <DeclareRectStatement> ::= declare <rect> as <var> <statement_end>
   PROD_DECLAREAX12STATEMENT_DECLARE_AS                            = 107, // <DeclareAx12Statement> ::= declare <ax12_identifier> as <var> <statement_end>
   PROD_TELEPORTSTATEMENT_TELEPORT                                 = 108, // <TeleportStatement> ::= teleport <point_or_var> <statement_end>
   PROD_MOVETOSTATEMENT_GO_TO                                      = 109, // <MoveToStatement> ::= go to <point_or_var> <opt_speed> <opt_direction> <statement_end>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                                = 110, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}' <statement_end>
   PROD_CONCURRENTSTATEMENT_CONCURRENT                             = 111, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                            = 112, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_SETPARAMSTATEMENT_SET_EQ                                   = 113, // <SetParamStatement> ::= set <parameter_or_var> '=' <num> <statement_end>
   PROD_ENABLESENSORSTATEMENT_ENABLE                               = 114, // <EnableSensorStatement> ::= enable <sensor_or_var> <statement_end>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                           = 115, // <EnableSensorStatement> ::= enable all <sensor> <statement_end>
   PROD_DISABLESENSORSTATEMENT_DISABLE                             = 116, // <DisableSensorStatement> ::= disable <sensor_or_var> <statement_end>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                         = 117, // <DisableSensorStatement> ::= disable all <sensor> <statement_end>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT_STRING            = 118, // <Ax12MovementStatement> ::= make 'ax12' movement String <opt_speed> <statement_end>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT_STRING = 119, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement String <opt_speed> <statement_end>
   PROD_MOVEAX12STATEMENT_MOVE_TO                                  = 120, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <opt_speed> <statement_end>
   PROD_ACTUATORSTATEMENT_DO                                       = 121, // <ActuatorStatement> ::= do <action_or_var> <statement_end>
   PROD_CONDITIONISOPERATOR_IS                                     = 122, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_ISNOT                                  = 123, // <ConditionIsOperator> ::= 'is not'
   PROD_CONDITIONINOPERATOR_IN                                     = 124, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOTIN                                  = 125, // <ConditionInOperator> ::= 'not in'
   PROD_SENSORCONDITION                                            = 126, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                           = 127, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                                 = 128, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                                 = 129, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                         = 130, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                                = 131, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                               = 132, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                               = 133, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                               = 134, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                               = 135, // <IfCondition> ::= <reverseCondition>
   PROD_CONDITIONSTATEMENT_IF                                      = 136, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                                 = 137, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_WHILESTATEMENT_WHILE                                       = 138, // <WhileStatement> ::= while <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_SWITCHSTATEMENT_SWITCH_LBRACE_RBRACE                       = 139, // <SwitchStatement> ::= switch <SwitchCondition> <nl Opt> '{' <nl Opt> <switch_cases> <nl Opt> '}' <statement_end>
   PROD_SWITCHCONDITION_ORIENTATION                                = 140, // <SwitchCondition> ::= orientation
   PROD_SWITCHCONDITION_POSITION                                   = 141, // <SwitchCondition> ::= position
   PROD_SWITCHCONDITION_OPPONENT                                   = 142, // <SwitchCondition> ::= opponent
   PROD_SWITCHCONDITION                                            = 143, // <SwitchCondition> ::= <sensor_or_var>
   PROD_SWITCH_CASES                                               = 144, // <switch_cases> ::= <switch_case> <switch_cases>
   PROD_SWITCH_CASES2                                              = 145, // <switch_cases> ::= 
   PROD_SWITCH_CASE_CASE                                           = 146, // <switch_case> ::= case <switch_value> <nl Opt> <Statements>
   PROD_SWITCH_CASE_DEFAULTS                                       = 147, // <switch_case> ::= defaults <nl Opt> <Statements>
   PROD_SWITCH_VALUE                                               = 148, // <switch_value> ::= <sensorValue>
   PROD_SWITCH_VALUE2                                              = 149, // <switch_value> ::= <angleRange>
   PROD_SWITCH_VALUE3                                              = 150  // <switch_value> ::= <rect_or_var>
};
