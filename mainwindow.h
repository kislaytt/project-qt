#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "facedetectionthread.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //Q_PROPERTY(bool mIsCameraOn READ getMIsCameraOn WRITE setMIsCameraOn NOTIFY mIsCameraOnChanged);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public Q_SLOTS:
    void onFaceDetectionThreadFinished();
    void onFrameCaptured(cv::Mat frame);

private Q_SLOTS:
    void onPushButtonClicked();

    void on_cameraOnOffBut_clicked();

    void on_cameraOnOffBut_released();

    void on_detectEmotion_clicked();

    void on_selectImage_clicked();

    void on_detectFace_clicked();



private:
    Ui::MainWindow *ui;
    cv::VideoCapture videoCapture;
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    cv::CascadeClassifier noseCascade;
    cv::CascadeClassifier mouthCascade;
    cv::Mat faceROI;
    cv::Mat image;
    QString filePath;
    bool mIsCameraOn = false;
    FaceDetectionThread* mFaceDetectionThread = nullptr;
    void detectAndDrawFaces();
    void loadImage(bool detectFace = false);
    QImage putImage(const cv::Mat &mat);
    QString analyzeEmotion(const cv::Mat &frame);
};
#endif // MAINWINDOW_H
