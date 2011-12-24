#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "ofxDirList.h"

// needed for glutGetModifiers() to detect SHIFT
#include <GLUT/GLUT.h>  

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
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	bool displayTests, displayChat, moviePlaying;
	bool lockAspect, networkCapture, webcamCapture;
	bool fullscreen, osd, debug;
	
	string cameraUrl, fontPath, storagePath, seqPath;
	string p;
	
	int displayWidth, displayHeight, captureFreq;
	int yCursor, m;
	unsigned int time;
	int startHour, endHour, newSequence;
	
	int xpos, ypos;
	double xscale, yscale;
	
	vector<string> files;
	vector<string> sequenceDir;
	
	string message;
	
	char buffer[1024], path[500], imgPath[1000];
	
	ofTexture* tex;
	vector <ofTexture> texture;
	int bufferSize;
	
	ofVideoGrabber camera;
	int camWidth, camHeight;
	unsigned char * oldPixels, * newPixels;
	float diff, diffMin, diffMax;
	
	void updateSequenceList();
	int imageTypeToGLType(int imageType);
	//ofxImageSequence sequence;
	//vector<ofTexture*> sequence;
	ofImage	loader;
	
	
	int sequenceLength, sequenceFrame;
	int framesPerImage;
	string currentImageFileName, currentImageDirName;
	
	ofxXmlSettings xmlConfig;
	ofxDirList dir;
	ofImage webcam;
	
	ofTrueTypeFont font;
	
	int shutter;
	
	vector<string> strParts;
	int theHour, theMinute;
	float clock;
	
	
};
