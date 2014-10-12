#ifndef PERSISTENTFILESMANAGER_H
#define PERSISTENTFILESMANAGER_H

#include <QDir>
#include <QHash>

class PersistentFilesManager
{
public:
	PersistentFilesManager();
	void addSet(int setIndex, const QString& name);

	QStringList getFiles(int setIndex) const;
	QByteArray getFileData(int setIndex, const QString& filename) const;

	bool setFile(int setIndex, const QString& filename, const QByteArray& fileData);
	bool removeFile(int setIndex, const QString& filename);

	bool hasFile(int setIndex, const QString& filename);

private:
	QHash<int, QString> _sets;
	QDir _mainDir;

	void getAllFiles(const QDir& setDir, const QDir& subDir, QStringList& results) const;
};

#endif // PERSISTENTFILESMANAGER_H
