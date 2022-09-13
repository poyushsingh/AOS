gio: file:///home/piyush_singh/Desktop/SSD/webpage/normal_mode.cpp: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/normal_mode.cpp”: No such file or directory
gio: file:///home/piyush_singh/Desktop/SSD/webpage/%8B%85%D4%0F%C2U: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/����U”: No such file or directory
gio: file:///home/piyush_singh/Desktop/SSD/webpage/%03: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/”: No such file or directory
gio: file:///home/piyush_singh/Desktop/SSD/webpage/%FF%FF%FF%FF%FF%FF%FF%7F: Error when getting information for file “/home/piyush_singh/Desktop/SSD/webpage/�������”: No such file or directory
t stat state;
	if(stat(tempfilepath,&state)==0){
		if(state.st_mode & S_IFDIR){
			//directory
			dirname.push_back(temppath);
			cout<<temppath;
		}
		else if(state.st_mode & S_IFREG){
			filename.push_back(temppath);
			cout<<temppath;
		}
	}

	// sort(filename.begin(),filename.end());
	// sort(dirname.begin,dirname.end());
	// for(int i=0;i<dirname.size();i++){
	// 	allfileanddir.push_back({dirname[i],0});

	// }
	// for(int i=0;i<filename.size();i++){
	// 	allfileanddir.push_back({filename[i],1});
	// }

	closedir(dh);

	// for(int i=0;i<allfileanddir.size();i++){
	// 	cout<<allfileanddir[i].first;
	// 	cout<<allfileanddir[i].second;
	// 	cout<<endl;
	// }

}

void NORMAL_MODE(string path){
	vector<string> allfileanddirinnormal;
	GET_ALL_INFO(path); 
	// for(auto i:dirname){
	// 	cout<<"dirname:"<<i<<endl;
	// }
	// cout<<endl;
	// for(auto i:filename){
	// 	cout<<"filename:"<<i<<endl;
	// }
	// cout<<endl;

	// PRINT_ALL(dirname);
	// PRINT_ALL(filename);
	// int size=allfileanddirinnormal.size();
	// start=0;
	// endlast=size;
	// if(size<20){
	// 	PRINT_ALL(allfileanddirinnormal,0,size);
	// }
	// else{
	// 	PRINT_ALL(allfileanddirinnormal,0,20);
	// }
	// ENABLE_SCROLLING();
}

string currentdirectory(){
	char tmp[256];
    getcwd(tmp,256);
    string maindirectory=tmp;
	return maindirectory;
}


int main(){
	string maindirectory=currentdirectory();
	leftpath.push(maindirectory);
	NORMAL_MODE(leftpath.top());
    return 0;
}









