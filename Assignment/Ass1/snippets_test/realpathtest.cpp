#include<stdio.h>
#include<bits/stdc++.h>
#include<sys/wait.h>
#include<pwd.h>
#include<grp.h>
#include<limits.h>
#include<dirent.h>
#include<signal.h> 
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
using namespace std;

string maindirectory="/home/piyush_singh/Desktop";

string gethomepath(){
	string HOMEEEE;
	uid_t userid=getuid();
	struct passwd *pw=getpwuid(userid);
	HOMEEEE=pw->pw_dir;
	return HOMEEEE;
}

string real_resolvepath(string handle){
	char * fullpath;
	fullpath=realpath(handle.c_str(),NULL);
	if(fullpath==NULL) {
		cout<<"INVALID PATH !!"<<endl;
		return "0";
	}
	else{
		string strpath(fullpath);
		return strpath;
	}
}


string resolvepath(string path){
	string temptildehandle=path;
	if(temptildehandle[0]=='~'){			//w.r.t home/user
		string tilde=gethomepath();
		temptildehandle.erase(0,1);
		temptildehandle=tilde+temptildehandle;
		string checkpath=real_resolvepath(temptildehandle);
		if(checkpath=="0"){
			return "0";
		}
		else{
			return checkpath;
		}
	}
	else if(temptildehandle[0]=='.'){			//considers both cases . and ..
		string temppathwithdot=maindirectory+"/"+temptildehandle;
		string absolutepathwithoutdot=real_resolvepath(temppathwithdot);
		if(absolutepathwithoutdot=="0"){
			return "0";
		}
		else{
			return absolutepathwithoutdot;
		}

	}
	else if(temptildehandle[0]=='/'){			//user gives absolute path.
		return temptildehandle;
	}
	else{
		return (maindirectory+"/"+temptildehandle);
	}
}

	// else{
	// 	char * fullpath;
	// 	fullpath=realpath(path.c_str(),NULL);
	// 	if(fullpath==NULL) {
	// 		cout<<"INVALID PATH !!"<<endl;
	// 		return "0";
	// 	}
	// 	else{
	// 		string strpath(fullpath);
	// 		return strpath;
	// 	}
	// }




// string resolvepath(string path){
// 	string temptildehandle=path;
// 	if(temptildehandle[0]=='~'){
// 		string tilde=gethomepath();
// 		if(temptildehandle.size()==1){
// 			return tilde;
// 		}
// 		else{
// 			temptildehandle.erase(0,1);
// 			temptildehandle=tilde+temptildehandle;
// 			return temptildehandle;
// 		}
// 	}
// 	else{
// 		char * fullpath;
// 		fullpath=realpath(path.c_str(),NULL);
// 		if(fullpath==NULL) {
// 			cout<<"INVALID PATH !!"<<endl;
// 			return "0";
// 		}
// 		else{
// 			string strpath(fullpath);
// 			return strpath;
// 		}
// 	}
// }



// string currentdirectory(){
// 	char tmp[512];
//     getcwd(tmp,512);
//     string maindirectory=tmp;
// 	return maindirectory;
// }







int main(){
	string op="~/Desktop/DSAP/..//////////AOS";
	// string pass=maindirectory+"/"+op;
	// cout<<resolvepath(op)<<endl;





	string name_copyfile="";
	string path_copyfile="";
	std::vector<string> fullcommands;
	fullcommands.push_back("/home/piyush_singh/s1.txt");
	int i=25-1;
	while(fullcommands[0][i]!='/'){
		name_copyfile=fullcommands[0][i]+name_copyfile;
		i--;
	}
	for(int j=0;j<i;j++){
		path_copyfile=path_copyfile+fullcommands[0][j];
	}
	cout<<"path_copyfile:-"<<path_copyfile<<endl;
	cout<<"name_copyfile:-"<<name_copyfile<<endl;

}








