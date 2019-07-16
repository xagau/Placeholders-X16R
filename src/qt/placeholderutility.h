
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

class PlaceholderUtility {

	private:
	
		// defaults
		
		QString artifactDetailURL = "http://explore.placeh.io:8080/details/";
		QString artifactSeedAnnounceURL = "http://explore.placeh.io:8080/raw/";
		QString seedListURL = "http://explore.placeh.io:8080/repository/list.json";
		QString repositoryPath = "C:/repository";
		QString repositoryListFile = "C:/repository/list.json";
		QString aria2cPath = "C:/Progra~2/Placeholders";
		QString mktorrentPath = "C:/Progra~2/Placeholders";
		QString placeholderPath = "C:/Progra~2/Placeholders";
		
		QString vdiPath = "C:/vdi";
		QString virtualBoxManage = "VBoxManage.exe";
		QString virtualBoxPath = "C:/Progra~1/Oracle/VirtualBox/";
		QString provideServiceEndPointURL = "https://www.placeh.io/ProvideServiceServlet";
		QString deployEntryPointURL = "http://explore.placeh.io:8080/deploy";
		bool exists(const std::string& name);
		
		QString userid = "nobody@placeh.io"; // portal userid
		QString password = "none"; // portal password
		
		
		
		
	public:
	
		PlaceholderUtility() ;
		
		QString getProvideServiceEndPointURL();
		QJsonObject objectFromString(const QString& in);
		
		void download(QString artifact);
		QString getExtensionByContentType(QString str);
		QString getContentTypeByExtension(QString fileName);
		bool isMachineConfiguredForVirtualBox();
		void updateList();
		void seed(QString artifact);
		void seedRepository();
		int getNumberArtifacts();
		QString getPlaceholderPath();
		QString getRepositoryListFile();
		QString getArtifactDetailURL();
		QString getDeployEntryPointURL();
		QString getVDIPath();
		
		QString getUserID();
		QString getPassword();
		
};
