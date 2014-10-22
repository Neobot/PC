#ifndef FILEENVREPLICATOR_H
#define FILEENVREPLICATOR_H

#include "NetworkClientCommListener.h"

#include <QDir>
#include <QTemporaryDir>

class NetworkConnection;

class FileEnvReplicator : public NetworkClientCommListener
{
public:
	FileEnvReplicator(NetworkConnection* connection, int category, const QString& filter = QString());
	~FileEnvReplicator();

	bool isValid() const;

	void refresh();
	void refresh(const QString& filename);
	void refreshWithData(const QString& filename, const QByteArray& data) const;

	const QDir& getReplicatedDir() const;
	QStringList getFileNames() const;

private:
	NetworkConnection* _connection;
	int _category;
	QString _filter;
	QDir _repDir;
	QTemporaryDir _tempDir;
	QStringList _filenames;

	void configurationFiles(int category, const QStringList& filenames);
	void configurationFileData(int category, const QString& filename, const QByteArray& data);
	void configurationFileEvent(int category, const QString& filename, int event);

	void saveFile(const QString &fileName, const QByteArray &data) const;
	void removeFile(const QString &fileName);
};


#endif // FILEENVREPLICATOR_H
