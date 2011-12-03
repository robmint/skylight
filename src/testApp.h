#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFileLoader.h"
#include "ofxDirList.h"
//#include "ofxImageSequence.h" 
#include "ofxOpenCv.h"
#include "ofxThread.h"


#include "httpThread.h"
//#include "imageLoaderThread.h"

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
		bool fullscreen, osd;
		
		string cameraUrl, fontPath, storagePath, seqPath;
		string p;

		int displayWidth, displayHeight, captureFreq;
		int yCursor, m, time;
		int startHour, endHour, newSequence;
	
		int xpos, ypos;
		double xscale, yscale;
			
		HttpThread Http;
		//ImageLoaderThread ImageLoader;


		vector<string> files;
		vector<string> sequenceDir;
		
		string message;
		
		char buffer[1024], path[500], imgPath[1000];

		ofTexture frameTexture;
		ofTexture texture;

		ofxCvGrayscaleImage grayImageNow, grayImageThen, grayImageDiff;
		ofVideoGrabber camera;
		int camWidth, camHeight;

		ofxFileLoader imageLoader;
	
		void updateSequenceList();
		int imageTypeToGLType(int imageType);
		//ofxImageSequence sequence;
		vector<ofTexture*> sequence;
		ofImage	loader;


		int sequenceLength, sequenceFrame;
	
		ofxXmlSettings xmlConfig;
		ofxDirList dir;
		ofImage webcam;
		
		ofTrueTypeFont font;

};

#endif
