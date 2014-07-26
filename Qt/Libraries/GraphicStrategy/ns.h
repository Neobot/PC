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
   SYM_SWITCH_CASE                = 142, // <switch_case>
   SYM_SWITCH_CASES               = 143, // <switch_cases>
   SYM_SWITCH_VALUE               = 144, // <switch_value>
   SYM_SWITCHCONDITION            = 145, // <SwitchCondition>
   SYM_SWITCHSTATEMENT            = 146, // <SwitchStatement>
   SYM_TELEPORTSTATEMENT          = 147, // <TeleportStatement>
   SYM_TIME                       = 148, // <time>
   SYM_TIME_UNIT                  = 149, // <time_unit>
   SYM_VAR                        = 150, // <var>
   SYM_WAITSTATEMENT              = 151, // <WaitStatement>
   SYM_WHILESTATEMENT             = 152  // <WhileStatement>
};

enum ProductionConstants
{
   PROD_NL_NEWLINE                                                  =   0, // <nl> ::= NewLine <nl>
   PROD_NL_NEWLINE2                                                 =   1, // <nl> ::= NewLine
   PROD_NLOPT_NEWLINE                                               =   2, // <nl Opt> ::= NewLine <nl Opt>
   PROD_NLOPT                                                       =   3, // <nl Opt> ::= 
   PROD_STATEMENT_END_SEMI                                          =   4, // <statement_end> ::= ';' <nl Opt>
   PROD_STATEMENT_END                                               =   5, // <statement_end> ::= <nl>
   PROD_STATEMENT_END_EOF                                           =   6, // <statement_end> ::= eof
   PROD_OPT_BRACKET_LPAREN                                          =   7, // <opt_bracket> ::= '('
   PROD_OPT_BRACKET                                                 =   8, // <opt_bracket> ::= 
   PROD_ID_INTEGER                                                  =   9, // <id> ::= Integer
   PROD_NUM_FLOAT                                                   =  10, // <num> ::= Float
   PROD_NUM_INTEGER                                                 =  11, // <num> ::= Integer
   PROD_SPEED_SPEED_INTEGER_PERCENT                                 =  12, // <speed> ::= speed Integer '%'
   PROD_FIXED_ANGLE_DEG                                             =  13, // <fixed_angle> ::= <num> deg
   PROD_FIXED_ANGLE_RAD                                             =  14, // <fixed_angle> ::= <num> rad
   PROD_FIXED_ANGLE                                                 =  15, // <fixed_angle> ::= <num>
   PROD_ANGLE                                                       =  16, // <angle> ::= <fixed_angle>
   PROD_ANGLE_AUTO_REVERSED                                         =  17, // <angle> ::= auto reversed <fixed_angle>
   PROD_ANGLE_REVERSED                                              =  18, // <angle> ::= reversed <fixed_angle>
   PROD_TIME_UNIT_S                                                 =  19, // <time_unit> ::= s
   PROD_TIME_UNIT_MS                                                =  20, // <time_unit> ::= ms
   PROD_TIME_INTEGER                                                =  21, // <time> ::= Integer <time_unit>
   PROD_OPT_TIME                                                    =  22, // <opt_time> ::= <time>
   PROD_OPT_TIME2                                                   =  23, // <opt_time> ::= 
   PROD_VAR_IDENTIFIER                                              =  24, // <var> ::= Identifier
   PROD_ANGLERANGE_MINUS                                            =  25, // <angleRange> ::= <angle> '-' <angle>
   PROD_FIXED_POINT_COMMA                                           =  26, // <fixed_point> ::= <opt_bracket> <num> ',' <num> <opt_bracket>
   PROD_FIXED_POINT_COMMA_COMMA                                     =  27, // <fixed_point> ::= <opt_bracket> <num> ',' <num> ',' <fixed_angle> <opt_bracket>
   PROD_POINT                                                       =  28, // <point> ::= <fixed_point>
   PROD_POINT_AUTO_REVERSED                                         =  29, // <point> ::= auto reversed <fixed_point>
   PROD_POINT_REVERSED                                              =  30, // <point> ::= reversed <fixed_point>
   PROD_POINT_OR_VAR                                                =  31, // <point_or_var> ::= <point>
   PROD_POINT_OR_VAR2                                               =  32, // <point_or_var> ::= <var>
   PROD_FIXED_RECT_RECT_COMMA_COMMA_COMMA                           =  33, // <fixed_rect> ::= rect <num> ',' <num> ',' <num> ',' <num>
   PROD_RECT                                                        =  34, // <rect> ::= <fixed_rect>
   PROD_RECT_AUTO_REVERSED                                          =  35, // <rect> ::= auto reversed <fixed_rect>
   PROD_RECT_REVERSED                                               =  36, // <rect> ::= reversed <fixed_rect>
   PROD_RECT_OR_VAR                                                 =  37, // <rect_or_var> ::= <rect>
   PROD_RECT_OR_VAR2                                                =  38, // <rect_or_var> ::= <var>
   PROD_DIRECTION_FORWARD                                           =  39, // <direction> ::= forward
   PROD_DIRECTION_BACKWARD                                          =  40, // <direction> ::= backward
   PROD_DIRECTION_AUTO                                              =  41, // <direction> ::= auto
   PROD_SENSOR_COLOR_SENSOR                                         =  42, // <sensor> ::= color sensor
   PROD_SENSOR_MICROSWITCH                                          =  43, // <sensor> ::= microswitch
   PROD_SENSOR_SHARP                                                =  44, // <sensor> ::= sharp
   PROD_SENSOR_IDENTIFIER                                           =  45, // <sensor_identifier> ::= <sensor> <id>
   PROD_SENSOR_OR_VAR                                               =  46, // <sensor_or_var> ::= <sensor_identifier>
   PROD_SENSOR_OR_VAR2                                              =  47, // <sensor_or_var> ::= <var>
   PROD_COLORSENSORVALUE_UNKNOWN                                    =  48, // <colorSensorValue> ::= unknown
   PROD_COLORSENSORVALUE_RED                                        =  49, // <colorSensorValue> ::= red
   PROD_COLORSENSORVALUE_GREEN                                      =  50, // <colorSensorValue> ::= green
   PROD_COLORSENSORVALUE_BLUE                                       =  51, // <colorSensorValue> ::= blue
   PROD_COLORSENSORVALUE_YELLOW                                     =  52, // <colorSensorValue> ::= yellow
   PROD_COLORSENSORVALUE_WHITE                                      =  53, // <colorSensorValue> ::= white
   PROD_COLORSENSORVALUE_BLACK                                      =  54, // <colorSensorValue> ::= black
   PROD_SHARPVALUE_DETECTED                                         =  55, // <sharpValue> ::= detected
   PROD_SHARPVALUE_FAR                                              =  56, // <sharpValue> ::= far
   PROD_SHARPVALUE_CLOSE                                            =  57, // <sharpValue> ::= close
   PROD_MICROSWITCHVALUE_ON                                         =  58, // <microSwitchValue> ::= on
   PROD_MICROSWITCHVALUE_OFF                                        =  59, // <microSwitchValue> ::= off
   PROD_SENSORVALUE                                                 =  60, // <sensorValue> ::= <colorSensorValue>
   PROD_SENSORVALUE2                                                =  61, // <sensorValue> ::= <sharpValue>
   PROD_SENSORVALUE3                                                =  62, // <sensorValue> ::= <microSwitchValue>
   PROD_SENSORVALUE4                                                =  63, // <sensorValue> ::= <num>
   PROD_CONCURRENCYCONDITION_ALLACTIONSFINISHED                     =  64, // <concurrencyCondition> ::= AllActionsFinished
   PROD_CONCURRENCYCONDITION_ONEACTIONFINISHED                      =  65, // <concurrencyCondition> ::= OneActionFinished
   PROD_CONCURRENCYCONDITION_FIRSTACTIONFINISHED                    =  66, // <concurrencyCondition> ::= FirstActionFinished
   PROD_CONCURRENCYCONDITION_LASTACTIONFINISHED                     =  67, // <concurrencyCondition> ::= LastActionFinished
   PROD_ACTION_ACTION_INTEGER_COMMA_INTEGER_COMMA                   =  68, // <action> ::= action Integer ',' Integer ',' <time>
   PROD_ACTION_OR_VAR                                               =  69, // <action_or_var> ::= <action>
   PROD_ACTION_OR_VAR2                                              =  70, // <action_or_var> ::= <var>
   PROD_PARAMETER_IDENTIFIER_PARAMETER                              =  71, // <parameter_identifier> ::= parameter <id>
   PROD_PARAMETER_OR_VAR                                            =  72, // <parameter_or_var> ::= <parameter_identifier>
   PROD_PARAMETER_OR_VAR2                                           =  73, // <parameter_or_var> ::= <var>
   PROD_AX12_IDENTIFIER_AX12                                        =  74, // <ax12_identifier> ::= 'ax12' <id>
   PROD_AX12_OR_VAR                                                 =  75, // <ax12_or_var> ::= <ax12_identifier>
   PROD_AX12_OR_VAR2                                                =  76, // <ax12_or_var> ::= <var>
   PROD_START                                                       =  77, // <Start> ::= <nl Opt> <Program>
   PROD_PROGRAM                                                     =  78, // <Program> ::= <Statements>
   PROD_STATEMENTS                                                  =  79, // <Statements> ::= <Statement> <statement_end> <Statements>
   PROD_STATEMENTS2                                                 =  80, // <Statements> ::= <Statement>
   PROD_STATEMENTS3                                                 =  81, // <Statements> ::= 
   PROD_STATEMENT                                                   =  82, // <Statement> ::= <ImportStatement>
   PROD_STATEMENT2                                                  =  83, // <Statement> ::= <WaitStatement>
   PROD_STATEMENT3                                                  =  84, // <Statement> ::= <DeclareStatement>
   PROD_STATEMENT4                                                  =  85, // <Statement> ::= <TeleportStatement>
   PROD_STATEMENT5                                                  =  86, // <Statement> ::= <MoveToStatement>
   PROD_STATEMENT6                                                  =  87, // <Statement> ::= <SetParamStatement>
   PROD_STATEMENT7                                                  =  88, // <Statement> ::= <EnableSensorStatement>
   PROD_STATEMENT8                                                  =  89, // <Statement> ::= <DisableSensorStatement>
   PROD_STATEMENT9                                                  =  90, // <Statement> ::= <Ax12MovementStatement>
   PROD_STATEMENT10                                                 =  91, // <Statement> ::= <Ax12AsyncMovementStatement>
   PROD_STATEMENT11                                                 =  92, // <Statement> ::= <MoveAx12Statement>
   PROD_STATEMENT12                                                 =  93, // <Statement> ::= <ActuatorStatement>
   PROD_STATEMENT13                                                 =  94, // <Statement> ::= <ListStatement>
   PROD_STATEMENT14                                                 =  95, // <Statement> ::= <ConcurrentStatement>
   PROD_STATEMENT15                                                 =  96, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT16                                                 =  97, // <Statement> ::= <WhileStatement>
   PROD_STATEMENT17                                                 =  98, // <Statement> ::= <SwitchStatement>
   PROD_IMPORTSTATEMENT_IMPORT_STRING                               =  99, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                          = 100, // <WaitStatement> ::= wait <time>
   PROD_DECLARESTATEMENT_DECLARE_AS                                 = 101, // <DeclareStatement> ::= declare <action> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS2                                = 102, // <DeclareStatement> ::= declare <sensor_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS3                                = 103, // <DeclareStatement> ::= declare <parameter_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS4                                = 104, // <DeclareStatement> ::= declare <point> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS5                                = 105, // <DeclareStatement> ::= declare <rect> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS6                                = 106, // <DeclareStatement> ::= declare <ax12_identifier> as <var>
   PROD_TELEPORTSTATEMENT_TELEPORT                                  = 107, // <TeleportStatement> ::= teleport <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO                                       = 108, // <MoveToStatement> ::= go to <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO2                                      = 109, // <MoveToStatement> ::= go to <point_or_var> <speed>
   PROD_MOVETOSTATEMENT_GO_TO3                                      = 110, // <MoveToStatement> ::= go to <point_or_var> <direction>
   PROD_MOVETOSTATEMENT_GO_TO4                                      = 111, // <MoveToStatement> ::= go to <point_or_var> <speed> <direction>
   PROD_MOVETOSTATEMENT_GO_TO5                                      = 112, // <MoveToStatement> ::= go to <point_or_var> <direction> <speed>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                                 = 113, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}' <nl Opt>
   PROD_CONCURRENTSTATEMENT_CONCURRENT                              = 114, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                             = 115, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_SETPARAMSTATEMENT_SET_EQ                                    = 116, // <SetParamStatement> ::= set <parameter_or_var> '=' <num>
   PROD_ENABLESENSORSTATEMENT_ENABLE                                = 117, // <EnableSensorStatement> ::= enable <sensor_or_var>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                            = 118, // <EnableSensorStatement> ::= enable all <sensor>
   PROD_DISABLESENSORSTATEMENT_DISABLE                              = 119, // <DisableSensorStatement> ::= disable <sensor_or_var>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                          = 120, // <DisableSensorStatement> ::= disable all <sensor>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT_STRING             = 121, // <Ax12MovementStatement> ::= make 'ax12' movement String
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT_STRING2            = 122, // <Ax12MovementStatement> ::= make 'ax12' movement String <speed>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT_STRING  = 123, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement String
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT_STRING2 = 124, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement String <speed>
   PROD_MOVEAX12STATEMENT_MOVE_TO                                   = 125, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle>
   PROD_MOVEAX12STATEMENT_MOVE_TO2                                  = 126, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <speed>
   PROD_ACTUATORSTATEMENT_DO                                        = 127, // <ActuatorStatement> ::= do <action_or_var>
   PROD_CONDITIONISOPERATOR_IS                                      = 128, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_ISNOT                                   = 129, // <ConditionIsOperator> ::= 'is not'
   PROD_CONDITIONINOPERATOR_IN                                      = 130, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOTIN                                   = 131, // <ConditionInOperator> ::= 'not in'
   PROD_SENSORCONDITION                                             = 132, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                            = 133, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                                  = 134, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                                  = 135, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                          = 136, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                                 = 137, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                                = 138, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                                = 139, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                                = 140, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                                = 141, // <IfCondition> ::= <reverseCondition>
   PROD_CONDITIONSTATEMENT_IF                                       = 142, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                                  = 143, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_WHILESTATEMENT_WHILE                                        = 144, // <WhileStatement> ::= while <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_SWITCHSTATEMENT_SWITCH_LBRACE_RBRACE                        = 145, // <SwitchStatement> ::= switch <SwitchCondition> <nl Opt> '{' <nl Opt> <switch_cases> <nl Opt> '}'
   PROD_SWITCHCONDITION_ORIENTATION                                 = 146, // <SwitchCondition> ::= orientation
   PROD_SWITCHCONDITION_POSITION                                    = 147, // <SwitchCondition> ::= position
   PROD_SWITCHCONDITION_OPPONENT                                    = 148, // <SwitchCondition> ::= opponent
   PROD_SWITCHCONDITION                                             = 149, // <SwitchCondition> ::= <sensor_or_var>
   PROD_SWITCH_CASES                                                = 150, // <switch_cases> ::= <switch_case> <switch_cases>
   PROD_SWITCH_CASES2                                               = 151, // <switch_cases> ::= 
   PROD_SWITCH_CASE_CASE                                            = 152, // <switch_case> ::= case <switch_value> <nl Opt> <Statements>
   PROD_SWITCH_CASE_DEFAULTS                                        = 153, // <switch_case> ::= defaults <nl Opt> <Statements>
   PROD_SWITCH_VALUE                                                = 154, // <switch_value> ::= <sensorValue>
   PROD_SWITCH_VALUE2                                               = 155, // <switch_value> ::= <angleRange>
   PROD_SWITCH_VALUE3                                               = 156  // <switch_value> ::= <rect_or_var>
};
