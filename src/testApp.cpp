#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(255, 255, 255);
//    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(256);
    
    font.loadFont("AkkoRoundedPro-Thin.otf", 20);
    font_pos.x = 60;
    font_pos.y = 50;
    
    font_label.loadFont("AkkoRoundedPro-Thin.otf", 30);
    for (int i=0; i < FFTcont; i++) {
        font_label_pos[i].x = 900;
        font_label_pos[i].y = 190 + 180*i;
    }
    
    //    Sync
    sync = 0;
    
//    metronome
    metro.setup(130, ofGetWidth(), ofGetHeight());
    

    //    LoadSets
    
    //    We need exception handling!!!!
    std::stringstream _add;
    
    for (int i=0; i < numSets; i++) {
        ofSoundPlayer temp1;
        _add << "bass" << i << ".wav";
        temp1.loadSound(_add.str());
        temp1.setLoop(true);
        temp1.setMultiPlay(false);
        bass.push_back(temp1);
        _add.str(" ");
        
        ofSoundPlayer temp2;
        _add << "drum" << i << ".wav";
        temp2.loadSound(_add.str());
        temp2.setLoop(true);
        temp2.setMultiPlay(false);
        drum.push_back(temp2);
        _add.str(" ");
        
        ofSoundPlayer temp3;
        _add << "tops" << i << ".wav";
        temp3.loadSound(_add.str());
        temp3.setLoop(true);
        temp3.setMultiPlay(false);
        tops.push_back(temp3);
        _add.str(" ");
        
        ofSoundPlayer temp0;
        _add << "synth" << i << ".wav";
        temp0.loadSound(_add.str());
        temp0.setLoop(true);
        temp0.setMultiPlay(false);
        synth.push_back(temp0);
        _add.str(" ");
    }
    
    //    OSC setup
    received_mes.setup(port);
    
    //UDP
    rec.Create();
    rec.Bind(port);
    rec.SetTimeoutReceive(0.005);

    
}

//--------------------------------------------------------------
void testApp::update(){
//    metronome
    metro.start();
    
    click=0;
    if (metro.getBang() != metro_hist) {
        if (metro_count != 1) {
            metro_count--;
        }else{
        metro_count = 4;
        }
        click = 1;
    }
    metro_hist = metro.getBang();
    std::cout << metro_count << endl;
    
    //    Get OSC Message
    while (received_mes.hasWaitingMessages()) {
        ofxOscMessage m;
        received_mes.getNextMessage(&m);
        
        for (int i=0; i < FFTcont; i++) {
            if (m.getAddress() == "/FFT") {
                fft[i] = m.getArgAsFloat(i);
            }
            
        }
        
    }
    
//    UDP receive
//    rec.Receive(buf_mes, 100);
//    rec_mes = buf_mes;
//    vector<string> strPnt = ofSplitString(rec_mes, "/");
//    
//    for (int i = 0; i < strPnt.size(); i++) {
//        std::cout << strPnt[i].c_str() << endl;
//        fft[i] = atof(strPnt[i].c_str());
//    }

    
    //    normalize
    debug_values.str("");
    sum=0;
    for (int i=0; i < FFTcont; i++) {
        sum = sum + fft[i];
    }
    //    printf("%f \n", sum);
    for (int i=0; i < FFTcont; i++) {
        fft[i] = fft[i] / sum;
        debug_values << fft[i] << endl;
    }//got percentage
    
    //    Threshold detector
    for (int j=0; j < numSets; j++) {
        for (int i=0; i < FFTcont; i++) {
            if (j == 0) {
                fft_activate_hist[i] = fft_activate[i];
            }
            if (fft[i] >= thresh[j] && fft[i] < thresh[j+1]) {
                fft_activate[i] = j;
            }
        }
    }
    
    //    Player
    for (int i=0; i < FFTcont; i++) {
        if (fft_activate[i] != fft_activate_hist[i]) {
            trans_flag[i] = true;
        }
        
        if ((metro_count == 4) && (trans_flag[i] == true) && (click == 1)) {
            trans_flag[i] = false;
            
            for (int j=0; j < FFTcont; j++) {
                std::cout << fft_activate[j];
            }
            std::cout << endl;
            
            if (i == 0) {
                for (int j=0; j < numSets; j++) {
                    bass[j].stop();
                }
                bass[fft_activate[i]].play();
                playing[i] = fft_activate[i];
            }
            if (i == 1) {
                for (int j=0; j < numSets; j++) {
                    drum[j].stop();
                }
                drum[fft_activate[i]].play();
                playing[i] = fft_activate[i];
            }
            if (i==2) {
                for (int j=0; j < numSets; j++) {
                    tops[j].stop();
                }
                tops[fft_activate[i]].play();
                playing[i] = fft_activate[i];
            }
            if (i==3) {
                for (int j=0; j < numSets; j++) {
                    synth[j].stop();
                }
                synth[fft_activate[i]].play();
                playing[i] = fft_activate[i];
            }
            
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 0, 0, 50);
    for (int i=0; i < numSets; i++) {
        for (int j=0; j < FFTcont; j++) {
            if (playing[j]==i) {
                ofSetColor(255, 10, 10, 100);
            }
            ofRect(50 + i*240, 20 + j*180, 210, 161);
            ofSetColor(255, 0, 0, 50);
        }
        
    }
    
    if (click == 1) {
        ofSetColor(255, 0, 0, 155);
        ofCircle(80, 60, metro_count*40);
    }
    
    ofSetColor(255, 0, 0,100);
    font.drawString(debug_values.str(), font_pos.x, font_pos.y);
    
    for (int i=0; i < FFTcont; i++) {
        font_label.drawString(labels[i], font_label_pos[i].x, font_label_pos[i].y);
    }
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
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