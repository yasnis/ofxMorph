//
//  ofxMorphImage.cpp
//  Morphing
//
//  Created by X006819_nishimura on 2016/12/05.
//
//

#include "ofxMorphImage.h"

ofxMorphImage::ofxMorphImage(){
    
}

ofxMorphImage::~ofxMorphImage(){
    
}


/*
 *
 */
void ofxMorphImage::draw() {
    if(_texture.isAllocated() && getNumVertices()>0){
        _texture.bind();
        ofMesh::draw();
        _texture.unbind();
        
    }else{
        _texture.draw(0, 0);    //TODO:座標指定
    }
}

void ofxMorphImage::draw(ofPoint p, bool useOffset) {
    draw(p.x, p.y);
}
void ofxMorphImage::draw(float x, float y, bool useOffset) {
    if(useOffset){
        ofPushMatrix();
        ofTranslate(x, y);
        draw();
        ofPopMatrix();
    }else{
        draw();
    }
}

/*
 * 頂点、テクスチャ座標、インデックスを設定する
 */
void ofxMorphImage::setGeometry(vector<ofVec3f> *verts, vector<ofIndexType> *inds) {
    setDefaultVerticies(verts);
    setIndicies(inds);
}


/*
 * private
 * 基準の頂点を設定する
 * テクスチャ座標や変形も初期化する
 */
void ofxMorphImage::setDefaultVerticies(vector<ofVec3f> *verts) {
    clearVertices();
    clearTexCoords();
    defaultVerticies.clear();
    for (int i = 0; i<verts->size(); i++) {
        ofVec3f v = verts->at(i);
//        cout << v << endl;
        addVertex(v);
        addTexCoord(ofVec2f(v.x, v.y));
        defaultVerticies.push_back(v);
    }
}

/*
 * private
 * メッシュのインデックスを設定する
 */
void ofxMorphImage::setIndicies(vector<ofIndexType> *inds) {
    clearIndices();
    for (int i = 0; i < inds->size(); i++) {
        addIndex(inds->at(i));
    }
}


/*
 * 現在の頂点の位置を設定する(動かす)
 */
void ofxMorphImage::setVercities(vector<ofVec3f> *verts) {
    if(getNumVertices() == 0) return;
    vector<ofVec3f> currentVerts = getVertices();
    int n = min(currentVerts.size(), verts->size());
    for (int i = 0; i<n; i++) {
        setVertex(i, verts->at(i));
    }
}

/*
 * 変形をリセットする
 */
void ofxMorphImage::resetVerticies() {
    clear();
    addVertices(defaultVerticies);
}

/*
 * 基準の頂点を返す
 */
const vector<ofVec3f> ofxMorphImage::getDefaultVerticies(){
    return defaultVerticies;
 }

/*
 * 画像のパスを設定する
 * TODO:パスを設定しておくとシリアライズの時に画像パス込みのxmlを吐き出せるようにする
 */
void ofxMorphImage::setImagePath(string path) {
    if(path.size()>0){
        _imagePath = path;
        ofLoadImage(_texture, _imagePath);
    }
}

/*
 * テクスチャを設定する
 */
void ofxMorphImage::setTexture(ofTexture texture) {
    _texture = texture;
}


/*
 * ofImageからテクスチャを設定する
 */
void ofxMorphImage::setImage(ofImage image) {
    _texture = image.getTexture();
}

/*
 * テクスチャの幅を返す
 */
int ofxMorphImage::getWidth() {
    if(_texture.isAllocated()) {
        return _texture.getWidth();
    }
    return 0;
}

/*
 * テクスチャの高さを返す
 */
int ofxMorphImage::getHeight() {
    if(_texture.isAllocated()) {
        return _texture.getHeight();
    }
    return 0;
}


ofXml ofxMorphImage::toXml() {
    cout << "--  ofxMorphImage::serialize  --" << endl;
    ofXml xml;
    xml.addChild("MorphImage");
    xml.setTo("MorphImage");
    //画像のパス
    xml.addValue("imagePath", _imagePath);
    //頂点情報
    xml.addChild("verticies");
    xml.setTo("verticies");
    for (int i=0; i<defaultVerticies.size(); i++) {
        ofVec3f v = defaultVerticies[i];
        ofXml x;
        x.addChild("vertex");
        x.setTo("vertex");
        x.addValue("x", v.x);
        x.addValue("y", v.y);
        xml.addXml(x);
    }
    xml.setToParent();
    //インデックス情報
    xml.addChild("indicies");
    xml.setTo("indicies");
    for (int i=0; i<getIndices().size(); i++) {
        xml.addValue("index", getIndices()[i]);
    }
    
//    for (int i=0; i<defaultVerticies.size(); i++) {
//        cout << defaultVerticies[i] << endl;
//    }
//    for (int i=0; i<getIndices().size(); i++) {
//        cout << getIndices()[i] << endl;
//    }
    return xml;
    
}

void ofxMorphImage::loadSetting(ofXml xml) {
    if(xml.exists("imagePath")) {
        string path = xml.getValue<string>("//imagePath");
        setImagePath(path);
    }
    vector<ofVec3f> _verticies;
    vector<ofIndexType> _indicies;
    if(xml.exists("verticies") && xml.exists("indicies") ){
        //頂点情報
        xml.setTo("verticies");
        if(xml.getName() == "verticies" && xml.setTo("vertex[0]")){
            do {
                float x = xml.getValue<float>("x");
                float y = xml.getValue<float>("y");
                _verticies.push_back(ofVec3f(x, y));
            }while (xml.setToSibling());
        }
        xml.setToParent(2);
        
        //インデックス情報
        xml.setTo("indicies");
        if(xml.getName() == "indicies" && xml.setTo("index[0]")){
            do {
                _indicies.push_back(xml.getIntValue());
            }while (xml.setToSibling());
        }
    }
    setGeometry(&_verticies, &_indicies);
    
//    for (int i=0; i<_verticies.size(); i++) {
//        cout << _verticies[i] << endl;
//    }
//    for (int i=0; i<_indicies.size(); i++) {
//        cout << _indicies[i] << endl;
//    }
}