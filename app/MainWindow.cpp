#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include <QStyle>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_runner(new PngQuantRunner(this))
{
    setupUI();

    // Connect signals
    connect(m_selectButton, &QPushButton::clicked, this, &MainWindow::onSelectFilesClicked);
    connect(m_compressButton, &QPushButton::clicked, this, &MainWindow::onCompressClicked);
    connect(m_runner, &PngQuantRunner::progressUpdated, this, &MainWindow::onProgressUpdated);

    // Enable drag and drop
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("Shrinkly - PNG压缩工具");
    setMinimumSize(600, 500);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Drop zone area
    QWidget* dropZone = new QWidget();
    dropZone->setMinimumHeight(120);
    dropZone->setStyleSheet(
        "QWidget {"
        "    background-color: #f0f0f0;"
        "    border: 2px dashed #999;"
        "    border-radius: 8px;"
        "}"
    );

    QVBoxLayout* dropLayout = new QVBoxLayout(dropZone);
    dropLayout->setAlignment(Qt::AlignCenter);

    m_dropZoneLabel = new QLabel("拖拽 PNG 图片到这里");
    m_dropZoneLabel->setAlignment(Qt::AlignCenter);
    m_dropZoneLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 16px;"
        "    color: #666;"
        "    border: none;"
        "}"
    );
    dropLayout->addWidget(m_dropZoneLabel);

    QLabel* orLabel = new QLabel("或");
    orLabel->setAlignment(Qt::AlignCenter);
    orLabel->setStyleSheet("border: none; color: #999;");
    dropLayout->addWidget(orLabel);

    m_selectButton = new QPushButton("点击选择文件");
    m_selectButton->setMinimumHeight(36);
    m_selectButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    padding: 8px 24px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
    );
    dropLayout->addWidget(m_selectButton, 0, Qt::AlignCenter);

    mainLayout->addWidget(dropZone);

    // File list
    QLabel* fileListLabel = new QLabel("文件列表:");
    fileListLabel->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(fileListLabel);

    m_fileListWidget = new QListWidget();
    m_fileListWidget->setMinimumHeight(200);
    m_fileListWidget->setStyleSheet(
        "QListWidget {"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    padding: 4px;"
        "}"
    );
    mainLayout->addWidget(m_fileListWidget);

    // Status label
    m_statusLabel = new QLabel("就绪");
    m_statusLabel->setStyleSheet(
        "QLabel {"
        "    padding: 8px;"
        "    background-color: #e8f5e9;"
        "    border-radius: 4px;"
        "    color: #2e7d32;"
        "}"
    );
    mainLayout->addWidget(m_statusLabel);

    // Compress button
    m_compressButton = new QPushButton("开始压缩");
    m_compressButton->setMinimumHeight(40);
    m_compressButton->setEnabled(false);
    m_compressButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover:enabled {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed:enabled {"
        "    background-color: #0D47A1;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #ccc;"
        "    color: #666;"
        "}"
    );
    mainLayout->addWidget(m_compressButton);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    // Accept drag if it contains URLs (files)
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();

        // Visual feedback
        m_dropZoneLabel->setText("松开以添加文件");
        m_dropZoneLabel->parentWidget()->setStyleSheet(
            "QWidget {"
            "    background-color: #e3f2fd;"
            "    border: 2px dashed #2196F3;"
            "    border-radius: 8px;"
            "}"
        );
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    // Reset visual feedback
    m_dropZoneLabel->setText("拖拽 PNG 图片到这里");
    m_dropZoneLabel->parentWidget()->setStyleSheet(
        "QWidget {"
        "    background-color: #f0f0f0;"
        "    border: 2px dashed #999;"
        "    border-radius: 8px;"
        "}"
    );

    QStringList filePaths;
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        for (const QUrl& url : urls) {
            QString filePath = url.toLocalFile();
            if (filePath.endsWith(".png", Qt::CaseInsensitive)) {
                filePaths.append(filePath);
            }
        }

        if (!filePaths.isEmpty()) {
            addFiles(filePaths);
        } else {
            QMessageBox::warning(this, "提示", "请拖拽 PNG 格式的图片文件");
        }
    }

    event->acceptProposedAction();
}

