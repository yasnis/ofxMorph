#include "ofMain.h"
#include "ofxMorphUtility.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{
    //--------------------------------------------------------------
    ofxMorphUtility image01;
    ofxMorphUtility image02;
    
    bool debugFlag = true;
    
    ofxDatGui* gui;
    ofxDatGuiFolder* gui_01;
    ofxDatGuiFolder* gui_02;
    ofxDatGuiFolder* gui_preview;
    
    ofxDatGuiButton* add_01;
    ofxDatGuiButton* generate_01;
    ofxDatGuiButton* save_01;
    ofxDatGuiButton* save_02;
    ofxDatGuiButton* load_01;
    ofxDatGuiButton* load_02;
    ofxDatGuiButton* reset_01;
    ofxDatGuiButton* play;
    
    float _morph = 0.0;
    ofFbo fbo;
    
    ofParameter<ofVec3f> test;
    
    void setup(){
        ofBackground(0);
        ofSetFrameRate(60);
        ofSetVerticalSync(true);
        
        image01.setup("01.jpg");
        image02.setup("02.jpg");
        image01.setOffset(ofVec2f(10, 10));
        image02.setOffset(ofVec2f(420, 10));
        fbo.allocate(max(image01.getWidth(), image02.getWidth()), max(image01.getHeight(), image02.getHeight()));
        initGUI();
    }
    
    
    void initGUI() {
        gui = new ofxDatGui();
        
        vector<string> opts = {"Setting - 01", "Setting - 02", "Preview"};
        add_01 = gui -> addButton("Add Marker");
        generate_01 = gui -> addButton("Generate Mesh");
        save_01 = gui -> addButton("Save 01");
        save_02 = gui -> addButton("Save 02");
        load_01 = gui -> addButton("Load 01");
        load_02 = gui -> addButton("Load 02");
        
        reset_01 = gui -> addButton("Reset Marker");
        
        reset_01->setLabelColor(ofColor(128));
        gui -> addBreak();
//        play = gui -> addButton("Play Morph");
        ofxDatGuiSlider *slider = gui -> addSlider("Morph Value", 0.0, 1.0);
        slider->setValue(0);
        
        gui -> onButtonEvent(this, &ofApp::onButtonDown);
        gui -> onSliderEvent(this, &ofApp::onSliderChange);
        
        gui->setPosition(420, 420);
    }
    
    //--------------------------------------------------------------
    void update(){
        
        image01.update();
        image02.update();
        
        vector<ofVec3f> _src = image01.getDefaultVerticies();
        vector<ofVec3f> _dst = image02.getDefaultVerticies();
        vector<ofVec3f> verts;
        for (int i = 0; i<min(_src.size(), _dst.size()); i++) {
            verts.push_back(_src[i].getInterpolated(_dst[i], _morph));
        }
        image01.setVercities(&verts);
        image02.setVercities(&verts);
    }
    
    //--------------------------------------------------------------
    void draw(){
        image01.draw(debugFlag);
        image02.draw(debugFlag);
        
        fbo.begin();
        ofFill();
        ofSetColor(0, 0, 0, 0);
        ofDrawRectangle(0, 0, 400, 400);
        ofSetColor(255);
        image01.drawNoOffset(debugFlag);
        ofSetColor(255, 255, 255, 255*_morph);
        image02.drawNoOffset(debugFlag);
        ofSetColor(255);
        fbo.end();
        
        fbo.draw(10, 420);
    }
    
    
    void onButtonDown(ofxDatGuiButtonEvent e) {
        if(e.target == add_01) {
            cout << "Add" << endl;
            image01.addMarker();
            image02.addMarker();
        }else if(e.target == generate_01) {
            cout << "Generate" << endl;
            image01.generateMesh();
            image02.generateMesh();
        }else if(e.target == save_01) {
            cout << "Save 01" << endl;
            ofXml xml = image01.toXml();
            ofFileDialogResult result = ofSystemSaveDialog("setting01.xml", "Save Settings");
            if(result.bSuccess) {
                string path = result.getPath();
                xml.save(path);
            }
        }else if(e.target == save_02) {
            cout << "Save 02" << endl;
            ofXml xml = image02.toXml();
            ofFileDialogResult result = ofSystemSaveDialog("setting02.xml", "Save Settings");
            if(result.bSuccess) {
                string path = result.getPath();
                xml.save(path);
            }
        }else if(e.target == load_01) {
            cout << "Load 01" << endl;
            ofFileDialogResult result = ofSystemLoadDialog();
            if(result.bSuccess) {
                string path = result.getPath();
                ofXml xml;
                xml.load(path);
                image01.loadSetting(xml);
            }
        }else if(e.target == load_02) {
            cout << "Load 02" << endl;
            ofFileDialogResult result = ofSystemLoadDialog();
            if(result.bSuccess) {
                string path = result.getPath();
                ofXml xml;
                xml.load(path);
                image02.loadSetting(xml);
            }
        }else if(e.target == reset_01) {
            cout << "Reset 01" << endl;
            image01.reset();
            image02.reset();
        }else if(e.target == play) {
            cout << "Play" << endl;
        }
    }
    
    void onSliderChange(ofxDatGuiSliderEvent e) {
        _morph = e.value;
    }
    
    void keyPressed(int key){
        if(key == 'f'){
            ofToggleFullscreen();
        }else if(key == 'd'){
            debugFlag = !debugFlag;
        }
    }
};

//========================================================================
int main( ){
	ofSetupOpenGL(830,830, OF_WINDOW);
	ofRunApp( new ofApp());
}
