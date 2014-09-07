#ifndef NSPARSER_H
#define NSPARSER_H

#include <QStringList>
#include <QTextStream>
#include <QRectF>

#include "RPoint.h"
#include "RMovement.h"
#include "Instructions.h"

#include "NSParsingError.h"

#include "CGTFile.h"

class Symbol;
class GPError;

class AbstractAction;
class ActionFactory;

class NSParser
{
public:
	NSParser(ActionFactory* factory = nullptr);
	virtual ~NSParser();
	
	void setActionFactory(ActionFactory* factory);

	bool parse(const QString& scriptCode, QList<AbstractAction*>& actions);
	bool parseFile(const QString& filepath, QList<AbstractAction*>& actions);

	bool verify(const QString& scriptCode);
	bool verifyFile(const QString& filepath);

	const QList<NSParsingError>& getErrors() const;
	
	void print(QTextStream& out);

protected:
	struct DelclaredVariable
	{
		enum DeclaredType
		{
			None,
			Point,
			Rect,
			Action,
			Ax12,
			Param,
			Sensor,
			String
		};
		
		DeclaredType type;
		QVariantList data;

		DelclaredVariable() : type(None) {}
		bool isValid() const {return type != None;}
		
		Tools::RPoint toPoint() const;
		QRectF toRect() const;
		int toAx12() const;
		int toParameter() const;
		void toSensor(int& id, int& type) const;
		void toAction(int& id, int& param, int& timeMs) const;
		QString toString() const;

		bool isPoint() const {return type == Point;}
		bool isRect() const {return type == Rect;}
		bool isAction() const {return type == Action;}
		bool isAx12() const {return type == Ax12;}
		bool isParameter() const {return type == Param;}
		bool isSensor() const {return type == Sensor;}
		bool isString() const {return type == String;}
		bool isDefined() const {return type != None;}
		
		static DelclaredVariable fromPoint(const Tools::RPoint& p);
		static DelclaredVariable fromRect(const QRectF& rect);
		static DelclaredVariable fromAction(int id, int param, int timeMs);
		static DelclaredVariable fromAx12(int id);
		static DelclaredVariable fromParameter(int id);
		static DelclaredVariable fromSensor(int id, int type);
		static DelclaredVariable fromString(const QString& str);
	};
	
	typedef QHash<QString, DelclaredVariable> VariableList;

	struct ConditionInfo
	{
		enum ConditionType
		{
			InvalidCondition,
			AlwaysTrueCondition,
			AlwaysFalseCondition,
			RobotPosCondition,
			RobotOrientationCondition,
			OpponentPosCondition,
			SharpValueCondition,
			MicroswitchValueCondition,
			ColorSensorValueCondition,
			ReversedStrategyCondition
		};

		ConditionInfo() : type(InvalidCondition), neg(false) {}
		bool isValid() const {return type != InvalidCondition;}
		void setInvalid() {type = InvalidCondition;}

		ConditionType type;
		int sensorId;
		int sensorValue;
		double angleMin;
		double angleMax;
		QRectF rect;
		bool neg;
	};

	struct FunctionInfo
	{
		FunctionInfo() : actionListSymbol(nullptr) {}
		Symbol* actionListSymbol;
		QStringList parameterNames;
	};

	typedef QHash<QString, FunctionInfo> FunctionList;
	
	bool parse(const QString& scriptCode, QList<AbstractAction*>& actions, const QString& originalFilename);
	Symbol* getParsedTree(const QString& scriptCode);
	void buildActions(Symbol* symbol, QList<AbstractAction*>& actions, VariableList& variables, FunctionList &functions);
	
