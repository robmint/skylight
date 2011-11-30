#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFileLoader.h"
#include "ofxDirList.h"
#include "ofxImageSequence.h" 
#include "ofxOpenCv.h"
#include "ofxThread.h"


#include "httpThread.h"
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
		bool lockAspect, networkCapture, webcamCapture;
		bool fullscreen;
		
		string cameraUrl, fontPath, storagePath, seqPath;
		string p;

		int displayWidth, displayHeight, captureFreq;
		int yCursor, m, time;
		int startHour, endHour, newSequence;
	
		int xpos, ypos;
		double xscale, yscale;
			
		SimpleThread HttpThread;


		vector<string> files;
		
		string message;
		
		char buffer[1024], path[500], imgPath[1000];

		ofTexture *frameTexture;

		ofVideoGrabber camera;
		int camWidth, camHeight;

		ofxFileLoader imageLoader;
		ofxImageSequence sequence;
		ofxXmlSettings xmlConfig;
		ofxDirList dir;
		ofImage webcam;
		
		ofTrueTypeFont font;

};

#endif
