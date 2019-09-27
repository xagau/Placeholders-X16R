
#if defined(HAVE_CONFIG_H)
#include "config/placeh-config.h"
#endif

#include <fstream>
#include <iostream>
#include <QProcess>
#include <QString>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <QDebug>
#include <QApplication>

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkInterface>

class PlaceholderUtility {

	private:
	
		// defaults
		QString launchCmd = "CMD /C ";
		QString javaName = "java.exe";
		QString aria2cName = "aria2c.exe";
		QString pathSeperator = "/";
		
		QString artifactDetailURL = "http://explore.placeh.io:8080/details/";
		QString artifactSeedAnnounceURL = "http://explore.placeh.io:8080/raw/";
		QString seedListURL = "http://explore.placeh.io:8080/repository/";
		QString artifactJson = "http://explore.placeh.io:8080/artifact/";
		QString repositoryPath = "C:/repository";
		QString repositoryListFile = "C:/repository/list.json";
		QString aria2cPath = "C:/Progra~2/Placeholders";
		QString mktorrentPath = "C:/Progra~2/Placeholders";
		QString placeholderPath = "C:/Progra~2/Placeholders";
		QString paymentAddress = "F9ddp3zXbNd5zwxBngvRBe4zQgmoViNuma";
		
		QString vdiPath = "C:/vdi";
		QString virtualBoxManage = "VBoxManage.exe";
		QString virtualBoxPath = "C:/Progra~1/Oracle/VirtualBox/";
		QString provideServiceEndPointURL = "https://www.placeh.io/ProvideServiceServlet";
		QString registerServiceEndPointURL = "https://www.placeh.io/RegisterServiceServlet";
		QString deployEntryPointURL = "http://explore.placeh.io:8080/deploy";
		QString earn = "true";
		
		bool exists(const std::string& name);
		
		QString userid = "nobody@placeh.io"; // portal userid
		QString password = "none"; // portal password
		
		
		
		
	public:
	
		PlaceholderUtility();
		
		QString getProvideServiceEndPointURL();
		QJsonObject objectFromString(const QString& in);
		
		void consume(QString artifact);
		void download(QString artifact);
		QString getExtensionByContentType(QString str);
		QString getContentTypeByExtension(QString fileName);
		bool isMachineConfiguredForVirtualBox();
		void updateList();
		void updateList(QString criteria);
		void seed(QString artifact);
		void seedRepository();
		int getNumberArtifacts();
		QString getRegisterServiceEndPointURL();
		QString getPlaceholderPath();
		QString getRepositoryListFile();
		QString getArtifactDetailURL();
		QString getArtifactJson();
		QString getDeployEntryPointURL();
		QString getVDIPath();
		QString getMacAddress();
		QString getPaymentAddress();
		QString getEarn();
		
		QString getUserID();
		QString getPassword();
		
};
