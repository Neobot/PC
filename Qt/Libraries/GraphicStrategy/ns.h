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
   SYM_LASTACTIONFINISHED         =  51, // LastActionFinished
   SYM_MAKE                       =  52, // make
   SYM_MICROSWITCH                =  53, // microswitch
   SYM_MOVE                       =  54, // move
   SYM_MOVEMENT                   =  55, // movement
   SYM_MS                         =  56, // ms
   SYM_NEWLINE                    =  57, // NewLine
   SYM_OFF                        =  58, // off
   SYM_ON                         =  59, // on
   SYM_ONEACTIONFINISHED          =  60, // OneActionFinished
   SYM_ORIENTATION                =  61, // orientation
   SYM_PARAMETER                  =  62, // parameter
   SYM_POSITION                   =  63, // position
   SYM_RECT                       =  64, // Rect
   SYM_RED                        =  65, // red
   SYM_S                          =  66, // s
   SYM_SENSOR                     =  67, // sensor
   SYM_SET                        =  68, // set
   SYM_SHARP                      =  69, // sharp
   SYM_SPEED                      =  70, // speed
   SYM_STRING                     =  71, // String
   SYM_SWITCH                     =  72, // switch
   SYM_TELEPORT                   =  73, // teleport
   SYM_TO                         =  74, // to
   SYM_UNKNOWN                    =  75, // unknown
   SYM_WAIT                       =  76, // wait
   SYM_WHITE                      =  77, // white
   SYM_YELLOW                     =  78, // yellow
   SYM_ACTION2                    =  79, // <action>
   SYM_ACTION_OR_VAR              =  80, // <action_or_var>
   SYM_ACTUATORSTATEMENT          =  81, // <ActuatorStatement>
   SYM_ANGLE                      =  82, // <angle>
   SYM_ANGLERANGE                 =  83, // <angleRange>
   SYM_AX12_IDENTIFIER            =  84, // <ax12_identifier>
   SYM_AX12_OR_VAR                =  85, // <ax12_or_var>
   SYM_AX12ASYNCMOVEMENTSTATEMENT =  86, // <Ax12AsyncMovementStatement>
   SYM_AX12MOVEMENTSTATEMENT      =  87, // <Ax12MovementStatement>
   SYM_COLORSENSORVALUE           =  88, // <colorSensorValue>
   SYM_CONCURRENCYCONDITION       =  89, // <concurrencyCondition>
   SYM_CONCURRENTSTATEMENT        =  90, // <ConcurrentStatement>
   SYM_CONDITIONSTATEMENT         =  91, // <ConditionStatement>
   SYM_DECLAREACTIONSTATEMENT     =  92, // <DeclareActionStatement>
   SYM_DECLAREAX12STATEMENT       =  93, // <DeclareAx12Statement>
   SYM_DECLAREPARAMSTATEMENT      =  94, // <DeclareParamStatement>
   SYM_DECLAREPOINTSTATEMENT      =  95, // <DeclarePointStatement>
   SYM_DECLARERECTSTATEMENT       =  96, // <DeclareRectStatement>
   SYM_DECLARESENSORSTATEMENT     =  97, // <DeclareSensorStatement>
   SYM_DIRECTION                  =  98, // <direction>
   SYM_DISABLESENSORSTATEMENT     =  99, // <DisableSensorStatement>
   SYM_ENABLESENSORSTATEMENT      = 100, // <EnableSensorStatement>
   SYM_ID                         = 101, // <id>
   SYM_IFCONDITION                = 102, // <IfCondition>
   SYM_IMPORTSTATEMENT            = 103, // <ImportStatement>
   SYM_LISTSTATEMENT              = 104, // <ListStatement>
   SYM_MICROSWITCHVALUE           = 105, // <microSwitchValue>
   SYM_MOVEAX12STATEMENT          = 106, // <MoveAx12Statement>
   SYM_MOVETOSTATEMENT            = 107, // <MoveToStatement>
   SYM_NL                         = 108, // <nl>
   SYM_NLOPT                      = 109, // <nl Opt>
   SYM_NUM                        = 110, // <num>
   SYM_OPT_BRACKET                = 111, // <opt_bracket>
   SYM_OPT_DIRECTION              = 112, // <opt_direction>
   SYM_OPT_SPEED                  = 113, // <opt_speed>
   SYM_ORIENTATIONCONDITION       = 114, // <OrientationCondition>
   SYM_PARAMETER_IDENTIFIER       = 115, // <parameter_identifier>
   SYM_PARAMETER_OR_VAR           = 116, // <parameter_or_var>
   SYM_POINT                      = 117, // <point>
   SYM_POINT_OR_VAR               = 118, // <point_or_var>
   SYM_POSITIONCONDITION          = 119, // <PositionCondition>
   SYM_PROGRAM                    = 120, // <Program>
   SYM_RECT2                      = 121, // <rect>
   SYM_RECT_OR_VAR                = 122, // <rect_or_var>
   SYM_SENSOR2                    = 123, // <sensor>
   SYM_SENSOR_IDENTIFIER          = 124, // <sensor_identifier>
   SYM_SENSOR_OR_VAR              = 125, // <sensor_or_var>
   SYM_SENSORCONDITION            = 126, // <SensorCondition>
   SYM_SENSORVALUE                = 127, // <sensorValue>
   SYM_SETPARAMSTATEMENT          = 128, // <SetParamStatement>
   SYM_SHARPVALUE                 = 129, // <sharpValue>
   SYM_SPEED2                     = 130, // <speed>
   SYM_START                      = 131, // <Start>
   SYM_STATEMENT                  = 132, // <Statement>
   SYM_STATEMENT_END              = 133, // <statement_end>
   SYM_STATEMENTS                 = 134, // <Statements>
   SYM_SWITCH_CASE                = 135, // <switch_case>
   SYM_SWITCH_CASES               = 136, // <switch_cases>
   SYM_SWITCH_VALUE               = 137, // <switch_value>
   SYM_SWITCHCONDITION            = 138, // <SwitchCondition>
   SYM_SWITCHSTATEMENT            = 139, // <SwitchStatement>
   SYM_TELEPORTSTATEMENT          = 140, // <TeleportStatement>
   SYM_TIME                       = 141, // <time>
   SYM_TIME_UNIT                  = 142, // <time_unit>
   SYM_VAR                        = 143, // <var>
   SYM_WAITSTATEMENT              = 144  // <WaitStatement>
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
   PROD_ANGLE_DEGREE                                               =  15, // <angle> ::= <num> '°'
   PROD_TIME_UNIT_S                                                =  16, // <time_unit> ::= s
   PROD_TIME_UNIT_MS                                               =  17, // <time_unit> ::= ms
   PROD_TIME_INTEGER                                               =  18, // <time> ::= Integer <time_unit>
   PROD_VAR_IDENTIFIER                                             =  19, // <var> ::= Identifier
   PROD_ANGLERANGE_MINUS                                           =  20, // <angleRange> ::= <angle> '-' <angle>
   PROD_POINT_COMMA                                                =  21, // <point> ::= <opt_bracket> <num> ',' <num> <opt_bracket>
   PROD_POINT_COMMA_COMMA                                          =  22, // <point> ::= <opt_bracket> <num> ',' <num> ',' <angle> <opt_bracket>
   PROD_POINT_OR_VAR                                               =  23, // <point_or_var> ::= <point>
   PROD_POINT_OR_VAR2                                              =  24, // <point_or_var> ::= <var>
   PROD_RECT_RECT_LPAREN_COMMA_COMMA_COMMA_RPAREN                  =  25, // <rect> ::= Rect '(' <num> ',' <num> ',' <num> ',' <num> ')'
   PROD_RECT_OR_VAR                                                =  26, // <rect_or_var> ::= <rect>
   PROD_RECT_OR_VAR2                                               =  27, // <rect_or_var> ::= <var>
   PROD_DIRECTION_FORWARD                                          =  28, // <direction> ::= forward
   PROD_DIRECTION_BACKWARD                                         =  29, // <direction> ::= backward
   PROD_DIRECTION_AUTO                                             =  30, // <direction> ::= auto
   PROD_OPT_DIRECTION                                              =  31, // <opt_direction> ::= <direction>
   PROD_OPT_DIRECTION2                                             =  32, // <opt_direction> ::= 
   PROD_SENSOR_COLOR_SENSOR                                        =  33, // <sensor> ::= color sensor
   PROD_SENSOR_MICROSWITCH                                         =  34, // <sensor> ::= microswitch
   PROD_SENSOR_SHARP                                               =  35, // <sensor> ::= sharp
   PROD_SENSOR_IDENTIFIER                                          =  36, // <sensor_identifier> ::= <sensor> <id>
   PROD_SENSOR_OR_VAR                                              =  37, // <sensor_or_var> ::= <sensor_identifier>
   PROD_SENSOR_OR_VAR2                                             =  38, // <sensor_or_var> ::= <var>
   PROD_COLORSENSORVALUE_UNKNOWN                                   =  39, // <colorSensorValue> ::= unknown
   PROD_COLORSENSORVALUE_RED                                       =  40, // <colorSensorValue> ::= red
   PROD_COLORSENSORVALUE_GREEN                                     =  41, // <colorSensorValue> ::= green
   PROD_COLORSENSORVALUE_BLUE                                      =  42, // <colorSensorValue> ::= blue
   PROD_COLORSENSORVALUE_YELLOW                                    =  43, // <colorSensorValue> ::= yellow
   PROD_COLORSENSORVALUE_WHITE                                     =  44, // <colorSensorValue> ::= white
   PROD_COLORSENSORVALUE_BLACK                                     =  45, // <colorSensorValue> ::= black
   PROD_SHARPVALUE_DETECTED                                        =  46, // <sharpValue> ::= detected
   PROD_SHARPVALUE_FAR                                             =  47, // <sharpValue> ::= far
   PROD_SHARPVALUE_CLOSE                                           =  48, // <sharpValue> ::= close
   PROD_MICROSWITCHVALUE_ON                                        =  49, // <microSwitchValue> ::= on
   PROD_MICROSWITCHVALUE_OFF                                       =  50, // <microSwitchValue> ::= off
   PROD_SENSORVALUE                                                =  51, // <sensorValue> ::= <colorSensorValue>
   PROD_SENSORVALUE2                                               =  52, // <sensorValue> ::= <sharpValue>
   PROD_SENSORVALUE3                                               =  53, // <sensorValue> ::= <microSwitchValue>
   PROD_SENSORVALUE4                                               =  54, // <sensorValue> ::= <num>
   PROD_CONCURRENCYCONDITION_ALLACTIONSFINISHED                    =  55, // <concurrencyCondition> ::= AllActionsFinished
   PROD_CONCURRENCYCONDITION_ONEACTIONFINISHED                     =  56, // <concurrencyCondition> ::= OneActionFinished
   PROD_CONCURRENCYCONDITION_FIRSTACTIONFINISHED                   =  57, // <concurrencyCondition> ::= FirstActionFinished
   PROD_CONCURRENCYCONDITION_LASTACTIONFINISHED                    =  58, // <concurrencyCondition> ::= LastActionFinished
   PROD_ACTION_ACTION_INTEGER_COMMA_INTEGER_COMMA                  =  59, // <action> ::= action Integer ',' Integer ',' <time>
   PROD_ACTION_OR_VAR                                              =  60, // <action_or_var> ::= <action>
   PROD_ACTION_OR_VAR2                                             =  61, // <action_or_var> ::= <var>
   PROD_PARAMETER_IDENTIFIER_PARAMETER                             =  62, // <parameter_identifier> ::= parameter <id>
   PROD_PARAMETER_OR_VAR                                           =  63, // <parameter_or_var> ::= <parameter_identifier>
   PROD_PARAMETER_OR_VAR2                                          =  64, // <parameter_or_var> ::= <var>
   PROD_AX12_IDENTIFIER_AX12                                       =  65, // <ax12_identifier> ::= 'ax12' <id>
   PROD_AX12_OR_VAR                                                =  66, // <ax12_or_var> ::= <ax12_identifier>
   PROD_AX12_OR_VAR2                                               =  67, // <ax12_or_var> ::= <var>
   PROD_START                                                      =  68, // <Start> ::= <nl Opt> <Program>
   PROD_PROGRAM                                                    =  69, // <Program> ::= <Statements>
   PROD_STATEMENTS                                                 =  70, // <Statements> ::= <Statement> <Statements>
   PROD_STATEMENTS2                                                =  71, // <Statements> ::= 
   PROD_STATEMENT                                                  =  72, // <Statement> ::= <ImportStatement>
   PROD_STATEMENT2                                                 =  73, // <Statement> ::= <WaitStatement>
   PROD_STATEMENT3                                                 =  74, // <Statement> ::= <DeclareActionStatement>
   PROD_STATEMENT4                                                 =  75, // <Statement> ::= <DeclareSensorStatement>
   PROD_STATEMENT5                                                 =  76, // <Statement> ::= <DeclareParamStatement>
   PROD_STATEMENT6                                                 =  77, // <Statement> ::= <DeclarePointStatement>
   PROD_STATEMENT7                                                 =  78, // <Statement> ::= <DeclareRectStatement>
   PROD_STATEMENT8                                                 =  79, // <Statement> ::= <DeclareAx12Statement>
   PROD_STATEMENT9                                                 =  80, // <Statement> ::= <TeleportStatement>
   PROD_STATEMENT10                                                =  81, // <Statement> ::= <MoveToStatement>
   PROD_STATEMENT11                                                =  82, // <Statement> ::= <ListStatement>
   PROD_STATEMENT12                                                =  83, // <Statement> ::= <ConcurrentStatement>
   PROD_STATEMENT13                                                =  84, // <Statement> ::= <SetParamStatement>
   PROD_STATEMENT14                                                =  85, // <Statement> ::= <EnableSensorStatement>
   PROD_STATEMENT15                                                =  86, // <Statement> ::= <DisableSensorStatement>
   PROD_STATEMENT16                                                =  87, // <Statement> ::= <Ax12MovementStatement>
   PROD_STATEMENT17                                                =  88, // <Statement> ::= <Ax12AsyncMovementStatement>
   PROD_STATEMENT18                                                =  89, // <Statement> ::= <MoveAx12Statement>
   PROD_STATEMENT19                                                =  90, // <Statement> ::= <ActuatorStatement>
   PROD_STATEMENT20                                                =  91, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT21                                                =  92, // <Statement> ::= <SwitchStatement>
   PROD_IMPORTSTATEMENT_IMPORT_STRING                              =  93, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                         =  94, // <WaitStatement> ::= wait <time> <statement_end>
   PROD_DECLAREACTIONSTATEMENT_DECLARE_AS                          =  95, // <DeclareActionStatement> ::= declare <action> as <var> <statement_end>
   PROD_DECLARESENSORSTATEMENT_DECLARE_AS                          =  96, // <DeclareSensorStatement> ::= declare <sensor_identifier> as <var> <statement_end>
   PROD_DECLAREPARAMSTATEMENT_DECLARE_AS                           =  97, // <DeclareParamStatement> ::= declare <parameter_identifier> as <var> <statement_end>
   PROD_DECLAREPOINTSTATEMENT_DECLARE_AS                           =  98, // <DeclarePointStatement> ::= declare <point> as <var> <statement_end>
   PROD_DECLARERECTSTATEMENT_DECLARE_AS                            =  99, // <DeclareRectStatement> ::= declare <rect> as <var> <statement_end>
   PROD_DECLAREAX12STATEMENT_DECLARE_AS                            = 100, // <DeclareAx12Statement> ::= declare <ax12_identifier> as <var> <statement_end>
   PROD_TELEPORTSTATEMENT_TELEPORT                                 = 101, // <TeleportStatement> ::= teleport <point_or_var> <statement_end>
   PROD_MOVETOSTATEMENT_GO_TO                                      = 102, // <MoveToStatement> ::= go to <point_or_var> <opt_speed> <opt_direction> <statement_end>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                                = 103, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}' <statement_end>
   PROD_CONCURRENTSTATEMENT_CONCURRENT                             = 104, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                            = 105, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_SETPARAMSTATEMENT_SET_EQ                                   = 106, // <SetParamStatement> ::= set <parameter_or_var> '=' <num> <statement_end>
   PROD_ENABLESENSORSTATEMENT_ENABLE                               = 107, // <EnableSensorStatement> ::= enable <sensor_or_var> <statement_end>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                           = 108, // <EnableSensorStatement> ::= enable all <sensor> <statement_end>
   PROD_DISABLESENSORSTATEMENT_DISABLE                             = 109, // <DisableSensorStatement> ::= disable <sensor_or_var> <statement_end>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                         = 110, // <DisableSensorStatement> ::= disable all <sensor> <statement_end>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT_STRING            = 111, // <Ax12MovementStatement> ::= make 'ax12' movement String <opt_speed> <statement_end>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT_STRING = 112, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement String <opt_speed> <statement_end>
   PROD_MOVEAX12STATEMENT_MOVE_TO                                  = 113, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <opt_speed> <statement_end>
   PROD_ACTUATORSTATEMENT_DO                                       = 114, // <ActuatorStatement> ::= do <action_or_var> <statement_end>
   PROD_SENSORCONDITION_IS                                         = 115, // <SensorCondition> ::= <sensor_or_var> is <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION_IN                        = 116, // <OrientationCondition> ::= orientation in <angleRange>
   PROD_POSITIONCONDITION_POSITION_IN                              = 117, // <PositionCondition> ::= position in <rect_or_var>
   PROD_IFCONDITION                                                = 118, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                               = 119, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                               = 120, // <IfCondition> ::= <PositionCondition>
   PROD_CONDITIONSTATEMENT_IF                                      = 121, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                                 = 122, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_SWITCHSTATEMENT_SWITCH_LBRACE_RBRACE                       = 123, // <SwitchStatement> ::= switch <SwitchCondition> <nl Opt> '{' <nl Opt> <switch_cases> <nl Opt> '}' <statement_end>
   PROD_SWITCHCONDITION_ORIENTATION                                = 124, // <SwitchCondition> ::= orientation
   PROD_SWITCHCONDITION_POSITION                                   = 125, // <SwitchCondition> ::= position
   PROD_SWITCHCONDITION                                            = 126, // <SwitchCondition> ::= <sensor_or_var>
   PROD_SWITCH_CASES                                               = 127, // <switch_cases> ::= <switch_case> <switch_cases>
   PROD_SWITCH_CASES2                                              = 128, // <switch_cases> ::= 
   PROD_SWITCH_CASE_CASE                                           = 129, // <switch_case> ::= case <switch_value> <nl Opt> <Statements>
   PROD_SWITCH_CASE_DEFAULTS                                       = 130, // <switch_case> ::= defaults <nl Opt> <Statements>
   PROD_SWITCH_VALUE                                               = 131, // <switch_value> ::= <sensorValue>
   PROD_SWITCH_VALUE2                                              = 132, // <switch_value> ::= <angleRange>
   PROD_SWITCH_VALUE3                                              = 133  // <switch_value> ::= <rect_or_var>
};
