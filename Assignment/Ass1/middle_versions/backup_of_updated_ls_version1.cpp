




//version1 atpresent yahi dena hai













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


//status bar daalna hai.
//scrolling. 
//multiple files/folder handle karna hai in move ans copy.
//resize.
//comments daalna hai.

// signal(SIGWINCH,&hello_window_changed);


string gethomepath(){
	string HOMEEEE;
	uid_t userid=getuid();
	struct passwd *pw=getpwuid(userid);
	HOMEEEE=pw->pw_dir;
	return HOMEEEE;
}


void COMMAND_MODE();
void ENABLE_SCROLLING();
void enableRawMode();
void disableRawMode();
long GetFileSize(const char*);
string permissions(const char *);
void PRINT_ALL_DETAILS(const char*);
void PRINT_ALL(vector<pair<string,bool>>,int,int);
void NORMAL_MODE(const char*);
string currentdirectory();
string global_for_search_path="";
int start1,endlast;
int row,column;
int x,y;
int flag=1;
bool tflag=true;
bool normal_flag=true;
// vector<string> dirname;
// vector<string> filename;
stack<string> leftpath;
stack<string> rightpath;
vector<pair<string,bool>> allfileanddir;
string commands;
string partcommand;
vector<string> fullcommands;
string maindirectory=currentdirectory();
struct termios orig_termios;
struct winsize windowsize;

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

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
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


long GetFileSize(const char* file){
    struct stat buff;
    int rc=stat(file,&buff);
    if(rc==0){
    	return buff.st_size;
    }
    else{
    	return -1;
    }
}


string permissions(const char *file){
    struct stat st;
    string permval="";
    if(stat(file,&st) == 0){
        // mode_t perm=st.st_mode;
        permval+=(st.st_mode & S_ISDIR(st.st_mode))?'d':'-';
        permval+=(st.st_mode & S_IWUSR)?'w':'-';
        permval+=(st.st_mode & S_IXUSR)?'x':'-';
        permval+=(st.st_mode & S_IRGRP)?'r':'-';
        permval+=(st.st_mode & S_IWGRP)?'w':'-';
        permval+=(st.st_mode & S_IXGRP)?'x':'-';
        permval+=(st.st_mode & S_IROTH)?'r':'-';
        permval+=(st.st_mode & S_IWOTH)?'w':'-';
        permval+=(st.st_mode & S_IXOTH)?'x':'-';
        // modeval+='\0';
        return permval;     
    }
    else{
        strerror(errno);
        return "0";
    }   
}


void ENABLE_SCROLLING(){
	gotoxy(1,1);
	x=1,y=1;
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
					if(x>tracker && x>1){		
						x--;
						tracker--;
						// cout<<"X:-"<<x;
						gotoxy(x,y);
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
			if(temppath3!="/home"){
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
					// char* argument[3];
					// string file=tmppathy;
					// string commandvi="vi";
					// string filetype=file.substr(file.size()-3,file.size()-1);
					// if(filetype=="pdf" || filetype=="jpg" || filetype=="png"){
					// commandvi = "xdg-open";										//code for opening in vim.
					// }
					// argument[0]=(char*)commandvi.c_str();
					// argument[1]=(char*)file.c_str();
					// argument[2]=NULL;
					// if(execvp(argument[0],argument)==-1){
					// 	perror("exec");
					// }
					execlp("xdg-open","xdg-open",tmppathy.c_str(),(char*)0);
					exit(0);
				}
				wait(NULL);
			}
			else if(allfileanddir[tracker].second==1){
				//directory
				maindirectory=maindirectory+'/'+allfileanddir[tracker].first;
				leftpath.push(maindirectory);
				NORMAL_MODE(leftpath.top().c_str());
			}
		}
		else if(c=='h' || c=='H'){				//home key pressed.
			leftpath.push(maindirectory);
			maindirectory=gethomepath();
			while(!rightpath.empty()){
					rightpath.pop();
			}	
			NORMAL_MODE(maindirectory.c_str());
		}
		else if(c==':'){
			normal_flag=0;			//switch to command mode.
			flag=0;
			COMMAND_MODE();
		}
		else if(c=='q'){
			flag=0;
			tflag=0;
			normal_flag=0;
			printf("\033[H\033[2J\033[3J");		//clear
			printf("\033[0;0f");				//cursot_to_00

			exit(0);
		}
	}
}

