#ifndef AX12STATUSLISTENER_H
#define AX12STATUSLISTENER_H

#include <QSet>
#include <QList>
#include <QObject>

namespace Comm
{
	class AX12CommManager;
	
	class AX12StatusListener : public QObject
	{
		Q_OBJECT
		
		signals:
			void allServoUpdated();
			
		public:
			AX12StatusListener(AX12CommManager* manager, QObject* parent = 0);
			void setIds(const QList<quint8>& ids);
			
			bool hasUnavailableServo() const;
			const QList<quint8>& getUnavailableServo() const;

            bool isListening() const;
			
		public slots:
			void startListening();
			void stopListening();
			
		private:
			AX12CommManager* _manager;
			QSet<quint8> _idsToListen;
			QSet<quint8> _updatedIds;
			QSet<quint8> _currentUnavailableIds;
			QList<quint8> _unavailableIds;
            bool _isListenning;
			
			void sendAllServoUpdatedMessage();

            void registerId(quint8 id);
			
		private slots:
            void registerId(const QList<quint8>& ids);
            void registerUnavailableId(const QList<quint8>& id);
	};
}

#endif
