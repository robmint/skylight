#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
//#include "ofxThread.h"
#include "ofxFileLoader.h"
// #include "ofFileUtils.h" 007 only
#include "ofxImageSequence.h" 
#include "util.h"
#include <string.h>

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
		long freeSpace;
		
		char buffer[1024], path[500], imgPath[1000];

		ofxFileLoader imageLoader;
		ofxImageSequence sequence;
		ofxXmlSettings xmlConfig;
		
		//ofImage remoteImage, testImage;
		//ofImage movieBuffer[3600];
		
		ofVideoPlayer videoPlayer;

	
/*		ArrayList output = new ArrayList(); 
		ArrayList chat = new ArrayList(); 
*/		
		ofTrueTypeFont font;
		
/*		HttpThread httpThread;
		CameraThread cameraThread;
		ImageLoaderThread imageLoaderThread;
		MoviePlayer moviePlayer;
	*/	

};

#endif
