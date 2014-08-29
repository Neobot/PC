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
   SYM_COMMA                      =  10, // ','
   SYM_SEMI                       =  11, // ';'
   SYM_LBRACE                     =  12, // '{'
   SYM_RBRACE                     =  13, // '}'
   SYM_EQ                         =  14, // '='
   SYM_ACTION                     =  15, // action
   SYM_ALL                        =  16, // all
   SYM_ALLACTIONSFINISHED         =  17, // AllActionsFinished
   SYM_AS                         =  18, // as
   SYM_ASYNC                      =  19, // async
   SYM_AUTO                       =  20, // auto
   SYM_AX12                       =  21, // 'ax12'
   SYM_BACKWARD                   =  22, // backward
   SYM_BLACK                      =  23, // black
   SYM_BLUE                       =  24, // blue
   SYM_CASE                       =  25, // case
   SYM_CLOSE                      =  26, // close
   SYM_COLOR                      =  27, // color
   SYM_CONCURRENT                 =  28, // concurrent
   SYM_DECLARE                    =  29, // declare
   SYM_DEFAULTS                   =  30, // defaults
   SYM_DEG                        =  31, // deg
   SYM_DETECTED                   =  32, // detected
   SYM_DISABLE                    =  33, // disable
   SYM_DO                         =  34, // do
   SYM_ELSE                       =  35, // else
   SYM_ENABLE                     =  36, // enable
   SYM_EOF2                       =  37, // eof
   SYM_FAR                        =  38, // far
   SYM_FIRSTACTIONFINISHED        =  39, // FirstActionFinished
   SYM_FLOAT                      =  40, // Float
   SYM_FORWARD                    =  41, // forward
   SYM_GO                         =  42, // go
   SYM_GREEN                      =  43, // green
   SYM_IDENTIFIER                 =  44, // Identifier
   SYM_IF                         =  45, // if
   SYM_IMPORT                     =  46, // import
   SYM_IN                         =  47, // in
   SYM_INTEGER                    =  48, // Integer
   SYM_IS                         =  49, // is
   SYM_ISNOT                      =  50, // 'is not'
   SYM_LASTACTIONFINISHED         =  51, // LastActionFinished
   SYM_MAKE                       =  52, // make
   SYM_MICROSWITCH                =  53, // microswitch
   SYM_MOVE                       =  54, // move
   SYM_MOVEMENT                   =  55, // movement
   SYM_MS                         =  56, // ms
   SYM_NEWLINE                    =  57, // NewLine
   SYM_NOTIN                      =  58, // 'not in'
   SYM_OFF                        =  59, // off
   SYM_ON                         =  60, // on
   SYM_ONEACTIONFINISHED          =  61, // OneActionFinished
   SYM_OPPONENT                   =  62, // opponent
   SYM_ORIENTATION                =  63, // orientation
   SYM_PARAMETER                  =  64, // parameter
   SYM_POSITION                   =  65, // position
   SYM_RAD                        =  66, // rad
   SYM_RECT                       =  67, // rect
   SYM_RED                        =  68, // red
   SYM_REVERSED                   =  69, // reversed
   SYM_S                          =  70, // s
   SYM_SENSOR                     =  71, // sensor
   SYM_SET                        =  72, // set
   SYM_SHARP                      =  73, // sharp
   SYM_SPEED                      =  74, // speed
   SYM_STRATEGY                   =  75, // strategy
   SYM_STRING                     =  76, // String
   SYM_SWITCH                     =  77, // switch
   SYM_TELEPORT                   =  78, // teleport
   SYM_TO                         =  79, // to
   SYM_UNKNOWN                    =  80, // unknown
   SYM_WAIT                       =  81, // wait
   SYM_WHILE                      =  82, // while
   SYM_WHITE                      =  83, // white
   SYM_YELLOW                     =  84, // yellow
   SYM_ACTION2                    =  85, // <action>
   SYM_ACTION_OR_VAR              =  86, // <action_or_var>
   SYM_ACTUATORSTATEMENT          =  87, // <ActuatorStatement>
   SYM_ANGLE                      =  88, // <angle>
   SYM_ANGLERANGE                 =  89, // <angleRange>
   SYM_AX12_IDENTIFIER            =  90, // <ax12_identifier>
   SYM_AX12_OR_VAR                =  91, // <ax12_or_var>
   SYM_AX12ASYNCMOVEMENTSTATEMENT =  92, // <Ax12AsyncMovementStatement>
   SYM_AX12MOVEMENTSTATEMENT      =  93, // <Ax12MovementStatement>
   SYM_COLORSENSORVALUE           =  94, // <colorSensorValue>
   SYM_CONCURRENCYCONDITION       =  95, // <concurrencyCondition>
   SYM_CONCURRENTSTATEMENT        =  96, // <ConcurrentStatement>
   SYM_CONDITIONINOPERATOR        =  97, // <ConditionInOperator>
   SYM_CONDITIONISOPERATOR        =  98, // <ConditionIsOperator>
   SYM_CONDITIONSTATEMENT         =  99, // <ConditionStatement>
   SYM_DECLARESTATEMENT           = 100, // <DeclareStatement>
   SYM_DIRECTION                  = 101, // <direction>
   SYM_DISABLESENSORSTATEMENT     = 102, // <DisableSensorStatement>
   SYM_ENABLESENSORSTATEMENT      = 103, // <EnableSensorStatement>
   SYM_FIXED_ANGLE                = 104, // <fixed_angle>
   SYM_FIXED_POINT                = 105, // <fixed_point>
   SYM_FIXED_RECT                 = 106, // <fixed_rect>
   SYM_ID                         = 107, // <id>
   SYM_IFCONDITION                = 108, // <IfCondition>
   SYM_IMPORTSTATEMENT            = 109, // <ImportStatement>
   SYM_LISTSTATEMENT              = 110, // <ListStatement>
   SYM_MICROSWITCHVALUE           = 111, // <microSwitchValue>
   SYM_MOVEAX12STATEMENT          = 112, // <MoveAx12Statement>
   SYM_MOVETOSTATEMENT            = 113, // <MoveToStatement>
   SYM_NL                         = 114, // <nl>
   SYM_NLOPT                      = 115, // <nl Opt>
   SYM_NUM                        = 116, // <num>
   SYM_OPPONENTCONDITION          = 117, // <OpponentCondition>
   SYM_OPT_BRACKET                = 118, // <opt_bracket>
   SYM_OPT_TIME                   = 119, // <opt_time>
   SYM_ORIENTATIONCONDITION       = 120, // <OrientationCondition>
   SYM_PARAMETER_IDENTIFIER       = 121, // <parameter_identifier>
   SYM_PARAMETER_OR_VAR           = 122, // <parameter_or_var>
   SYM_POINT                      = 123, // <point>
   SYM_POINT_OR_VAR               = 124, // <point_or_var>
   SYM_POSITIONCONDITION          = 125, // <PositionCondition>
   SYM_PROGRAM                    = 126, // <Program>
   SYM_RECT2                      = 127, // <rect>
   SYM_RECT_OR_VAR                = 128, // <rect_or_var>
   SYM_REVERSECONDITION           = 129, // <reverseCondition>
   SYM_SENSOR2                    = 130, // <sensor>
   SYM_SENSOR_IDENTIFIER          = 131, // <sensor_identifier>
   SYM_SENSOR_OR_VAR              = 132, // <sensor_or_var>
   SYM_SENSORCONDITION            = 133, // <SensorCondition>
   SYM_SENSORVALUE                = 134, // <sensorValue>
   SYM_SETPARAMSTATEMENT          = 135, // <SetParamStatement>
   SYM_SHARPVALUE                 = 136, // <sharpValue>
   SYM_SPEED2                     = 137, // <speed>
   SYM_START                      = 138, // <Start>
   SYM_STATEMENT                  = 139, // <Statement>
   SYM_STATEMENT_END              = 140, // <statement_end>
   SYM_STATEMENTS                 = 141, // <Statements>
   SYM_STRING_OR_VAR              = 142, // <string_or_var>
   SYM_SWITCH_CASE                = 143, // <switch_case>
   SYM_SWITCH_CASES               = 144, // <switch_cases>
   SYM_SWITCH_VALUE               = 145, // <switch_value>
   SYM_SWITCHCONDITION            = 146, // <SwitchCondition>
   SYM_SWITCHSTATEMENT            = 147, // <SwitchStatement>
   SYM_TELEPORTSTATEMENT          = 148, // <TeleportStatement>
   SYM_TIME                       = 149, // <time>
   SYM_TIME_UNIT                  = 150, // <time_unit>
   SYM_VAR                        = 151, // <var>
   SYM_WAITSTATEMENT              = 152, // <WaitStatement>
   SYM_WHILESTATEMENT             = 153  // <WhileStatement>
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
   PROD_IMPORTSTATEMENT_IMPORT_STRING                        = 101, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                   = 102, // <WaitStatement> ::= wait <time>
   PROD_DECLARESTATEMENT_DECLARE_AS                          = 103, // <DeclareStatement> ::= declare <action> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS2                         = 104, // <DeclareStatement> ::= declare <sensor_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS3                         = 105, // <DeclareStatement> ::= declare <parameter_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS4                         = 106, // <DeclareStatement> ::= declare <point> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS5                         = 107, // <DeclareStatement> ::= declare <rect> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS6                         = 108, // <DeclareStatement> ::= declare <ax12_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_STRING_AS                   = 109, // <DeclareStatement> ::= declare String as <var>
   PROD_TELEPORTSTATEMENT_TELEPORT                           = 110, // <TeleportStatement> ::= teleport <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO                                = 111, // <MoveToStatement> ::= go to <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO2                               = 112, // <MoveToStatement> ::= go to <point_or_var> <speed>
   PROD_MOVETOSTATEMENT_GO_TO3                               = 113, // <MoveToStatement> ::= go to <point_or_var> <direction>
   PROD_MOVETOSTATEMENT_GO_TO4                               = 114, // <MoveToStatement> ::= go to <point_or_var> <speed> <direction>
   PROD_MOVETOSTATEMENT_GO_TO5                               = 115, // <MoveToStatement> ::= go to <point_or_var> <direction> <speed>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                          = 116, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}' <nl Opt>
   PROD_CONCURRENTSTATEMENT_CONCURRENT                       = 117, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                      = 118, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_SETPARAMSTATEMENT_SET_EQ                             = 119, // <SetParamStatement> ::= set <parameter_or_var> '=' <num>
   PROD_ENABLESENSORSTATEMENT_ENABLE                         = 120, // <EnableSensorStatement> ::= enable <sensor_or_var>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                     = 121, // <EnableSensorStatement> ::= enable all <sensor>
   PROD_DISABLESENSORSTATEMENT_DISABLE                       = 122, // <DisableSensorStatement> ::= disable <sensor_or_var>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                   = 123, // <DisableSensorStatement> ::= disable all <sensor>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT             = 124, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT2            = 125, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var> <speed>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT  = 126, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT2 = 127, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var> <speed>
   PROD_MOVEAX12STATEMENT_MOVE_TO                            = 128, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle>
   PROD_MOVEAX12STATEMENT_MOVE_TO2                           = 129, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <speed>
   PROD_ACTUATORSTATEMENT_DO                                 = 130, // <ActuatorStatement> ::= do <action_or_var>
   PROD_CONDITIONISOPERATOR_IS                               = 131, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_ISNOT                            = 132, // <ConditionIsOperator> ::= 'is not'
   PROD_CONDITIONINOPERATOR_IN                               = 133, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOTIN                            = 134, // <ConditionInOperator> ::= 'not in'
   PROD_SENSORCONDITION                                      = 135, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                     = 136, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                           = 137, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                           = 138, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                   = 139, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                          = 140, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                         = 141, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                         = 142, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                         = 143, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                         = 144, // <IfCondition> ::= <reverseCondition>
   PROD_CONDITIONSTATEMENT_IF                                = 145, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                           = 146, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_WHILESTATEMENT_WHILE                                 = 147, // <WhileStatement> ::= while <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_SWITCHSTATEMENT_SWITCH_LBRACE_RBRACE                 = 148, // <SwitchStatement> ::= switch <SwitchCondition> <nl Opt> '{' <nl Opt> <switch_cases> <nl Opt> '}'
   PROD_SWITCHCONDITION_ORIENTATION                          = 149, // <SwitchCondition> ::= orientation
   PROD_SWITCHCONDITION_POSITION                             = 150, // <SwitchCondition> ::= position
   PROD_SWITCHCONDITION_OPPONENT                             = 151, // <SwitchCondition> ::= opponent
   PROD_SWITCHCONDITION                                      = 152, // <SwitchCondition> ::= <sensor_or_var>
   PROD_SWITCH_CASES                                         = 153, // <switch_cases> ::= <switch_case> <switch_cases>
   PROD_SWITCH_CASES2                                        = 154, // <switch_cases> ::= 
   PROD_SWITCH_CASE_CASE                                     = 155, // <switch_case> ::= case <switch_value> <nl Opt> <Statements>
   PROD_SWITCH_CASE_DEFAULTS                                 = 156, // <switch_case> ::= defaults <nl Opt> <Statements>
   PROD_SWITCH_VALUE                                         = 157, // <switch_value> ::= <sensorValue>
   PROD_SWITCH_VALUE2                                        = 158, // <switch_value> ::= <angleRange>
   PROD_SWITCH_VALUE3                                        = 159  // <switch_value> ::= <rect_or_var>
};
