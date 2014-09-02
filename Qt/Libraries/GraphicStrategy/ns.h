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
   SYM_LBRACKET                   =  13, // '['
   SYM_RBRACKET                   =  14, // ']'
   SYM_LBRACE                     =  15, // '{'
   SYM_RBRACE                     =  16, // '}'
   SYM_EQ                         =  17, // '='
   SYM_ACTION                     =  18, // action
   SYM_ALL                        =  19, // all
   SYM_ALLACTIONSFINISHED         =  20, // AllActionsFinished
   SYM_AS                         =  21, // as
   SYM_ASYNC                      =  22, // async
   SYM_AUTO                       =  23, // auto
   SYM_AX12                       =  24, // 'ax12'
   SYM_BACKWARD                   =  25, // backward
   SYM_BLACK                      =  26, // black
   SYM_BLUE                       =  27, // blue
   SYM_CASE                       =  28, // case
   SYM_CLOSE                      =  29, // close
   SYM_COLOR                      =  30, // color
   SYM_CONCURRENT                 =  31, // concurrent
   SYM_DECLARE                    =  32, // declare
   SYM_DEFAULTS                   =  33, // defaults
   SYM_DEG                        =  34, // deg
   SYM_DETECTED                   =  35, // detected
   SYM_DISABLE                    =  36, // disable
   SYM_DO                         =  37, // do
   SYM_ELSE                       =  38, // else
   SYM_ENABLE                     =  39, // enable
   SYM_EOF2                       =  40, // eof
   SYM_FAR                        =  41, // far
   SYM_FIRSTACTIONFINISHED        =  42, // FirstActionFinished
   SYM_FLOAT                      =  43, // Float
   SYM_FORWARD                    =  44, // forward
   SYM_FUNCTION                   =  45, // function
   SYM_GO                         =  46, // go
   SYM_GREEN                      =  47, // green
   SYM_IDENTIFIER                 =  48, // Identifier
   SYM_IF                         =  49, // if
   SYM_IMPORT                     =  50, // import
   SYM_IN                         =  51, // in
   SYM_INTEGER                    =  52, // Integer
   SYM_IS                         =  53, // is
   SYM_ISNOT                      =  54, // 'is not'
   SYM_LASTACTIONFINISHED         =  55, // LastActionFinished
   SYM_MAKE                       =  56, // make
   SYM_MICROSWITCH                =  57, // microswitch
   SYM_MOVE                       =  58, // move
   SYM_MOVEMENT                   =  59, // movement
   SYM_MS                         =  60, // ms
   SYM_NEWLINE                    =  61, // NewLine
   SYM_NOTIN                      =  62, // 'not in'
   SYM_OFF                        =  63, // off
   SYM_ON                         =  64, // on
   SYM_ONEACTIONFINISHED          =  65, // OneActionFinished
   SYM_OPPONENT                   =  66, // opponent
   SYM_ORIENTATION                =  67, // orientation
   SYM_PARAMETER                  =  68, // parameter
   SYM_POSITION                   =  69, // position
   SYM_RAD                        =  70, // rad
   SYM_RECT                       =  71, // rect
   SYM_RED                        =  72, // red
   SYM_REVERSED                   =  73, // reversed
   SYM_S                          =  74, // s
   SYM_SENSOR                     =  75, // sensor
   SYM_SET                        =  76, // set
   SYM_SHARP                      =  77, // sharp
   SYM_SPEED                      =  78, // speed
   SYM_STRATEGY                   =  79, // strategy
   SYM_STRING                     =  80, // String
   SYM_SWITCH                     =  81, // switch
   SYM_TELEPORT                   =  82, // teleport
   SYM_TIMEOUT                    =  83, // timeout
   SYM_TO                         =  84, // to
   SYM_UNKNOWN                    =  85, // unknown
   SYM_WAIT                       =  86, // wait
   SYM_WHILE                      =  87, // while
   SYM_WHITE                      =  88, // white
   SYM_YELLOW                     =  89, // yellow
   SYM_ACTION2                    =  90, // <action>
   SYM_ACTION_OR_VAR              =  91, // <action_or_var>
   SYM_ACTUATORSTATEMENT          =  92, // <ActuatorStatement>
   SYM_ANGLE                      =  93, // <angle>
   SYM_ANGLERANGE                 =  94, // <angleRange>
   SYM_AX12_IDENTIFIER            =  95, // <ax12_identifier>
   SYM_AX12_OR_VAR                =  96, // <ax12_or_var>
   SYM_AX12ASYNCMOVEMENTSTATEMENT =  97, // <Ax12AsyncMovementStatement>
   SYM_AX12MOVEMENTSTATEMENT      =  98, // <Ax12MovementStatement>
   SYM_CALLARG                    =  99, // <CallArg>
   SYM_CALLARGLIST                = 100, // <CallArgList>
   SYM_CALLSTATEMENT              = 101, // <CallStatement>
   SYM_COLORSENSORVALUE           = 102, // <colorSensorValue>
   SYM_CONCURRENCYCONDITION       = 103, // <concurrencyCondition>
   SYM_CONCURRENTSTATEMENT        = 104, // <ConcurrentStatement>
   SYM_CONDITIONINOPERATOR        = 105, // <ConditionInOperator>
   SYM_CONDITIONISOPERATOR        = 106, // <ConditionIsOperator>
   SYM_CONDITIONSTATEMENT         = 107, // <ConditionStatement>
   SYM_DECLARESTATEMENT           = 108, // <DeclareStatement>
   SYM_DIRECTION                  = 109, // <direction>
   SYM_DISABLESENSORSTATEMENT     = 110, // <DisableSensorStatement>
   SYM_ENABLESENSORSTATEMENT      = 111, // <EnableSensorStatement>
   SYM_FIXED_ANGLE                = 112, // <fixed_angle>
   SYM_FIXED_POINT                = 113, // <fixed_point>
   SYM_FIXED_RECT                 = 114, // <fixed_rect>
   SYM_FUNCTIONSTATEMENT          = 115, // <FunctionStatement>
   SYM_ID                         = 116, // <id>
   SYM_IFCONDITION                = 117, // <IfCondition>
   SYM_IMPORTSTATEMENT            = 118, // <ImportStatement>
   SYM_LISTSTATEMENT              = 119, // <ListStatement>
   SYM_MICROSWITCHVALUE           = 120, // <microSwitchValue>
   SYM_MOVEAX12STATEMENT          = 121, // <MoveAx12Statement>
   SYM_MOVETOSTATEMENT            = 122, // <MoveToStatement>
   SYM_NL                         = 123, // <nl>
   SYM_NLOPT                      = 124, // <nl Opt>
   SYM_NUM                        = 125, // <num>
   SYM_OPPONENTCONDITION          = 126, // <OpponentCondition>
   SYM_OPT_BRACKET                = 127, // <opt_bracket>
   SYM_OPT_TIME                   = 128, // <opt_time>
   SYM_ORIENTATIONCONDITION       = 129, // <OrientationCondition>
   SYM_PARAMETER_IDENTIFIER       = 130, // <parameter_identifier>
   SYM_PARAMETER_OR_VAR           = 131, // <parameter_or_var>
   SYM_POINT                      = 132, // <point>
   SYM_POINT_OR_VAR               = 133, // <point_or_var>
   SYM_POSITIONCONDITION          = 134, // <PositionCondition>
   SYM_PROGRAM                    = 135, // <Program>
   SYM_RECT2                      = 136, // <rect>
   SYM_RECT_OR_VAR                = 137, // <rect_or_var>
   SYM_REVERSECONDITION           = 138, // <reverseCondition>
   SYM_SENSOR2                    = 139, // <sensor>
   SYM_SENSOR_IDENTIFIER          = 140, // <sensor_identifier>
   SYM_SENSOR_OR_VAR              = 141, // <sensor_or_var>
   SYM_SENSORCONDITION            = 142, // <SensorCondition>
   SYM_SENSORVALUE                = 143, // <sensorValue>
   SYM_SETPARAMSTATEMENT          = 144, // <SetParamStatement>
   SYM_SHARPVALUE                 = 145, // <sharpValue>
   SYM_SPEED2                     = 146, // <speed>
   SYM_START                      = 147, // <Start>
   SYM_STATEMENT                  = 148, // <Statement>
   SYM_STATEMENT_END              = 149, // <statement_end>
   SYM_STATEMENTS                 = 150, // <Statements>
   SYM_STRING_OR_VAR              = 151, // <string_or_var>
   SYM_SWITCH_CASE                = 152, // <switch_case>
   SYM_SWITCH_CASES               = 153, // <switch_cases>
   SYM_SWITCH_VALUE               = 154, // <switch_value>
   SYM_SWITCHCONDITION            = 155, // <SwitchCondition>
   SYM_SWITCHSTATEMENT            = 156, // <SwitchStatement>
   SYM_TELEPORTSTATEMENT          = 157, // <TeleportStatement>
   SYM_TIME                       = 158, // <time>
   SYM_TIME_UNIT                  = 159, // <time_unit>
   SYM_TIMEOUT2                   = 160, // <timeout>
   SYM_VAR                        = 161, // <var>
   SYM_VARLIST                    = 162, // <varList>
   SYM_WAITSTATEMENT              = 163, // <WaitStatement>
   SYM_WHILESTATEMENT             = 164  // <WhileStatement>
};

