#include "ofMain.h"
#include "ofxMorphUtility.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{
    //--------------------------------------------------------------
    ofxMorphUtility image01;
    ofxMorphUtility image02;
    
    bool debugFlag = true;
    int currentMode = 0;
    
    ofxDatGui* gui;
    ofxDatGuiFolder* gui_01;
    ofxDatGuiFolder* gui_02;
    ofxDatGuiFolder* gui_preview;
    
    ofxDatGuiButton* add_01;
    ofxDatGuiButton* add_02;
    ofxDatGuiButton* copy_01;
    ofxDatGuiButton* copy_02;
    ofxDatGuiButton* generate_01;
    ofxDatGuiButton* generate_02;
    ofxDatGuiButton* save_01;
    ofxDatGuiButton* save_02;
    ofxDatGuiButton* load_01;
    ofxDatGuiButton* load_02;
    ofxDatGuiButton* reset_01;
    ofxDatGuiButton* reset_02;
    ofxDatGuiButton* play;
    
    float _morph = 0.2;
    ofFbo fbo;
    
    ofParameter<ofVec3f> test;
    
    void setup(){
        ofBackground(0);
        ofSetFrameRate(60);
        ofSetVerticalSync(true);
        
        image01.setup("01.png");
        image02.setup("02.png");
        fbo.allocate(max(image01.getWidth(), image02.getWidth()), max(image01.getHeight(), image02.getHeight()));
        initGUI();
        
//        ofXml xml;
//        xml.addChild("test");
//        xml.setTo("test");
////        xml.addChild("10");
//        xml.addValue("v", ofToString(10));
//        xml.save("test.xml");
//        ofBaseSerializer
////        xml.serialize(test);
////        cout << test.isSerializable() << endl;
////        xml.addValue(test, "test");
////        test.getValue();
//        xml.serialize(test);
//        cout << &xml << endl;
    }
    
    
    void initGUI() {
        gui = new ofxDatGui();
        
        vector<string> opts = {"Setting - 01", "Setting - 02", "Preview"};
        ofxDatGuiDropdown* modeSelect = gui->addDropdown("Select Current Mode", opts);
        modeSelect->setBackgroundColor(ofColor(64));
        modeSelect->setStripeColor(ofColor(196));
        modeSelect->select(0);
        gui->addBreak();
        
        gui_01 = gui->addFolder("Setting - 01");
        gui_01 -> setStripeColor(ofColor(128, 0, 0));
        add_01 = gui_01 -> addButton("Add Marker");
        copy_01 = gui_01 -> addButton("Copy Geometry from 02");
        generate_01 = gui_01 -> addButton("Generate Mesh");
        save_01 = gui_01 -> addButton("Save Settings");
        load_01 = gui_01 -> addButton("Load Settings");
        reset_01 = gui_01 -> addButton("Reset Marker");
        reset_01->setLabelColor(ofColor(128));
        gui_01 -> addBreak();
        
        gui_02 = gui->addFolder("Setting - 02");
        gui_02 -> setStripeColor(ofColor(0, 128, 0));
        add_02 = gui_02 -> addButton("Add Marker");
        copy_02 = gui_02 -> addButton("Copy Geometry from 01");
        generate_02 = gui_02 -> addButton("Generate Mesh");
        save_02 = gui_02 -> addButton("Save Settings");
        load_02 = gui_02 -> addButton("Load Settings");
        reset_02 = gui_02 -> addButton("Reset Marker");
        reset_02->setLabelColor(ofColor(128));
        gui_02 -> addBreak();
        
        gui_preview = gui->addFolder("Preview");
        gui_preview -> setStripeColor(ofColor(0, 0, 128));
        play = gui_preview -> addButton("Play Morph");
        gui_preview -> addSlider("Morph Value", 0.0, 1.0);
        
        modeSelect->onDropdownEvent(this, &ofApp::onModeSelectEvent);
        gui -> onButtonEvent(this, &ofApp::onButtonDown);
        gui -> onSliderEvent(this, &ofApp::onSliderChange);
        
        gui_01 -> expand();
        gui_02 -> expand();
        gui_preview -> expand();
        
    }
    
    //--------------------------------------------------------------
    void update(){
        gui->setPosition(ofGetWindowWidth()-gui->getWidth()-15, 15);
        if(currentMode == 0) {
            image01.update();
        } else if(currentMode == 1) {
            image02.update();
        } else {
            vector<ofVec3f> _src = image01.getDefaultVerticies();
            vector<ofVec3f> _dst = image02.getDefaultVerticies();
            vector<ofVec3f> verts;
            for (int i = 0; i<min(_src.size(), _dst.size()); i++) {
                verts.push_back(_src[i].getInterpolated(_dst[i], _morph));
            }
            image01.setVercities(&verts);
            image02.setVercities(&verts);
        }
    }
    
    //--------------------------------------------------------------
    void draw(){
        if(currentMode == 0) {
            image01.draw(debugFlag);
        } else if(currentMode == 1) {
            image02.draw(debugFlag);
        } else {
            fbo.begin();
            ofSetColor(255, 255, 255, 255*(1-_morph));
            image01.draw(debugFlag);
            ofSetColor(255, 255, 255, 255*_morph);
            image02.draw(debugFlag);
            ofSetColor(255);
            fbo.end();
            fbo.draw(100, 100, 400, 400);
        }
    }
    
    
    void onButtonDown(ofxDatGuiButtonEvent e) {
        if(e.target == add_01 && currentMode == 0) {
            cout << "Add 01" << endl;
            image01.addMarker();
        }else if(e.target == add_02 && currentMode == 1) {
            cout << "Add 02" << endl;
            image02.addMarker();
        }else if(e.target == copy_01 && currentMode == 0) {
            cout << "Copy 01" << endl;
            image01.copyFrom(&image02);
        }else if(e.target == copy_02 && currentMode == 1) {
            cout << "Copy 02" << endl;
            image02.copyFrom(&image01);
        }else if(e.target == generate_01 && currentMode == 0) {
            cout << "Generate 01" << endl;
            image01.generateMesh();
        }else if(e.target == generate_02 && currentMode == 1) {
            cout << "Generate 02" << endl;
            image02.generateMesh();
        }else if(e.target == save_01 && currentMode == 0) {
            cout << "Save 01" << endl;
            ofXml xml = image01.toXml();
            ofFileDialogResult result = ofSystemSaveDialog("setting01.xml", "Save Settings");
            if(result.bSuccess) {
                string path = result.getPath();
                xml.save(path);
            }
        }else if(e.target == save_02 && currentMode == 1) {
            cout << "Save 02" << endl;
            ofXml xml = image02.toXml();
            ofFileDialogResult result = ofSystemSaveDialog("setting02.xml", "Save Settings");
            if(result.bSuccess) {
                string path = result.getPath();
                xml.save(path);
            }
        }else if(e.target == load_01 && currentMode == 0) {
            cout << "Load 01" << endl;
            ofFileDialogResult result = ofSystemLoadDialog();
            if(result.bSuccess) {
                string path = result.getPath();
                ofXml xml;
                xml.load(path);
                image01.loadSetting(xml);
            }
        }else if(e.target == load_02 && currentMode == 1) {
            cout << "Load 02" << endl;
            ofFileDialogResult result = ofSystemLoadDialog();
            if(result.bSuccess) {
                string path = result.getPath();
                ofXml xml;
                xml.load(path);
                image02.loadSetting(xml);
            }
        }else if(e.target == reset_01 && currentMode == 0) {
            cout << "Reset 01" << endl;
            image01.reset();
        }else if(e.target == reset_02 && currentMode == 1) {
            cout << "Reset 02" << endl;
            image02.reset();
        }else if(e.target == play && currentMode == 2) {
            cout << "Play" << endl;
        }
    }
    
    void onSliderChange(ofxDatGuiSliderEvent e) {
        if(currentMode != 2) return;
        _morph = e.value;
    }
    
    void onModeSelectEvent(ofxDatGuiDropdownEvent e) {
        currentMode = e.child;
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
	ofSetupOpenGL(900,600, OF_WINDOW);
	ofRunApp( new ofApp());
}
