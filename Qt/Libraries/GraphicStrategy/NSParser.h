#ifndef NSPARSER_H
#define NSPARSER_H

#include <QStringList>
#include <QTextStream>

#include "RPoint.h"
#include "RMovement.h"
#include "Instructions.h"

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

	bool parse(const QString& scriptCode, QStringList& messages, QList<AbstractAction*>& actions);
	bool parseFile(const QString& filepath, QStringList& messages, QList<AbstractAction*>& actions);

	bool verify(const QString& scriptCode, QStringList& messages);
	bool verifyFile(const QString& filepath, QStringList& messages);
	
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
			Sensor
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
		void toAction(int& id, int& param, int& timeMs);
		
		static DelclaredVariable fromPoint(const Tools::RPoint& p);
		static DelclaredVariable fromRect(const QRectF& rect);
		static DelclaredVariable fromAction(int id, int param, int timeMs);
		static DelclaredVariable fromAx12(int id);
		static DelclaredVariable fromParameter(int id);
		static DelclaredVariable fromSensor(int id, int type);
	};
	
	typedef QHash<QString, DelclaredVariable> VariableList;
	
	Symbol* getParsedTree(const QString& scriptCode, QStringList& messages);
	void buildActions(Symbol* symbol, QList<AbstractAction*>& actions, VariableList& variables);
	
	AbstractAction* buildWaitAction(Symbol* symbol);
	AbstractAction *buildTeleportAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildGoToAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildSetParameterAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildEnableSensorAction(Symbol *symbol, VariableList &variables);
	AbstractAction *buildDisableSensorAction(Symbol *symbol, VariableList &variables);
	void readVariable(Symbol* symbol, VariableList& variables);

	int readParameterOrVar(Symbol* symbol, VariableList &variables);
	int readAx12OrVar(Symbol* symbol, VariableList &variables);

	void readAction(Symbol* symbol, int &actionId, int& param, int& time);
	void readActionOrVar(Symbol* symbol, VariableList &variables, int &actionId, int& param, int& time);

	void readSensorIdentifier(Symbol* symbol, int &sensorType, int& id);
	void readSensorOrVar(Symbol* symbol, VariableList &variables, int &sensorType, int& id);
	int readSensorType(Symbol* symbol);

	QString readIdentifier(Symbol* symbol);
	QString readString(Symbol* symbol);
	double readFloat(Symbol* symbol);
	int readInteger(Symbol* symbol);
	double readNum(Symbol* symbol);
	int readSubId(Symbol* symbol);
	QString readVar(Symbol *symbol);

	double readFixedAngleInRadian(Symbol *symbol);
	double readAngleInRadian(Symbol *symbol);

	Tools::RPoint readFixedPoint(Symbol* symbol);
	Tools::RPoint readPoint(Symbol *symbol);
	bool readPointOrVar(Symbol *symbol, VariableList &variables, Tools::RPoint& point);

	QRectF readRect(Symbol* symbol);
	QRectF readFixedRect(Symbol* symbol);
	bool readRectOrVar(Symbol* symbol, VariableList &variables, QRectF &r);

	int readSpeed(Symbol *symbol);
	Tools::Direction readDirection(Symbol *symbol);
	int readTimeInMs(Symbol* symbol);
	int readTimeUnitFactor(Symbol* symbol);

	Symbol *searchChild(Symbol *symbol, unsigned short symbolIndex, bool recursive = false);
	
	QString composeErrorMsg(GPError* err) const;
	void printTree(QTextStream& out, Symbol *s, int level);

private:
	CGTFile _cgtFile;
	bool _invalidGrammar;
	
	Symbol* _tree;
	ActionFactory* _factory;

};

#endif // NSPARSER_H
