#pragma once

#include "ofMain.h"
#include "ofxRuiThread.h"

class HttpThread:public ofxRuiThread{
public:
	int count;
	ofxFileLoader imageLoader;
	string cameraUrl, imgPath;
	bool loading;
	
	HttpThread(){
		count = -1;
		loading = false;
		cout<<"Thread init\n";
	}
	
		
	void updateThread(){
		loading = true;
		if(imageLoader.loadFromUrl(cameraUrl,imgPath)) {
			cout<<"HTTP thread: Saved image to: "<<imgPath<<"\n";
		}
		loading = false;
	}
};
