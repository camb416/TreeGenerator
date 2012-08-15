#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Turtle{
public:
    Vec3f curPos;
    Vec3f originPos;
    float forwardVelocity;
    float rotation;
    float rotation2;
    Vec3f velocity;
    void setup();
    Turtle();
    Turtle(Vec3f startPos);
    void setup(Vec3f startPos, Vec3f repellant);
    void update();
    void draw();
    float friction;
    float zVel;

};
Turtle::Turtle(){
    
}
Turtle::Turtle(Vec3f startPos){
    friction = 0.95f;
    forwardVelocity = Rand::randFloat(1.0f);
    rotation = Rand::randFloat(6.28f);
    originPos = curPos = startPos;
    zVel = Rand::randFloat(4.0f)-2.0f;
}
void Turtle::setup()
{
    friction = 0.95f;
    forwardVelocity = Rand::randFloat(1.0f);
    rotation = Rand::randFloat(6.28f);
    rotation2 = Rand::randFloat(6.28f);
    originPos = curPos = Vec3f(0.0f,0.0f,0.0f);
    zVel = Rand::randFloat(4.0f)-2.0f;
}
void Turtle::setup(Vec3f startPos, Vec3f repellant)
{
    friction = Rand::randFloat(0.95f);
    forwardVelocity = Rand::randFloat(8.0f)+2.0f;
    rotation = atan2(startPos.y-repellant.y,startPos.x-repellant.x)+Rand::randFloat(3.14f/4.0f)-3.14f/8.0f;
    //console() << rotation << "..." << endl;
    originPos = curPos = startPos;
    zVel = Rand::randFloat(4.0f)-2.0f;
}
void Turtle::update(){
    forwardVelocity*=0.90f;
    velocity.x = cos(rotation)*forwardVelocity;
    velocity.y = sin(rotation)*forwardVelocity;
    velocity.z = zVel*forwardVelocity;
    curPos += velocity;
    
}
void Turtle::draw(){
    
gl::color(1.0f, 1.0f, 1.0f, 0.5f);
    if(forwardVelocity>0.0f){
        gl::pushMatrices();
        gl::translate(0,0,curPos.z);
        gl::drawSolidCircle(Vec2f(curPos.x,curPos.y), forwardVelocity+1.0f);
        gl::popMatrices();
    }
    gl::color(1.0f, 1.0f, 1.0f, 0.25f);
    gl::drawLine(curPos, originPos);
}

class TreeGeneratorApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void reset();
    int maxNum;
    Vec2f screenRotation;
    Vec2f dRotation;
    void mouseMove(MouseEvent event);
    vector<Turtle> turtles;
   
};

void TreeGeneratorApp::setup()
{
    screenRotation = dRotation = Vec2f(0.0f,0.0f);
    maxNum = 128;
    reset();
}
void TreeGeneratorApp::reset(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::enableAlphaBlending();
    turtles.clear();
    turtles.push_back(Turtle(Vec3f(0.0f,0.0f,1.0f)));
    turtles.push_back(Turtle(Vec3f(0.0f,0.0f,0.0f)));
    turtles.push_back(Turtle(Vec3f(0.0f,0.0f,-1.0f)));
    turtles.push_back(Turtle(Vec3f(0.0f,1.0f,1.0f)));
  //  for(int i=0;i<turtles.size();i++) turtles.at(i).setup();
}




void TreeGeneratorApp::mouseDown( MouseEvent event )
{
    maxNum*=2;
    reset();
}
void TreeGeneratorApp::mouseMove(MouseEvent event){
    dRotation = event.getPos();
    
}

void TreeGeneratorApp::update()
{
    screenRotation += (dRotation-screenRotation) / 16.0f;
    Vec3f centerOfEverything = Vec3f(0,0,0);
    if(turtles.size()>0){
    for(int i=0;i<turtles.size();i++){
        centerOfEverything += turtles.at(i).curPos;
    }
    }
    centerOfEverything /= turtles.size();
   // console() << "CENTER: " << centerOfEverything.x << ", " << centerOfEverything.y << "." << endl;
    
    for(int i=0;i<turtles.size();i++) {
        turtles.at(i).update();
        if(turtles.at(i).forwardVelocity<0.05f && turtles.at(i).forwardVelocity>0.0f){
            turtles.at(i).forwardVelocity=0.0f;
            for(int j=0;j<2;j++){
                if(turtles.size()<maxNum){
                Turtle newTurtle = Turtle();
            newTurtle.setup(turtles.at(i).curPos,centerOfEverything);
          
                turtles.push_back(newTurtle);
                }
            }
            }
    }
}

void TreeGeneratorApp::draw()
{
	// clear out the window with black
    
	  gl::clear( Color( 0, 0, 0 ) );
    gl::pushMatrices();
    gl::translate(getWindowCenter());
    gl::rotate(Vec3f(screenRotation.y,screenRotation.x,0.0f));
    
     for(int i=0;i<turtles.size();i++) turtles.at(i).draw();
    gl::popMatrices();
}


CINDER_APP_BASIC( TreeGeneratorApp, RendererGl )