void MainWindow::onSelectFilesClicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "选择 PNG 图片",
        QString(),
        "PNG 图片 (*.png)"
    );

    if (!filePaths.isEmpty()) {
        addFiles(filePaths);
    }
}

void MainWindow::addFiles(const QStringList& filePaths)
{
    for (const QString& path : filePaths) {
        // Check if file already exists in task list
        bool exists = false;
        for (const ImageTask& task : m_tasks) {
            if (task.inputPath == path) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            m_tasks.append(ImageTask(path));
        }
    }

    updateFileList();
    m_compressButton->setEnabled(!m_tasks.isEmpty());
}

void MainWindow::updateFileList()
{
    m_fileListWidget->clear();

    for (const ImageTask& task : m_tasks) {
        QFileInfo fileInfo(task.inputPath);
        QString displayText = QString("%1     %2")
            .arg(fileInfo.fileName())
            .arg(task.getStatusText());

        QListWidgetItem* item = new QListWidgetItem(displayText);

        // Set color based on status
        switch (task.status) {
            case TaskStatus::Success:
                item->setForeground(QColor("#2e7d32"));
                break;
            case TaskStatus::Failed:
                item->setForeground(QColor("#c62828"));
                break;
            case TaskStatus::Processing:
                item->setForeground(QColor("#1976d2"));
                break;
            default:
                item->setForeground(QColor("#666"));
                break;
        }

        m_fileListWidget->addItem(item);
    }
}

void MainWindow::onCompressClicked()
{
    if (m_tasks.isEmpty()) {
        return;
    }

    m_compressButton->setEnabled(false);
    m_selectButton->setEnabled(false);

    int successCount = 0;
    int failCount = 0;
    qint64 totalSaved = 0;

    for (int i = 0; i < m_tasks.size(); ++i) {
        ImageTask& task = m_tasks[i];

        // Update UI
        updateFileList();
        QApplication::processEvents();

        // Compress
        bool success = m_runner->compressImage(task);

        if (success) {
            successCount++;
            totalSaved += (task.originalSize - task.compressedSize);
        } else {
            failCount++;
        }

        // Update UI
        updateFileList();
        QApplication::processEvents();
    }

    // Update summary
    QString summary = QString("完成! 成功: %1, 失败: %2")
        .arg(successCount)
        .arg(failCount);

    if (totalSaved > 0) {
        summary += QString(", 节省空间: %1").arg(ImageTask::formatSize(totalSaved));
    }

    m_statusLabel->setText(summary);

    if (failCount > 0) {
        m_statusLabel->setStyleSheet(
            "QLabel {"
            "    padding: 8px;"
            "    background-color: #fff3e0;"
            "    border-radius: 4px;"
            "    color: #e65100;"
            "}"
        );
    } else {
        m_statusLabel->setStyleSheet(
            "QLabel {"
            "    padding: 8px;"
            "    background-color: #e8f5e9;"
            "    border-radius: 4px;"
            "    color: #2e7d32;"
            "}"
        );
    }

    m_compressButton->setEnabled(true);
    m_selectButton->setEnabled(true);
}

void MainWindow::onProgressUpdated(const QString& message)
{
    m_statusLabel->setText(message);
    QApplication::processEvents();
}

void MainWindow::updateSummary()
{
    int total = m_tasks.size();
    int completed = 0;
    int success = 0;
    int failed = 0;

    for (const ImageTask& task : m_tasks) {
        if (task.status == TaskStatus::Success) {
            completed++;
            success++;
        } else if (task.status == TaskStatus::Failed) {
            completed++;
            failed++;
        }
    }

    QString summary = QString("总计: %1 | 完成: %2 | 成功: %3 | 失败: %4")
        .arg(total)
        .arg(completed)
        .arg(success)
        .arg(failed);

    m_statusLabel->setText(summary);
}