void PRINT_ALL_DETAILS(const char* veci,string nameofi){
		char c='\0';
		struct stat ids;
		stat(veci,&ids); 
		struct passwd *pw=getpwuid(ids.st_uid);		//getting username.
		struct group  *gr=getgrgid(ids.st_gid);		//getting groupname.
		long size=(GetFileSize(veci)/1024);
		string permq=permissions(veci);
		if(permq=="0"){
			cout<<"ERROR WHILE READING PERMISSIONS ."<<"\n";
			return;
		}
		printf("%s  %s  %s  %6ld",permq.c_str(),pw->pw_name,gr->gr_name,size); 
		printf("KB\t");
		printf("  %-25s\t",nameofi.c_str());
		printf("%s",ctime(&ids.st_mtime));

}


void PRINT_ALL(vector<pair<string,bool>> name,int start,int end){
	printf("\033[H\033[2J\033[3J");		//clear
	printf("\033[0;0f");				//cursot_to_00
	while(start<end){

		string temppath2=maindirectory+'/'+name[start].first;

		PRINT_ALL_DETAILS(temppath2.c_str(),name[start].first);

		start++;
	}

	ioctl(STDIN_FILENO, TIOCGWINSZ, &windowsize);
	row=windowsize.ws_row-6;
	column=windowsize.ws_col+5;
	gotoxy(row,column);
	cout<<"---------------------------------- STATUS_BAR --------------------------------- "<<endl;
	cout<<"CURRENT_PATH --> "<<maindirectory<<endl;
	
}


