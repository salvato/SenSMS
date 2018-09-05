// Taken from:
// http://grimaldi.univ-tln.fr/une-application-qt-android-qui-envoie-des-sms.html

#include "mainwindow.h"

#include <QtAndroidExtras/QAndroidJniObject> // For JNI, need to select Android Build
#include <QAndroidJniObject>
#include <QtAndroid>

#include <QMessageBox>
#include <QGridLayout>

/*
int
My_Procedure_Call_SendSMS(int n) {
    int Answer1;
    Answer1 =  QAndroidJniObject::callStaticMethod<jint>("org/qtproject/qt5/android/bindings/MyActivity","JAVA_Send_SMS","(I)I",n);
    return Answer1;
}


int
My_Procedure_Call_ReceiveSMS(int n) {
    int Answer2;
    Answer2 =  QAndroidJniObject::callStaticMethod<jint>("org/qtproject/qt5/android/bindings/MyActivity","JAVA_Receive_SMS","(I)I",n);
    return Answer2;
}
*/

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    pSendButton           = new QPushButton(tr("Send"));
    pLineEditDestinataire = new QLineEdit("+393498122332");
    pLineEditTexte        = new QLineEdit("Caspita Funziona !!!");

    connect(pSendButton, SIGNAL(clicked()),
            this, SLOT(on_pushButton_clicked()));

    QGridLayout* pLayout = new QGridLayout();
    pLayout->addWidget(pLineEditDestinataire, 0, 0, 1, 3);
    pLayout->addWidget(pLineEditTexte       , 1, 0, 1, 3);
    pLayout->addWidget(pSendButton          , 2, 1, 1, 1);
    setLayout(pLayout);
}


// Taken from https://bugreports.qt.io/browse/QTBUG-50759
bool
MainWindow::check_permission() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() <<
                                           "android.permission.SEND_SMS");
        r = QtAndroid::checkPermission("android.permission.SEND_SMS");
        if(r == QtAndroid::PermissionResult::Denied) {
            QMessageBox::critical(Q_NULLPTR, Q_FUNC_INFO,
                                     QString("No Permission to SEND_SMS"));
            return false;
        }
    }
    return true;
}


void
MainWindow::on_pushButton_clicked() {
    // get the Qt android activity
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",
                                                                            "activity",
                                                                            "()Landroid/app/Activity;");
    if (activity.isValid()){
        //get the default SmsManager
        QAndroidJniObject mySmsManager = QAndroidJniObject::callStaticObjectMethod("android/telephony/SmsManager",
                                                                                   "getDefault",
                                                                                   "()Landroid/telephony/SmsManager;");
        if (!mySmsManager.isValid()) {
            QMessageBox::critical(Q_NULLPTR, Q_FUNC_INFO,
                                     QString("SMS manager is Invalid"));
        }
        else {
            // get phone number & text from UI and convert to Java String
            QAndroidJniObject myPhoneNumber = QAndroidJniObject::fromString(pLineEditDestinataire->text());
            QAndroidJniObject myTextMessage = QAndroidJniObject::fromString(pLineEditTexte->text());
            QAndroidJniObject scAddress = nullptr;
            //QAndroidJniObject sentIntent = NULL;
            //QAndroidJniObject deliveryIntent = NULL;

            // call the java function:
            // public void SmsManager.sendTextMessage(String destinationAddress,
            //                                        String scAddress, String text,
            //                                        PendingIntent sentIntent, PendingIntent deliveryIntent)
            // see: http://developer.android.com/reference/android/telephony/SmsManager.html

            mySmsManager.callMethod<void>("sendTextMessage",
                                          "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Landroid/app/PendingIntent;Landroid/app/PendingIntent;)V",
                                          myPhoneNumber.object<jstring>(),
                                          scAddress.object<jstring>(),
                                          myTextMessage.object<jstring>(), NULL, NULL );
        }

    }
    else {
        QMessageBox::critical(Q_NULLPTR, Q_FUNC_INFO,
                                 QString("Something wrong with Qt activity..."));
    }
}