enum ProductionConstants
{
   PROD_NL_NEWLINE                                           =   0, // <nl> ::= NewLine <nl>
   PROD_NL_NEWLINE2                                          =   1, // <nl> ::= NewLine
   PROD_NLOPT_NEWLINE                                        =   2, // <nl Opt> ::= NewLine <nl Opt>
   PROD_NLOPT                                                =   3, // <nl Opt> ::= 
   PROD_STATEMENT_END_SEMI                                   =   4, // <statement_end> ::= ';' <nl Opt>
   PROD_STATEMENT_END                                        =   5, // <statement_end> ::= <nl>
   PROD_STATEMENT_END_EOF                                    =   6, // <statement_end> ::= eof
   PROD_OPT_BRACKET_LPAREN                                   =   7, // <opt_bracket> ::= '('
   PROD_OPT_BRACKET                                          =   8, // <opt_bracket> ::= 
   PROD_ID_INTEGER                                           =   9, // <id> ::= Integer
   PROD_NUM_FLOAT                                            =  10, // <num> ::= Float
   PROD_NUM_INTEGER                                          =  11, // <num> ::= Integer
   PROD_SPEED_SPEED_INTEGER_PERCENT                          =  12, // <speed> ::= speed Integer '%'
   PROD_FIXED_ANGLE_DEG                                      =  13, // <fixed_angle> ::= <num> deg
   PROD_FIXED_ANGLE_RAD                                      =  14, // <fixed_angle> ::= <num> rad
   PROD_FIXED_ANGLE                                          =  15, // <fixed_angle> ::= <num>
   PROD_ANGLE                                                =  16, // <angle> ::= <fixed_angle>
   PROD_ANGLE_AUTO_REVERSED                                  =  17, // <angle> ::= auto reversed <fixed_angle>
   PROD_ANGLE_REVERSED                                       =  18, // <angle> ::= reversed <fixed_angle>
   PROD_TIME_UNIT_S                                          =  19, // <time_unit> ::= s
   PROD_TIME_UNIT_MS                                         =  20, // <time_unit> ::= ms
   PROD_TIME_INTEGER                                         =  21, // <time> ::= Integer <time_unit>
   PROD_OPT_TIME                                             =  22, // <opt_time> ::= <time>
   PROD_OPT_TIME2                                            =  23, // <opt_time> ::= 
   PROD_STRING_OR_VAR_STRING                                 =  24, // <string_or_var> ::= String
   PROD_STRING_OR_VAR                                        =  25, // <string_or_var> ::= <var>
   PROD_VAR_IDENTIFIER                                       =  26, // <var> ::= Identifier
   PROD_ANGLERANGE_MINUS                                     =  27, // <angleRange> ::= <angle> '-' <angle>
   PROD_FIXED_POINT_COMMA                                    =  28, // <fixed_point> ::= <opt_bracket> <num> ',' <num> <opt_bracket>
   PROD_FIXED_POINT_COMMA_COMMA                              =  29, // <fixed_point> ::= <opt_bracket> <num> ',' <num> ',' <fixed_angle> <opt_bracket>
   PROD_POINT                                                =  30, // <point> ::= <fixed_point>
   PROD_POINT_AUTO_REVERSED                                  =  31, // <point> ::= auto reversed <fixed_point>
   PROD_POINT_REVERSED                                       =  32, // <point> ::= reversed <fixed_point>
   PROD_POINT_OR_VAR                                         =  33, // <point_or_var> ::= <point>
   PROD_POINT_OR_VAR2                                        =  34, // <point_or_var> ::= <var>
   PROD_FIXED_RECT_RECT_COMMA_COMMA_COMMA                    =  35, // <fixed_rect> ::= rect <num> ',' <num> ',' <num> ',' <num>
   PROD_RECT                                                 =  36, // <rect> ::= <fixed_rect>
   PROD_RECT_AUTO_REVERSED                                   =  37, // <rect> ::= auto reversed <fixed_rect>
   PROD_RECT_REVERSED                                        =  38, // <rect> ::= reversed <fixed_rect>
   PROD_RECT_OR_VAR                                          =  39, // <rect_or_var> ::= <rect>
   PROD_RECT_OR_VAR2                                         =  40, // <rect_or_var> ::= <var>
   PROD_DIRECTION_FORWARD                                    =  41, // <direction> ::= forward
   PROD_DIRECTION_BACKWARD                                   =  42, // <direction> ::= backward
   PROD_DIRECTION_AUTO                                       =  43, // <direction> ::= auto
   PROD_SENSOR_COLOR_SENSOR                                  =  44, // <sensor> ::= color sensor
   PROD_SENSOR_MICROSWITCH                                   =  45, // <sensor> ::= microswitch
   PROD_SENSOR_SHARP                                         =  46, // <sensor> ::= sharp
   PROD_SENSOR_IDENTIFIER                                    =  47, // <sensor_identifier> ::= <sensor> <id>
   PROD_SENSOR_OR_VAR                                        =  48, // <sensor_or_var> ::= <sensor_identifier>
   PROD_SENSOR_OR_VAR2                                       =  49, // <sensor_or_var> ::= <var>
   PROD_COLORSENSORVALUE_UNKNOWN                             =  50, // <colorSensorValue> ::= unknown
   PROD_COLORSENSORVALUE_RED                                 =  51, // <colorSensorValue> ::= red
   PROD_COLORSENSORVALUE_GREEN                               =  52, // <colorSensorValue> ::= green
   PROD_COLORSENSORVALUE_BLUE                                =  53, // <colorSensorValue> ::= blue
   PROD_COLORSENSORVALUE_YELLOW                              =  54, // <colorSensorValue> ::= yellow
   PROD_COLORSENSORVALUE_WHITE                               =  55, // <colorSensorValue> ::= white
   PROD_COLORSENSORVALUE_BLACK                               =  56, // <colorSensorValue> ::= black
   PROD_SHARPVALUE_DETECTED                                  =  57, // <sharpValue> ::= detected
   PROD_SHARPVALUE_FAR                                       =  58, // <sharpValue> ::= far
   PROD_SHARPVALUE_CLOSE                                     =  59, // <sharpValue> ::= close
   PROD_MICROSWITCHVALUE_ON                                  =  60, // <microSwitchValue> ::= on
   PROD_MICROSWITCHVALUE_OFF                                 =  61, // <microSwitchValue> ::= off
   PROD_SENSORVALUE                                          =  62, // <sensorValue> ::= <colorSensorValue>
   PROD_SENSORVALUE2                                         =  63, // <sensorValue> ::= <sharpValue>
   PROD_SENSORVALUE3                                         =  64, // <sensorValue> ::= <microSwitchValue>
   PROD_SENSORVALUE4                                         =  65, // <sensorValue> ::= <num>
   PROD_CONCURRENCYCONDITION_ALLACTIONSFINISHED              =  66, // <concurrencyCondition> ::= AllActionsFinished
   PROD_CONCURRENCYCONDITION_ONEACTIONFINISHED               =  67, // <concurrencyCondition> ::= OneActionFinished
   PROD_CONCURRENCYCONDITION_FIRSTACTIONFINISHED             =  68, // <concurrencyCondition> ::= FirstActionFinished
   PROD_CONCURRENCYCONDITION_LASTACTIONFINISHED              =  69, // <concurrencyCondition> ::= LastActionFinished
   PROD_ACTION_ACTION_INTEGER_COMMA_INTEGER_COMMA            =  70, // <action> ::= action Integer ',' Integer ',' <time>
   PROD_ACTION_OR_VAR                                        =  71, // <action_or_var> ::= <action>
   PROD_ACTION_OR_VAR2                                       =  72, // <action_or_var> ::= <var>
   PROD_PARAMETER_IDENTIFIER_PARAMETER                       =  73, // <parameter_identifier> ::= parameter <id>
   PROD_PARAMETER_OR_VAR                                     =  74, // <parameter_or_var> ::= <parameter_identifier>
   PROD_PARAMETER_OR_VAR2                                    =  75, // <parameter_or_var> ::= <var>
   PROD_AX12_IDENTIFIER_AX12                                 =  76, // <ax12_identifier> ::= 'ax12' <id>
   PROD_AX12_OR_VAR                                          =  77, // <ax12_or_var> ::= <ax12_identifier>
   PROD_AX12_OR_VAR2                                         =  78, // <ax12_or_var> ::= <var>
   PROD_START                                                =  79, // <Start> ::= <nl Opt> <Program>
   PROD_PROGRAM                                              =  80, // <Program> ::= <Statements>
   PROD_STATEMENTS                                           =  81, // <Statements> ::= <Statement> <statement_end> <Statements>
   PROD_STATEMENTS2                                          =  82, // <Statements> ::= <Statement>
   PROD_STATEMENTS3                                          =  83, // <Statements> ::= 
   PROD_STATEMENT                                            =  84, // <Statement> ::= <ImportStatement>
   PROD_STATEMENT2                                           =  85, // <Statement> ::= <WaitStatement>
   PROD_STATEMENT3                                           =  86, // <Statement> ::= <DeclareStatement>
   PROD_STATEMENT4                                           =  87, // <Statement> ::= <TeleportStatement>
   PROD_STATEMENT5                                           =  88, // <Statement> ::= <MoveToStatement>
   PROD_STATEMENT6                                           =  89, // <Statement> ::= <SetParamStatement>
   PROD_STATEMENT7                                           =  90, // <Statement> ::= <EnableSensorStatement>
   PROD_STATEMENT8                                           =  91, // <Statement> ::= <DisableSensorStatement>
   PROD_STATEMENT9                                           =  92, // <Statement> ::= <Ax12MovementStatement>
   PROD_STATEMENT10                                          =  93, // <Statement> ::= <Ax12AsyncMovementStatement>
   PROD_STATEMENT11                                          =  94, // <Statement> ::= <MoveAx12Statement>
   PROD_STATEMENT12                                          =  95, // <Statement> ::= <ActuatorStatement>
   PROD_STATEMENT13                                          =  96, // <Statement> ::= <ListStatement>
   PROD_STATEMENT14                                          =  97, // <Statement> ::= <ConcurrentStatement>
   PROD_STATEMENT15                                          =  98, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT16                                          =  99, // <Statement> ::= <WhileStatement>
   PROD_STATEMENT17                                          = 100, // <Statement> ::= <SwitchStatement>
   PROD_STATEMENT18                                          = 101, // <Statement> ::= <FunctionStatement>
   PROD_STATEMENT19                                          = 102, // <Statement> ::= <CallStatement>
   PROD_IMPORTSTATEMENT_IMPORT_STRING                        = 103, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                   = 104, // <WaitStatement> ::= wait <time>
   PROD_DECLARESTATEMENT_DECLARE_AS                          = 105, // <DeclareStatement> ::= declare <action> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS2                         = 106, // <DeclareStatement> ::= declare <sensor_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS3                         = 107, // <DeclareStatement> ::= declare <parameter_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS4                         = 108, // <DeclareStatement> ::= declare <point> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS5                         = 109, // <DeclareStatement> ::= declare <rect> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS6                         = 110, // <DeclareStatement> ::= declare <ax12_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_STRING_AS                   = 111, // <DeclareStatement> ::= declare String as <var>
   PROD_TELEPORTSTATEMENT_TELEPORT                           = 112, // <TeleportStatement> ::= teleport <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO                                = 113, // <MoveToStatement> ::= go to <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO2                               = 114, // <MoveToStatement> ::= go to <point_or_var> <speed>
   PROD_MOVETOSTATEMENT_GO_TO3                               = 115, // <MoveToStatement> ::= go to <point_or_var> <direction>
   PROD_MOVETOSTATEMENT_GO_TO4                               = 116, // <MoveToStatement> ::= go to <point_or_var> <speed> <direction>
   PROD_MOVETOSTATEMENT_GO_TO5                               = 117, // <MoveToStatement> ::= go to <point_or_var> <direction> <speed>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                          = 118, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}'
   PROD_CONCURRENTSTATEMENT_CONCURRENT                       = 119, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                      = 120, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_SETPARAMSTATEMENT_SET_EQ                             = 121, // <SetParamStatement> ::= set <parameter_or_var> '=' <num>
   PROD_ENABLESENSORSTATEMENT_ENABLE                         = 122, // <EnableSensorStatement> ::= enable <sensor_or_var>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                     = 123, // <EnableSensorStatement> ::= enable all <sensor>
   PROD_DISABLESENSORSTATEMENT_DISABLE                       = 124, // <DisableSensorStatement> ::= disable <sensor_or_var>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                   = 125, // <DisableSensorStatement> ::= disable all <sensor>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT             = 126, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT2            = 127, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var> <speed>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT  = 128, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT2 = 129, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var> <speed>
   PROD_MOVEAX12STATEMENT_MOVE_TO                            = 130, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle>
   PROD_MOVEAX12STATEMENT_MOVE_TO2                           = 131, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <speed>
   PROD_ACTUATORSTATEMENT_DO                                 = 132, // <ActuatorStatement> ::= do <action_or_var>
   PROD_CONDITIONISOPERATOR_IS                               = 133, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_ISNOT                            = 134, // <ConditionIsOperator> ::= 'is not'
   PROD_CONDITIONINOPERATOR_IN                               = 135, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOTIN                            = 136, // <ConditionInOperator> ::= 'not in'
   PROD_SENSORCONDITION                                      = 137, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                     = 138, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                           = 139, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                           = 140, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                   = 141, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                          = 142, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                         = 143, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                         = 144, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                         = 145, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                         = 146, // <IfCondition> ::= <reverseCondition>
   PROD_CONDITIONSTATEMENT_IF                                = 147, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                           = 148, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_TIMEOUT_TIMEOUT                                      = 149, // <timeout> ::= timeout <time>
   PROD_TIMEOUT                                              = 150, // <timeout> ::= 
   PROD_WHILESTATEMENT_WHILE                                 = 151, // <WhileStatement> ::= while <timeout> <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_SWITCHSTATEMENT_SWITCH_LBRACE_RBRACE                 = 152, // <SwitchStatement> ::= switch <SwitchCondition> <nl Opt> '{' <nl Opt> <switch_cases> <nl Opt> '}'
   PROD_SWITCHCONDITION_ORIENTATION                          = 153, // <SwitchCondition> ::= orientation
   PROD_SWITCHCONDITION_POSITION                             = 154, // <SwitchCondition> ::= position
   PROD_SWITCHCONDITION_OPPONENT                             = 155, // <SwitchCondition> ::= opponent
   PROD_SWITCHCONDITION                                      = 156, // <SwitchCondition> ::= <sensor_or_var>
   PROD_SWITCH_CASES                                         = 157, // <switch_cases> ::= <switch_case> <switch_cases>
   PROD_SWITCH_CASES2                                        = 158, // <switch_cases> ::= 
   PROD_SWITCH_CASE_CASE                                     = 159, // <switch_case> ::= case <switch_value> <nl Opt> <Statements>
   PROD_SWITCH_CASE_DEFAULTS                                 = 160, // <switch_case> ::= defaults <nl Opt> <Statements>
   PROD_SWITCH_VALUE                                         = 161, // <switch_value> ::= <sensorValue>
   PROD_SWITCH_VALUE2                                        = 162, // <switch_value> ::= <angleRange>
   PROD_SWITCH_VALUE3                                        = 163, // <switch_value> ::= <rect_or_var>
   PROD_VARLIST_COMMA                                        = 164, // <varList> ::= <var> ',' <varList>
   PROD_VARLIST                                              = 165, // <varList> ::= <var>
   PROD_VARLIST2                                             = 166, // <varList> ::= 
   PROD_FUNCTIONSTATEMENT_FUNCTION_LPAREN_RPAREN             = 167, // <FunctionStatement> ::= function <var> '(' <varList> ')' <nl Opt> <ListStatement>
   PROD_CALLARG                                              = 168, // <CallArg> ::= <var>
   PROD_CALLARG_LBRACKET_RBRACKET                            = 169, // <CallArg> ::= '[' <point> ']'
   PROD_CALLARG_LBRACKET_RBRACKET2                           = 170, // <CallArg> ::= '[' <rect> ']'
   PROD_CALLARG2                                             = 171, // <CallArg> ::= <action>
   PROD_CALLARG3                                             = 172, // <CallArg> ::= <sensor_identifier>
   PROD_CALLARG4                                             = 173, // <CallArg> ::= <parameter_identifier>
   PROD_CALLARG5                                             = 174, // <CallArg> ::= <ax12_identifier>
   PROD_CALLARGLIST_COMMA                                    = 175, // <CallArgList> ::= <CallArg> ',' <CallArgList>
   PROD_CALLARGLIST                                          = 176, // <CallArgList> ::= <CallArg>
   PROD_CALLARGLIST2                                         = 177, // <CallArgList> ::= 
   PROD_CALLSTATEMENT_LPAREN_RPAREN                          = 178  // <CallStatement> ::= <var> '(' <CallArgList> ')'
};
