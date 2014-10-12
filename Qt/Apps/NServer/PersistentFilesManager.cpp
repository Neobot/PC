#include "PersistentFilesManager.h"
#include "ToolsLib.h"

#include <QFileInfoList>

const QString FILE_MANAGER_MAIN_DIR = "NeobotConfigurationFiles";

PersistentFilesManager::PersistentFilesManager()
{
	_mainDir = Tools::getDataDirectory();
	_mainDir.mkdir(FILE_MANAGER_MAIN_DIR);
	_mainDir.cd(FILE_MANAGER_MAIN_DIR);
}

void PersistentFilesManager::addSet(int setIndex, const QString& name)
{
	_mainDir.mkdir(name);
	_sets[setIndex] = name;
}

QStringList PersistentFilesManager::getFiles(int setIndex) const
{
	QStringList results;
	if (_sets.contains(setIndex))
	{
		QDir setDir(_mainDir);
		setDir.cd(_sets[setIndex]);

		getAllFiles(setDir, setDir, results);
	}

	return results;
}

QByteArray PersistentFilesManager::getFileData(int setIndex, const QString &filename) const
{
	QByteArray data;
	if (_sets.contains(setIndex))
	{
		QDir setDir(_mainDir);
		setDir.cd(_sets[setIndex]);

		QString realFileName = setDir.absoluteFilePath(filename);
		QFile file(realFileName);
		if (file.open(QIODevice::ReadOnly))
			data = file.readAll();
	}

	return data;
}

void PersistentFilesManager::getAllFiles(const QDir& setDir, const QDir& subDir, QStringList& results) const
{
	QFileInfoList list = subDir.entryInfoList(QDir::NoDotDot | QDir::Files | QDir::AllDirs);
	for(const QFileInfo& info : list)
	{
		if (info.isDir())
		{
			QDir d(subDir);
			d.cd(info.fileName());
			getAllFiles(setDir, d, results);
		}
		else
		{
			results << setDir.relativeFilePath(info.absoluteFilePath());
		}
	}
}

bool PersistentFilesManager::setFile(int setIndex, const QString& filename, const QByteArray& fileData)
{
	bool result = false;
	if (_sets.contains(setIndex))
	{
		QDir setDir(_mainDir);
		setDir.cd(_sets[setIndex]);

		QString realFileName = setDir.absoluteFilePath(filename);
		QFile file(realFileName);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(fileData);
			result = true;
		}
	}

	return result;
}

bool PersistentFilesManager::removeFile(int setIndex, const QString& filename)
{
	bool result = false;
	if (_sets.contains(setIndex))
	{
		QDir setDir(_mainDir);
		setDir.cd(_sets[setIndex]);

		QString realFileName = setDir.absoluteFilePath(filename);
		result = QFile::remove(realFileName);
	}

	return result;
}

bool PersistentFilesManager::hasFile(int setIndex, const QString &filename)
{
	bool result = false;
	if (_sets.contains(setIndex))
	{
		QDir setDir(_mainDir);
		setDir.cd(_sets[setIndex]);

		QString realFileName = setDir.absoluteFilePath(filename);
		result = QFile::exists(realFileName);
	}

	return result;
}
