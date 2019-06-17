
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
		QString repositoryListFile = "C:/repository/list.json";
		QString aria2cPath = "C:/Progra~2/Placeholders";
		QString mktorrentPath = "C:/Progra~2/Placeholders";
		
		QString deployEntryPointURL = "http://explore.placeh.io:8080/deploy";
		
	public:
	
		void updateList();
		void seedRepository();
		int getNumberArtifacts();
		QString getRepositoryListFile();
		QString getDeployEntryPointURL();
};
