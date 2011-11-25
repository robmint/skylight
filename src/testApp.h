#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
//#include "ofxThread.h"
#include "ofxFileLoader.h"
#include "util.h"

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
		string cameraUrl, fontPath;
		int displayWidth, displayHeight;
		int yCursor, m, time;
		long freeSpace;
		
		ofxFileLoader imageLoader;

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
