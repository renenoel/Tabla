//
//  PinballWorld.hpp
//  PaperBounce3
//
//  Created by Chaim Gingold on 12/5/16.
//
//

#ifndef PinballWorld_hpp
#define PinballWorld_hpp

#include "BallWorld.h"
#include "PureDataNode.h"
#include "GamepadManager.h"

class PinballWorld : public BallWorld
{
public:

	PinballWorld();
	~PinballWorld();

	void setParams( XmlTree ) override;
	
	string getSystemName() const override { return "PinballWorld"; }

	void gameWillLoad() override;
	void update() override;
	void updateVision( const ContourVector &c, Pipeline& ) override;
	void draw( DrawType ) override;
	
	void worldBoundsPolyDidChange() override;

	void keyDown( KeyEvent ) override;

protected:
	virtual void onBallBallCollide   ( const Ball&, const Ball& ) override;
	virtual void onBallContourCollide( const Ball&, const Contour& ) override;
	virtual void onBallWorldBoundaryCollide	( const Ball& ) override;

private:

	// params
	vec2  mUpVec = vec2(0,1);
	float mGravity=0.1f;
	float mPartMaxContourRadius = 5.f; // contour radius lt => part
	float mFlipperDistToEdge = 10.f; // how close to the edge does a flipper appear?
	float mBumperRadius = 5.f;
	
	// world orientation
	vec2 getUpVec() const { return mUpVec; }
	vec2 getLeftVec() const { return vec2(cross(vec3(mUpVec,0),vec3(0,0,1))); }
	vec2 getRightVec() const { return -getLeftVec(); }
	vec2 getGravityVec() const { return -mUpVec; }
	
	class Part
	{
	public:
		enum class Type
		{
			FlipperLeft,
			FlipperRight,
			Bumper
		};
		
		Type  mType;
		vec2  mLoc;
		float mRadius;
		
		vec2  mFlipperLoc2; // second point of capsule that forms flipper
		float mFlipperLength;
		
		PolyLine2 mPoly;
	};
	typedef vector<Part> PartVec;
	
	struct tAdjSpace
	{
		float mLeft=0.f;
		float mRight=0.f;
	};
	
	PartVec getPartsFromContours( const ContourVector& ) const;
	Part getFlipperPart( vec2 pin, float contourRadius, Part::Type type ) const; // type is left or right
	Part getBumperPart ( vec2 pin, float contourRadius, tAdjSpace adjSpace ) const;
	void getContoursFromParts( const PartVec&, ContourVec& contours ) const;
	
	PartVec mParts;
	void drawParts() const;
	
	// are flippers depressed
	bool  mIsFlipperDown[2]; // left, right
	float mFlipperState[2]; // left, right; 0..1
	
	void tickFlippers();
	
	// drawing
	void drawFlipperOrientationRays() const;
	
	// vision
	tAdjSpace getAdjacentLeftRightSpace( vec2, const ContourVector& ) const ; // how much adjacent space is to the left, right?
	
	// geometry
	PolyLine2 getCirclePoly ( vec2 c, float r ) const;
	PolyLine2 getCapsulePoly( vec2 c[2], float r[2] ) const;
	Contour contourFromPoly( PolyLine2 ) const; // area, radius, center, bounds, etc... is approximate
	void addContourToVec( Contour, ContourVec& ) const;
	
	// keymap (deprecated)
	map<char,string> mKeyToInput; // maps keystrokes to input names
	map<string,function<void()>> mInputToFunction; // maps input names to code handlers

	// game pad
	GamepadManager mGamepadManager;
	map<unsigned int,string> mGamepadButtons;
	map<string,function<void()>> mGamepadFunctions;
	
	// synthesis
	cipd::PureDataNodeRef	mPureDataNode;	// synth engine
	cipd::PatchRef			mPatch;			// pong patch
	
	void setupSynthesis();
	void shutdownSynthesis();

};

class PinballWorldCartridge : public GameCartridge
{
public:
	virtual string getSystemName() const override { return "PinballWorld"; }

	virtual std::shared_ptr<GameWorld> load() const override
	{
		return std::make_shared<PinballWorld>();
	}
};

#endif /* PinballWorld_hpp */