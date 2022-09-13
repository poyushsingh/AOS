gio: file:///home/piyush_singh/Desktop/SSD/webpage/cursor.cpp: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/cursor.cpp”: No such file or directory
.h>
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

void gotoxy(int x,int y){
	printf("%c[%d;%dH",27,x,y);
}


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



// int main(){
// 	gotoxy(0,0);
// 	cout<<"hello";
// 	while(1){
// 		gotoxy(1,1);
// 		cout<<"hell";

// 	}
// 	return 0;
// }


int main(){
	string path="./DSAP";
	cout<<resolvepath(path.c_str());

}
































bool search(string keyword,const char* path){
	int iserror=chdir(path);

	if(iserror==-1){			//error occured
			
		return 0;
	}
	char tmp2[512];
	getcwd(tmp2,512);

	struct dirent *entity;

	DIR *dh2=opendir(path);

	mode_t filemode;

	struct stat searchstat;
	
	entity=readdir(dh2);

	while(entity!=NULL){
		if(entity->d_name[0]=='.'){
			entity=readdir(dh2);
		}
		else if(entity->d_name==keyword){
			cout<<"Found at path:"<<path<<endl;
			return 1;
		}
		else{
			stat(entity->d_name,&searchstat);

			if(entity->d_type==DT_DIR && searchstat.st_mode){
				string searchindirectory=tmp2;
				searchindirectory.append("/");
				searchindirectory.append(entity->d_name);
				if(search(keyword,searchindirectory.c_str())){
					return 1;
				}

			}
			entity=readdir(dh2);
		}
	}
	return 0;

}