#include <C:/Dev-Cpp5.4/mingw32/include/GL/glew.h>
#include <GL/gl.h>
#include <C:\Dev-Cpp5.4\mingw32\i686-w64-mingw32\include\GL\freeglut.h>
#include <C:\Dev-Cpp5.4\mingw32\i686-w64-mingw32\include\GL\freeglut_ext.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#include <math.h>
#include <vector>
#include <map>
#include <list>
#include <string.h>
#include <Winuser.h>
#include <time.h>

#include <stdlib.h>
#include <C:\Dev-Cpp5.4\mingw32\i686-w64-mingw32\include\AL/al.h>
#include <C:\Dev-Cpp5.4\mingw32\i686-w64-mingw32\include\AL/alut.h>

//#include <C:\Dev-Cpp5.4\mingw32\i686-w64-mingw32\include\FTGL/ftgl.h>

#include <iostream>
#include <fstream>


// This is some crazy magic that produces __StaticExecute__247
// Vanilla interpolation of __StaticExecute__##__LINE__ would produce __StaticExecute____LINE__
// I still can't figure out why it works, but it has to do with macro resolution ordering
// If you already have Boost, you can omit this part
#define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)
#define BOOST_PP_CAT_I(a, b) BOOST_PP_CAT_II(~, a ## b)
#define BOOST_PP_CAT_II(p, res) res

// This avoids repeating the BOOST_PP_CAT 5X
#define STATIC_EXECUTE \
  STATIC_EXECUTE_I(BOOST_PP_CAT(__StaticExecute__, __LINE__))

// This is the meat, a static instance of a class whose constructor runs your code
#define STATIC_EXECUTE_I(uniq_name)               \
static struct uniq_name {                         \
  uniq_name();                                    \
} BOOST_PP_CAT(uniq_name, __var);                 \
uniq_name::uniq_name()  // followed by { ... }

#define M_PI_8 0.3926990816987241548

using namespace std;

//bool KeyStates[256];
bool MouseStates[3];

int ScrW = 0;
int ScrH = 0;	

typedef unsigned char byte;

class Vec2i;

class Vec2d{
    public:
        float x,y;
        void operator = ( double val) { x=  val; y = val; };
        
        Vec2d()
        {
        	x = 0; y = 0;
        }
        Vec2d(int val) {
        	x = val; y = val;
        }
        
        Vec2d(float ix, float iy) {
        	x = ix; y = iy;
        }
        
        Vec2d(Vec2i);
        
        Vec2d operator+(Vec2d vec){
             Vec2d newvec;
             newvec.x = x+vec.x;
             newvec.y = y+vec.y;
             return newvec;
        };
        Vec2d operator+(float f){
             Vec2d newvec;
             newvec.x = x+f;
             newvec.y = y+f;
             return newvec;
        };        
        Vec2d operator-(float f){
             Vec2d newvec;
             newvec.x = x-f;
             newvec.y = y-f;
             return newvec;
        };
        Vec2d operator-(Vec2d f){
             Vec2d newvec;
             newvec.x = x-f.x;
             newvec.y = y-f.y;
             return newvec;
        };
        void operator+=(Vec2d f){
             x = x+f.x;
             y = y+f.y;
        };
        
        void operator-=(Vec2d f){
             x = x-f.x;
             y = y-f.y;
        };
        
        Vec2d operator*(Vec2d f){
             Vec2d newvec;
             newvec.x = x*f.x;
             newvec.y = y*f.y;
             return newvec;
        };
        Vec2d operator*(float f){
             Vec2d newvec;
             newvec.x = x*f;
             newvec.y = y*f;
             return newvec;
        };
        Vec2d operator/(float f){
             Vec2d newvec;
             newvec.x = x/f;
             newvec.y = y/f;
             return newvec;
        };
        
        
        void operator*=(float f){
             x = x*f;
             y = y*f;
        };
        void operator/=(float f){
             x = x/f;
             y = y/f;
        };
        
        Vec2d operator-()
        {
            Vec2d newvec;
            newvec.x = -x;
            newvec.y = -y;
            return newvec;
        };
        
        bool operator == (Vec2d vec)
        {
            return (x == vec.x) & (y == vec.y);
        };
        Vec2d Normalize(){
            Vec2d newvec;
            float veclen = sqrt(x*x+y*y);
                newvec.y = y/veclen;
                newvec.x = x/veclen;
            return newvec;
        };
        
    	Vec2d Normalize2(){
            Vec2d newvec;
            float veclen = sqrt(x*x+y*y);
            veclen*=veclen;
                newvec.y = y/veclen;
                newvec.x = x/veclen;
            return newvec;
        };
        
