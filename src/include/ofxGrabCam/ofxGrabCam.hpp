//
//  ofxGrabCam.hpp
//  of9
//
//  Created by Esteban on 31/05/23.
//
#pragma once

#ifndef ofxGrabCam_hpp
#define ofxGrabCam_hpp

#include <stdio.h>

//
//  ofxGrabCam.h
//  ofxGrabCam
//
//  Created by Elliot Woods on 10/11/2011.
//    http://www.kimchiandchips.com
//

#include "ofRectangle.h"
#include "ofPixels.h"
#include "ofCamera.h"
#include "ofGraphics.h"
#include "ofVectorMath.h"

class ofxGrabCam : public ofCamera {
public:
    ofxGrabCam();
    virtual ~ofxGrabCam();
    
    void    begin(ofRectangle viewport = ofGetCurrentViewport());
    void    end(); ///< Overrides virtual end
    void    reset();
    
    void                updateCursorWorld();
    const glm::vec3 &    getCursorWorld() const;
    glm::vec3            getCursorProjected() const; // note we use this in findCursor
    
    void    setCursorDrawEnabled(bool);
    bool    getCursorDrawEnabled() const;
    void    toggleCursorDrawEnabled();

    void    setCursorDrawSize(float);
    float    getCursorDrawSize() const;
    
    void    setMouseActionsEnabled(bool);
    bool    getMouseActionsEnabled() const;
    void    toggleMouseActionsEnabled();
    
    void    setFixUpDirectionEnabled(bool);
    bool    getFixUpDirectionEnabled() const;
    void    toggleFixUpDirectionEnabled();
    
    void    setTrackballRadius(float);
    float    getTrackballRadius() const;
    
    void    setListenersEnabled(bool);
    bool    getListenersEnabled() const;
    
    ////
    //events
    //
    void    update(ofEventArgs & args);
    void    mouseMoved(ofMouseEventArgs & args);
    void    mousePressed(ofMouseEventArgs & args);
    void    mouseReleased(ofMouseEventArgs & args);
    void    mouseDragged(ofMouseEventArgs & args);
    void    keyPressed(ofKeyEventArgs & args);
    void    keyReleased(ofKeyEventArgs & args);
    //
    ////

    const ofShortPixels & getSampleNeighbourhood() const; // for debugging depth reading
protected:
    struct MouseInViewport {
        glm::vec2 position; /// in viewport space
        bool withinViewport;
    };

    void addListeners();
    void removeListeners();

    MouseInViewport getMouseInViewport(const ofMouseEventArgs &);

    void findCursor();

    struct {
        bool listenersAttached; // we manage this. only ever set by update or destructor

        struct {
            bool down;
            int button;
        } mouseDown;

        struct {
            bool h;
            bool r;
            uint64_t resetHoldStartTime;
        } keysDown;
    } inputState;

    struct {
        glm::quat rotation;
        bool findMouseThisFrame;

        struct {
            MouseInViewport viewport; // within viewport pixels
            glm::vec3 world;
            float projectedDepth;
        } mouse;
    } tracking;

    struct {
        ofRectangle viewport;
        ofShortPixels sampleNeighbourhood;

        struct {
            GLint viewport[4];
        } opengl;
    } view;

    struct {
        bool listenersEnabled;
        bool mouseActionsEnabled;
        bool fixUpDirection;
        float trackballRadius;

        struct {
            bool enabled;
            float size;
        } cursorDraw;

        struct {
            glm::vec3 position;
            glm::quat rotation;
        } defaultView;
    } userSettings;
};
#endif /* ofxGrabCam_hpp */
