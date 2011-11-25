#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	videoPlayer.loadMovie("movies/fingers.mov");
	bool loaded = videoPlayer.bLoaded;
	if(loaded) 	printf("movie loaded\n");

	videoPlayer.play();
	//videoPlayer.setPaused(false);

	// get config values from XML file in ./data
	if( !xmlConfig.loadFile("skylight.xml") ) {
		printf("config not found");
		exit();
	}

	displayTests = xmlConfig.getValue("test:display", false);
	displayChat = xmlConfig.getValue("chat:display", false);
	cameraUrl = xmlConfig.getValue("camera:url", "http://192.168.0.6/jpeg.cgi?0");
	displayWidth = xmlConfig.getValue("display:width", 1280);
	displayHeight = xmlConfig.getValue("display:height", 1024);
	fontPath = xmlConfig.getValue("font:path", "mono.ttf");
	
	// load font
	font.loadFont(fontPath, 32);

	
	videoPlayer.loadMovie("movies/ironsand.mov");
	videoPlayer.play();
	

	ofBackground(0,0,0);

}

//--------------------------------------------------------------
void testApp::update(){
	//videoPlayer.idleMovie();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);

	videoPlayer.draw(0,0,displayWidth,displayHeight);

	string str = ofToString(ofGetFrameRate(), 2)+"fps";
	font.drawString(str, 5, 40);
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