        Vec2d abs(){
             Vec2d newvec;
             newvec.x = fabs(x);
             newvec.y = fabs(y);
             return newvec;
        };
        float Length(){
            return sqrt(x*x+y*y);    
        };
        
        float Dot(Vec2d vec)
        {
        	return (x*vec.x + y*vec.y);
        };
        
        Vec2d Rotate(float angle){
            Vec2d newvecrotated;
            newvecrotated.x = x*cos(angle/180*M_PI) + y*sin(angle/180*M_PI);
            newvecrotated.y = y*cos(-angle/180*M_PI) + x*sin(-angle/180*M_PI);
            return newvecrotated;
        };
        
        Vec2i I();
};

class Vec2i{
    public:
        int x,y;
        void operator = ( int val) { x=  val; y = val; };
        
        Vec2i()
        {
        	x = 0; y = 0;
        }
        Vec2i(int val) {
        	x = val; y = val;
        }
        
        Vec2i(int ix, int iy) {
        	x = ix; y = iy;
        }
        
        Vec2i(Vec2d vec)
        {
        	x = vec.x; y = vec.y;
        }
        
        Vec2i operator+(Vec2i vec){
             Vec2i newvec;
             newvec.x = x+vec.x;
             newvec.y = y+vec.y;
             return newvec;
        };
        Vec2i operator+(int f){
             Vec2i newvec;
             newvec.x = x+f;
             newvec.y = y+f;
             return newvec;
        };        
        Vec2i operator-(int f){
             Vec2i newvec;
             newvec.x = x-f;
             newvec.y = y-f;
             return newvec;
        };
        Vec2i operator-(Vec2i f){
             Vec2i newvec;
             newvec.x = x-f.x;
             newvec.y = y-f.y;
             return newvec;
        };
        void operator+=(Vec2i f){
             x = x+f.x;
             y = y+f.y;
        };
        
        void operator-=(Vec2i f){
             x = x-f.x;
             y = y-f.y;
        };
        
        void operator*=(float f)
        {
        	x*=f;
			y*=f;	
        }
        
        void operator/=(float f)
        {
        	x/=f;
			y/=f;	
        }
        
        Vec2i operator*(Vec2i f){
             Vec2i newvec;
             newvec.x = x*f.x;
             newvec.y = y*f.y;
             return newvec;
        };
        
        Vec2i operator*(float f){
             Vec2i newvec;
             newvec.x = (int)round(x*f);
             newvec.y = (int)round(y*f);
             return newvec;
        };
        
        Vec2i operator/(float f){
             Vec2i newvec;
             newvec.x = (int)round(x/f);
             newvec.y = (int)round(y/f);
             return newvec;
        };
        
        Vec2i operator-()
        {
            Vec2i newvec;
            newvec.x = -x;
            newvec.y = -y;
            return newvec;
        };
        
        bool operator == (Vec2i vec)
        {
            return (x == vec.x) & (y == vec.y);
        };
        
        bool operator != (Vec2i vec)
        {
        	return (x != vec.x) || (y != vec.y);
        };

		Vec2d Normalize()
		{
			Vec2d result;
			float len = sqrt(x*x+y*y);
			result.x = (float)x/len;
			result.y = (float)y/len;
			return result;
		};
		
		Vec2d Normalize2()
		{
			Vec2d result;
			float len = sqrt(x*x+y*y);
			len*=len;
			result.x = (float)x/len;
			result.y = (float)y/len;
			return result;
		};
		
        Vec2i vabs(){
             Vec2i newvec;
             newvec.x = abs(x);
             newvec.y = abs(y);
             return newvec;
        };
        float Length(){
            return sqrt(x*x+y*y);    
        };
        
        Vec2i Rotate(float angle){
            Vec2i newvecrotated;
            newvecrotated.x = (int)round(x*cos(angle/180*M_PI) + y*sin(angle/180*M_PI));
            newvecrotated.y = (int)round(y*cos(-angle/180*M_PI) + x*sin(-angle/180*M_PI));
            return newvecrotated;
        };
        
        float Dot(Vec2i vec)
        {
        	return (x*vec.x + y*vec.y);
        };
        
        Vec2d D()
        {
            Vec2d newvec;
            newvec.x = x; newvec.y = y;
            return newvec;
        }
        
};

Vec2i Vec2d::I()
{
	Vec2i result;
	result.x = (int)( floor(x) );
	result.y = (int)( floor(y) );
	return result;
}

Vec2d::Vec2d( Vec2i vec )
{
	x = vec.x; y = vec.y;
}

void glVertex( Vec2d pos )
{
    glVertex2f(pos.x,pos.y);
}

