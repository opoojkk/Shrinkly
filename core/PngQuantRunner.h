#ifndef PNGQUANTRUNNER_H
#define PNGQUANTRUNNER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include "ImageTask.h"

class PngQuantRunner : public QObject {
    Q_OBJECT

public:
    explicit PngQuantRunner(QObject* parent = nullptr);

    // Compress a single image
    bool compressImage(ImageTask& task);

    // Get pngquant executable path
    static QString getPngQuantPath();

signals:
    void progressUpdated(const QString& message);

private:
    QString m_pngquantPath;

    // Validate input file
    bool validateInput(ImageTask& task);
};

#endif // PNGQUANTRUNNER_H
