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
   SYM_CIRCLE                     =  28, // circle
   SYM_COLOR                      =  29, // color
   SYM_CONCURRENT                 =  30, // concurrent
   SYM_DECLARE                    =  31, // declare
   SYM_DEG                        =  32, // deg
   SYM_DISABLE                    =  33, // disable
   SYM_DO                         =  34, // do
   SYM_ELSE                       =  35, // else
   SYM_ENABLE                     =  36, // enable
   SYM_EOF2                       =  37, // eof
   SYM_FALSE                      =  38, // false
   SYM_FIRSTACTIONFINISHED        =  39, // FirstActionFinished
   SYM_FLOAT                      =  40, // Float
   SYM_FORWARD                    =  41, // forward
   SYM_FUNCTION                   =  42, // function
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
   SYM_NEARBY                     =  57, // nearby
   SYM_NEWLINE                    =  58, // NewLine
   SYM_NOT                        =  59, // not
   SYM_OFF                        =  60, // off
   SYM_ON                         =  61, // on
   SYM_ONEACTIONFINISHED          =  62, // OneActionFinished
   SYM_OPPONENT                   =  63, // opponent
   SYM_ORIENTATION                =  64, // orientation
   SYM_PARAMETER                  =  65, // parameter
   SYM_POSITION                   =  66, // position
   SYM_RAD                        =  67, // rad
   SYM_RECT                       =  68, // rect
   SYM_RED                        =  69, // red
   SYM_REVERSED                   =  70, // reversed
   SYM_ROTATE                     =  71, // rotate
   SYM_S                          =  72, // s
   SYM_SENSOR                     =  73, // sensor
   SYM_SET                        =  74, // set
   SYM_SHARP                      =  75, // sharp
   SYM_SPEED                      =  76, // speed
   SYM_STRATEGY                   =  77, // strategy
   SYM_STRING                     =  78, // String
   SYM_TELEPORT                   =  79, // teleport
   SYM_TIMEOUT                    =  80, // timeout
   SYM_TO                         =  81, // to
   SYM_TRUE                       =  82, // true
   SYM_UNKNOWN                    =  83, // unknown
   SYM_WAIT                       =  84, // wait
   SYM_WHILE                      =  85, // while
   SYM_WHITE                      =  86, // white
   SYM_YELLOW                     =  87, // yellow
   SYM_ACTION2                    =  88, // <action>
   SYM_ACTION_OR_VAR              =  89, // <action_or_var>
   SYM_ACTUATORSTATEMENT          =  90, // <ActuatorStatement>
   SYM_ANGLE                      =  91, // <angle>
   SYM_ANGLERANGE                 =  92, // <angleRange>
   SYM_AX12_IDENTIFIER            =  93, // <ax12_identifier>
   SYM_AX12_OR_VAR                =  94, // <ax12_or_var>
   SYM_AX12ASYNCMOVEMENTSTATEMENT =  95, // <Ax12AsyncMovementStatement>
   SYM_AX12MOVEMENTSTATEMENT      =  96, // <Ax12MovementStatement>
   SYM_CALLARG                    =  97, // <CallArg>
   SYM_CALLARGLIST                =  98, // <CallArgList>
   SYM_CALLSTATEMENT              =  99, // <CallStatement>
   SYM_COLORSENSORVALUE           = 100, // <colorSensorValue>
   SYM_CONCURRENCYCONDITION       = 101, // <concurrencyCondition>
   SYM_CONCURRENTSTATEMENT        = 102, // <ConcurrentStatement>
   SYM_CONDITIONINOPERATOR        = 103, // <ConditionInOperator>
   SYM_CONDITIONISOPERATOR        = 104, // <ConditionIsOperator>
   SYM_CONDITIONSTATEMENT         = 105, // <ConditionStatement>
   SYM_DECLARESTATEMENT           = 106, // <DeclareStatement>
   SYM_DIRECTION                  = 107, // <direction>
   SYM_DISABLESENSORSTATEMENT     = 108, // <DisableSensorStatement>
   SYM_ENABLESENSORSTATEMENT      = 109, // <EnableSensorStatement>
   SYM_FIXED_ANGLE                = 110, // <fixed_angle>
   SYM_FIXED_POINT                = 111, // <fixed_point>
   SYM_FIXED_RECT                 = 112, // <fixed_rect>
   SYM_FUNCTIONSTATEMENT          = 113, // <FunctionStatement>
   SYM_ID                         = 114, // <id>
   SYM_IFCONDITION                = 115, // <IfCondition>
   SYM_IMPORTSTATEMENT            = 116, // <ImportStatement>
   SYM_LISTSTATEMENT              = 117, // <ListStatement>
   SYM_MICROSWITCHVALUE           = 118, // <microSwitchValue>
   SYM_MOVEAX12STATEMENT          = 119, // <MoveAx12Statement>
   SYM_MOVETOSTATEMENT            = 120, // <MoveToStatement>
   SYM_NL                         = 121, // <nl>
   SYM_NLOPT                      = 122, // <nl Opt>
   SYM_NUM                        = 123, // <num>
   SYM_OPPONENTCONDITION          = 124, // <OpponentCondition>
   SYM_OPT_BRACKET                = 125, // <opt_bracket>
   SYM_OPT_TIME                   = 126, // <opt_time>
   SYM_ORIENTATIONCONDITION       = 127, // <OrientationCondition>
   SYM_PARAMETER_IDENTIFIER       = 128, // <parameter_identifier>
   SYM_PARAMETER_OR_VAR           = 129, // <parameter_or_var>
   SYM_POINT                      = 130, // <point>
   SYM_POINT_OR_VAR               = 131, // <point_or_var>
   SYM_POSITIONCONDITION          = 132, // <PositionCondition>
   SYM_PROGRAM                    = 133, // <Program>
   SYM_RECT2                      = 134, // <rect>
   SYM_RECT_OR_VAR                = 135, // <rect_or_var>
   SYM_REVERSECONDITION           = 136, // <reverseCondition>
   SYM_ROTATETOSTATEMENT          = 137, // <RotateToSTatement>
   SYM_SENSOR2                    = 138, // <sensor>
   SYM_SENSOR_IDENTIFIER          = 139, // <sensor_identifier>
   SYM_SENSOR_OR_VAR              = 140, // <sensor_or_var>
   SYM_SENSORCONDITION            = 141, // <SensorCondition>
   SYM_SENSORVALUE                = 142, // <sensorValue>
   SYM_SETPARAMSTATEMENT          = 143, // <SetParamStatement>
   SYM_SHARPVALUE                 = 144, // <sharpValue>
   SYM_SPEED2                     = 145, // <speed>
   SYM_START                      = 146, // <Start>
   SYM_STATEMENT                  = 147, // <Statement>
   SYM_STATEMENT_END              = 148, // <statement_end>
   SYM_STATEMENTS                 = 149, // <Statements>
   SYM_STRING_OR_VAR              = 150, // <string_or_var>
   SYM_TELEPORTSTATEMENT          = 151, // <TeleportStatement>
   SYM_TIME                       = 152, // <time>
   SYM_TIME_UNIT                  = 153, // <time_unit>
   SYM_TIMEOUTSTATEMENT           = 154, // <TimeOutStatement>
   SYM_VAR                        = 155, // <var>
   SYM_VARLIST                    = 156, // <varList>
   SYM_WAITSTATEMENT              = 157, // <WaitStatement>
   SYM_WHILESTATEMENT             = 158  // <WhileStatement>
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
   PROD_FIXED_RECT_CIRCLE_LBRACKET_RBRACKET_COMMA            =  36, // <fixed_rect> ::= circle '[' <point> ']' ',' <num>
   PROD_FIXED_RECT_CIRCLE_COMMA                              =  37, // <fixed_rect> ::= circle <var> ',' <num>
   PROD_RECT                                                 =  38, // <rect> ::= <fixed_rect>
   PROD_RECT_AUTO_REVERSED                                   =  39, // <rect> ::= auto reversed <fixed_rect>
   PROD_RECT_REVERSED                                        =  40, // <rect> ::= reversed <fixed_rect>
   PROD_RECT_OR_VAR                                          =  41, // <rect_or_var> ::= <rect>
   PROD_RECT_OR_VAR2                                         =  42, // <rect_or_var> ::= <var>
   PROD_DIRECTION_FORWARD                                    =  43, // <direction> ::= forward
   PROD_DIRECTION_BACKWARD                                   =  44, // <direction> ::= backward
   PROD_DIRECTION_AUTO                                       =  45, // <direction> ::= auto
   PROD_SENSOR_COLOR_SENSOR                                  =  46, // <sensor> ::= color sensor
   PROD_SENSOR_MICROSWITCH                                   =  47, // <sensor> ::= microswitch
   PROD_SENSOR_SHARP                                         =  48, // <sensor> ::= sharp
   PROD_SENSOR_IDENTIFIER                                    =  49, // <sensor_identifier> ::= <sensor> <id>
   PROD_SENSOR_OR_VAR                                        =  50, // <sensor_or_var> ::= <sensor_identifier>
   PROD_SENSOR_OR_VAR2                                       =  51, // <sensor_or_var> ::= <var>
   PROD_COLORSENSORVALUE_UNKNOWN                             =  52, // <colorSensorValue> ::= unknown
   PROD_COLORSENSORVALUE_RED                                 =  53, // <colorSensorValue> ::= red
   PROD_COLORSENSORVALUE_GREEN                               =  54, // <colorSensorValue> ::= green
   PROD_COLORSENSORVALUE_BLUE                                =  55, // <colorSensorValue> ::= blue
   PROD_COLORSENSORVALUE_YELLOW                              =  56, // <colorSensorValue> ::= yellow
   PROD_COLORSENSORVALUE_WHITE                               =  57, // <colorSensorValue> ::= white
   PROD_COLORSENSORVALUE_BLACK                               =  58, // <colorSensorValue> ::= black
   PROD_SHARPVALUE_NEARBY                                    =  59, // <sharpValue> ::= nearby
   PROD_SHARPVALUE_ON                                        =  60, // <sharpValue> ::= on
   PROD_SHARPVALUE_OFF                                       =  61, // <sharpValue> ::= off
   PROD_MICROSWITCHVALUE_ON                                  =  62, // <microSwitchValue> ::= on
   PROD_MICROSWITCHVALUE_OFF                                 =  63, // <microSwitchValue> ::= off
   PROD_SENSORVALUE                                          =  64, // <sensorValue> ::= <colorSensorValue>
   PROD_SENSORVALUE_NEARBY                                   =  65, // <sensorValue> ::= nearby
   PROD_SENSORVALUE_ON                                       =  66, // <sensorValue> ::= on
   PROD_SENSORVALUE_OFF                                      =  67, // <sensorValue> ::= off
   PROD_SENSORVALUE2                                         =  68, // <sensorValue> ::= <num>
   PROD_CONCURRENCYCONDITION_ALLACTIONSFINISHED              =  69, // <concurrencyCondition> ::= AllActionsFinished
   PROD_CONCURRENCYCONDITION_ONEACTIONFINISHED               =  70, // <concurrencyCondition> ::= OneActionFinished
   PROD_CONCURRENCYCONDITION_FIRSTACTIONFINISHED             =  71, // <concurrencyCondition> ::= FirstActionFinished
   PROD_CONCURRENCYCONDITION_LASTACTIONFINISHED              =  72, // <concurrencyCondition> ::= LastActionFinished
   PROD_ACTION_ACTION_INTEGER_COMMA_INTEGER_COMMA            =  73, // <action> ::= action Integer ',' Integer ',' <time>
   PROD_ACTION_OR_VAR                                        =  74, // <action_or_var> ::= <action>
   PROD_ACTION_OR_VAR2                                       =  75, // <action_or_var> ::= <var>
   PROD_PARAMETER_IDENTIFIER_PARAMETER                       =  76, // <parameter_identifier> ::= parameter <id>
   PROD_PARAMETER_OR_VAR                                     =  77, // <parameter_or_var> ::= <parameter_identifier>
   PROD_PARAMETER_OR_VAR2                                    =  78, // <parameter_or_var> ::= <var>
   PROD_AX12_IDENTIFIER_AX12                                 =  79, // <ax12_identifier> ::= 'ax12' <id>
   PROD_AX12_OR_VAR                                          =  80, // <ax12_or_var> ::= <ax12_identifier>
   PROD_AX12_OR_VAR2                                         =  81, // <ax12_or_var> ::= <var>
   PROD_START                                                =  82, // <Start> ::= <nl Opt> <Program>
   PROD_PROGRAM                                              =  83, // <Program> ::= <Statements>
   PROD_STATEMENTS                                           =  84, // <Statements> ::= <Statement> <Statements>
   PROD_STATEMENTS2                                          =  85, // <Statements> ::= 
   PROD_STATEMENT                                            =  86, // <Statement> ::= <ImportStatement> <statement_end>
   PROD_STATEMENT2                                           =  87, // <Statement> ::= <WaitStatement> <statement_end>
   PROD_STATEMENT3                                           =  88, // <Statement> ::= <DeclareStatement> <statement_end>
   PROD_STATEMENT4                                           =  89, // <Statement> ::= <TeleportStatement> <statement_end>
   PROD_STATEMENT5                                           =  90, // <Statement> ::= <MoveToStatement> <statement_end>
   PROD_STATEMENT6                                           =  91, // <Statement> ::= <RotateToSTatement> <statement_end>
   PROD_STATEMENT7                                           =  92, // <Statement> ::= <SetParamStatement> <statement_end>
   PROD_STATEMENT8                                           =  93, // <Statement> ::= <EnableSensorStatement> <statement_end>
   PROD_STATEMENT9                                           =  94, // <Statement> ::= <DisableSensorStatement> <statement_end>
   PROD_STATEMENT10                                          =  95, // <Statement> ::= <Ax12MovementStatement> <statement_end>
   PROD_STATEMENT11                                          =  96, // <Statement> ::= <Ax12AsyncMovementStatement> <statement_end>
   PROD_STATEMENT12                                          =  97, // <Statement> ::= <MoveAx12Statement> <statement_end>
   PROD_STATEMENT13                                          =  98, // <Statement> ::= <ActuatorStatement> <statement_end>
   PROD_STATEMENT14                                          =  99, // <Statement> ::= <ListStatement> <statement_end>
   PROD_STATEMENT15                                          = 100, // <Statement> ::= <ConcurrentStatement> <statement_end>
   PROD_STATEMENT16                                          = 101, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT17                                          = 102, // <Statement> ::= <WhileStatement>
   PROD_STATEMENT18                                          = 103, // <Statement> ::= <FunctionStatement> <statement_end>
   PROD_STATEMENT19                                          = 104, // <Statement> ::= <CallStatement> <statement_end>
   PROD_STATEMENT20                                          = 105, // <Statement> ::= <TimeOutStatement>
   PROD_IMPORTSTATEMENT_IMPORT_STRING                        = 106, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                   = 107, // <WaitStatement> ::= wait <time>
   PROD_DECLARESTATEMENT_DECLARE_AS                          = 108, // <DeclareStatement> ::= declare <action> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS2                         = 109, // <DeclareStatement> ::= declare <sensor_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS3                         = 110, // <DeclareStatement> ::= declare <parameter_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS4                         = 111, // <DeclareStatement> ::= declare <point> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS5                         = 112, // <DeclareStatement> ::= declare <rect> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS6                         = 113, // <DeclareStatement> ::= declare <ax12_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_STRING_AS                   = 114, // <DeclareStatement> ::= declare String as <var>
   PROD_TELEPORTSTATEMENT_TELEPORT                           = 115, // <TeleportStatement> ::= teleport <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO                                = 116, // <MoveToStatement> ::= go to <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO2                               = 117, // <MoveToStatement> ::= go to <point_or_var> <speed>
   PROD_ROTATETOSTATEMENT_ROTATE_TO                          = 118, // <RotateToSTatement> ::= rotate to <angle>
   PROD_ROTATETOSTATEMENT_ROTATE_TO2                         = 119, // <RotateToSTatement> ::= rotate to <angle> <speed>
   PROD_ROTATETOSTATEMENT_ROTATE_TO3                         = 120, // <RotateToSTatement> ::= rotate to <angle> <direction>
   PROD_ROTATETOSTATEMENT_ROTATE_TO4                         = 121, // <RotateToSTatement> ::= rotate to <angle> <speed> <direction>
   PROD_ROTATETOSTATEMENT_ROTATE_TO5                         = 122, // <RotateToSTatement> ::= rotate to <angle> <direction> <speed>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                          = 123, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}'
   PROD_CONCURRENTSTATEMENT_CONCURRENT                       = 124, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                      = 125, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_TIMEOUTSTATEMENT_TIMEOUT                             = 126, // <TimeOutStatement> ::= timeout <time> <nl Opt> <Statement>
   PROD_SETPARAMSTATEMENT_SET_EQ                             = 127, // <SetParamStatement> ::= set <parameter_or_var> '=' <num>
   PROD_ENABLESENSORSTATEMENT_ENABLE                         = 128, // <EnableSensorStatement> ::= enable <sensor_or_var>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                     = 129, // <EnableSensorStatement> ::= enable all <sensor>
   PROD_DISABLESENSORSTATEMENT_DISABLE                       = 130, // <DisableSensorStatement> ::= disable <sensor_or_var>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                   = 131, // <DisableSensorStatement> ::= disable all <sensor>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT             = 132, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT2            = 133, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var> <speed>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT  = 134, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT2 = 135, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var> <speed>
   PROD_MOVEAX12STATEMENT_MOVE_TO                            = 136, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle>
   PROD_MOVEAX12STATEMENT_MOVE_TO2                           = 137, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <speed>
   PROD_ACTUATORSTATEMENT_DO                                 = 138, // <ActuatorStatement> ::= do <action_or_var>
   PROD_CONDITIONISOPERATOR_IS                               = 139, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_IS_NOT                           = 140, // <ConditionIsOperator> ::= is not
   PROD_CONDITIONINOPERATOR_IN                               = 141, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOT_IN                           = 142, // <ConditionInOperator> ::= not in
   PROD_SENSORCONDITION                                      = 143, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                     = 144, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                           = 145, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                           = 146, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                   = 147, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                          = 148, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                         = 149, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                         = 150, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                         = 151, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                         = 152, // <IfCondition> ::= <reverseCondition>
   PROD_IFCONDITION_TRUE                                     = 153, // <IfCondition> ::= true
   PROD_IFCONDITION_FALSE                                    = 154, // <IfCondition> ::= false
   PROD_CONDITIONSTATEMENT_IF                                = 155, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                           = 156, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_WHILESTATEMENT_WHILE                                 = 157, // <WhileStatement> ::= while <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_VARLIST_COMMA                                        = 158, // <varList> ::= <var> ',' <varList>
   PROD_VARLIST                                              = 159, // <varList> ::= <var>
   PROD_VARLIST2                                             = 160, // <varList> ::= 
   PROD_FUNCTIONSTATEMENT_FUNCTION_LPAREN_RPAREN             = 161, // <FunctionStatement> ::= function <var> '(' <varList> ')' <nl Opt> <ListStatement>
   PROD_CALLARG                                              = 162, // <CallArg> ::= <var>
   PROD_CALLARG_LBRACKET_RBRACKET                            = 163, // <CallArg> ::= '[' <point> ']'
   PROD_CALLARG_LBRACKET_RBRACKET2                           = 164, // <CallArg> ::= '[' <rect> ']'
   PROD_CALLARG2                                             = 165, // <CallArg> ::= <action>
   PROD_CALLARG3                                             = 166, // <CallArg> ::= <sensor_identifier>
   PROD_CALLARG4                                             = 167, // <CallArg> ::= <parameter_identifier>
   PROD_CALLARG5                                             = 168, // <CallArg> ::= <ax12_identifier>
   PROD_CALLARG_STRING                                       = 169, // <CallArg> ::= String
   PROD_CALLARGLIST_COMMA                                    = 170, // <CallArgList> ::= <CallArg> ',' <CallArgList>
   PROD_CALLARGLIST                                          = 171, // <CallArgList> ::= <CallArg>
   PROD_CALLARGLIST2                                         = 172, // <CallArgList> ::= 
   PROD_CALLSTATEMENT_LPAREN_RPAREN                          = 173  // <CallStatement> ::= <var> '(' <CallArgList> ')'
};