Vec2d Mirror(Vec2d pos, Vec2d pos1, Vec2d pos2)
{
    Vec2d Vec = (pos1-pos2).Normalize();
    Vec2d Norm;
    Norm.x = -Vec.y;
    Norm.y = Vec.x;
    if (Vec.x > 0) Norm = -Norm;
            
            
    float l1 = (pos-pos1).Length();
    float l2 = (pos-pos2).Length();
    float l3 = (pos1-pos2).Length();
        
    float dist = (((l1-l2)*(l1+l2) - l3*l3)*0.5) / l3;
    Vec2d Center = (pos2-pos1).Normalize() * dist + pos2;
    Vec = pos-Center;
    dist = Vec.Length();
    if (Vec.y < 0) dist = -dist;
            
    return Norm*dist+Center;
};

Vec2d AngForward(float ang){
    Vec2d vec;
        vec.x = sin(ang/180*M_PI);
        vec.y = -cos(ang/180*M_PI);
    return vec;
};

Vec2d AngRight(float ang){
    Vec2d vec;
        vec.x = cos(ang/180*M_PI);
        vec.y = sin(ang/180*M_PI);
    return vec;
};

Vec2d Vector2d(double x,double y){
      Vec2d vec;
      vec.x = x;
      vec.y = y;
      return vec;
};

Vec2i Vector2i(int x,int y){
      Vec2i vec;
      vec.x = x;
      vec.y = y;
      return vec;
};

Vec2i Vector2i(Vec2d vec){
      Vec2i newvec;
      vec.x = (int)round(vec.x);
      vec.y = (int)round(vec.y);
      return newvec;
};


void circle(float x,float y,float r) {
     
     glBegin(GL_POINTS);
     glVertex2f(x, y);
     glEnd();
     
     int i;
     int iter = 60;
     glBegin(GL_LINE_LOOP);
     for(i= 0; i < iter; i++) {
            float px = cos((6.2832/iter)*i)*r+x;
            float py = sin((6.2832/iter)*i)*r+y;
            glVertex2f(px,py); 
     }
     glEnd();
     
}

void fcircle(float x,float y,float r) {
     
     glBegin(GL_POINTS);
     glVertex2f(x, y);
     glEnd();
     
     int i;
     int iter = 16;
     glBegin(GL_TRIANGLE_FAN);
     for(i= 0; i < iter; i++) {
            float px = cos((6.2832/iter)*i)*r+x;
            float py = sin((6.2832/iter)*i)*r+y;
            glVertex2f(px,py); 
     }
     glEnd();
}

void fcircle(Vec2d pos, float r) {fcircle(pos.x,pos.y,r);};
void circle(Vec2d pos, float r) {circle(pos.x,pos.y,r);};

void drawarrow(Vec2d pos1, Vec2d pos2, float size)
{
	glBegin(GL_LINES);
		glVertex2f(pos1.x,pos1.y);
		glVertex2f(pos2.x,pos2.y);
	glEnd();
		pos2 = (pos2*3+pos1)*0.25;
		
		//Vec2d a1 = (pos2*9+pos1)*0.1;
		Vec2d a1 = pos2-(pos2-pos1).Normalize()*size*4;
		Vec2d a2 = pos1-pos2;	
		a2 = Vec2d(-a2.y,a2.x).Normalize();
	
		a1 = a1+a2*size;
		a2 = a1-a2*size*2;
	glBegin(GL_TRIANGLES);
		glVertex2f(pos2.x,pos2.y);
		glVertex2f(a1.x,a1.y);
		//glVertex2f(pos2.x,pos2.y);
		glVertex2f(a2.x,a2.y);
	glEnd();
}

