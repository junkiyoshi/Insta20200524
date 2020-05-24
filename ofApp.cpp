#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(0.5);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	
	this->noise_seed = ofRandom(1000);
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	auto base_seed = ofRandom(10000);
	for (int k = 0; k < 600; k++) {

		auto base_x = ofRandom(-100, ofGetWidth() + 100);
		auto base_location = glm::vec2(base_x, ofMap(ofNoise(base_seed, base_x * 0.003 + this->noise_seed), 0, 1, 100, ofGetHeight() - 100));

		auto location = base_location + glm::vec2(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0025), 0, 1, -60, 60), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0025), 0, 1, -120, 120));

		this->face.addVertex(glm::vec3(location, 0));
		this->frame.addVertex(glm::vec3(location, 0));

		ofColor color;
		color.setHsb((int)ofMap(location.x, 0, ofGetWidth(), 240, 240 + 255) % 255, 255, 255);

		this->face.addColor(ofColor(color, 64));
		this->frame.addColor(color);

	}

	for (int i = 0; i < this->face.getNumVertices(); i++) {

		vector<int> near_index_list;
		auto location = this->face.getVertex(i);
		for (int k = 0; k < this->face.getNumVertices(); k++) {

			auto other = this->face.getVertex(k);
			auto distance = glm::distance(location, other);

			if (distance < 30) {

				near_index_list.push_back(k);
			}
		}

		if (near_index_list.size() > 3) {

			for (int k = 0; k < near_index_list.size() - 2; k++) {

				this->face.addIndex(i);
				this->face.addIndex(near_index_list[k]);
				this->face.addIndex(near_index_list[k + 1]);

				this->frame.addIndex(i); this->frame.addIndex(near_index_list[k]);
				this->frame.addIndex(i); this->frame.addIndex(near_index_list[k + 1]);
			}
		}
	}

	if (ofGetFrameNum() % 120 < 80) {

		this->noise_seed += ofMap(ofGetFrameNum() % 120, 0, 80, 0.03, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->face.draw();
	this->frame.drawWireframe();

	for (int i = 0; i < this->frame.getNumVertices(); i++) {

		ofSetColor(this->frame.getColor(i));
		ofDrawCircle(this->frame.getVertex(i), 2);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}