	//Action parsers
	AbstractAction* buildWaitAction(Symbol* symbol);
	AbstractAction *buildTeleportAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildGoToAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildSetParameterAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildEnableSensorAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildDisableSensorAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildActuatorAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildMoveAx12Action(Symbol *symbol, VariableList &variables);
	AbstractAction *buildAx12MovementAction(Symbol *symbol, VariableList &variables, bool async);
	AbstractAction *buildListAction(Symbol *symbol, VariableList &variables, FunctionList &functions);
	AbstractAction *buildConcurrentListAction(Symbol *symbol, VariableList &variables, FunctionList& functions);
	AbstractAction *buildIfAction(Symbol *symbol, VariableList &variables, FunctionList& functions);
	AbstractAction *buildWhileAction(Symbol *symbol, VariableList &variables, FunctionList& functions);
	AbstractAction* buildCalledFunctionActions(Symbol *symbol, VariableList &variables, FunctionList& functions);
	void readVariable(Symbol* symbol, VariableList& variables);
	void readFunction(Symbol* symbol, FunctionList& functions);

	//Variable parsers
	bool readParameterOrVar(Symbol* symbol, VariableList &variables, int &paramId);
	bool readParameterVar(Symbol* symbol, VariableList &variables, int &paramId);

	bool readAx12OrVar(Symbol* symbol, VariableList &variables, int &ax12Id);
	bool readAx12Var(Symbol* symbol, VariableList &variables, int &ax12Id);

	void readAction(Symbol* symbol, int &actionId, int& param, int& time);
	bool readActionOrVar(Symbol* symbol, VariableList &variables, int &actionId, int& param, int& time);
	bool readActionVar(Symbol* symbol, VariableList &variables, int &actionId, int& param, int& time);

	void readSensorIdentifier(Symbol* symbol, int &sensorType, int& id);
	bool readSensorOrVar(Symbol* symbol, VariableList &variables, int &sensorType, int& id);
	bool readSensorVar(Symbol* symbol, VariableList &variables, int &sensorType, int& id);
	int readSensorType(Symbol* symbol);

	Tools::RPoint readFixedPoint(Symbol* symbol);
	Tools::RPoint readPoint(Symbol *symbol);
	bool readPointOrVar(Symbol *symbol, VariableList &variables, Tools::RPoint& point);
	bool readPointVar(Symbol* symbol, VariableList& variables, Tools::RPoint &point);

	QRectF readRect(Symbol* symbol);
	QRectF readFixedRect(Symbol* symbol);
	bool readRectOrVar(Symbol* symbol, VariableList &variables, QRectF &r);
	bool readRectVar(Symbol* symbol, VariableList &variables, QRectF &r);

	bool readStringOrVar(Symbol* symbol, VariableList& variables, QString &str);
	bool readStringVar(Symbol* symbol, VariableList& variables, QString &str);

	//Advanced types parsers
	int readSpeed(Symbol *symbol);
	Tools::Direction readDirection(Symbol *symbol);
	int readTimeInMs(Symbol* symbol);
	int readTimeUnitFactor(Symbol* symbol);
	double readFixedAngleInRadian(Symbol *symbol);
	double readAngleInRadian(Symbol *symbol);
	void readAngleRangeInRadian(Symbol *symbol, double& min, double& max);
	int readConcurrencyStopCondition(Symbol *symbol);
	ConditionInfo readCondition(Symbol *symbol, VariableList& variables);
	int readSensorValue(Symbol *symbol, Comm::SensorType &type);
	bool readCallArg(Symbol *symbol, VariableList &variables, NSParser::DelclaredVariable& callArgVariable);
	void readCallArgList(Symbol *symbol, VariableList &variables, QList<NSParser::DelclaredVariable>& callArgVariableList);

	//Basic types parsers
	QString readIdentifier(Symbol* symbol);
	QString readString(Symbol* symbol);
	double readFloat(Symbol* symbol);
	int readInteger(Symbol* symbol);
	double readNum(Symbol* symbol);
	int readSubId(Symbol* symbol);
	QString readVar(Symbol *symbol);
	void readVarList(Symbol *symbol, QStringList& varList);

	//Tools
	Symbol *searchChild(Symbol *symbol, unsigned short symbolIndex, bool recursive = false);
	QString readTerminals(Symbol *symbol);
	
	void printTree(QTextStream& out, Symbol *s, int level);
	void addError(const NSParsingError &error);

private:
	CGTFile _cgtFile;
	bool _invalidGrammar;
	
	Symbol* _tree;
	ActionFactory* _factory;

	QString _currentFile;
	QList<NSParsingError> _errors;
};

#endif // NSPARSER_H
