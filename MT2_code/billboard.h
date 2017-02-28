#ifndef  _BILL_BOARD_
#define _BILL_BOARD_

#include "vec3.h"
#include "glut.h"

enum bb_render_mode
{
	un_orient = 0,
	semi_orient,
	omni_orient,
	multi_orient,
	rotated_multi_orient,
	multi_orient_with_end,
	rotated_multi_orient_with_end
};

enum bb_blend_mode
{
	alpha = 0, 
	addictive
};

enum bb_orientation
{
	verticle = 0,
	horizental
};

class billboard
{
protected:
	vec3 pos, normal;
	bb_orientation orientation;
	GLfloat omniRotate; // the rotate around z axis in omni-directional mode. 
	GLfloat width, height; 
	GLfloat end_width, end_height;
	GLuint texHandle, end_texHandle; 
	GLfloat upAngle_m; // the angle to rotate vector (0, 0, -1) to the Up vector of bill board. 
	vec3 rotAxis_m; // the axis for the rotation above. 
	bb_render_mode render_mode; 
	bb_blend_mode blend_mode; 
	void drawBoardAdd(GLfloat w, GLfloat h, GLuint handle); // draw board in addictive blend mode. 
	void drawBoardAlpha(GLfloat w, GLfloat h, GLuint handle); // draw board in alpha blend mode. 
	void drawBoard(GLfloat w, GLfloat h);
	void renderSemiDirctional();
	void renderOmniDirectional();
	void renderMultiDirectional();
	void renderRotatedMultiDirectional();
	void renderMultiDirectionalWithEndinMiddle();
	void renderRotatedMultiDirectionalWithEndinMiddle();

public:
	void setPos(vec3 p); 
	void setOmniRot(GLfloat angle); // Set rotation angle in omni-directional mode. 
	inline void setSize(GLfloat w, GLfloat h);
	void setupTex(GLuint tex, GLfloat w, GLfloat h);
	inline void setMultiEnd(GLuint handle);
	inline void setOri(bb_orientation ori);
	inline void setRot(vec3 axis, GLfloat angle); // set up rotation angles for multi directional bill board. 
	inline void render(); // render the bill board according to internal status.  
	billboard(GLuint tex, bb_render_mode r, bb_blend_mode b, bb_orientation ori, GLfloat w = 1, GLfloat h = 1);
	~billboard();
};

void billboard::render()
{
	switch (render_mode)
	{
	case un_orient:
		drawBoard(width, height); 
		break;
	case semi_orient:
		renderSemiDirctional(); 
		break;
	case omni_orient:
		renderOmniDirectional(); 
		break;
	case multi_orient:
		renderMultiDirectional();
		break;
	case rotated_multi_orient:
		renderRotatedMultiDirectional();
		break;
	case multi_orient_with_end:
		renderMultiDirectionalWithEndinMiddle();
		break;
	case rotated_multi_orient_with_end:
		renderRotatedMultiDirectionalWithEndinMiddle();
		break;
	default:
		printf("undefined render mode : %d", render_mode); 
		break;
	}
}

void billboard::setMultiEnd(GLuint handle)
{
	end_texHandle = handle; 
}

void billboard::setOri(bb_orientation ori)
{
	orientation = ori; 
}

inline void billboard::setSize(GLfloat w, GLfloat h)
{
	width = w;
	height = h;
}


void billboard::setRot(vec3 axis, GLfloat angle)
{
	upAngle_m = angle;
	
	rotAxis_m = axis;
}


#endif // ! _BILL_BOARD_