void GET_ALL_INFO(const char* rasta){
	struct dirent *d;
	DIR *dh=opendir(rasta);
	if(dh==NULL){
		cout<<"NOT ABLE TO OPEN FILE."<<endl;
		return;
	}
	
	d=readdir(dh);
	while(d != NULL){
		if(d->d_type == DT_DIR){					//directory
			// dirname.push_back(d->d_name);
			allfileanddir.push_back({d->d_name,1});
		}
		else if(d->d_type == DT_REG){				//file
			// filename.push_back(d->d_name);
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
	
	allfileanddir.clear();
	
	GET_ALL_INFO(rasta);
	
	sort(allfileanddir.begin(),allfileanddir.end()); 

	int size=allfileanddir.size();
	start1=0;
	endlast=size;
	if(size<50){
	
		PRINT_ALL(allfileanddir,0,size);
		
	}
	else{
		PRINT_ALL(allfileanddir,0,50);
	}

	if(normal_flag){
		ENABLE_SCROLLING();
	}
}


///////////////////////////////////////////////////////////////////COMMAND_MODE//////////////////////////////////////////////////////////////////////


void createfile(string pathfile){
	int fd=open(pathfile.c_str(),O_RDWR|O_CREAT,S_IRWXU | S_IRWXG | S_IRWXO);
}

void createfolder(string pathfolder){
	const char *temppathfolder=pathfolder.c_str();
	mkdir(temppathfolder,0777);
}

void copyfile(const char *SRC, const char* DEST){
    ifstream src(SRC, ios::binary);
    ofstream dest(DEST, ios::binary);
    dest << src.rdbuf();
}

void first_copyfile(string custompath,string s1,string s2){			//made this first_copyfile for generic use in move and other commands
	string absolutepath1=custompath+"/"+s1;
	createfile(absolutepath1);
	string absolutepath2=resolvepath(s2);
	if(absolutepath2=="0"){
		return;
	}
	else{
		absolutepath2=absolutepath2+"/"+s1;
		copyfile(absolutepath1.c_str(),absolutepath2.c_str());
		struct stat st2;
		stat(absolutepath2.c_str(),&st2);							//changing file permission to their original state.
		chmod(absolutepath2.c_str(),st2.st_mode);
	}

}

void copyfolder(string curpath,string name,string cfpath){
	string cfpathupdated=cfpath+"/"+name;
	createfolder(cfpathupdated);
	string curpathupdated=curpath+"/"+name;
	const char* temp=curpathupdated.c_str();
	int iserror=chdir(temp);
	if(iserror==-1){			//error occured			
		cout<<"NOT ABLE TO OPEN FILE IN copyfolder"<<"\n";
		return;
	}
	char tmp2[512];
	getcwd(tmp2,512);

	struct dirent *entity;

	DIR *dh2=opendir(curpathupdated.c_str());
	if(dh2==NULL){
		cout<<"NOT ABLE TO OPEN FILE IN copyfolder/DIR"<<endl;
		return;
	}

	mode_t filemode;

	struct stat searchstat;
	
	entity=readdir(dh2);

	while(entity!=NULL){
		if(entity->d_name[0]=='.'){
			entity=readdir(dh2);
		}
		else{
			stat(entity->d_name,&searchstat);

			if(entity->d_type==DT_DIR && searchstat.st_mode){
				string searchindirectory=curpathupdated;
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


bool search(string keyword,const char* path){
	int iserror=chdir(path);

	if(iserror==-1){			//error occured
		cout<<"NOT ABLE TO OPEN FILE IN search"<<"\n";
		return 0;
	}
	char tmp2[512];
	getcwd(tmp2,512);

	struct dirent *entity;

	DIR *dh2=opendir(path);
	if(dh2==NULL){
		cout<<"NOT ABLE TO OPEN FILE IN search/DIR"<<endl;
		return 0;
	}

	mode_t filemode;

	struct stat searchstat;
	
	entity=readdir(dh2);

	while(entity!=NULL){
		if(entity->d_name[0]=='.'){
			entity=readdir(dh2);
		}
		else if(entity->d_name==keyword){
			global_for_search_path=path;
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


void deletefile(const char* str1){
	remove(str1);
}


void deletefolder(string dpath){
	int iserror=chdir(dpath.c_str());
	if(iserror==-1){			//error occured			
		cout<<"NOT ABLE TO OPEN FILE IN deletefolder"<<"\n";
		return;
	}
	char tmp2[512];
	getcwd(tmp2,512);
	struct dirent *entity;

	DIR *dh2=opendir(dpath.c_str());
	if(dh2==NULL){
		cout<<"NOT ABLE TO OPEN FILE IN deletefolder/DIR"<<endl;
		return;
	}
	mode_t filemode;

	struct stat searchstat;
	
	entity=readdir(dh2);

	while(entity!=NULL){
		if(entity->d_name[0]=='.'){
			entity=readdir(dh2);
			continue;
		}
		else{
			stat(entity->d_name,&searchstat);

			if(entity->d_type==DT_DIR){
				string temppath=dpath+"/"+entity->d_name;
				deletefolder(temppath);
			}
			else{
				string temppath2=dpath+"/"+entity->d_name;
				deletefile(temppath2.c_str());
			}
			entity=readdir(dh2);
		}
	}
	closedir(dh2);
	rmdir(dpath.c_str());
	cout << dpath << "\n";
	return;
}



void movefile(string curpath,string name,string mfpath){
	first_copyfile(curpath,name,mfpath);
	string tempmfpath=mfpath+"/"+name;
	deletefile((curpath+"/"+name).c_str());
}


void movefolder(string src,string name,string dst){
	copyfolder(src,name,dst);
	string tempmovefolder=src+"/"+name;
	deletefolder(tempmovefolder);
}


void COMMAND_MODE(){

	gotoxy(windowsize.ws_row-2,windowsize.ws_col+2);
	cout<<"<<<<<<<<<<-----------------COMMAND_MODE----------------->>>>>>>>>>"<<"\n";
	while(1){
		char c;
		// string commands;
		// string partcommand;
		// vector<string> fullcommands;
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
		printf("\n");
		if(c==10){										//Enter
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
			if(fullcommands.size()!=2){
				NORMAL_MODE(maindirectory.c_str());
				printf("---->  INVALID COMMAND,ENTER FILENAME TO SEARCH.\n");
				// printf("\33[2K\r");  		//clear line
				break;

			}
	        else if(search(fullcommands[1],maindirectory.c_str())){
	        	NORMAL_MODE(maindirectory.c_str());
	        	cout<<"---->  Present ,at Path : "<<global_for_search_path<<"\n";
	     	}
	     	else{
	     		NORMAL_MODE(maindirectory.c_str());
	     		cout<<"---->  Not Present"<<"\n";
	     	}
	    }



	    else if(fullcommands[0]=="create_file"){
	    	if(fullcommands.size()<3){
	    		// NORMAL_MODE(maindirectory.c_str());
				printf("---->  INVALID COMMAND IN create_file\n");
				// printf("\33[2K\r");  		//clear line
				break;
			}
			if((fullcommands.size()%2) == 0){			//matlab pair me nahi hai filename and destination.
				// NORMAL_MODE(maindirectory.c_str());
				printf("----> MISSING FILENAME OR DESTINATION,RE-ENTER IN create_file/size is even\n");
			}
			else{
				// NORMAL_MODE(maindirectory.c_str());
				for(int i=1;i<fullcommands.size();i+=2){
					string createfilename=fullcommands[i];
					// cout<<"createfilename: "<<createfilename<<endl;
			    	string absolutecreatefilepathe=resolvepath(fullcommands[i+1]);
			    	if(absolutecreatefilepathe=="0"){
			    		printf("ENTERED PATH IS INCORRECT,RE-ENTER IN create_file/normal_mode\n");
			    		
			    	}
			    	else{
				    	// cout<<"absolutecreatefilepathe: "<<absolutecreatefilepathe<<endl;
				    	createfile(absolutecreatefilepathe+"/"+createfilename);
				    	cout<<"SUCCESSFULLY CREATED FILE(S)."<<"\n";
				    }
		    	}
	    	}
	    }


	    else if(fullcommands[0]=="create_dir"){
	    	if(fullcommands.size()<3){
	    		// NORMAL_MODE(maindirectory.c_str());
				printf("---->  INVALID COMMAND");
				// printf("\33[2K\r");  		//clear line
				break;
			}
			if((fullcommands.size()%2) == 0){			//matlab pair me nahi hai filename and destination.
				// NORMAL_MODE(maindirectory.c_str());
				printf("----> MISSING FOLDERNAME OR DESTINATION,RE-ENTER IN create_dir\n");
				
			}
			else{
				// NORMAL_MODE(maindirectory.c_str());
				for(int i=1;i<fullcommands.size();i+=2){
					// string createfilename=fullcommands[i];
					// cout<<"createfilename: "<<createfilename<<endl;
					string absolutecreatefolderpathe=resolvepath(fullcommands[i+1]);
					string createfoldername=fullcommands[i];
			    	// string absolutecreatefilepathe=resolvepath(fullcommands[i+1]);
			    	// createfolder(absolutecreatefolderpathe+"/"+createfoldername);
			    	if(absolutecreatefolderpathe=="0"){
			    		printf("ENTERED PATH IS INCORRECT,RE-ENTER IN create_dir/normal_mode\n");
			    		
			    	}
			    	else{
				    	// cout<<"absolutecreatefilepathe: "<<absolutecreatefilepathe<<endl;
				    	createfolder(absolutecreatefolderpathe+"/"+createfoldername);
				    	cout<<"SUCCESSFULLY CREATED FOLDER(S)."<<"\n";
				    }
		    	}
	    	}
	    							
	    }

	  //   else if(fullcommands[0]=="copy"){
	  //   	if(fullcommands.size()<3){
	  //   		// NORMAL_MODE(maindirectory.c_str());
			// 	printf("INVALID COMMAND IN copy");
			// 	// printf("\33[2K\r");  		//clear line
			// 	break;
			// }

			// int fullcommands_size=fullcommands.size();
			// string checkpath_in_copy=resolvepath(fullcommands[fullcommands_size-1]);
			// if(checkpath_in_copy=="0"){
			// 	printf("ENTERED PATH IS INCORRECT,RE-ENTER IN copy\n");
				
			// }

			// for(int i=1;i<fullcommands_size-1;i++){
			// 	string name_in_copy="";
			// 	string path_in_copy="";
			// 	string temp_checkpath_in_copy=resolvepath(fullcommands[i]);
			// 	if(temp_checkpath_in_copy=="0" || checkpath_in_copy=="0"){
			// 		printf("ENTERED PATH IS INCORRECT,RE-ENTER IN copy\n");
			// 	}
			// 	else{
			// 		int k=temp_checkpath_in_copy.size()-1;
			// 		while(temp_checkpath_in_copy[k]!='/'){
			// 			name_in_copy=temp_checkpath_in_copy[k]+name_in_copy;
			// 			k--;
			// 		}

			// 		// cout<<"name_in_copy===> "<<name_in_copy<<endl;

			// 		for(int j=0;j<k;j++){
			// 			path_in_copy=path_in_copy+temp_checkpath_in_copy[j];
			// 		}

			// 		// cout<<"path_in_copy===> "<<path_in_copy<<endl;

			// 		struct stat st3;
			// 		char is_directory;
			// 		stat(temp_checkpath_in_copy.c_str(),&st3);
			// 		is_directory=(st3.st_mode & S_ISDIR(st3.st_mode))?'d':'f';
			// 		if(is_directory=='d'){
			// 			// NORMAL_MODE(maindirectory.c_str());
			// 			// cout<<"name_in_copy_in_dir===> "<<name_in_copy<<endl;
			// 			// cout<<"path_in_copy_in_dir===> "<<path_in_copy<<endl;
			// 			// cout<<"checkpath_in_copy_in_dir===> "<<checkpath_in_copy;
			// 			copyfolder(path_in_copy,name_in_copy,checkpath_in_copy);	
			// 			printf("---->  COPIED FOLDER SUCCESSFULLY.\n");

			// 		}
			// 		else{
			// 			// NORMAL_MODE(maindirectory.c_str());
			// 			// cout<<"name_in_copy_in_file===> "<<name_in_copy<<endl;
			// 			// cout<<"path_in_copy_in_file===> "<<path_in_copy<<endl;
			// 			// cout<<"checkpath_in_copy_in_file===> "<<checkpath_in_copy;
			// 			first_copyfile(path_in_copy,name_in_copy,checkpath_in_copy);	
			// 			printf("---->  COPIED FILE SUCCESSFULLY.\n");

			// 		}

			// 	}

			// }

	  //   }

	    else if(fullcommands[0]=="copy_file"){
	    	if(fullcommands.size()<3){
	    		// NORMAL_MODE(maindirectory.c_str());
				printf("INVALID COMMAND IN copy_file");
				// printf("\33[2K\r");  		//clear line
				break;
			}
			string name_copyfile="";
			string path_copyfile="";
			string temp_checkpath_copyfile=resolvepath(fullcommands[1]);
			string checkpath_copyfile=resolvepath(fullcommands[2]);
			if(temp_checkpath_copyfile!="0" && checkpath_copyfile!="0"){
				int i=temp_checkpath_copyfile.size()-1;
				while(temp_checkpath_copyfile[i]!='/'){
					name_copyfile=temp_checkpath_copyfile[i]+name_copyfile;
					i--;
				}
				for(int j=0;j<i;j++){
					path_copyfile=path_copyfile+temp_checkpath_copyfile[j];
				}
				// cout<<"temp_checkpath_copyfile:- "<<temp_checkpath_copyfile<<endl;
				// cout<<"path_copyfile:-"<<path_copyfile<<endl;
				// cout<<"name_copyfile:-"<<name_copyfile<<endl;
				
				// cout<<"checkpath_copyfile:- "<<checkpath_copyfile<<endl;
				
				// NORMAL_MODE(maindirectory.c_str());
				first_copyfile(path_copyfile,name_copyfile,checkpath_copyfile);	
				printf("---->  COPIED FILE SUCCESSFULLY.\n");
				
			}
			else{
				printf("ENTERED PATH IS INCORRECT,RE-ENTER IN copy_file\n");
			}
	    }

	    else if(fullcommands[0]=="copy_folder"){
	    	if(fullcommands.size()<3){
	    		// NORMAL_MODE(maindirectory.c_str());
				printf("---->  INVALID COMMAND IN copy_folder");
				printf("\33[2K\r");  		//clear line
				break;
			}

			string name_copyfolder="";
			string path_copyfolder="";
			string temp_checkpath_copyfolder=resolvepath(fullcommands[1]);
			string checkpath_copyfolder=resolvepath(fullcommands[2]);
			if(temp_checkpath_copyfolder!="0" && checkpath_copyfolder!="0"){
				int i=temp_checkpath_copyfolder.size()-1;
				while(temp_checkpath_copyfolder[i]!='/'){
					name_copyfolder=temp_checkpath_copyfolder[i]+name_copyfolder;
					i--;
				}
				for(int j=0;j<i;j++){
					path_copyfolder=path_copyfolder+temp_checkpath_copyfolder[j];
				}
				// cout<<"temp_checkpath_copyfile:- "<<temp_checkpath_copyfile<<endl;
				// cout<<"path_copyfile:-"<<path_copyfile<<endl;
				// cout<<"name_copyfile:-"<<name_copyfile<<endl;
				
				// cout<<"checkpath_copyfile:- "<<checkpath_copyfile<<endl;
				
				// NORMAL_MODE(maindirectory.c_str());
				copyfolder(path_copyfolder,name_copyfolder,checkpath_copyfolder);	
				printf("---->  COPIED FOLDER SUCCESSFULLY.\n");
			}
			else{
				printf("ENTERED PATH IS INCORRECT,RE-ENTER IN copy_folder\n");
			}
			// string updatedcommand2=resolvepath(fullcommands[2]);
			// copyfolder(maindirectory,fullcommands[1],updatedcommand2);
	    	
	    }

	    else if(fullcommands[0]=="move_file"){

	    }


	    else if(fullcommands[0]=="goto"){		//goto
	    	// NORMAL_MODE(maindirectory.c_str());
	    	if(fullcommands.size()<2){
				printf("---->  INVALID COMMAND");
				// printf("\33[2K\r");  		//clear line
				break;
			}
	    	leftpath.push(maindirectory);	

	    	string gotopath=resolvepath(fullcommands[1]);
	    	if(gotopath=="0"){
	    		printf("ENTERED PATH IS INCORRECT,RE-ENTER.\n");
	    	}
	    	struct stat s2;
			if(stat(gotopath.c_str(),&s2)==0  && (s2.st_mode & S_IFDIR)){
			    //it's a valid directory
				maindirectory=gotopath;
				// NORMAL_MODE(maindirectory.c_str());
			}
			else{
				//not a valid directory
				printf("ENTERED PATH IS INCORRECT,RE-ENTER.\n");
				leftpath.pop();
			}

	    }


	    else if(fullcommands[0]=="rename"){
	    	NORMAL_MODE(maindirectory.c_str());
	    	if(fullcommands.size()<3){
				printf("---->  INVALID COMMAND");
				printf("\33[2K\r");  		//clear line
				break;
			}
	    	string temps1=resolvepath(fullcommands[1]);
	    	string temps2=resolvepath(fullcommands[2]);
	    	if(temps1=="0" || temps2=="0"){
	    		printf("ENTERED PATH IS INCORRECT,RE-ENTER.\n");
	    	}
	    	else{
		    	rename(temps1.c_str(),temps2.c_str());
		    	NORMAL_MODE(maindirectory.c_str());
		    	cout<<"---->  RENAMED SUCCESSFULLY"<<"\n";
		    }
	    }


	    else if(c==27){			//escape
			normal_flag=1;
			flag=1;
			NORMAL_MODE(maindirectory.c_str());
			break;
		}


	    else if(fullcommands[0]=="quit"){
	    	printf("\033[H\033[2J\033[3J");		//clear
			printf("\033[0;0f");			//cursor_0_0
			tflag=0;
			// cout<<"exit";
			exit(0);
	    }


	    else{
	    	NORMAL_MODE(maindirectory.c_str());
	    	cout<<"---->  INVALID COMMAND."<<"\n";
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
	
    return 0;
}



