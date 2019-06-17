
#include "placeholderutility.h"


void PlaceholderUtility::updateList() 
{
		try { 
			std::remove(repositoryListFile.toUtf8().constData());
	
			QProcess grabListProcess;
			QString grabList =  aria2cPath + "/aria2c.exe --allow-overwrite --out=list.json --dir=" + repositoryPath + " " + seedListURL;
			grabListProcess.start(grabList);
			grabListProcess.waitForFinished();
			grabListProcess.close();

		} catch(...) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("An error was encountered trying to update the Placeholder Artifact list for synchronization");
			msgBoxError.exec();
		}
	
}

QString PlaceholderUtility::getRepositoryListFile()
{
	return repositoryListFile;
}

QString PlaceholderUtility::getDeployEntryPointURL()
{
	return deployEntryPointURL;
}

bool PlaceholderUtility::isMachineConfiguredForVirtualBox()
{
		bool result = true;
		if( !exists(virtualBoxPath.toUtf8().constData()) ) {
			result = false;
		}
		else if( !exists((virtualBoxPath + virtualBoxManage).toUtf8().constData()) ) {
			result = false;
		}		
		return result;
}

inline bool PlaceholderUtility::exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

void PlaceholderUtility::seedRepository()
{
	std::string line;
	std::string cd;
    cd = repositoryListFile.toUtf8().constData();
	
	std::ifstream myfile (cd);
	if (myfile.is_open())
	{
		while ( std::getline (myfile,line) )
		{
			QProcess process;
			QString q = QString::fromLocal8Bit(line.c_str());		
			QString torrentFile =  aria2cPath + "/aria2c.exe --allow-overwrite --seed-ratio=1.0 --out=" + q + ".vdi --dir=" + vdiPath + " " + artifactSeedAnnounceURL + q;
			process.start(torrentFile);
			process.waitForFinished();
			process.close();	
		}
		myfile.close();
	}
	else {
		QMessageBox msgBoxError;
		msgBoxError.setText("Unable to begin seeding from repository file list.json in {repository}");
		msgBoxError.exec();
	}		
}

int PlaceholderUtility::getNumberArtifacts()
{
	int num = 0;
	std::string fcline;
	std::string cd;
    cd = repositoryListFile.toUtf8().constData();
	
    std::ifstream lineCountFile (cd);

	
    if(lineCountFile.is_open()){
        while(!lineCountFile.eof()){
            std::getline(lineCountFile,fcline);
            num++;
        }
        lineCountFile.close();
    }
	
	return num;
}

