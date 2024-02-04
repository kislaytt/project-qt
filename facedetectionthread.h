#ifndef FACEDETECTIONTHREAD_H
#define FACEDETECTIONTHREAD_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class FaceDetectionThread : public QThread
{
    Q_OBJECT

Q_SIGNALS:
    void frameCaptured(cv::Mat frame);

public:
    FaceDetectionThread(QObject *parent = nullptr);
    void run() override;
private:
    cv::CascadeClassifier faceCascade;
};

#endif // FACEDETECTIONTHREAD_H
