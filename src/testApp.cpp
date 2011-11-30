#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	camWidth = 320;
	camHeight = 240;
	
//	camera.videoSettings();
//	camera.listDevices();
//	camera.videoSettings();
	
//	camera.setVerbose(true);
	camera.initGrabber(camWidth,camHeight);
	
	// get config values from XML file in ./data
	if( !xmlConfig.loadFile("skylight.xml") ) {
		printf("config not found");
		exit();
	}

	osd = xmlConfig.getValue("config:display:osd", false);
	
	xpos = xmlConfig.getValue("config:display:xpos", 0);
	ypos = xmlConfig.getValue("config:display:ypos", 0);
	xscale = xmlConfig.getValue("config:display:xscale", 1.0f);
	yscale = xmlConfig.getValue("config:display:yscale", 1.0f);
	lockAspect = xmlConfig.getValue("config:display:lockaspect", true);

	startHour = xmlConfig.getValue("config:camera:starthour", 6);
	endHour = xmlConfig.getValue("config:camera:endhour", 21);

	newSequence = xmlConfig.getValue("config:display:newsequence", 600);
	
	networkCapture	= xmlConfig.getValue("config:camera:networkcapture", true);
	webcamCapture = xmlConfig.getValue("config:camera:webcamcapture", false);
	
	cameraUrl = xmlConfig.getValue("config:camera:url", "http://192.168.0.6/jpeg.cgi?0");
	captureFreq = xmlConfig.getValue("config:camera:freq", 500);
	storagePath = xmlConfig.getValue("config:storage:path", "/Volumes/skylight");

	displayWidth = xmlConfig.getValue("config:display:width", 1280);
	displayHeight = xmlConfig.getValue("config:display:height", 1024);
	
	fontPath = xmlConfig.getValue("config:font:path", "mono.ttf");

	
	// display size
	ofSetWindowShape(displayWidth, displayHeight);
	
	// load font
	font.loadFont(fontPath, 12);
	
	seqPath = "/images/2011-11-30-13";
	
	
	// get list of sequences - currently 1 hour time frame
	updateSequenceList(storagePath);

	// set bg to black
	ofBackground(0,0,0);
	
	// initial values
	time = 0;
	message = "";
	ofSetFrameRate(30);
	
	// start thread
	HttpThread.initAndSleep();
	

}

void testApp::updateSequenceList( string s ) {
	
	sequenceDir.clear();
	
	// directory listing
	dir.allowExt("*");	
	int numFiles = dir.listDir(s+"/images");
	if(numFiles==0) { 
		cout<<"ERROR: no dirs found in "<<s<<"/images\n";
		return;
	}
	// push the file names into a vector of strings
	for(int i = 0; i < numFiles; i++){
		sequenceDir.push_back(dir.getPath(i));
	}
	message = "Sequence update: "+ofToString(numFiles)+" sequences available\n";
	
	int size = sequenceDir.size();
	int r = ofRandom(0,size);
	message += sequenceDir[r]+"\n";
	
	files.clear();
	
	// file listing
	dir.allowExt("jpg");	
	numFiles = dir.listDir(sequenceDir[r]);
	if(numFiles==0) { 
		cout<<"ERROR: no files found in "<<sequenceDir[r];
		exit();
	}
	
	// hard limit for testing
	if(numFiles>120) numFiles=120;
	
	// push the file names into a vector of strings
	for(int i = 0; i < numFiles; i++){
		files.push_back(dir.getPath(i));
	}
	
	// initialise imageSequence
	sequence.loadSequence(files);
//	sequence.preloadAllFrames();
//	sequence.setFrameRate(25);
	
}

