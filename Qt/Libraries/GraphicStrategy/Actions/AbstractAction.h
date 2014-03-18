#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <QObject>

class StrategyManager;

/**
 * @brief Represent an action to be executed in the strategy.
 * An action can be executed only once. As a consequence it is not necessary to reset the action internal state at the end.\n
 * The method #execute have to be reimplemented to define what the action should do.
 * The method #end can be reimplemented to clean the state after the execution.
 * The method #stop can be reimplemented to support cancellation.
 */
class AbstractAction : public QObject
{
    Q_OBJECT
public:
    AbstractAction(QObject* parent = 0);

	/**
	 * @brief Called when the action need to be started.
	 */
    virtual void execute() = 0;

	/**
	 * @brief Called after the action has finished.
	 */
	virtual void end() {}

	/**
	 * @brief Called when the action execution is in process but must be canceled.
	 * The signal finished must be be emitted otherwise the cancelation will have no effect.
	 */
	virtual void stop() {}

	/**
	 * @brief Should return a textual description of the action.
	 */
    virtual QString getActionName() const = 0;

signals:
	/**
	 * @brief Must be emitted when the action is finished with the appropriate result.
	 */
    void finished(bool result);

protected slots:
	/**
	 * @brief Emit finished(true)
	 */
    void succeed();

	/**
	 * @brief Emit finished(false)
	 */
    void failed();
};

#endif // ABSTRACTACTION_H
