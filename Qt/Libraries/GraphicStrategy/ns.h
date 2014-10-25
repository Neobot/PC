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
   SYM_CLOSE                      =  29, // close (i.e. nearby)
   SYM_COLOR                      =  30, // color
   SYM_CONCURRENT                 =  31, // concurrent
   SYM_DECLARE                    =  32, // declare
   SYM_DEG                        =  33, // deg
   SYM_DETECTED                   =  34, // detected
   SYM_DISABLE                    =  35, // disable
   SYM_DO                         =  36, // do
   SYM_ELSE                       =  37, // else
   SYM_ENABLE                     =  38, // enable
   SYM_EOF2                       =  39, // eof
   SYM_FALSE                      =  40, // false
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
   SYM_LASTACTIONFINISHED         =  54, // LastActionFinished
   SYM_MAKE                       =  55, // make
   SYM_MICROSWITCH                =  56, // microswitch
   SYM_MOVE                       =  57, // move
   SYM_MOVEMENT                   =  58, // movement
   SYM_MS                         =  59, // ms
   SYM_NEWLINE                    =  60, // NewLine
   SYM_NOT                        =  61, // not
   SYM_OFF                        =  62, // off
   SYM_ON                         =  63, // on
   SYM_ONEACTIONFINISHED          =  64, // OneActionFinished
   SYM_OPPONENT                   =  65, // opponent
   SYM_ORIENTATION                =  66, // orientation
   SYM_PARAMETER                  =  67, // parameter
   SYM_POSITION                   =  68, // position
   SYM_RAD                        =  69, // rad
   SYM_RECT                       =  70, // rect
   SYM_RED                        =  71, // red
   SYM_REVERSED                   =  72, // reversed
   SYM_ROTATE                     =  73, // rotate
   SYM_S                          =  74, // s
   SYM_SENSOR                     =  75, // sensor
   SYM_SET                        =  76, // set
   SYM_SHARP                      =  77, // sharp
   SYM_SPEED                      =  78, // speed
   SYM_STRATEGY                   =  79, // strategy
   SYM_STRING                     =  80, // String
   SYM_TELEPORT                   =  81, // teleport
   SYM_TIMEOUT                    =  82, // timeout
   SYM_TO                         =  83, // to
   SYM_TRUE                       =  84, // true
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
   SYM_ROTATETOSTATEMENT          = 139, // <RotateToSTatement>
   SYM_SENSOR2                    = 140, // <sensor>
   SYM_SENSOR_IDENTIFIER          = 141, // <sensor_identifier>
   SYM_SENSOR_OR_VAR              = 142, // <sensor_or_var>
   SYM_SENSORCONDITION            = 143, // <SensorCondition>
   SYM_SENSORVALUE                = 144, // <sensorValue>
   SYM_SETPARAMSTATEMENT          = 145, // <SetParamStatement>
   SYM_SHARPVALUE                 = 146, // <sharpValue>
   SYM_SPEED2                     = 147, // <speed>
   SYM_START                      = 148, // <Start>
   SYM_STATEMENT                  = 149, // <Statement>
   SYM_STATEMENT_END              = 150, // <statement_end>
   SYM_STATEMENTS                 = 151, // <Statements>
   SYM_STRING_OR_VAR              = 152, // <string_or_var>
   SYM_TELEPORTSTATEMENT          = 153, // <TeleportStatement>
   SYM_TIME                       = 154, // <time>
   SYM_TIME_UNIT                  = 155, // <time_unit>
   SYM_TIMEOUTSTATEMENT           = 156, // <TimeOutStatement>
   SYM_VAR                        = 157, // <var>
   SYM_VARLIST                    = 158, // <varList>
   SYM_WAITSTATEMENT              = 159, // <WaitStatement>
   SYM_WHILESTATEMENT             = 160  // <WhileStatement>
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
   PROD_SHARPVALUE_DETECTED                                  =  59, // <sharpValue> ::= detected
   PROD_SHARPVALUE_FAR                                       =  60, // <sharpValue> ::= far
   PROD_SHARPVALUE_CLOSE                                     =  61, // <sharpValue> ::= close
   PROD_MICROSWITCHVALUE_ON                                  =  62, // <microSwitchValue> ::= on
   PROD_MICROSWITCHVALUE_OFF                                 =  63, // <microSwitchValue> ::= off
   PROD_SENSORVALUE                                          =  64, // <sensorValue> ::= <colorSensorValue>
   PROD_SENSORVALUE2                                         =  65, // <sensorValue> ::= <sharpValue>
   PROD_SENSORVALUE3                                         =  66, // <sensorValue> ::= <microSwitchValue>
   PROD_SENSORVALUE4                                         =  67, // <sensorValue> ::= <num>
   PROD_CONCURRENCYCONDITION_ALLACTIONSFINISHED              =  68, // <concurrencyCondition> ::= AllActionsFinished
   PROD_CONCURRENCYCONDITION_ONEACTIONFINISHED               =  69, // <concurrencyCondition> ::= OneActionFinished
   PROD_CONCURRENCYCONDITION_FIRSTACTIONFINISHED             =  70, // <concurrencyCondition> ::= FirstActionFinished
   PROD_CONCURRENCYCONDITION_LASTACTIONFINISHED              =  71, // <concurrencyCondition> ::= LastActionFinished
   PROD_ACTION_ACTION_INTEGER_COMMA_INTEGER_COMMA            =  72, // <action> ::= action Integer ',' Integer ',' <time>
   PROD_ACTION_OR_VAR                                        =  73, // <action_or_var> ::= <action>
   PROD_ACTION_OR_VAR2                                       =  74, // <action_or_var> ::= <var>
   PROD_PARAMETER_IDENTIFIER_PARAMETER                       =  75, // <parameter_identifier> ::= parameter <id>
   PROD_PARAMETER_OR_VAR                                     =  76, // <parameter_or_var> ::= <parameter_identifier>
   PROD_PARAMETER_OR_VAR2                                    =  77, // <parameter_or_var> ::= <var>
   PROD_AX12_IDENTIFIER_AX12                                 =  78, // <ax12_identifier> ::= 'ax12' <id>
   PROD_AX12_OR_VAR                                          =  79, // <ax12_or_var> ::= <ax12_identifier>
   PROD_AX12_OR_VAR2                                         =  80, // <ax12_or_var> ::= <var>
   PROD_START                                                =  81, // <Start> ::= <nl Opt> <Program>
   PROD_PROGRAM                                              =  82, // <Program> ::= <Statements>
   PROD_STATEMENTS                                           =  83, // <Statements> ::= <Statement> <Statements>
   PROD_STATEMENTS2                                          =  84, // <Statements> ::= 
   PROD_STATEMENT                                            =  85, // <Statement> ::= <ImportStatement> <statement_end>
   PROD_STATEMENT2                                           =  86, // <Statement> ::= <WaitStatement> <statement_end>
   PROD_STATEMENT3                                           =  87, // <Statement> ::= <DeclareStatement> <statement_end>
   PROD_STATEMENT4                                           =  88, // <Statement> ::= <TeleportStatement> <statement_end>
   PROD_STATEMENT5                                           =  89, // <Statement> ::= <MoveToStatement> <statement_end>
   PROD_STATEMENT6                                           =  90, // <Statement> ::= <RotateToSTatement> <statement_end>
   PROD_STATEMENT7                                           =  91, // <Statement> ::= <SetParamStatement> <statement_end>
   PROD_STATEMENT8                                           =  92, // <Statement> ::= <EnableSensorStatement> <statement_end>
   PROD_STATEMENT9                                           =  93, // <Statement> ::= <DisableSensorStatement> <statement_end>
   PROD_STATEMENT10                                          =  94, // <Statement> ::= <Ax12MovementStatement> <statement_end>
   PROD_STATEMENT11                                          =  95, // <Statement> ::= <Ax12AsyncMovementStatement> <statement_end>
   PROD_STATEMENT12                                          =  96, // <Statement> ::= <MoveAx12Statement> <statement_end>
   PROD_STATEMENT13                                          =  97, // <Statement> ::= <ActuatorStatement> <statement_end>
   PROD_STATEMENT14                                          =  98, // <Statement> ::= <ListStatement> <statement_end>
   PROD_STATEMENT15                                          =  99, // <Statement> ::= <ConcurrentStatement> <statement_end>
   PROD_STATEMENT16                                          = 100, // <Statement> ::= <ConditionStatement>
   PROD_STATEMENT17                                          = 101, // <Statement> ::= <WhileStatement>
   PROD_STATEMENT18                                          = 102, // <Statement> ::= <FunctionStatement> <statement_end>
   PROD_STATEMENT19                                          = 103, // <Statement> ::= <CallStatement> <statement_end>
   PROD_STATEMENT20                                          = 104, // <Statement> ::= <TimeOutStatement>
   PROD_IMPORTSTATEMENT_IMPORT_STRING                        = 105, // <ImportStatement> ::= import String
   PROD_WAITSTATEMENT_WAIT                                   = 106, // <WaitStatement> ::= wait <time>
   PROD_DECLARESTATEMENT_DECLARE_AS                          = 107, // <DeclareStatement> ::= declare <action> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS2                         = 108, // <DeclareStatement> ::= declare <sensor_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS3                         = 109, // <DeclareStatement> ::= declare <parameter_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS4                         = 110, // <DeclareStatement> ::= declare <point> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS5                         = 111, // <DeclareStatement> ::= declare <rect> as <var>
   PROD_DECLARESTATEMENT_DECLARE_AS6                         = 112, // <DeclareStatement> ::= declare <ax12_identifier> as <var>
   PROD_DECLARESTATEMENT_DECLARE_STRING_AS                   = 113, // <DeclareStatement> ::= declare String as <var>
   PROD_TELEPORTSTATEMENT_TELEPORT                           = 114, // <TeleportStatement> ::= teleport <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO                                = 115, // <MoveToStatement> ::= go to <point_or_var>
   PROD_MOVETOSTATEMENT_GO_TO2                               = 116, // <MoveToStatement> ::= go to <point_or_var> <speed>
   PROD_ROTATETOSTATEMENT_ROTATE_TO                          = 117, // <RotateToSTatement> ::= rotate to <angle>
   PROD_ROTATETOSTATEMENT_ROTATE_TO2                         = 118, // <RotateToSTatement> ::= rotate to <angle> <speed>
   PROD_ROTATETOSTATEMENT_ROTATE_TO3                         = 119, // <RotateToSTatement> ::= rotate to <angle> <direction>
   PROD_ROTATETOSTATEMENT_ROTATE_TO4                         = 120, // <RotateToSTatement> ::= rotate to <angle> <speed> <direction>
   PROD_ROTATETOSTATEMENT_ROTATE_TO5                         = 121, // <RotateToSTatement> ::= rotate to <angle> <direction> <speed>
   PROD_LISTSTATEMENT_LBRACE_RBRACE                          = 122, // <ListStatement> ::= '{' <nl Opt> <Statements> <nl Opt> '}'
   PROD_CONCURRENTSTATEMENT_CONCURRENT                       = 123, // <ConcurrentStatement> ::= concurrent <nl Opt> <ListStatement>
   PROD_CONCURRENTSTATEMENT_CONCURRENT2                      = 124, // <ConcurrentStatement> ::= concurrent <concurrencyCondition> <nl Opt> <ListStatement>
   PROD_TIMEOUTSTATEMENT_TIMEOUT                             = 125, // <TimeOutStatement> ::= timeout <time> <nl Opt> <Statement>
   PROD_SETPARAMSTATEMENT_SET_EQ                             = 126, // <SetParamStatement> ::= set <parameter_or_var> '=' <num>
   PROD_ENABLESENSORSTATEMENT_ENABLE                         = 127, // <EnableSensorStatement> ::= enable <sensor_or_var>
   PROD_ENABLESENSORSTATEMENT_ENABLE_ALL                     = 128, // <EnableSensorStatement> ::= enable all <sensor>
   PROD_DISABLESENSORSTATEMENT_DISABLE                       = 129, // <DisableSensorStatement> ::= disable <sensor_or_var>
   PROD_DISABLESENSORSTATEMENT_DISABLE_ALL                   = 130, // <DisableSensorStatement> ::= disable all <sensor>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT             = 131, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var>
   PROD_AX12MOVEMENTSTATEMENT_MAKE_AX12_MOVEMENT2            = 132, // <Ax12MovementStatement> ::= make 'ax12' movement <string_or_var> <string_or_var> <speed>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT  = 133, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var>
   PROD_AX12ASYNCMOVEMENTSTATEMENT_MAKE_AX12_ASYNC_MOVEMENT2 = 134, // <Ax12AsyncMovementStatement> ::= make 'ax12' async movement <string_or_var> <string_or_var> <speed>
   PROD_MOVEAX12STATEMENT_MOVE_TO                            = 135, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle>
   PROD_MOVEAX12STATEMENT_MOVE_TO2                           = 136, // <MoveAx12Statement> ::= move <ax12_or_var> to <angle> <speed>
   PROD_ACTUATORSTATEMENT_DO                                 = 137, // <ActuatorStatement> ::= do <action_or_var>
   PROD_CONDITIONISOPERATOR_IS                               = 138, // <ConditionIsOperator> ::= is
   PROD_CONDITIONISOPERATOR_IS_NOT                           = 139, // <ConditionIsOperator> ::= is not
   PROD_CONDITIONINOPERATOR_IN                               = 140, // <ConditionInOperator> ::= in
   PROD_CONDITIONINOPERATOR_NOT_IN                           = 141, // <ConditionInOperator> ::= not in
   PROD_SENSORCONDITION                                      = 142, // <SensorCondition> ::= <sensor_or_var> <ConditionIsOperator> <sensorValue>
   PROD_ORIENTATIONCONDITION_ORIENTATION                     = 143, // <OrientationCondition> ::= orientation <ConditionInOperator> <angleRange>
   PROD_POSITIONCONDITION_POSITION                           = 144, // <PositionCondition> ::= position <ConditionInOperator> <rect_or_var>
   PROD_OPPONENTCONDITION_OPPONENT                           = 145, // <OpponentCondition> ::= opponent <ConditionInOperator> <rect_or_var>
   PROD_REVERSECONDITION_STRATEGY_REVERSED                   = 146, // <reverseCondition> ::= strategy <ConditionIsOperator> reversed
   PROD_IFCONDITION                                          = 147, // <IfCondition> ::= <SensorCondition>
   PROD_IFCONDITION2                                         = 148, // <IfCondition> ::= <OrientationCondition>
   PROD_IFCONDITION3                                         = 149, // <IfCondition> ::= <PositionCondition>
   PROD_IFCONDITION4                                         = 150, // <IfCondition> ::= <OpponentCondition>
   PROD_IFCONDITION5                                         = 151, // <IfCondition> ::= <reverseCondition>
   PROD_IFCONDITION_TRUE                                     = 152, // <IfCondition> ::= true
   PROD_IFCONDITION_FALSE                                    = 153, // <IfCondition> ::= false
   PROD_CONDITIONSTATEMENT_IF                                = 154, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement>
   PROD_CONDITIONSTATEMENT_IF_ELSE                           = 155, // <ConditionStatement> ::= if <IfCondition> <nl Opt> <Statement> else <nl Opt> <Statement>
   PROD_WHILESTATEMENT_WHILE                                 = 156, // <WhileStatement> ::= while <IfCondition> <opt_time> <nl Opt> <Statement>
   PROD_VARLIST_COMMA                                        = 157, // <varList> ::= <var> ',' <varList>
   PROD_VARLIST                                              = 158, // <varList> ::= <var>
   PROD_VARLIST2                                             = 159, // <varList> ::= 
   PROD_FUNCTIONSTATEMENT_FUNCTION_LPAREN_RPAREN             = 160, // <FunctionStatement> ::= function <var> '(' <varList> ')' <nl Opt> <ListStatement>
   PROD_CALLARG                                              = 161, // <CallArg> ::= <var>
   PROD_CALLARG_LBRACKET_RBRACKET                            = 162, // <CallArg> ::= '[' <point> ']'
   PROD_CALLARG_LBRACKET_RBRACKET2                           = 163, // <CallArg> ::= '[' <rect> ']'
   PROD_CALLARG2                                             = 164, // <CallArg> ::= <action>
   PROD_CALLARG3                                             = 165, // <CallArg> ::= <sensor_identifier>
   PROD_CALLARG4                                             = 166, // <CallArg> ::= <parameter_identifier>
   PROD_CALLARG5                                             = 167, // <CallArg> ::= <ax12_identifier>
   PROD_CALLARGLIST_COMMA                                    = 168, // <CallArgList> ::= <CallArg> ',' <CallArgList>
   PROD_CALLARGLIST                                          = 169, // <CallArgList> ::= <CallArg>
   PROD_CALLARGLIST2                                         = 170, // <CallArgList> ::= 
   PROD_CALLSTATEMENT_LPAREN_RPAREN                          = 171  // <CallStatement> ::= <var> '(' <CallArgList> ')'
};
