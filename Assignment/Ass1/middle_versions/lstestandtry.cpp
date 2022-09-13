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
using namespace std;

string gethomepath(){
	string HOMEEEE;
	uid_t userid=getuid();
	struct passwd *pw=getpwuid(userid);
	HOMEEEE=pw->pw_dir;
	return HOMEEEE;
}

//status bar daalna hai.
//scrolling. 
//resize.
//comments daalna hai.

// string resolvepath(const char * path){
// 	return realpath(path,NULL);
// }





void COMMAND_MODE();
void ENABLE_SCROLLING();
void enableRawMode();
void disableRawMode();
long GetFileSize(const char*);
char* permissions(const char *);
void PRINT_ALL_DETAILS(const char*);
void PRINT_ALL(vector<pair<string,bool>>,int,int);
void NORMAL_MODE(const char*);
string currentdirectory();

int start1,endlast;
int localstart,localend;
int flag=1;
bool tflag=true;
bool normal_flag=true;

struct termios orig_termios;

int row,column;

vector<string> dirname;
vector<string> filename;
stack<string> leftpath;
stack<string> rightpath;
vector<pair<string,bool>> allfileanddir;
string maindirectory=currentdirectory();


//for command mode
// vector<string> fullcommands;


string currentdirectory(){
	char tmp[512];
    getcwd(tmp,512);
    string maindirectory=tmp;
	return maindirectory;
}

void disableRawMode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void gotoxy(int x,int y){
	printf("%c[%d;%dH",27,x,y);
}
int x,y;

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


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


void ENABLE_SCROLLING(){
	gotoxy(1,1);
	x=0,y=0;
	int tracker=0;
	while(flag==1){
		char c;
		c=getchar();
		if(c==static_cast<char>(27)){
			c=getchar();
			if(c==static_cast<char>(91)){
				c=getchar();
				if(c==static_cast<char>(65)){	//up
					//up
					if(tracker>x){		
						x--;
						tracker--;
						gotoxy(x,y);
					}
					else if(tracker>start1){
						tracker--;
						start1--;
						endlast--;
						PRINT_ALL(allfileanddir,start1,endlast);
					}
				}
				else if(c==static_cast<char>(66)){	//DOWN
					if(x<endlast){	
						x++;
						tracker++;		
						gotoxy(x,y);
					}	
				}
				else if(c==static_cast<char>(67)){  //right
					if(rightpath.empty()!=1){
						leftpath.push(maindirectory);
						maindirectory=rightpath.top();
						rightpath.pop();
						NORMAL_MODE(maindirectory.c_str());
					}	
				}
				else if(c==static_cast<char>(68)){	//left			
					if(leftpath.empty()!=1){
						rightpath.push(maindirectory);
						maindirectory=leftpath.top();
						leftpath.pop();
						NORMAL_MODE(maindirectory.c_str());
					}
				}
			}
		}
		else if(c==static_cast<char>(127)){	
		string temppath3=maindirectory;			//backspace
			if(temppath3!=gethomepath()){
				leftpath.push(maindirectory);
				int i=temppath3.size()-1;
				while(temppath3[i]!='/'){
					i--;
				}
				string ans="";
				for(int j=0;j<i;j++){
					ans+=temppath3[j];
				}
				maindirectory=ans;
				while(!rightpath.empty()){
					rightpath.pop();
				}
				NORMAL_MODE(maindirectory.c_str());
			}
		}
		else if(c==static_cast<char>(10)){		//ENTER
			if(allfileanddir[tracker].second==0){
				//file
				char localpath[512];
				strcpy(localpath,allfileanddir[tracker].first.c_str());
				int opendir=open(localpath,O_WRONLY);
				dup2(opendir,2);
				close(opendir);
				string tmppathy=maindirectory+'/'+allfileanddir[tracker].first;
				
				if(!fork()){
					char* argument[3];
					string file=tmppathy;
					string commandvi="vi";
					string filetype=file.substr(file.size()-3,file.size()-1);
					if(filetype=="pdf" || filetype=="jpg" || filetype=="png"){
					commandvi = "xdg-open";
					}
					argument[0]=(char*)commandvi.c_str();
					argument[1]=(char*)file.c_str();
					argument[2]=NULL;
					if(execvp(argument[0],argument)==-1){
						perror("exec");
					}
					// execlp("xdg-open","xdg-open",tmppathy.c_str(),(char*)0);
					exit(0);
				}
				wait(NULL);
			}
			else if(allfileanddir[tracker].second==1){
				//directory
				// cout<<"allfileanddir[tracker].first:"<<allfileanddir[tracker].first<<endl;
				maindirectory=maindirectory+'/'+allfileanddir[tracker].first;
				leftpath.push(maindirectory);
				// cout<<"leftpath.top()"<<leftpath.top()<<endl;
				// cout<<"leftpath.top().c_str()"<<leftpath.top().c_str()<<endl;
				NORMAL_MODE(leftpath.top().c_str());
			}
		}
		else if(c=='h' || c=='H'){
			leftpath.push(maindirectory);
			maindirectory=gethomepath();
			while(!rightpath.empty()){
					rightpath.pop();
			}	
			NORMAL_MODE(maindirectory.c_str());
		}
		else if(c==':'){
			normal_flag=0;
			flag=0;
			COMMAND_MODE();
		}
		else if(c=='q'){
			flag=0;
			tflag=0;
			normal_flag=0;
			printf("\033[H\033[2J\033[3J");		//clear
			printf("\033[0;0f");

			exit(0);
		}
	}
}


