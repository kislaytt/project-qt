#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    faceCascade.load(cv::samples::findFile("haarcascades/haarcascade_frontalface_default.xml"));
    mFaceDetectionThread = new FaceDetectionThread(this);
    // Add this line to the constructor or setup function
    connect(mFaceDetectionThread, &FaceDetectionThread::finished, this, &MainWindow::onFaceDetectionThreadFinished);
    connect(mFaceDetectionThread, &FaceDetectionThread::frameCaptured, this, &MainWindow::onFrameCaptured);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPushButtonClicked()
{
    if (mIsCameraOn)
    {
        if (mFaceDetectionThread->isRunning())
        {
            mFaceDetectionThread->terminate();
            mFaceDetectionThread->wait();
            mIsCameraOn = false;
            qDebug() << "Camera stopped.";
        }
    }
    else
    {
        if (!mFaceDetectionThread->isRunning())
        {
            mFaceDetectionThread->start();
            mIsCameraOn = true;
            qDebug() << "Camera started.";
        }
    }
}
void MainWindow::on_cameraOnOffBut_clicked()
{
    onPushButtonClicked();
}


void MainWindow::on_cameraOnOffBut_released()
{
    videoCapture.release();
}



void MainWindow::on_detectEmotion_clicked()
{
    std::vector<cv::Rect> mouths;
    mouthCascade.detectMultiScale(faceROI, mouths, 1.8, 20, 0, cv::Size(20, 20));

    // Check if a mouth is detected
    if (!mouths.empty()) {
        // Assume the person is happy if a mouth is detected
        QString emotion = "<p style=\"color:orange\">Happy</p>";

        // Display the emotion label on the GUI
        ui->emotionLabel->setText(emotion);
    } else {
        // No mouth detected, assume neutral emotion
        QString emotion = "<p style=\"color:red\">Neutral</p>";

        // Display the emotion label on the GUI
        ui->emotionLabel->setText(emotion);
    }
}

QImage MainWindow::putImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qCritical() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
void MainWindow::loadImage(bool detectFace)
{
    if (!filePath.isEmpty())
    {
        // Load the selected image
        cv::Mat originalImage = cv::imread(filePath.toStdString());

        if (!originalImage.empty())
        {
            // Resize the image to fit within the QLabel while maintaining aspect ratio
            if (filePath.compare("temp.png") != 0)
            {
                QSize labelSize = ui->cameraLabel->size();
                double aspectRatio = static_cast<double>(originalImage.cols) / originalImage.rows;
                int newWidth = static_cast<int>(labelSize.height() * aspectRatio);
                cv::resize(originalImage, image, cv::Size(newWidth, labelSize.height()));
            }
            else
            {
                image = originalImage;
            }

            if (detectFace)
            {
                detectAndDrawFaces();
            }

            // Display the processed image in the QLabel widget
            ui->cameraLabel->setPixmap(QPixmap::fromImage(putImage(image)));
        }
        else
        {
            qCritical() << "Error: Failed to load the image.";
        }
    }
}
void MainWindow::on_selectImage_clicked()
{
    // Open a file dialog to select an image
    filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));
    loadImage();
    ui->emotionLabel->setText("");
}

void MainWindow::detectAndDrawFaces()
{
    // Convert the image to grayscale for face detection
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayImage, grayImage);

    // Detect faces in the image
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(grayImage, faces, 1.1, 3, 0, cv::Size(30, 30));

    // Draw rectangles around detected faces
    for (const cv::Rect &faceRect : faces)
    {
        cv::rectangle(image, faceRect, cv::Scalar(255, 0, 0), 2);
        faceROI = image;
    }
}


void MainWindow::on_detectFace_clicked()
{
    ui->emotionLabel->setText("");
    loadImage(true);
}


// Add this slot to the MainWindow class implementation
void MainWindow::onFaceDetectionThreadFinished()
{
    ui->cameraLabel->clear();
    ui->emotionLabel->setText("");
    qInfo() << "Face detection thread finished.";
}

// Show all the frames captured from the webcam
void MainWindow::onFrameCaptured(cv::Mat frame)
{
    image = frame;
    // Calling function to detect the face
    detectAndDrawFaces();
    // Showing the frames on the main window
    cv::Mat rgbMat;
    cv::cvtColor(frame, rgbMat, cv::COLOR_BGR2RGB);
    QImage img(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888);
    ui->cameraLabel->setPixmap(QPixmap::fromImage(img));
}
