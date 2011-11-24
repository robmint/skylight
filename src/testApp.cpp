#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	if( !xmlConfig.loadFile("skylight.xml") ) {
		printf("config not found");
		exit();
	}

	// get config values from XML file in ./data
	displayTests = xmlConfig.getValue("test:display", false);
	displayChat = xmlConfig.getValue("chat:display", false);
	cameraUrl = xmlConfig.getValue("camera:url", "http://192.168.0.6/jpeg.cgi?0");
	displayWidth = xmlConfig.getValue("display:width", 1280);
	displayHeight = xmlConfig.getValue("display:height", 1024);
	fontPath = xmlConfig.getValue("font:path", "mono.ttf");

}

//--------------------------------------------------------------
void testApp::update(){
	//ofSoundUpdate();
	//insect.setPan(ofRandomuf());
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0x000000);


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
void testApp::windowResized(int w, int h){

}

