
#if defined(HAVE_CONFIG_H)
#include "config/placeh-config.h"
#endif

#include <fstream>
#include <iostream>
#include <QProcess>
#include <QString>
#include <QMessageBox>

class PlaceholderUtility {

	private:
	
		QString artifactSeedAnnounceURL = "http://explore.placeh.io:8080/raw/";
		QString seedListURL = "http://explore.placeh.io:8080/repository/list.json";
		QString repositoryPath = "C:/repository";
		QString repositoryListFile = "C:/repository/list.json";
		QString aria2cPath = "C:/Progra~2/Placeholders";
		QString mktorrentPath = "C:/Progra~2/Placeholders";
		QString vdiPath = "C:/vdi";
		QString virtualBoxManage = "VBoxManage.exe";
		QString virtualBoxPath = "C:/Progra~1/Oracle/VirtualBox/";
		
		QString deployEntryPointURL = "http://explore.placeh.io:8080/deploy";
		bool exists(const std::string& name);
		
	public:
	
		bool isMachineConfiguredForVirtualBox();
		void updateList();
		void seedRepository();
		int getNumberArtifacts();
		QString getRepositoryListFile();
		QString getDeployEntryPointURL();
};
