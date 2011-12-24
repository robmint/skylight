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
	fullscreen = xmlConfig.getValue("config:display:fullscreen", true);
	
	startHour = xmlConfig.getValue("config:camera:starthour", 6);
	endHour = xmlConfig.getValue("config:camera:endhour", 21);
	
	newSequence = xmlConfig.getValue("config:display:newsequence", 600);
	
	networkCapture	= xmlConfig.getValue("config:camera:networkcapture", true);
	webcamCapture = xmlConfig.getValue("config:camera:webcamcapture", false);
	
	cameraUrl = xmlConfig.getValue("config:camera:url", "http://createrevolutionaryart.net/wp-content/uploads/2011/12/Screen-shot-2011-12-07-at-8.31.48-AM.jpg");
	captureFreq = xmlConfig.getValue("config:camera:freq", 500);
	storagePath = xmlConfig.getValue("config:storage:path", "./images");
	
	displayWidth = xmlConfig.getValue("config:display:width", 1280);
	displayHeight = xmlConfig.getValue("config:display:height", 1024);
	
	fontPath = xmlConfig.getValue("config:font:path", "mono.ttf");
	
	
	// display settings
	ofSetWindowShape(displayWidth, displayHeight);
	ofSetFullscreen(fullscreen);
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);
	ofSetFrameRate(25);
	
	// load font
	font.loadFont(fontPath, 12);
	
	// frame grabber
	oldPixels = new unsigned char[camWidth*camHeight*3];
	newPixels = new unsigned char[camWidth*camHeight*3];
	
	// TODO make into a config option
	sequenceFrame = 0;
	framesPerImage = 1;
	
	// texture buffer
	bufferSize = 20;
	texture.resize(bufferSize);
	
	// set bg to black
	ofBackground(0,0,0);
	
	
	// initial values
	diff = 0;
	diffMin = 10000;
	diffMax = 20000;
	time = 0;
	message = "";
	debug = false;
	
	
	// get list of sequences - currently 1 hour time frame
	updateSequenceList();
	
}

//--------------------------------------------------------------
void testApp::update(){
	/* TODO captureFreq relates to how often update() is called (ie framerate) but
	 should be related to time (elapsed millis) so it is the same frequency
	 regardless of a fast or slow computer */
	
	// look for movement
	camera.grabFrame();
	if(camera.isFrameNew()) {
		newPixels = camera.getPixels();
		
		for(int i=0; i< camWidth*camHeight*3;i+=3) {
			int newluma = (0.2126*newPixels[i]) + (0.7152*newPixels[i+1]) + (0.0722*newPixels[i+2]);
			int oldluma = (0.2126*oldPixels[i]) + (0.7152*oldPixels[i+1]) + (0.0722*oldPixels[i+2]);
			
			diff += abs(newluma-oldluma);
			//if(debug) printf("a: %i b: %i c: %i\n",a,b,c);
			//printf("a: %i\n",a);
		}
		//diff = abs(diff);
		if(diff<diffMin) diffMin = diff;
		if(diff>diffMax) diffMax = diff;
		
		memcpy(oldPixels,newPixels,camWidth*camHeight*3);
		diff = ofMap(diff, 0, 9300000, 0, 1);
		//message = "diff: "+ofToString(diff)+" diffMin: "+ofToString(diffMin)+" diffMax: "+ofToString(diffMax);
		//		cout<<"diff: "<<(diff*100)<<"\n";
	}
	
	
	// check time 
	if(ofGetHours()>=startHour && ofGetHours()<=endHour) {
		// time to capture?
		if(time%captureFreq==0) {
			// net cam capture on?
			if(networkCapture) {
				// for this to work you need to set finder permissions for the user to read/write on the external drive path
				sprintf(path, "%s/images/%i-%02i-%02i-%02i", storagePath.c_str(), ofGetYear(),ofGetMonth(),ofGetDay(), ofGetHours() );
				
				sprintf(buffer, "mkdir -p %s", path);
				
				// TODO this should probably done in a non-blocking way
				// eg ofDirectory using of007
				system(buffer);
				
				// loads a file from a url and saves it with a specific name
				// the resulting file can be loaded into a ofImage for display
				sprintf(imgPath, "%s/sky-%02i-%02i.jpg", path, ofGetMinutes(), ofGetSeconds());
				
				if(ofSaveURLAsync(cameraUrl,imgPath)) {
					message = "Network camera capture: OK";
					shutter = 255;
				} else {
					message = "Network camera capture: Fail";
				}
			} 		
		}
	}
	
	if(message.empty()) { message = "time: "+ofToString(time); }
	
	if(time%newSequence==0) {
		updateSequenceList();
	}
	
	time++;
	
	if(shutter>8) shutter-=8;
	
	
}


