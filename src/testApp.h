#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFileLoader.h"
// #include "ofFileUtils.h" 007 only
#include "ofxDirList.h"
#include "ofxImageSequence.h" 
#include "util.h"
#include <string.h>
#include <vector.h>

#define CONFIG_FILE "data/skylight.xml"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);
		
		bool displayTests, displayChat, moviePlaying;
		bool networkCapture;
		string cameraUrl, fontPath, storagePath;
		int displayWidth, displayHeight, captureFreq;
		int yCursor, m, time;
	
		int xpos, ypos;
		double xscale, yscale;
		bool lockAspect;
	
		long freeSpace;

		vector<string> files;
		
		string message;
		
		char buffer[1024], path[500], imgPath[1000];

		ofTexture *frameTexture;
		ofxFileLoader imageLoader;
		ofxImageSequence sequence;
		ofxXmlSettings xmlConfig;
		ofxDirList dir;
		
		ofVideoPlayer videoPlayer;
		ofTrueTypeFont font;

};

#endif
