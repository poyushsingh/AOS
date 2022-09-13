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

void gotoxy(int x,int y){
	printf("%c[%d;%dH",27,x,y);
}

string currentdirectory(){
	char tmp[512];
    getcwd(tmp,512);
    string maindirectory=tmp;
	return maindirectory;
}

string maindirectory=currentdirectory();

string gethomepath()
{
    string h_path;
    char *homedir = getenv("HOME");
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    h_path=pw->pw_dir;
    return h_path;
}

// int main(){
// struct winsize windowsize;
// ioctl(STDIN_FILENO, TIOCGWINSZ, &windowsize);

// int l=windowsize.ws_row;
// int c=windowsize.ws_col;
// // l=l-1;
// // c=c-1;
// gotoxy(l,c);



	// string str="piyushsingh";
	// int i=0;
	// while(i!=12){
	// 	str.pop_back();
	// 	cout<<str<<endl;
	// 	i++;
	// }


	// cout<<gethomepath()<<endl;


// #include <iostream>
// #include <string.h>
// using namespace std;

// string pathoffoundelement="";

// string currentdirectory(){
// 	char tmp[512];
//     getcwd(tmp,512);
//     string maindirectory=tmp;
// 	return maindirectory;
// }


string resolvepath(string path){
	string temptildehandle=path;
	if(temptildehandle[0]=='~'){
		string tilde=gethomepath();
		temptildehandle.erase(0,1);
		temptildehandle=tilde+temptildehandle;
		return temptildehandle;
	}
	else if(temptildehandle[0]=='.'){
		if(temptildehandle[1]=='.'){		//.. wala case
			temptildehandle.erase(0,2);
			temptildehandle=maindirectory+temptildehandle;
			return temptildehandle;

		}
		else{								//. wala case
			temptildehandle.erase(0,1);
			temptildehandle=maindirectory+temptildehandle;
			return temptildehandle;
		}
	}
	else{
		return path;
	}

}

// bool search(string keyword,const char* path){
// 	int iserror=chdir(path);

// 	if(iserror==-1){
// 		//error occured
// 		return 0;
// 	}
// 	char tmp2[512];
// 	getcwd(tmp2,512);

// 	struct dirent *entity;

// 	DIR *dh2=opendir(path);

// 	mode_t filemode;

// 	struct stat searchstat;
	
// 	entity=readdir(dh2);

// 	while(entity!=NULL){
// 		if(entity->d_name[0]=='.'){
// 			entity=readdir(dh2);
// 		}
// 		else if(entity->d_name==keyword){
// 			pathoffoundelement=path;
// 			cout<<"Found at path:"<<path<<endl;
// 			return 1;
// 		}
// 		else{
// 			stat(entity->d_name,&searchstat);

// 			if(entity->d_type==DT_DIR && searchstat.st_mode){
// 				string searchindirectory=tmp2;
// 				searchindirectory.append("/");
// 				searchindirectory.append(entity->d_name);
// 				if(search(keyword,searchindirectory.c_str())){
// 					return 1;
// 				}

// 			}
// 			entity=readdir(dh2);
// 		}
// 	}
// 	return 0;

// }







// void createfile(string pathfile){
// 	ofstream ifile;
// 	// pathfile+="/";
// 	// pathfile+="newfile.txt";
// 	ifile.open(pathfile);
// }

// void createfolder(string pathfolder){
// 	const char * temp=pathfolder.c_str();
// 	mkdir(temp,0777);

// }

// void copyfile(string src,string dst){
// 	ifstream sourcepath;
// 	ofstream destinationpath;
// 	sourcepath.open(src,ios::in|ios::binary);
// 	destinationpath.open(dst,ios::out|ios::binary);
// 	destinationpath<<sourcepath.rdbuf();
// 	sourcepath.close();
// 	destinationpath.close();

// }


// void rename123(string src,string dst){
// 	string curr=currentdirectory();
// 	if(!search(src,curr.c_str())){
// 		cout<<"pathoffoundelementinyesssssssssss="<<pathoffoundelement<<endl;
// 		cout<<"FILE NOT EXISTS !!!";
// 	}
// 	else{
// 		cout<<"pathoffoundelement"<<pathoffoundelement<<endl;
// 		pathoffoundelement+="/";
// 		string pathoffoundelementfordst=pathoffoundelement;
// 		pathoffoundelementfordst+=dst;
// 		pathoffoundelement+=src;
// 		rename(pathoffoundelement.c_str(),pathoffoundelementfordst.c_str());
// 	}


// }





// void check(string dst){
// 	int i=dst.size()-1;
// 	while(dst[i]!='/'){
// 		i--;
// 	}
// 	string dstwithoutfoldername="";
// 	for(int j=0;j<i;j++){
// 		dstwithoutfoldername+=dst[j];
// 	}

// 	string foldername="";
// 	for(int k=i+1;k<dst.size();k++){
// 		foldername+=dst[k];
// 	}

// 	cout<<"dstwithoutfoldername="<<dstwithoutfoldername<<endl;
// 	cout<<"foldername"<<foldername<<endl;

// }


