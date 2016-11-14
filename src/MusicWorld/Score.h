//
//  Score.h
//  PaperBounce3
//
//  Created by Luke Iannini on 11/14/16.
//
//

#ifndef Score_h
#define Score_h
#include "Instrument.h"

// scores
class Score
{
public:
	InstrumentRef mInstrument;
	MetaParamInfo mMetaParamInfo;
	gl::GlslProgRef mAdditiveShader;

	void draw( GameWorld::DrawType ) const;

	// shape
	vec2  mQuad[4];
	/*  Vertices are played back like so:17317412


	 0---1
	 |   |
	 3---2

	 --> time		*/

	// detection meta-data
	bool mIsZombie=false;
	bool mDoesZombieTouchOtherZombies=false;

	// image data
	cv::Mat		mImage;				// thresholded image
	cv::Mat		mQuantizedImagePreThreshold; // (for inter-frame smoothing)
	cv::Mat		mQuantizedImage;	// quantized image data for midi playback
	float		mMetaParamSliderValue=-1.f; // 0..1

	// synth parameters
	string		mInstrumentName; // which synth


	MetaParamInfo getMetaParamInfo( MetaParam ) const;

	float		mPhase=0;
	float		mDurationFrac=1;
	void tickPhase(float globalPhase);

	int			mOctave;
	float		mRootNote;
	int			mNoteCount;
	int         mBeatCount;
	int			mNoteInstrument;
	int         mNumOctaves;

	float		mPan;
	//float		mVolume;

	// constructing shape
	bool		setQuadFromPolyLine( PolyLine2, vec2 timeVec );

	// getting stuff
	PolyLine2	getPolyLine() const;
	float		getPlayheadFrac() const;
	void		getPlayheadLine( vec2 line[2] ) const; // line goes from [3..2] >> [0..1]
	vec2		getSizeInWorldSpace() const;
	// if not a perfect rect, then it's approximate (measures two bisecting axes)
	// x is axis along which playhead moves; y is perp to it (parallel to playhead)
	vec2		fracToQuad( vec2 frac ) const; // frac.x = time[0,1], frac.y = note_space[0,1]
	vec2		getCentroid() const { return fracToQuad(vec2(.5,.5)); }
	float		getQuadMaxInteriorAngle() const; // looking for concave-ish shapes...

	Scale mScale;
	int noteForY( int y ) const;

	// score vision
	bool  isScoreValueHigh( uchar ) const;
	float getNoteLengthAsScoreFrac( cv::Mat image, int x, int y ) const;
	int   getNoteLengthAsImageCols( cv::Mat image, int x, int y ) const;

private:
	void drawNotes		( GameWorld::DrawType drawType ) const;
	void drawScoreLines	( GameWorld::DrawType drawType ) const;
	void drawPlayhead	( GameWorld::DrawType drawType ) const;
	void drawMetaParam	( GameWorld::DrawType drawType ) const;
};

#endif /* Score_h */