long GetFileSize(const char* filename){
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


char* permissions(const char *file){
    struct stat st;
    char *modeval = (char*)malloc(sizeof(char) * 9 + 1);
    if(stat(file, &st) == 0){
        mode_t perm = st.st_mode;
        modeval[0] = (perm & S_ISDIR(st.st_mode)) ? 'd' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        modeval[9] = '\0';
        return modeval;     
    }
    else{
        return strerror(errno);
    }   
}

void PRINT_ALL_DETAILS(const char* veci,string nameofi){
		char c='\0';
		struct stat info;
		stat(veci, &info); 
		struct passwd *pw = getpwuid(info.st_uid);
		struct group  *gr = getgrgid(info.st_gid);
		long size=(GetFileSize(veci)/1024);
		

		printf("%s  %s  %s  %6ld",permissions(veci),pw->pw_name,gr->gr_name,size);
		printf("KB\t");
		// string name="";
		// int i=0;
		// while(nameofi[i]){
		// 	i++;
		// }
		// if(i>10){
		// 	name+=nameofi[i];
		// }
		printf("  %-25s\t",nameofi.c_str());
		printf("%s",ctime(&info.st_mtime));

}


void PRINT_ALL(vector<pair<string,bool>> name,int start,int end){
	printf("\033[H\033[2J\033[3J");		//clear
	printf("\033[0;0f");
	while(start<end){

		string temppath2=maindirectory+'/'+name[start].first;

		PRINT_ALL_DETAILS(temppath2.c_str(),name[start].first);

		start++;
	}

	struct winsize windowsize;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &windowsize);
	row=windowsize.ws_row-10;
	column=windowsize.ws_col+5;
	// row=row-5;
	// column=column+2;
	gotoxy(row,column);
	cout<<"--------------------------- STATUS_BAR --------------------------- "<<endl;
	cout<<"CURRENT_PATH --> "<<maindirectory<<endl;


	
}


void GET_ALL_INFO(const char* rasta){
	struct dirent *d;

	DIR *dh=opendir(rasta);
	
	d=readdir(dh);
	while(d != NULL){
		if(d->d_type == DT_DIR){
			//directory
			dirname.push_back(d->d_name);
			allfileanddir.push_back({d->d_name,1});
		}
		else if(d->d_type == DT_REG){
			//file
			filename.push_back(d->d_name);
			allfileanddir.push_back({d->d_name,0});
		}
		else{
			continue;
		}
		d=readdir(dh);

	}
	closedir(dh);
	

}

