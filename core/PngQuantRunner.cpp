#include "PngQuantRunner.h"
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

PngQuantRunner::PngQuantRunner(QObject* parent)
    : QObject(parent)
{
    m_pngquantPath = getPngQuantPath();
}

QString PngQuantRunner::getPngQuantPath()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString pngquantPath;

#ifdef Q_OS_WIN
    pngquantPath = appDir + "/resources/pngquant/win/pngquant.exe";
#elif defined(Q_OS_MAC)
    pngquantPath = appDir + "/../Resources/pngquant/mac/pngquant";
#else
    // Linux or other Unix-like systems
    pngquantPath = appDir + "/resources/pngquant/linux/pngquant";
#endif

    // Fallback: try to use system pngquant
    if (!QFile::exists(pngquantPath)) {
        pngquantPath = "pngquant";
    }

    return pngquantPath;
}

bool PngQuantRunner::validateInput(ImageTask& task)
{
    QFileInfo fileInfo(task.inputPath);

    // Check if file exists
    if (!fileInfo.exists()) {
        task.errorMessage = "文件不存在";
        return false;
    }

    // Check if it's a file (not directory)
    if (!fileInfo.isFile()) {
        task.errorMessage = "不是有效的文件";
        return false;
    }

    // Check if it's readable
    if (!fileInfo.isReadable()) {
        task.errorMessage = "文件不可读";
        return false;
    }

    // Check file extension
    if (!task.inputPath.endsWith(".png", Qt::CaseInsensitive)) {
        task.errorMessage = "不是PNG格式";
        return false;
    }

    // Get file size
    task.originalSize = fileInfo.size();

    return true;
}

bool PngQuantRunner::compressImage(ImageTask& task)
{
    task.status = TaskStatus::Processing;

    // Validate input
    if (!validateInput(task)) {
        task.status = TaskStatus::Failed;
        return false;
    }

    emit progressUpdated(QString("正在压缩: %1").arg(QFileInfo(task.inputPath).fileName()));

    // Build pngquant arguments
    QStringList args;
    args << "--quality=65-80"
         << "--speed" << "3"
         << "--force"
         << "--skip-if-larger"
         << "--output" << task.outputPath
         << task.inputPath;

    // Run pngquant
    QProcess process;
    process.start(m_pngquantPath, args);

    // Wait for completion (MVP: synchronous is acceptable)
    if (!process.waitForFinished(30000)) { // 30 seconds timeout
        task.status = TaskStatus::Failed;
        task.errorMessage = "压缩超时";
        return false;
    }

    // Check result
    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        task.status = TaskStatus::Failed;
        QString errorOutput = QString::fromUtf8(process.readAllStandardError());

        // Handle specific error cases
        if (errorOutput.contains("larger than original")) {
            task.errorMessage = "压缩后文件更大(已跳过)";
        } else if (!QFile::exists(m_pngquantPath)) {
            task.errorMessage = "找不到pngquant程序";
        } else {
            task.errorMessage = QString("压缩失败(错误码: %1)").arg(process.exitCode());
        }

        qDebug() << "pngquant error:" << errorOutput;
        return false;
    }

    // Check if output file was created
    QFileInfo outputInfo(task.outputPath);
    if (!outputInfo.exists()) {
        task.status = TaskStatus::Failed;
        task.errorMessage = "输出文件未生成";
        return false;
    }

    // Success
    task.compressedSize = outputInfo.size();
    task.status = TaskStatus::Success;

    emit progressUpdated(QString("完成: %1").arg(outputInfo.fileName()));

    return true;
}