void output(GLfloat x, GLfloat y,GLfloat sx,GLfloat sy, char *text) // Prints text
{
	char *p;

	glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
	glTranslatef(x, y, 0);
	glScalef(sx,sy,0);
	glRotatef(0,0,0,0);
	for (p = text; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
	glPopMatrix();
}

void* MainFont = GLUT_BITMAP_HELVETICA_12;//GLUT_BITMAP_HELVETICA_12; GLUT_BITMAP_9_BY_15
int MainFont_Height = 9;

int GetTextWidth(const char* text)
{
	int size = 0;
	
    for (text; *text; text++)
        size += glutBitmapWidth(MainFont, *text);
        
    return size;
}

int CountCharsToEnd(int bound, const char* itext)
{
	int size = 0;
	const char* text = itext;
    for (text; *text; text++)
    {
        size += glutBitmapWidth(MainFont, *text);
        if (size > bound) break;
    };
    
    return (int)(text-itext);
};

void drawtext(GLfloat x, GLfloat y, const char *text)
{
	glPushMatrix();
		glRasterPos2i(x,y);
		glutBitmapString( MainFont , (unsigned const char*)(text) );
	glPopMatrix();
}
inline void drawtext(Vec2i pos, char* text) {drawtext(pos.x,pos.y,text);};
inline void drawtext(GLfloat x, GLfloat y, string text) { drawtext(x,y,text.c_str()); };
inline void drawtext(Vec2i pos, string text) { drawtext(pos.x,pos.y,text.c_str()); };

void drawtext(GLfloat x, GLfloat y, const char* text, int bound)
{
		int size = 0;
		glPushMatrix();
			glRasterPos2i(x,y);
			for (text; *text; text++)
			{
				size += glutBitmapWidth(MainFont, *text);
				if ((size+4) >= bound)
				{
					glutBitmapCharacter(MainFont, '.');
					glutBitmapCharacter(MainFont, '.');
					glPopMatrix();
					return;
				} else glutBitmapCharacter(MainFont, *text);
			};
		glPopMatrix();	
}
inline void drawtext(Vec2i pos, string text, int bound) { drawtext(pos.x,pos.y,text.c_str(), bound); };
inline void drawtext(GLfloat x, GLfloat y, string text, int bound) { drawtext(x,y,text.c_str(), bound); };
/*
void drawtext(GLfloat x, GLfloat y, char *text) // Prints text
{
    glPushMatrix();
    glRasterPos2i((int)(x-GetTextWidth(text)*posx),(int)(y+MainFont_Height*posy));
	drawtext(x, y, text);
	glPopMatrix();
}
*/

void drawtextscaleable(GLfloat x, GLfloat y, GLfloat sizex, GLfloat sizey, float posx, float posy, char *text, int scale) // Prints text
{
	char *p;
    
    //glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glPushMatrix();
    void* font = GLUT_STROKE_ROMAN;
    int sumsize=0;
    for (p = text; *p; p++) sumsize += glutStrokeWidth(GLUT_STROKE_ROMAN, *p);
    glTranslatef(x+sumsize*posx,y+sizey*posy,0);
    glScalef(sizex*0.0078125*scale,-sizey*0.0078125*scale,1);
	for (p = text; *p; p++) glutStrokeCharacter(font, *p);
	glPopMatrix();
	//glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}




void moveint(int* arr, int oldsize){ // Sorts the array and removes 0's
     int i;
     int gap = 0;
     for (i = 0; i < oldsize; i++) {
         arr[i-gap] = arr[i];
         if (arr[i] == 0) {gap += 1;};
         
     };    
     arr = (int *)realloc((void *)arr,(oldsize - gap)*sizeof(int));
};

//-------------------------------IRGB---------------------------------------

class IRGBA;
class IRGB{
	public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	
	IRGB()
	{
		r = 0;
		g = 0;
		b = 0;
	}
	IRGB(IRGBA irgba);
	
	IRGB( unsigned char ir, unsigned char ig, unsigned char ib)
	{
		r = ir;
		g = ig;
		b = ib;
	}

	bool operator ==(IRGB col)
	{
		return ((col.r == r) && (col.g == g) && (col.b == b));	
	}
	
	bool operator !=(IRGB col)
	{
		return !((col.r == r) && (col.g == g) && (col.b == b));	
	}	
	
	template <class T> void operator *=(T a)
	{
		r*=a;
		g*=a;
		b*=a;	
	}
	
	template <class T> void operator /=(T a)
	{
		r/=a;
		g/=a;
		b/=a;	
	}
	
	void operator *=(IRGB col)
	{
		r*=col.r;
		g*=col.g;
		b*=col.b;	
	}
	
	void operator /=(IRGB col)
	{
		r/=col.r;
		g/=col.g;
		b/=col.b;	
	}
	
	IRGB operator +(IRGB col)
	{
		return IRGB(
		r+col.r,
		g+col.g,
		b+col.b);	
	}
	
	IRGB operator -(IRGB col)
	{
		return IRGB(
		r-col.r,
		g-col.g,
		b-col.b);	
	}
	
	template<class T> IRGB operator +(T a)
	{
		return IRGB(
		r+a,
		g+a,
		b+a);	
	}
	
	template<class T> IRGB operator -(T a)
	{
		return IRGB(
		r-a,
		g-a,
		b-a);	
	}
	
	template<class T> IRGB operator * (T a)
	{
		return IRGB(
		r*a,
		g*a,
		b*a);	
	}
	
	template <class T> IRGB operator / (T a)
	{
		return IRGB(
		r/a,
		g/a,
		b/a);	
	}
	
	IRGB operator *(IRGB col)
	{
		return IRGB(
		r*col.r,
		g*col.g,
		b*col.b);	
	}
	
	IRGB operator /(IRGB col)
	{
		return IRGB(
		r/col.r,
		g/col.g,
		b/col.b);	
	}
	
	
};

class IRGBA
{
	public:
	unsigned char r,g,b,a;	
	
	IRGBA()
	{
		r = 0; g = 0; b = 0; a = 255;	
	};
	
	IRGBA( IRGB col )
	{
		r = col.r; g = col.g; b = col.b; a = 255;	
	};
	
	IRGBA( unsigned char ir, unsigned char ig, unsigned char ib )
	{
		r = ir;
		g = ig;
		b = ib;
		a = 255;	
	};
	
	IRGBA( unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia )
	{
		r = ir;
		g = ig;
		b = ib;
		a = ia;	
	};
};

IRGB::IRGB(IRGBA irgba)
{
		r = irgba.r;
		g = irgba.g;
		b = irgba.b;
};

inline void glColor(IRGB col, float a) {
     glColor4ub(col.r,col.g,col.b, a*255);
};

inline void glColor(IRGB col)
{
	glColor3ub(col.r,col.g,col.b);
};

void drawrect(Vec2d pos1, Vec2d pos2){
     glBegin(GL_QUADS);
     glVertex2f(pos1.x,pos1.y); //upleft
     glVertex2f(pos1.x,pos2.y);        //downleft
     glVertex2f(pos2.x,pos2.y); //downright
     glVertex2f(pos2.x,pos1.y);   //upright  
     glEnd();
};

void drawrect(Vec2d pos1, Vec2d pos2, GLuint texture){
	glBindTexture( GL_TEXTURE_2D, texture );
	glEnable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
		glTexCoord2d(0.0,1.0); glVertex2f(pos1.x,pos1.y);
		glTexCoord2d(0.0,0.0); glVertex2f(pos1.x,pos2.y);
		glTexCoord2d(1.0,0.0); glVertex2f(pos2.x,pos2.y);
		glTexCoord2d(1.0,1.0); glVertex2f(pos2.x,pos1.y);
	glEnd();
	glDisable( GL_TEXTURE_2D );
};

void drawhrect(Vec2d pos1, Vec2d pos2){ // draws single color bevel
     glBegin(GL_LINE_LOOP);
     glVertex2f(pos1.x,pos1.y); //upleft
     glVertex2f(pos1.x,pos2.y);        //downleft
     glVertex2f(pos2.x,pos2.y); //downright
     glVertex2f(pos2.x,pos1.y);   //upright  
     glEnd();
};

void drawbevel(IRGB col1,IRGB col2,Vec2d pos1,Vec2d pos2) {
	pos2-=1;

	glColor(col1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos1.x,pos1.y); //upleft
	glVertex2f(pos1.x,pos2.y);        //downleft
	glVertex2f(pos2.x,pos2.y); //downright
	glVertex2f(pos2.x,pos1.y);   //upright  
	glEnd();
};

class brdres{
      public:
      short side;
      int dist;
};


char BRDRES_SIDE_LEFT = 1;
char BRDRES_SIDE_TOP = 2;
char BRDRES_SIDE_RIGHT = 4;
char BRDRES_SIDE_BOTTOM = 8;

brdres DistFromBoarder_Square(Vec2d p1, Vec2d p2,Vec2d p3 /* mouse pos */ ){
        brdres ret;
        ret.dist = 30;
        ret.side = 1;
        short sidex = BRDRES_SIDE_BOTTOM;
        short sidey = BRDRES_SIDE_LEFT;
        int ds[4];
        
        ds[0] = (int)(p1.x-p3.x);
        ds[1] = (int)(p3.x-p2.x);
        
        ds[2] = (int)(p1.y-p3.y);
        ds[3] = (int)(p3.y-p2.y);
        
        if (ds[0]<ds[1]) {ds[0] = ds[1]; sidex = BRDRES_SIDE_TOP;};
        if (ds[2]<ds[3]) {ds[2] = ds[3]; sidey = BRDRES_SIDE_RIGHT;};
       
        if (ds[0]<ds[2]) {ret.dist = ds[2]; ret.side = sidey;} else {ret.dist = ds[0]; ret.side = sidex;};
        ret.dist=ret.dist;
        return ret;
};

float Clamp(float f, float min, float max){
    if (f > max) return max;
    if (f < min) return min;
    return f;
};

Vec2d localtoworld(Vec2d lpos, Vec2d wpos, float scale, float angle){
    Vec2d newvec;
        newvec=(lpos-wpos)/scale;
    Vec2d newvecrotated = newvec;
    if (angle != 0) {
        newvecrotated.x = newvec.x*cos(angle/180*M_PI) + newvec.y*sin(angle/180*M_PI);
        newvecrotated.y = newvec.y*cos(-angle/180*M_PI) + newvec.x*sin(-angle/180*M_PI);
    };
    return newvecrotated;
};

unsigned char toVirtual(char key)
{
	HKL layout = GetKeyboardLayout(0);
	short int raw = VkKeyScanEx(key,layout);
	unsigned char virtualkey = *((unsigned char*)(&raw));
	return virtualkey;
};

char* itoch(int num){
      int size=0;
      int i=0;
      
      if (num == 0)
      {
      	char* result = new char;
      	*result = '0';
      	return result;	
      }
      int num2 = num;
      for (size=0;num2>=1;size++) num2 = (int)(num2*0.1);
      
      char* newchar = new char[size+1];
      for (i = 1; i <= size; i++){
            switch ( (num%10) ){
                case 1  : newchar[size-i] = '1'; break;
                case 2  : newchar[size-i] = '2'; break;
                case 3  : newchar[size-i] = '3'; break;
                case 4  : newchar[size-i] = '4'; break;
                case 5  : newchar[size-i] = '5'; break;
                case 6  : newchar[size-i] = '6'; break;
                case 7  : newchar[size-i] = '7'; break;
                case 8  : newchar[size-i] = '8'; break; 
                case 9  : newchar[size-i] = '9'; break;
                default : newchar[size-i] = '0'; break;
            }; 
            num = (int)(num*0.1);
      };
      newchar[size]=0;
      //newchar[1] = '1';
      return newchar;
};

void MemToChar(unsigned long long memory, char* buf, int size)
{
	if (memory > 102)
	{
		if (memory > 104858)
		{
			if (memory > 536870912) snprintf ( buf, size,"%0.2fGB",(float)memory/1073741824);
			else snprintf ( buf, size,"%0.2fMB",(float)memory/1048576);
		} else 
		{
			snprintf ( buf, size,"%0.2fKB",(float)memory/1024);	
		};
	} else 
	{
		snprintf ( buf, size,"%dB",memory);	
	};
};
int iabs( int i )
{
	return (i < 0) ? -i : i;	
};

char* itoch(int num, char* newchar){
      int size=0;
      int i=0;
      
      if (num == 0)
      {
      	char* result = new char;
      	*result = '0';
      	return result;	
      }
      int num2 = num;
      for (size=0;num2>=1;size++) num2 = (int)(num2*0.1);
      
      for (i = 1; i <= size; i++){
            switch ( (num%10) ){
                case 1  : newchar[size-i] = '1'; break;
                case 2  : newchar[size-i] = '2'; break;
                case 3  : newchar[size-i] = '3'; break;
                case 4  : newchar[size-i] = '4'; break;
                case 5  : newchar[size-i] = '5'; break;
                case 6  : newchar[size-i] = '6'; break;
                case 7  : newchar[size-i] = '7'; break;
                case 8  : newchar[size-i] = '8'; break; 
                case 9  : newchar[size-i] = '9'; break;
                default : newchar[size-i] = '0'; break;
            }; 
            num = (int)(num*0.1);
      };
      newchar[size]=0;
      //newchar[1] = '1';
      return newchar;
};

inline int clamp(int x, int min, int max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;	
};
// SHADES

inline void glTranslate(Vec2d vec)
{
	glTranslatef(vec.x,vec.y,0);
}

inline void glScale(Vec2d vec)
{
	glScalef(vec.x,vec.y,1);
}

inline void glScale(float scale)
{
	glScalef(scale,scale,1);
}

static unsigned long FAST_RAND_X=123456789, FAST_RAND_Y=362436069, FAST_RAND_Z=521288629, FAST_RAND_T;
void fr_randomize()
{
	FAST_RAND_Y=rand(); 
	FAST_RAND_Z=rand();	
};
unsigned long random(void) {          //period 2^96-1
    FAST_RAND_X ^= FAST_RAND_X << 16;
    FAST_RAND_X ^= FAST_RAND_X >> 5;
    FAST_RAND_X ^= FAST_RAND_X << 1;

   FAST_RAND_T = FAST_RAND_X;
   FAST_RAND_X = FAST_RAND_Y;
   FAST_RAND_Y = FAST_RAND_Z;
   FAST_RAND_Z = FAST_RAND_T ^ FAST_RAND_X ^ FAST_RAND_Y;

  return FAST_RAND_Z;
}

inline float random1()
{
	return (float)(random()&2047)/2047;
};

class MouseRes
{
	public:
	Vec2i pos;
	int btn;
	
	MouseRes(int ibtn, int x, int y) {
		pos.x = x;
		pos.y = y;
		btn = ibtn;
	}
	
	MouseRes() {

	}
};

char* readfile(char* filename, int& length)
{
	ifstream file(filename);
	if (!file.is_open()) {length = 0; return 0;};
	long begin = file.tellg();
  	file.seekg (0, ios::end);
  	long end = file.tellg();
  	file.seekg( 0, ios::beg );
  	length = end-begin;
	char* result = new char[length+1];
	int i = 0;	
	while( !file.eof() ) { result[i] = file.get(); ++i;};
	result[length] = 0;
	//file.read(result,length);
	file.close();
	length = i-1;
	return result;
}

GLuint LoadShader(char* filename)
{
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	GLint flength = 0;
	GLchar *FragmentShaderSource = readfile(filename,flength);
	cout << "FragmentShaderSource: " << FragmentShaderSource << endl;
	glShaderSourceARB(fragmentShaderObject, 1, (const char **)(&FragmentShaderSource), &flength);
	glCompileShaderARB(fragmentShaderObject);
	GLint blen = 0;	
	GLsizei slen = 0;
	glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH , &blen);    
	
	GLint compiled;
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
	if (compiled)
	{
	   cout << "shader compile success" << endl;
	} else
	{  
		if (blen > 1)
		{
			cout << "(blen > 1): ";
	 		GLchar compiler_log[blen];
	 		glGetInfoLogARB(fragmentShaderObject, blen, &slen, compiler_log);
	 		cout << ("compiler_log:\n", compiler_log);	
			return -1;
		};
	};
	
	GLuint ProgramObject = glCreateProgram();

	glAttachShader(ProgramObject, fragmentShaderObject);

	glLinkProgram(ProgramObject);
	
	GLint linked;
	glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
	if (linked)
	{
   		cout << "its ok, man. Shader loaded." << endl;
	} else
	{
		cout << "Shader didnt link? fail to load shader." << endl;
		return -1;
	}
	return ProgramObject;
};

class TexturesInfo
{
	public:
		int count;
		GLuint texture;
		Vec2i Size;
		unsigned char bpp;
	TexturesInfo()
	{
		texture = 0;	
	};
};

map<string, TexturesInfo > textures; // stores textures and their usage; where int - count of textures in use;
class TBitmapTex
{
	private:
		map<string, TexturesInfo >::iterator tex_it; // iterator to textures
	public:
		GLuint texture;
		Vec2i Size;
		unsigned char bpp; // Bytes Per Pixel
		TBitmapTex()
		{
			texture = 0;
		};
		
		~TBitmapTex()
		{
			UnloadTexture();
		}
		
		void LoadTexture( char* filename )
		{
			if (texture) glDeleteTextures( 1, &texture );
			tex_it = textures.emplace(filename,TexturesInfo()).first;
			texture = tex_it->second.texture;
			if (!texture)
			{
				//cout << "loading texture" << endl;
				glGenTextures( 1, &texture );
				glBindTexture( GL_TEXTURE_2D, texture );
				
	    		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	    		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	    		
	    		int length = 0;
	    		string newfilename = string("./Textures/") + filename + ".bmp";
	    		GLvoid* data = readfile((char*)(newfilename.c_str()),length);
	    		if (data == 0) {cout << "texture " << filename << " not found." << endl; texture = 0; return;};
	    		tagBITMAPFILEHEADER* fileheader = static_cast<tagBITMAPFILEHEADER*>(data);
	    		BITMAPINFOHEADER* header = static_cast<BITMAPINFOHEADER*>((void*)( (char*)data+14 ));
	    		
	    		Size.x = header->biWidth;
	    		Size.y = header->biHeight;
	    		bpp = header->biBitCount;
	    		
	    		if (bpp == 32)
	    			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header->biWidth, header->biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, ((char*)data+fileheader->bfOffBits) );
	    		else
	    			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header->biWidth, header->biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ((char*)data+fileheader->bfOffBits) );
	    		tex_it->second.texture = texture;
	    		tex_it->second.Size = Size;
	    		tex_it->second.bpp = bpp;
	    		tex_it->second.count = 1;
    		} else
    		{
    			tex_it->second.count+=1;
    			Size = tex_it->second.Size;
    			bpp = tex_it->second.bpp;
    		};
		};
		
		void UnloadTexture()
		{
			if (!texture) return;
			if (tex_it->second.count==1)
			{
				//cout << "clearing texture" << endl;
				if (texture) glDeleteTextures( 1, &texture );
				textures.erase(tex_it);
			} else tex_it->second.count-=1;
		};
		
};

