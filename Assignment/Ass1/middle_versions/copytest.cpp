gio: file:///home/piyush_singh/Desktop/SSD/webpage/copytest.cpp: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/copytest.cpp”: No such file or directory

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



string permissions(const char *file){
    struct stat st;
    string permval="";
    if(stat(file,&st) == 0){
        // mode_t perm=st.st_mode;
        permval+=(st.st_mode & S_ISDIR(st.st_mode)) ? 'd' : '-';
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
        return strerror(errno);
    }   
}

int main(){
	string perm="/home/piyush_singh/Desktop/ls.cpp";
	struct stat st;
	char is_dir;
	stat(perm.c_str(),&st);
	is_dir=(st.st_mode & S_ISDIR(st.st_mode))?'d':'f';
	if(is_dir=='d'){
		cout<<"directory"<<"\n";
	}
	else{
		cout<<"file"<<"\n";
	}

}