void copyfolder(string dirname,const char * destpath){
	chdir(destpath);
	char temp3[512];
	getcwd(temp3,512);
	struct dirent *entity;
	DIR *dh2=opendir(destpath);
	mode_t filemode;
	struct stat searchstat;
    entity=readdir(dh2);

    while(entity!=NULL){
    	if(entity->d_name[0]=='.'){
    		entity=readdir(dh2);
    	}
    	else{
    		stat(entity->d_name,&searchstat);
            if(entity->d_type==DT_DIR){
                string searchindirectory=tmp2;
                searchindirectory.append("/");
                searchindirectory.append(entity->d_name);
                if(copyfolder(dirname,searchindirectory.c_str())){
                    return ;
                }

            }
            else{
            	
            }

    	}
    }

}













	    




















// void copyfolder(string src,string dst){

// 	int i=dst.size()-1;
// 	while(dst[i]!='/'){
// 		i--;
// 	}
// 	string dstwithoutfoldername="";
// 	for(int j=0;j<i;j++){
// 		dstwithoutfoldername+=dst[j];
// 	}

// 	string foldername="";
// 	for(int k=i+1;k<dst.size();k++){
// 		foldername+=dst[k];
// 	}
	
// 	struct stat s;
// 	if(stat(dstwithoutfoldername.c_str(),&s)==0){
// 		createfolder(dst);
// 		// int iserror=chdir(dstwithoutfoldername);

// 		// if(iserror==-1){
// 		// 	//error occured
// 		// 	return ;
// 		// }

// 		// char tmp2[512];
// 		// getcwd(tmp2,512);

// 		struct dirent *entity;

// 		DIR *dh2=opendir(dstwithoutfoldername.c_str());

// 		mode_t filemode;

// 		struct stat searchstat;
		
// 		entity=readdir(dh2);

// 		while(entity!=NULL){
// 			if(entity->d_name[0]=='.'){
// 				entity=readdir(dh2);
// 			}
// 			// else if(entity->d_name==keyword){
// 			// 	cout<<"Found at path:"<<path<<endl;
// 			// 	return 1;
// 			// }
// 			else{
// 				stat(entity->d_name,&searchstat);

// 				if(entity->d_type==DT_DIR && searchstat.st_mode){
// 					string searchindirectory=dstwithoutfoldername;
// 					searchindirectory.append("/");
// 					searchindirectory.append(entity->d_name);
// 					copyfolder(src,searchindirectory);
// 				}
// 				else{
// 					createfile(dstwithoutfoldername);
// 					entity=readdir(dh2);
// 				}
// 			}
// 		}
// 	}
// 	return ;

// }




void createfile(string pathfile){
	ofstream ifile;
	ifile.open(pathfile);
}

void createfolder(string pathfolder){
	const char *temppathfolder=pathfolder.c_str();
	mkdir(temppathfolder,0777);
}

void copyfile(string src,string dst){
	ifstream sourcepath;
	ofstream destinationpath;
	sourcepath.open(src,ios::in|ios::binary);
	destinationpath.open(dst,ios::out|ios::binary);
	destinationpath<<sourcepath.rdbuf();
	sourcepath.close();
	destinationpath.close();
}



int main()
{

	
	vector<string> fullcommands;
	fullcommands[0]="copy_file";
	fullcommands[1]="git.txt";
	fullcommands[2]="/home/piyush_singh/Desktop/AOS/Assignment";


	if(fullcommands[0]=="copy_file"){
	string absolutepath1=maindirectory+"/"+fullcommands[1];
	createfile(absolutepath1);
	string absolutepath2=resolvepath(fullcommands[2]);
	copyfile(absolutepath1,absolutepath2);
	}

	



    // createfile("/home/piyush_singh/renametest1234567.txt");
    // createfolder("/home/piyush_singh/Desktop/DSAP/abc/uio");
    // copyfile("/home/piyush_singh/Downloads/git.txt","/home/piyush_singh/aosgit.txt");
    // copyfolder("/home/piyush_singh/Desktop/MATHS","/home/piyush_singh/Desktop/AOS/MATHSINAOS");
    // string s1="/home/piyush_singh/renametest12345678910.txt";
    // string s2="/home/piyush_singh/renametestuiop.txt";
    // rename(s1.c_str(),s2.c_str());
    // rename("try123.txt","jjjjjjjjjjjjjjjjjjjjrenametest12.txt");

/*
string temptildehandle="./try321.txt";
if(temptildehandle[0]=='~'){
	string tilde=gethomepath();
	temptildehandle.erase(0,1);
	temptildehandle=tilde+temptildehandle;
}
else{
	if(temptildehandle[0]=='.'){
		cout<<"temptildehandle1==="<<temptildehandle<<endl;
		temptildehandle=resolvepath(temptildehandle.c_str());
		cout<<"temptildehandle2==="<<temptildehandle<<endl;
	}
}
createfile(temptildehandle);

    return 0;
}*/
// string temptildehandle="../Desktop";
// // cout<<"hello;;;;;"<<resolvepath(re.c_str());

// if(temptildehandle[0]=='.'){
// 		if(temptildehandle[1]=='.'){		//.. wala case
// 			temptildehandle.erase(0,2);
// 			cout<<temptildehandle;
// 		}
// }


// createfile1("/home/piyush_singh/Downloads/hope.txt");
	// copy_file("/home/piyush_singh/Downloads/git.txt","/home/piyush_singh/Desktop/AOS/Assignment")


}










// check("/home/piyush_singh/Desktop/AOS/MATHSINAOS");

// }



// int getWindowSize(int *rows, int *cols) {
//   struct winsize ws;
//   if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
//     return -1;
//   } else {
//     *cols = ws.ws_col;
//     *rows = ws.ws_row;
//     return 0;
//   }
// }









































