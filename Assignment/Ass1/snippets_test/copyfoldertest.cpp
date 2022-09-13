#include<stdio.h>
#include<bits/stdc++.h>
#include<sys/wait.h>
#include<pwd.h>
#include<grp.h>
#include<limits.h>
#include<dirent.h>
#include<stdlib.h>
#include<iostream>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<vector>
#include<termios.h>
#include<stack>
#include<algorithm>
#include<ctype.h> 
#include<errno.h>
#include<time.h>
#include<fcntl.h>
#include<fstream>
#include <sys/fcntl.h>
// #include <bsd/libutil.h>
using namespace std;

string gethomepath(){
	string HOMEEEE;
	uid_t userid=getuid();
	struct passwd *pw=getpwuid(userid);
	HOMEEEE=pw->pw_dir;
	return HOMEEEE;
}

string resolvepath(string path){
	string temptildehandle=path;
	if(temptildehandle[0]=='~'){
		string tilde=gethomepath();
		temptildehandle.erase(0,1);
		temptildehandle=tilde+temptildehandle;
		return temptildehandle;
	}
	else{
		char * fullpath;
		fullpath=realpath(path.c_str(),NULL);
		if(fullpath==NULL) {
			cout<<"INVALID PATH !!"<<endl;
			return "0";
		}
		else{
			string strpath(fullpath);
			return strpath;
		}
	}
}


string currentdirectory(){
	char tmp[512];
    getcwd(tmp,512);
    string maindirectory=tmp;
	return maindirectory;
}

string maindirectory=currentdirectory();

void createfile(string pathfile){
	// ofstream ifile;
	// ifile.open(pathfile);
	int fd=open(pathfile.c_str(),O_RDWR|O_CREAT,S_IRWXU | S_IRWXG | S_IRWXO);
	// chmod(pathfile.c_str());
}


void createfolder(string pathfolder){
	const char *temppathfolder=pathfolder.c_str();
	mkdir(temppathfolder,0777);
}

void copyfile(const char *SRC, const char* DEST)
{
	// cout<<"INSIDEEEEEEEEEEE"<<endl;
	// cout<<"src----->>> "<<SRC<<endl;
	// cout<<"---><------ "<<DEST<<endl;
    ifstream src(SRC, ios::binary);
    ofstream dest(DEST, ios::binary);
    dest << src.rdbuf();
}

void first_copyfile(string custompath,string s1,string s2){
	string absolutepath1=custompath+"/"+s1;
	createfile(absolutepath1);
	string absolutepath2=resolvepath(s2);
	absolutepath2=absolutepath2+"/"+s1;
	copyfile(absolutepath1.c_str(),absolutepath2.c_str());

	struct stat st2;
	stat(absolutepath2.c_str(),&st2);
	chmod(absolutepath2.c_str(),st2.st_mode);

}


void copyfolder(string curpath,string name,string cfpath){
	string cfpathupdated=cfpath+"/"+name;
	// cout<<"cfpath-------------"<<cfpathupdated<<endl;
	createfolder(cfpathupdated);
	string curpathupdated=curpath+"/"+name;
	const char* temp=curpathupdated.c_str();
	int iserror=chdir(temp);
	// cout<<"iserror"<<iserror<<endl;
	if(iserror==-1){			//error occured			
		return;
	}
	char tmp2[512];
	getcwd(tmp2,512);

	struct dirent *entity;

	DIR *dh2=opendir(curpathupdated.c_str());

	mode_t filemode;

	struct stat searchstat;
	
	entity=readdir(dh2);

	while(entity!=NULL){
		if(entity->d_name[0]=='.'){
			entity=readdir(dh2);
		}
		// else if(entity->d_name==keyword){
		// 	cout<<"Found at path:"<<path<<endl;
		// 	return 1;
		// }
		else{
			stat(entity->d_name,&searchstat);

			if(entity->d_type==DT_DIR && searchstat.st_mode){
				string searchindirectory=curpathupdated;
				// searchindirectory.append("/");
				// searchindirectory.append(entity->d_name);
				copyfolder(searchindirectory,entity->d_name,cfpathupdated);
			}
			else{
				createfile(cfpathupdated+"/"+entity->d_name);
				first_copyfile(curpathupdated.c_str(),entity->d_name,cfpathupdated.c_str());

			}
			entity=readdir(dh2);
		}
	}
	return;
}



int main(){
	copyfolder("/home/piyush_singh/Desktop","DSAP","/home/piyush_singh/Downloads");
	// createfolder("/home/piyush_singh/Desktop/AOS/bharosa");
	// move();
	string s1="AOS_Assignment_1.pdf";
	string s2="/home/piyush_singh/Desktop/AOS";
	// first_copyfile("/home/piyush_singh/Downloads",s1,s2);

	// cout<<maindirectory<<endl;
	return 0;
}
