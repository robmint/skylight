#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
//#include "ofxThread.h"
#include "util.h"

#define CONFIG_FILE = "data/skylight.conf"

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
		void windowResized(int w, int h);
		
		bool displayTests, displayChat, moviePlaying;
		char *cameraUrl, *fontPath;
		int displayWidth, displayHeight;
		int yCursor, m;
		long freeSpace;
		
		ofxXmlSettings XmlConfig;
		
		ofImage remoteImage, testImage;
		ofImage movieBuffer[3600];
		
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