//--------------------------------------------------------------
void testApp::update(){
	/* TODO captureFreq relates to how often update() is called (ie framerate) but
	   should be related to time (elapsed millis) so it is the same frequency
	   regardless of a fast or slow computer */
	
	// check time 
	if(ofGetHours()>=startHour && ofGetHours()<=endHour) {
		if(time%captureFreq==0) {
			if(networkCapture) {
				// for this to work you need to set finder permissions for the user to read/write on the external drive path
				sprintf(path, "%s/images/%i-%i-%i-%i", storagePath.c_str(), ofGetYear(),ofGetMonth(),ofGetDay(), ofGetHours() );
				
				sprintf(buffer, "mkdir -p %s", path);
				
				// TODO this should probably done in a non-blocking way
				// eg ofDirectory using of007
				system(buffer);
				
				// loads a file from a url and saves it with a specific name
				// the resulting file can be loaded into a ofImage for display
				sprintf(imgPath, "%s/sky-%02i-%02i.jpg", path, ofGetMinutes(), ofGetSeconds());
				
				message = "Trigger: Network camera capture";
				HttpThread.imageLoader = imageLoader;
				HttpThread.cameraUrl = cameraUrl;
				HttpThread.imgPath = imgPath;
				HttpThread.updateOnce();
				
			} else if (webcamCapture) {
				camera.grabFrame();
				sprintf(path, "%s/images/%i-%i-%i-%i", storagePath.c_str(), ofGetYear(),ofGetMonth(),ofGetDay(), ofGetHours() );
				sprintf(buffer, "mkdir -p %s", path);
				system(buffer);
				sprintf(imgPath, "%s/sky-%02i-%02i.jpg", path, ofGetMinutes(), ofGetSeconds());
				
				webcam.grabScreen(20,20,camWidth, camHeight);
				p = imgPath;
				webcam.saveImage(p);
				message = "Save: Webcam capture";

			}
			
		}
	}
	
	if(time%newSequence==0) {
		updateSequenceList(storagePath);
		
	}
	
	time++;
		
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);


	frameTexture = sequence.getFrame(time%sequence.getTotalFrames());
	frameTexture->draw(xpos, ypos, frameTexture->getWidth()*xscale, frameTexture->getHeight()*yscale );

	if(osd) {
		camera.draw(20,20);
		string str = ofToString(ofGetFrameRate(), 0)+"fps";
		font.drawString(str, 5, 18);
		font.drawString(message, 5, 600);
	}
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if(key=='p') {
		message = "Play new sequence";
		
	}

	if(key=='d') {
		osd = !osd;
		message = "";
		xmlConfig.setValue("config:display:osd", osd, 0);
	}
	
	if(key=='f') {
		fullscreen = !fullscreen;
		ofSetFullscreen(fullscreen);
	}
	
	if(key=='h') {
		if(message=="") {
			message = "Help: [s]ave config, lock[a]spect, [arrows] position, [shift+arrows] xy scale\n";
			message += "Network [c]apture, [w]ebcam capture, [f]ullscreen, on screen [d]isplay";
		} else { message = ""; }
		osd = true;
	}

	if(key=='s') {
		xmlConfig.saveFile("skylight.xml");
		message = "Saved config file";
	}
	
	if(key=='c') {
		networkCapture = !networkCapture;
		message = networkCapture ? "Network capture: on" : "Network capture: off";
		xmlConfig.setValue("config:camera:networkcapture", networkCapture, 0);
	}

	if(key=='w') {
		webcamCapture = !webcamCapture;
		message = webcamCapture ? "webcamCapture: on" : "webcamCapture: off";
		xmlConfig.setValue("config:camera:webcamcapture", webcamCapture, 0);
	}
	
	

	if(key=='a') {
		lockAspect = !lockAspect;
		message = lockAspect ? "Lock aspect: on" : "Lock aspect: off";
		xmlConfig.setValue("config:display:lockaspect", lockAspect, 0);

	}
	
	if(key==OF_KEY_RIGHT) {
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {  
			xscale+=0.1;
			if(lockAspect) yscale+=0.1;
			xmlConfig.setValue("config:display:xscale", xscale);
			xmlConfig.setValue("config:display:yscale", yscale, 0);
			message = "xscale="+ofToString(xscale);
		} else {
			xpos++;
			xmlConfig.setValue("config:display:xpos", xpos, 0);
		}
	}
	
	if(key==OF_KEY_LEFT) {
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {  
			xscale-=0.1;
			if(lockAspect) yscale-=0.1;
			xmlConfig.setValue("config:display:xscale", xscale, 0);
			xmlConfig.setValue("config:display:yscale", yscale, 0);
			message = "xscale="+ofToString(xscale);
		} else {
			xpos--;
			xmlConfig.setValue("config:display:xpos", xpos, 0);
		}
	}
	
	if(key==OF_KEY_UP) {
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {  
			yscale-=0.1;
			if(lockAspect) xscale-=0.1;
			xmlConfig.setValue("config:display:xscale", xscale, 0);
			xmlConfig.setValue("config:display:yscale", yscale, 0);
			message = "yscale="+ofToString(yscale);
			
		} else {
			ypos--;
			xmlConfig.setValue("config:display:ypos", ypos, 0);
		}
	}
	
	if(key==OF_KEY_DOWN) {
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {  
			yscale+=0.1;
			if(lockAspect) xscale+=0.1;
			xmlConfig.setValue("config:display:xscale", xscale, 0);
			xmlConfig.setValue("config:display:yscale", yscale, 0);
			message = "yscale="+ofToString(yscale);
		} else {
			ypos++;
			xmlConfig.setValue("config:display:ypos", ypos, 0);
		}
	}
	
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