void drawrect(Vec2d pos1, TBitmapTex& bitmap){
	Vec2d pos2 = pos1+bitmap.Size;
	glBindTexture( GL_TEXTURE_2D, bitmap.texture );
	glEnable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
		glTexCoord2d(0.0,1.0); glVertex2f(pos1.x,pos1.y);
		glTexCoord2d(0.0,0.0); glVertex2f(pos1.x,pos2.y);
		glTexCoord2d(1.0,0.0); glVertex2f(pos2.x,pos2.y);
		glTexCoord2d(1.0,1.0); glVertex2f(pos2.x,pos1.y);
	glEnd();
	glDisable( GL_TEXTURE_2D );
};

int isExtensionSupported(const char *extension)
{
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;
  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;
  extensions = glGetString(GL_EXTENSIONS);
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *) start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return 1;
    start = terminator;
  }
  return 0;
}

long fast_floor (float _f) 
{
    DWORD dw = reinterpret_cast<DWORD &> (_f);
    if (long (dw) < 0) {
        //
        // For negative values.
        //
        dw &= 0x7FFFFFFF;
        if (dw == 0)
            return 0;

        const int sh = 23 + 127 - (dw >> 23);
        if (sh >= 24)
            return -1;
        else if (sh < 0) {
            // NOTE: precision is lost.
            return - long ((0x00800000 | (dw & 0x007FFFFF)) << (-sh));
        } else {
            if (dw & (0x007FFFFF >> (23 - sh)))
                // NOTE: the number has fractional part.
                return - long ((0x00800000 | (dw & 0x007FFFFF)) >> sh) - 1;
            else
                // NOTE: the number is whole.
                return - long ((0x00800000 | (dw & 0x007FFFFF)) >> sh);
        }
    } else {
        //
        // For positive values.
        //
        if (dw == 0)
            return 0;

        const int sh = 23 + 127 - (dw >> 23);
        if (sh >= 24)
            return 0;
        else if (sh < 0)
            // NOTE: the precision is lost.
            return (0x00800000 | (dw & 0x007FFFFF)) << (-sh);
        else 
            return (0x00800000 | (dw & 0x007FFFFF)) >> sh;
    }
}

