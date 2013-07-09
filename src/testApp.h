#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSimpleMetronome.h"
#include "ofxNetwork.h"  

#define numSets 4
#define port 11000
#define FFTcont 4

#define NUM_MSG_STRINGS 20

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector <ofSoundPlayer> bass;
    vector <ofSoundPlayer> drum;
    vector <ofSoundPlayer> tops;
    vector <ofSoundPlayer> synth;
    
    int sync;
    int click;
    
    std::stringstream numFFT;
    float sum;
    bool metro_hist = false;
    int metro_count = 4;
    
    void loop_trans(ofSoundPlayer from, ofSoundPlayer to);
    
    ofTrueTypeFont font;
    ofTrueTypeFont font_label;
    ofPoint font_pos;
    ofPoint font_label_pos[FFTcont];
    string labels[FFTcont] = {"bass", "drum", "tops", "synth"};
    std::stringstream debug_values;
    
    ofxMetronome metro;
    
    ofxUDPManager rec;
    char buf_mes[100];
    string rec_mes;
    
    int playing[numSets] = {0};
    
private:
    ofxOscReceiver received_mes;
    float fft[FFTcont] = {0};
    int fft_activate[FFTcont] = {0};
    int fft_activate_hist[FFTcont] = {0};
    float thresh[6] = {0.0, 0.15, 0.3, 0.4, 0.7, 1};
    bool trans_flag[FFTcont] = {false};
};