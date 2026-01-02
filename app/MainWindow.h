#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include "../core/ImageTask.h"
#include "../core/PngQuantRunner.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    // Enable drag and drop
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private slots:
    void onSelectFilesClicked();
    void onCompressClicked();
    void onProgressUpdated(const QString& message);

private:
    // UI components
    QWidget* m_centralWidget;
    QLabel* m_dropZoneLabel;
    QPushButton* m_selectButton;
    QListWidget* m_fileListWidget;
    QLabel* m_statusLabel;
    QPushButton* m_compressButton;

    // Data
    QVector<ImageTask> m_tasks;
    PngQuantRunner* m_runner;

    // Helper methods
    void setupUI();
    void addFiles(const QStringList& filePaths);
    void updateFileList();
    void updateSummary();
};

#endif // MAINWINDOW_H