//--------------------------------------------------------------
void testApp::updateSequenceList() {
	
	
	sequenceDir.clear();
	
	// directory listing
	//dir.allowExt("*");
	dir.allowExt("");	
	int numFiles = dir.listDir(storagePath+"/images");
	if(numFiles==0) { 
		cout<<"ERROR: no dirs found in "<<storagePath<<"/images\n";
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
	currentImageDirName = sequenceDir[r];
	
	
	// file listing
	dir.allowExt("jpg");	
	numFiles = dir.listDir(sequenceDir[r]);
	// TODO should not die if no files found
	if(numFiles!=0) { 
		files.clear();
		// push the file names into a vector of strings
		for(int i = 0; i < numFiles; i++){
			files.push_back(dir.getName(i));
		}
		sequenceLength = numFiles;
		
	}
	newSequence	= ofMap(diff*100, 0, 5, 800,25,true);
	message = "newSequence: "+ofToString(newSequence);
}


//--------------------------------------------------------------
void testApp::draw(){
	if(sequenceLength==0) return;
	
	if(time%framesPerImage==0) {
		ofBackground(0,0,0);
		
		int f = files.size();
		if(f>0) {
			int p = time%(files.size());
			//cout<<"file index:"<<p<<" of "<<files.size()<<" "<<files[p]<<"\n";
			texture.clear();
			texture.push_back(ofTexture());
			currentImageFileName = files[time%files.size()];
			if(loader.loadImage(currentImageDirName+"/"+currentImageFileName) ) {
				texture[0].allocate( loader.getWidth(), loader.getHeight(), imageTypeToGLType(loader.type) );				
				texture[0].loadData( loader.getPixels(), loader.getWidth(), loader.getHeight(), imageTypeToGLType(loader.type) );
				texture[0].draw(xpos, ypos, texture[0].getWidth()*xscale, texture[0].getHeight()*yscale );
			} else {
				cout<<"ERROR: couldn't load image\n";
			}
			
		}
	}
	
	if(osd) {
		camera.draw(200,200);
		string str = ofToString(ofGetFrameRate(), 0)+"fps";
		font.drawString(str, 5, 18);
		font.drawString(message, 5, 600);
	}
	
	
	// draw instruments
	ofPushMatrix();
	ofTranslate(60,60,0);
	ofScale(xscale,yscale,1);
	
	// shutter
	ofFill();
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,shutter);
	ofEllipse(0,0,8,8);
	
	// activity
	ofSetColor(255,255,255,64);
	ofFill();
	ofSetLineWidth(0);
	ofRect(20, -4, diff*200, 8);
	
	// sequence
	ofRect(-4, 20, 9, 100);
	// BUG where sequenceLength=0 (external disk not plugged in)
	float sy = ofMap(time%sequenceLength, 0, sequenceLength, 0, 100);
	ofColor(0,255,255,255);
	ofSetLineWidth(1);
	ofLine(-4,sy+20,5,sy+20);
	
	
	ofDisableAlphaBlending();	
	ofPopMatrix();
	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key=='p') {
		message = "Play new sequence";
		updateSequenceList();
	}
	
	if(key=='z') {
		debug = !debug;
	}
	
	if(key=='d') {
		osd = !osd;
		message = "";
		xmlConfig.setValue("config:display:osd", osd, 0);
	}
	
	if(key=='f') {
		fullscreen = !fullscreen;
		ofSetFullscreen(fullscreen);
		xmlConfig.setValue("config:display:fullscreen", fullscreen, 0);
		
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
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}


int testApp::imageTypeToGLType(int imageType)
{
	switch (imageType) {
		case OF_IMAGE_GRAYSCALE:
			return GL_LUMINANCE;
		case OF_IMAGE_COLOR:
			return GL_RGB;
		case OF_IMAGE_COLOR_ALPHA:
			return GL_RGBA;
		default:
			ofLog(OF_LOG_ERROR, "ofxImageSequence - unsupported image type for image");
			break;
	}
}
