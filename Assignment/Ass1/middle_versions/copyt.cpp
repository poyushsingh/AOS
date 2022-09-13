gio: file:///home/piyush_singh/Desktop/SSD/webpage/copyt.cpp: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/copyt.cpp”: No such file or directory
gio: file:///home/piyush_singh/Desktop/SSD/webpage/%18: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/”: No such file or directory
e<algorithm>
#include<ctype.h> 
#include<errno.h>
#include<time.h>
#include<fcntl.h>
#include<fstream>
using namespace std;

// void copyFile(const char *SRC, const char* DEST)
// {
//     ifstream src(SRC, ios::binary);
//     ofstream dest(DEST, ios::binary);
//     dest << src.rdbuf();
// }

// int main(){
//     string src1="/home/piyush_singh/Desktop/p.txt";
//     string dst2="/home/piyush_singh/Desktop/AOS/p.txt";
//     copyFile(src1.c_str(),dst2.c_str());

//     return 0;
// }

void gotoxy(int x,int y){
    printf("%c[%d;%dH",27,x,y);
}


void hello_window_changed(int a){
    gotoxy(30,40);
    cout<<"HELLO GUYS"<<endl;
    cout<<"HAPPY GANESH CHATURTHI"<<endl;
}

int main(){
    hello_window_changed(0);
    signal(SIGWINCH,&hello_window_changed);
    return 0;
}






























bool search(string keyword,const char* path){           //chdir is used to change the current working directory..
    int iserror=chdir(path);

    if(iserror==-1){            //error occured             //The getcwd() function copies an absolute pathname of the current 
                                                            //working directory to the array pointed to by buf, which is of length size.       
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