void drawGrid(Vec2d* start, Vec2d* end, Vec2d* trans, float scale, float gridsize){
    Vec2d first;
    Vec2d padding;
    float sgridsize = gridsize*scale; 
    first.y = trans->y/(sgridsize);
    first.x = trans->x/(sgridsize);
    padding.y = (first.y-(int)first.y)*sgridsize;
    padding.x = (first.x-(int)first.x)*sgridsize;
    if (padding.y < 0)padding.y+=sgridsize;
    if (padding.x < 0)padding.x+=sgridsize;
    //first.y=(int)first.y;
    //first.x=(int)first.x;
    int i;
    float d = 0;
    for (i=0;d<end->y;i+=1){
		d = (i*sgridsize)+padding.y+start->y;
        glVertex2f(start->x,d);
        glVertex2f(end->x,d);
    };
    d = 0;
    for (i=0;d<end->x;i+=1)
	{
        d = (i*sgridsize)+padding.x+start->x;
    	glVertex2f(d,start->y);	
        glVertex2f(d,end->y);
    };                            
    
};

/*

char*     alBuffer;             //data for the buffer
ALenum alFormatBuffer;    //buffer format
ALsizei   alFreqBuffer;       //frequency
long       alBufferLen;        //bit depth
ALboolean    alLoop=true;         //loop
ALuint Sources[100];      //source
ALuint SampleSet[100];

 
int currentbuf=0;
bool runned=false;

void SoundPlay(char * fileName){
    
if (currentbuf >= 20) currentbuf = 0;

if (!runned) {
    runned = true;
    alGenSources(20, Sources);
    alGenBuffers(20, SampleSet);
};

    alDeleteSources(1,&Sources[currentbuf]);
    alDeleteBuffers(1,&SampleSet[currentbuf]);
    
    alGenSources(1, &Sources[currentbuf] );
    alGenBuffers(1, &SampleSet[currentbuf] );

alutLoadWAVFile(fileName,&alFormatBuffer, (void **) &alBuffer,(unsigned int *)&alBufferLen, &alFreqBuffer, &alLoop);
    alBufferData(SampleSet[currentbuf], alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
    
    alSourcei(Sources[currentbuf], AL_BUFFER, SampleSet[currentbuf]);
    
alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer); 

alSourcePlay(Sources[currentbuf]);


currentbuf++;
    
};
*/

