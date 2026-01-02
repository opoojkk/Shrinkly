#ifndef IMAGETASK_H
#define IMAGETASK_H

#include <QString>

enum class TaskStatus {
    Pending,
    Processing,
    Success,
    Failed
};

struct ImageTask {
    QString inputPath;
    QString outputPath;
    TaskStatus status;
    qint64 originalSize;
    qint64 compressedSize;
    QString errorMessage;

    ImageTask(const QString& path)
        : inputPath(path)
        , status(TaskStatus::Pending)
        , originalSize(0)
        , compressedSize(0)
    {
        // Generate output path: xxx.png -> xxx-min.png
        if (path.endsWith(".png", Qt::CaseInsensitive)) {
            outputPath = path;
            outputPath.replace(outputPath.length() - 4, 4, "-min.png");
        }
    }

    QString getStatusText() const {
        switch (status) {
            case TaskStatus::Pending:
                return "等待中";
            case TaskStatus::Processing:
                return "压缩中...";
            case TaskStatus::Success:
                return QString("%1 → %2")
                    .arg(formatSize(originalSize))
                    .arg(formatSize(compressedSize));
            case TaskStatus::Failed:
                return QString("失败: %1").arg(errorMessage);
        }
        return "";
    }

    static QString formatSize(qint64 bytes) {
        if (bytes < 1024) {
            return QString("%1 B").arg(bytes);
        } else if (bytes < 1024 * 1024) {
            return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
        } else {
            return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 2);
        }
    }
};

#endif // IMAGETASK_H
