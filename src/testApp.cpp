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

	displayTests = xmlConfig.getValue("display:test", false);
	displayChat = xmlConfig.getValue("display:chat", false);
	cameraUrl = xmlConfig.getValue("camera:url", "http://192.168.0.6/jpeg.cgi?0");
	captureFreq = xmlConfig.getValue("camera:freq", 500);
	storagePath = xmlConfig.getValue("storage:path", "/Volumes/brick2/skylight");

	displayWidth = xmlConfig.getValue("display:width", 1280);
	displayHeight = xmlConfig.getValue("display:height", 1024);
	fontPath = xmlConfig.getValue("font:path", "mono.ttf");

	// load font
	font.loadFont(fontPath, 12);

	//videoPlayer.loadMovie("movies/ironsand.mov");
	//videoPlayer.play();
	
	// directory listing
	dir.allowExt("jpg");
	dir.setVerbose(true);
	
	int numFiles = dir.listDir("/Volumes/brick2/skylight/images/2011/11/27/15/");
	for(int i = 0; i < numFiles; i++){
		cout<<"name: "<<dir.getName(i)<<" dir: "<<dir.getPath(i)<<"\n";
	}
	
	// initialise imageSequence
	sequence.loadSequence("/Volumes/brick2/skylight/images/2011/11/27/15/ironsand", "jpg", 1, 80, 4);
	sequence.preloadAllFrames();
	sequence.setFrameRate(4);
	
	
	
	// set bg to black
	ofBackground(0,0,0);
	
	
	// initial values
	time = 0;
	networkCapture = true;
	ofSetFrameRate(25);
	
}

//--------------------------------------------------------------
void testApp::update(){
	videoPlayer.idleMovie();
	/* TODO captureFreq relates to how often update() is called (ie framerate) but
	   should be related to time (elapsed millis) so it is the same frequency
	   regardless of a fast or slow computer */
	if(time%captureFreq==0 && networkCapture) {

		// for this to work you need to set finder permissions for the user to read/write on the external drive path
		sprintf(path, "%s/images/%i/%i/%i/%i", storagePath.c_str(), ofGetYear(),ofGetMonth(),ofGetDay(), ofGetHours() );
		//printf("%s\n",buffer);
		
		sprintf(buffer, "mkdir -p %s", path);
		//printf("%s\n",buffer);
		// TODO this should probably done in a non-blocking way
		// eg ofDirectory using of007
		system(buffer);
		
		// loads a file from a url and saves it with a specific name
		// the resulting file can be loaded into a ofImage for display
		sprintf(imgPath, "%s/sky-%02i-%02i.jpg", path, ofGetMinutes(), ofGetSeconds());
		if(imageLoader.loadFromUrl(cameraUrl,(string)imgPath)) {
			printf("%s\n", imgPath);
		}
		
	}
	

	time++;
		
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);

	//videoPlayer.draw(0,0,displayWidth,displayHeight);

	sequence.getFrame(time%sequence.getTotalFrames())->draw(0, 0);

	string str = ofToString(ofGetFrameRate(), 0)+"fps";
	font.drawString(str, 5, 18);
	

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

