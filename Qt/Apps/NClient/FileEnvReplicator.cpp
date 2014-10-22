#include "FileEnvReplicator.h"
#include "NetworkClientCommInterface.h"
#include "NetworkConnection.h"
#include "ToolsLib.h"

#include <QtDebug>

FileEnvReplicator::FileEnvReplicator(NetworkConnection *connection, int category, const QString &filter)
	: _connection(connection), _category(category), _filter(filter), _tempDir()
{
	qDebug() << _tempDir.path();
	if (_tempDir.isValid())
	{
		_repDir = QDir(_tempDir.path());
	}

	_connection->registerNetworkResponder(this);
}

FileEnvReplicator::~FileEnvReplicator()
{
    _connection->unregisterNetworkResponder(this);
}

void FileEnvReplicator::refresh()
{
	_connection->getComm()->askFiles(_category);
}

void FileEnvReplicator::refresh(const QString &filename)
{
	if (_filenames.contains(filename))
		_connection->getComm()->askFileData(_category, filename);
}

void FileEnvReplicator::refreshWithData(const QString &filename, const QByteArray &data) const
{
	if (_filenames.contains(filename))
		saveFile(filename, data);
}

const QDir &FileEnvReplicator::getReplicatedDir() const
{
	return _repDir;
}

QStringList FileEnvReplicator::getFileNames() const
{
	return _filenames;
}

bool FileEnvReplicator::isValid() const
{
	return _tempDir.isValid();
}

void FileEnvReplicator::configurationFiles(int category, const QStringList& fileList)
{
	if (_category == category)
	{
		_filenames.clear();
		for(const QString& fileName : fileList)
		{
			QFileInfo info(fileName);
			if (_filter.isEmpty() || info.suffix() == _filter)
			{
				_filenames << fileName;
				_connection->getComm()->askFileData(_category, fileName);
			}
		}
	}
}

void FileEnvReplicator::configurationFileData(int category, const QString& fileName, const QByteArray& data)
{
	if (_category == category)
	{
		saveFile(fileName, data);
	}
}

void FileEnvReplicator::configurationFileEvent(int category, const QString& fileName, int status)
{
	if (_category == category)
	{
		if (status == Comm::FileRemoved)
		{
			removeFile(fileName);
		}
		else
		{
			if (status == Comm::FileAdded)
				_filenames << fileName;
			_connection->getComm()->askFileData(_category, fileName);
		}
	}
}

void FileEnvReplicator::saveFile(const QString& fileName,  const QByteArray& data) const
{
	_repDir.mkpath(QFileInfo(fileName).absolutePath());
	QFile f(_repDir.absoluteFilePath(fileName));
	if (f.open(QIODevice::WriteOnly))
	{
		f.write(data);
		f.close();
	}
}

void FileEnvReplicator::removeFile(const QString& fileName)
{
	QFile::remove(_repDir.absoluteFilePath(fileName));
}