void NORMAL_MODE(const char* rasta){
	// cout<<"rasta"<<rasta<<endl;
	// printf("\033[H\033[2J\033[3J");		//clear
	// printf("\033[0;0f");			//cursor to 0,0
	
	allfileanddir.clear();
	
	GET_ALL_INFO(rasta);
	
	sort(allfileanddir.begin(),allfileanddir.end()); 

	int size=allfileanddir.size();
	start1=0;
	endlast=size;
	localstart=0;
	localend=10;
	if(size<10){
	
		PRINT_ALL(allfileanddir,0,size);
		
	}
	else{
		PRINT_ALL(allfileanddir,0,10);
	}

	if(normal_flag){
		ENABLE_SCROLLING();
	}
}


//////////COMMAND_MODE////////////////////////////////////////////


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

bool search(string keyword,const char* path){
	int iserror=chdir(path);

	if(iserror==-1){
		//error occured
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




void COMMAND_MODE(){
	gotoxy(47,2);
	// cout<<"x"<<x<<endl;
	// cout<<"y"<<y<<endl;
	cout<<"<<<<<<<<<<----------COMMAND_MODE---------->>>>>>>>>>"<<endl;
	while(1){
		// gotoxy(47,2);
		// printf("\33[2K\r");	//clear line
		char c;
		string commands;
		string partcommand;
		vector<string> fullcommands;
		while((c=getchar())!=27 && c!=10){		//27-ESC,10-Enter,127-backspace
			if(c==127){
				printf("\33[2K\r");		//clear line
				int commands_len = commands.length();
				commands= commands.substr(0,commands_len-1);
				cout<<commands;
				continue;
			}
			commands +=c;
			cout<<c;
		}
		printf("\n\n");
		if(c==10){
			for(int i=0;i<commands.length();i++){
				if(commands[i]==' '){
					fullcommands.push_back(partcommand);
					partcommand.clear();
				}
				else{
					partcommand+=commands[i];
				}
			}
		}
		fullcommands.push_back(partcommand);

		if(fullcommands[0]=="search"){
			if(fullcommands.size()<2){
				printf("INVALID COMMAND");
				printf("\33[2K\r");  		//clear line
				break;
			}
	        if(search(fullcommands[1],maindirectory.c_str())){
	        	cout<<"\n"<<"Present"<<"\n";
	     	}
	     	else{
	     		cout<<"\n"<<"Not Present"<<"\n";
	     	}
	    }
	    else if(fullcommands[0]=="createfile"){
	    	string absolutecreatefilepathe=resolvepath(fullcommands[1]);
	    	createfile(absolutecreatefilepathe);
	    }
	    else if(fullcommands[0]=="createfolder"){
	    	string absolutecreatefolderpathe=resolvepath(fullcommands[1]);
	    	createfolder(absolutecreatefolderpathe);
	    }
	    else if(fullcommands[0]=="copyfile"){
	    	string absolutepath1=resolvepath(fullcommands[1]);
	    	string absolutepath2=resolvepath(fullcommands[2]);
	    	copyfile(absolutepath1,absolutepath2);
	    }
	    else if(fullcommands[0]=="goto"){		//goto
	    	if(fullcommands.size()<2){
				printf("INVALID COMMAND");
				printf("\33[2K\r");  		//clear line
				break;
			}
	    	leftpath.push(maindirectory);
	    	// string absoluteofgoto=resolvepath(fullcommands[1]);

	    	string tempofmaindirectory=maindirectory;
	    	string pathofgoto=fullcommands[1];
	    	int size_pathofgoto=pathofgoto.size();
	    	if(pathofgoto[0]=='~'){
	    		if(size_pathofgoto==1){
	    			maindirectory=gethomepath();
					// cout<<"maindirectory111--->"<<maindirectory<<endl;
					// cout<<"size_pathofgoto--->"<<size_pathofgoto<<endl;
					NORMAL_MODE(maindirectory.c_str());
	    		}
	    		else{
	    			maindirectory=gethomepath();
	    			// cout<<"maindirectory....|||||...."<<maindirectory<<endl;
	    			pathofgoto=pathofgoto.substr(1);
	    			// cout<<"pathofgoto.....|||||..."<<pathofgoto<<endl;
					string temppathcheck=maindirectory+pathofgoto;
					// cout<<"temppathcheck...|||||.."<<temppathcheck<<endl;
					struct stat s;
					if(stat(temppathcheck.c_str(),&s)==0  && (s.st_mode & S_IFDIR)){
						//it's a valid directory
						maindirectory=temppathcheck;
						// cout<<"maindirectory222--|||||->"<<maindirectory<<endl;
						NORMAL_MODE(maindirectory.c_str());
					}
					else{
						//not a valid directory
						cout<<"NOT A VALID PATH !!!"<<endl;
						leftpath.pop();
					}	
	    		}		
	    			
			}
				// cout<<"pathofgoto[0]--->"<<endl;
			else if(pathofgoto[0]=='/'){

				string temppathcheck=fullcommands[1];
				// cout<<"temppathcheck....."<<temppathcheck<<endl;
				struct stat s;
				if(stat(temppathcheck.c_str(),&s)==0  && (s.st_mode & S_IFDIR)){
				    //it's a valid directory
					maindirectory=temppathcheck;
					// cout<<"maindirectory222--->"<<maindirectory<<endl;
					NORMAL_MODE(maindirectory.c_str());
				}
				else{
					//not a valid directory
					// cout<<"NOT A VALID PATH !!!"<<endl;
					// leftpath.pop();

					string absolutegotopathe=resolvepath(fullcommands[1]);
					struct stat s;
					if(stat(absolutegotopathe.c_str(),&s)==0  && (s.st_mode & S_IFDIR)){
				    	//it's a valid directory
						maindirectory=absolutegotopathe;
						// cout<<"maindirectory222--->"<<maindirectory<<endl;
						NORMAL_MODE(maindirectory.c_str());
					}

				}
			}
			else{
				string temppathcheck2=tempofmaindirectory;
				temppathcheck2.append("/");
				temppathcheck2.append(pathofgoto);
				struct stat s2;
				if(stat(temppathcheck2.c_str(),&s2)==0  && (s2.st_mode & S_IFDIR)){
				    //it's a valid directory
					maindirectory=temppathcheck2;
					// cout<<"maindirectory333--->"<<maindirectory<<endl;
					NORMAL_MODE(maindirectory.c_str());
				}
				else{
					//not a valid directory
					cout<<"NOT A VALID PATH !!!"<<endl;
					leftpath.pop();
				}
			}	
	    }
	    else if(fullcommands[0]=="copy"){
	    	if(fullcommands.size()<3){
				printf("INVALID COMMAND");
				printf("\33[2K\r");  		//clear line
				break;
			}
	    	string source=fullcommands[1];
	    	string destination=fullcommands[2];



	    }
	    else if(fullcommands[0]=="rename"){
	    	if(fullcommands.size()<3){
				printf("INVALID COMMAND");
				printf("\33[2K\r");  		//clear line
				break;
			}
	    	string s1=fullcommands[1];
	    	string s2=fullcommands[2];
	    	rename(s1.c_str(),s2.c_str());

	    }
	    else if(c==27){			//escape
			normal_flag=1;
			flag=1;
			NORMAL_MODE(maindirectory.c_str());
			break;
		}
	    else if(fullcommands[0]=="quit"){
	    	printf("\033[H\033[2J\033[3J");		//clear
			printf("\033[0;0f");
			tflag=0;
			// cout<<"exit";
			exit(0);
	    }
	}

}

int main(){

	leftpath.push(maindirectory);

	enableRawMode();
	while(tflag)
	{
		if(normal_flag)
		{
			NORMAL_MODE(leftpath.top().c_str());
		}
		else{
			COMMAND_MODE();
		}
	}

	// string re="./Desktop";
	// string currdir=currentdirectory();
	// // cout<<"last "<<resolvepath(re.c_str());
	// cout<<currdir<<endl;


	
	
    return 0;
}


