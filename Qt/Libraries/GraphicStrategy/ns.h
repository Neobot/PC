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
   SYM_CLOSE                      =  28, // close
   SYM_COLOR                      =  29, // color
   SYM_CONCURRENT                 =  30, // concurrent
   SYM_DECLARE                    =  31, // declare
   SYM_DEG                        =  32, // deg
   SYM_DETECTED                   =  33, // detected
   SYM_DISABLE                    =  34, // disable
   SYM_DO                         =  35, // do
   SYM_ELSE                       =  36, // else
   SYM_ENABLE                     =  37, // enable
   SYM_EOF2                       =  38, // eof
   SYM_FALSE                      =  39, // false
   SYM_FAR                        =  40, // far
   SYM_FIRSTACTIONFINISHED        =  41, // FirstActionFinished
   SYM_FLOAT                      =  42, // Float
   SYM_FORWARD                    =  43, // forward
   SYM_FUNCTION                   =  44, // function
   SYM_GO                         =  45, // go
   SYM_GREEN                      =  46, // green
   SYM_IDENTIFIER                 =  47, // Identifier
   SYM_IF                         =  48, // if
   SYM_IMPORT                     =  49, // import
   SYM_IN                         =  50, // in
   SYM_INTEGER                    =  51, // Integer
   SYM_IS                         =  52, // is
   SYM_LASTACTIONFINISHED         =  53, // LastActionFinished
   SYM_MAKE                       =  54, // make
   SYM_MICROSWITCH                =  55, // microswitch
   SYM_MOVE                       =  56, // move
   SYM_MOVEMENT                   =  57, // movement
   SYM_MS                         =  58, // ms
   SYM_NEWLINE                    =  59, // NewLine
   SYM_NOT                        =  60, // not
   SYM_OFF                        =  61, // off
   SYM_ON                         =  62, // on
   SYM_ONEACTIONFINISHED          =  63, // OneActionFinished
   SYM_OPPONENT                   =  64, // opponent
   SYM_ORIENTATION                =  65, // orientation
   SYM_PARAMETER                  =  66, // parameter
   SYM_POSITION                   =  67, // position
   SYM_RAD                        =  68, // rad
   SYM_RECT                       =  69, // rect
   SYM_RED                        =  70, // red
   SYM_REVERSED                   =  71, // reversed
   SYM_ROTATE                     =  72, // rotate
   SYM_S                          =  73, // s
   SYM_SENSOR                     =  74, // sensor
   SYM_SET                        =  75, // set
   SYM_SHARP                      =  76, // sharp
   SYM_SPEED                      =  77, // speed
   SYM_STRATEGY                   =  78, // strategy
   SYM_STRING                     =  79, // String
   SYM_TELEPORT                   =  80, // teleport
   SYM_TIMEOUT                    =  81, // timeout
   SYM_TO                         =  82, // to
   SYM_TRUE                       =  83, // true
   SYM_UNKNOWN                    =  84, // unknown
   SYM_WAIT                       =  85, // wait
   SYM_WHILE                      =  86, // while
   SYM_WHITE                      =  87, // white
   SYM_YELLOW                     =  88, // yellow
   SYM_ACTION2                    =  89, // <action>
   SYM_ACTION_OR_VAR              =  90, // <action_or_var>
   SYM_ACTUATORSTATEMENT          =  91, // <ActuatorStatement>
   SYM_ANGLE                      =  92, // <angle>
   SYM_ANGLERANGE                 =  93, // <angleRange>
   SYM_AX12_IDENTIFIER            =  94, // <ax12_identifier>
   SYM_AX12_OR_VAR                =  95, // <ax12_or_var>
   SYM_AX12ASYNCMOVEMENTSTATEMENT =  96, // <Ax12AsyncMovementStatement>
   SYM_AX12MOVEMENTSTATEMENT      =  97, // <Ax12MovementStatement>
   SYM_CALLARG                    =  98, // <CallArg>
   SYM_CALLARGLIST                =  99, // <CallArgList>
   SYM_CALLSTATEMENT              = 100, // <CallStatement>
   SYM_COLORSENSORVALUE           = 101, // <colorSensorValue>
   SYM_CONCURRENCYCONDITION       = 102, // <concurrencyCondition>
   SYM_CONCURRENTSTATEMENT        = 103, // <ConcurrentStatement>
   SYM_CONDITIONINOPERATOR        = 104, // <ConditionInOperator>
   SYM_CONDITIONISOPERATOR        = 105, // <ConditionIsOperator>
   SYM_CONDITIONSTATEMENT         = 106, // <ConditionStatement>
   SYM_DECLARESTATEMENT           = 107, // <DeclareStatement>
   SYM_DIRECTION                  = 108, // <direction>
   SYM_DISABLESENSORSTATEMENT     = 109, // <DisableSensorStatement>
   SYM_ENABLESENSORSTATEMENT      = 110, // <EnableSensorStatement>
   SYM_FIXED_ANGLE                = 111, // <fixed_angle>
   SYM_FIXED_POINT                = 112, // <fixed_point>
   SYM_FIXED_RECT                 = 113, // <fixed_rect>
   SYM_FUNCTIONSTATEMENT          = 114, // <FunctionStatement>
   SYM_ID                         = 115, // <id>
   SYM_IFCONDITION                = 116, // <IfCondition>
   SYM_IMPORTSTATEMENT            = 117, // <ImportStatement>
   SYM_LISTSTATEMENT              = 118, // <ListStatement>
   SYM_MICROSWITCHVALUE           = 119, // <microSwitchValue>
   SYM_MOVEAX12STATEMENT          = 120, // <MoveAx12Statement>
   SYM_MOVETOSTATEMENT            = 121, // <MoveToStatement>
   SYM_NL                         = 122, // <nl>
   SYM_NLOPT                      = 123, // <nl Opt>
   SYM_NUM                        = 124, // <num>
   SYM_OPPONENTCONDITION          = 125, // <OpponentCondition>
   SYM_OPT_BRACKET                = 126, // <opt_bracket>
   SYM_OPT_TIME                   = 127, // <opt_time>
   SYM_ORIENTATIONCONDITION       = 128, // <OrientationCondition>
   SYM_PARAMETER_IDENTIFIER       = 129, // <parameter_identifier>
   SYM_PARAMETER_OR_VAR           = 130, // <parameter_or_var>
   SYM_POINT                      = 131, // <point>
   SYM_POINT_OR_VAR               = 132, // <point_or_var>
   SYM_POSITIONCONDITION          = 133, // <PositionCondition>
   SYM_PROGRAM                    = 134, // <Program>
   SYM_RECT2                      = 135, // <rect>
   SYM_RECT_OR_VAR                = 136, // <rect_or_var>
   SYM_REVERSECONDITION           = 137, // <reverseCondition>
   SYM_ROTATETOSTATEMENT          = 138, // <RotateToSTatement>
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
   SYM_TELEPORTSTATEMENT          = 152, // <TeleportStatement>
   SYM_TIME                       = 153, // <time>
   SYM_TIME_UNIT                  = 154, // <time_unit>
   SYM_TIMEOUTSTATEMENT           = 155, // <TimeOutStatement>
   SYM_VAR                        = 156, // <var>
   SYM_VARLIST                    = 157, // <varList>
   SYM_WAITSTATEMENT              = 158, // <WaitStatement>
   SYM_WHILESTATEMENT             = 159  // <WhileStatement>
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
   PROD_STATEMENTS                                           =  81, // <Statements> ::= <Statement> <Statements>
   PROD_STATEMENTS2                                          =  82, // <Statements> ::= 
   PROD_STATEMENT                                            =  83, // <Statement> ::= <ImportStatement> <statement_end>
   PROD_STATEMENT2                                           =  84, // <Statement> ::= <WaitStatement> <statement_end>
   PROD_STATEMENT3                                           =  85, // <Statement> ::= <DeclareStatement> <statement_end>
   PROD_STATEMENT4                                           =  86, // <Statement> ::= <TeleportStatement> <statement_end>
   PROD_STATEMENT5                                           =  87, // <Statement> ::= <MoveToStatement> <statement_end>
   PROD_STATEMENT6                                           =  88, // <Statement> ::= <RotateToSTatement> <statement_end>
   PROD_STATEMENT7                                           =  89, // <Statement> ::= <SetParamStatement> <statement_end>
   PROD_STATEMENT8                                           =  90, // <Statement> ::= <EnableSensorStatement> <statement_end>
   PROD_STATEMENT9                                           =  91, // <Statement> ::= <DisableSensorStatement> <statement_end>
   PROD_STATEMENT10                                          =  92, // <Statement> ::= <Ax12MovementStatement> <statement_end>
   PROD_STATEMENT11                                          =  93, // <Statement> ::= <Ax12AsyncMovementStatement> <statement_end>
   PROD_STATEMENT12                                          =  94, // <Statement> ::= <MoveAx12Statement> <statement_end>
   PROD_STATEMENT13                                          =  95, // <Statement> ::= <ActuatorStatement> <statement_end>
   PROD_STATEMENT14                                          =  96, // <Statement> ::= <ListStatement> <statement_end>
   PROD_STATEMENT15                                          =  97, // <Statement> ::= <ConcurrentStatement> <statement_end>
   PROD_STATEMENT16                                          =  98, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT17                                          =  99, // <Statement> ::= <WhileStatement>
   PROD_STATEMENT18                                          = 100, // <Statement> ::= <FunctionStatement> <statement_end>
   PROD_STATEMENT19                                          = 101, // <Statement> ::= <CallStatement> <statement_end>
   PROD_STATEMENT20                                          = 102, // <Statement> ::= <TimeOutStatement>
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
   PROD_ROTATETOSTATEMENT_ROTATE_TO                          = 115, // <RotateToSTatement> ::= rotate to <angle>
   PROD_ROTATETOSTATEMENT_ROTATE_TO2                         = 116, // <RotateToSTatement> ::= rotate to <angle> <speed>
   PROD_ROTATETOSTATEMENT_ROTATE_TO3                         = 117, // <RotateToSTatement> ::= rotate to <angle> <direction>
   PROD_ROTATETOSTATEMENT_ROTATE_TO4                         = 118, // <RotateToSTatement> ::= rotate to <angle> <speed> <direction>
   PROD_ROTATETOSTATEMENT_ROTATE_TO5                         = 119, // <RotateToSTatement> ::= rotate to <angle> <direction> <speed>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                          = 120, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}'
   PROD_CONCURRENTSTATEMENT_CONCURRENT                       = 121, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                      = 122, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_TIMEOUTSTATEMENT_TIMEOUT                             = 123, // <TimeOutStatement> ::= timeout <time> <nl Opt> <Statement>
   PROD_SETPARAMSTATEMENT_SET_EQ                             = 124, // <SetParamStatement> ::= set <parameter_or_var> '=' <num>
   PROD_ENABLESENSORSTATEMENT_ENABLE                         = 125, // <EnableSensorStatement> ::= enable <sensor_or_var>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                     = 126, // <EnableSensorStatement> ::= enable all <sensor>
   PROD_DISABLESENSORSTATEMENT_DISABLE                       = 127, // <DisableSensorStatement> ::= disable <sensor_or_var>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                   = 128, // <DisableSensorStatement> ::= disable all <sensor>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT             = 129, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT2            = 130, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var> <speed>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT  = 131, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT2 = 132, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var> <speed>
   PROD_MOVEAX12STATEMENT_MOVE_TO                            = 133, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle>
   PROD_MOVEAX12STATEMENT_MOVE_TO2                           = 134, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <speed>
   PROD_ACTUATORSTATEMENT_DO                                 = 135, // <ActuatorStatement> ::= do <action_or_var>
   PROD_CONDITIONISOPERATOR_IS                               = 136, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_IS_NOT                           = 137, // <ConditionIsOperator> ::= is not
   PROD_CONDITIONINOPERATOR_IN                               = 138, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOT_IN                           = 139, // <ConditionInOperator> ::= not in
   PROD_SENSORCONDITION                                      = 140, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                     = 141, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                           = 142, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                           = 143, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                   = 144, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                          = 145, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                         = 146, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                         = 147, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                         = 148, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                         = 149, // <IfCondition> ::= <reverseCondition>
   PROD_IFCONDITION_TRUE                                     = 150, // <IfCondition> ::= true
   PROD_IFCONDITION_FALSE                                    = 151, // <IfCondition> ::= false
   PROD_CONDITIONSTATEMENT_IF                                = 152, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                           = 153, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_WHILESTATEMENT_WHILE                                 = 154, // <WhileStatement> ::= while <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_VARLIST_COMMA                                        = 155, // <varList> ::= <var> ',' <varList>
   PROD_VARLIST                                              = 156, // <varList> ::= <var>
   PROD_VARLIST2                                             = 157, // <varList> ::= 
   PROD_FUNCTIONSTATEMENT_FUNCTION_LPAREN_RPAREN             = 158, // <FunctionStatement> ::= function <var> '(' <varList> ')' <nl Opt> <ListStatement>
   PROD_CALLARG                                              = 159, // <CallArg> ::= <var>
   PROD_CALLARG_LBRACKET_RBRACKET                            = 160, // <CallArg> ::= '[' <point> ']'
   PROD_CALLARG_LBRACKET_RBRACKET2                           = 161, // <CallArg> ::= '[' <rect> ']'
   PROD_CALLARG2                                             = 162, // <CallArg> ::= <action>
   PROD_CALLARG3                                             = 163, // <CallArg> ::= <sensor_identifier>
   PROD_CALLARG4                                             = 164, // <CallArg> ::= <parameter_identifier>
   PROD_CALLARG5                                             = 165, // <CallArg> ::= <ax12_identifier>
   PROD_CALLARGLIST_COMMA                                    = 166, // <CallArgList> ::= <CallArg> ',' <CallArgList>
   PROD_CALLARGLIST                                          = 167, // <CallArgList> ::= <CallArg>
   PROD_CALLARGLIST2                                         = 168, // <CallArgList> ::= 
   PROD_CALLSTATEMENT_LPAREN_RPAREN                          = 169  // <CallStatement> ::= <var> '(' <CallArgList> ')'
};
