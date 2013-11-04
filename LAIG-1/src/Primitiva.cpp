
#include <GL/glew.h>

#include "Primitiva.h"
using namespace std;

Rectangle::Rectangle(float x1,float x2,float y1,float y2)
{
	this->x1=x1;
	this->x2=x2;
	this->y1=y1;
	this->y2=y2;
}

void Rectangle::draw(){
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);
		glVertex2f(x1,y1);
		glTexCoord2f(1,0);
		glVertex2f(x2,y1);
		glTexCoord2f(1,1);
		glVertex2f(x2,y2);
		glTexCoord2f(0,1);
		glVertex2f(x1,y2);
	glEnd();
}

void Triangle::calculateTexCoords()
{
	float width, height;
	float angle;
	float dotProduct;

	float vecAC_x, vecAC_y, vecAC_z, vecAB_x, vecAB_y, vecAB_z;

	vecAC_x = x3-x1;
	vecAC_y = y3-y1;
	vecAC_z = z3-z1;

	vecAB_x = x2-x1;
	vecAB_y = y2-y1;
	vecAB_z = z2-z1;

	dotProduct = (vecAC_x)*(vecAB_x) + (vecAC_y)*(vecAB_y) + (vecAC_z)*(vecAB_z);

	lengthAC = sqrt((vecAC_x)*(vecAC_x) + (vecAC_y)*(vecAC_y) + (vecAC_z)*(vecAC_z));
	lengthAB = sqrt((vecAB_x)*(vecAB_x) + (vecAB_y)*(vecAB_y) + (vecAB_z)*(vecAB_z));

	angle = acos(dotProduct/(lengthAB*lengthAC));

	width = cos(angle)*lengthAC;
	height = sin(angle)*lengthAC;

	texCoord_x = width;
	texCoord_y = height;
}

Triangle::Triangle(float x1,float x2,float x3,float y1, float y2, float y3, float z1, float z2, float z3)
{
	this->x1=x1;
	this->x2=x2;
	this->x3=x3;
	this->y1=y1;
	this->y2=y2;
	this->y3=y3;
	this->z1=z1;
	this->z2=z2;
	this->z3=z3;

	normal_x = 0;
	normal_y = 0;
	normal_z = 0;

	normal_x += (y1-y2)*(z1-z2);
	normal_y += (z1-z2)*(x1-x2);
	normal_z += (x1-x2)*(y1-y2);

	normal_x += (y2-y3)*(z2-z3);
	normal_y += (z2-z3)*(x2-x3);
	normal_z += (x2-x3)*(y2-y3);

	normal_x += (y3-y1)*(z3-z1);
	normal_y += (z3-z1)*(x3-x1);
	normal_z += (x3-x1)*(y3-y1);

	calculateTexCoords();
}

void Triangle::draw()
{
	glBegin(GL_TRIANGLES);
		glNormal3f(normal_x, normal_y, normal_z);
		glTexCoord2d(0,0);
		glVertex3f(x1, y1, z1);
		glTexCoord2d(lengthAB,0);
        glVertex3f(x2, y2, z2);
		glTexCoord2d(texCoord_x,texCoord_y);
        glVertex3f(x3, y3, z3);
	glEnd();
}

Cylinder::Cylinder(float base, float top, float height, int slices, int stacks)
{
	this->base = base;
	this->top = top;
	this->height = height;
	this->slices = slices;
	this->stacks = stacks;
}

void Cylinder::draw()
{
	GLUquadric *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, true);
	gluCylinder(quadric, base, top, height, slices, stacks);
	glRotatef(180, 1, 0, 0);
	gluDisk(quadric, 0, base, slices, 1);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, 0, height);
	gluDisk(quadric, 0, top, slices, 1);
	glTranslatef(0, 0, -height);
	gluDeleteQuadric(quadric);
}

Sphere::Sphere(float radius, int slices, int stacks)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
}

void Sphere::draw()
{
	GLUquadric *quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, true);
	gluSphere(quadric, radius, slices, stacks);
	gluDeleteQuadric(quadric);
}

Torus::Torus(float inner, float outer, int slices, int loops)
{
	this->inner = inner;
	this->outer = outer;
	this->slices = slices;
	this->loops = loops;
}

void Torus::draw()
{

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidTorus(inner, outer, slices, loops); //n�o gera coordenadas de textura
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}


//coordenadas do quadrado 1x1
/*GLfloat ctrlpoints[2][2][3] = {
	{ { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } },
	{ { 0.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 } }
};*/

GLfloat ctrlpoints[4][3] = {
	 { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },
	 { 0.0, 0.0, 1.0 }, { 1.0, 0.0, 1.0 },

};

GLfloat nrmlcompon[4][3] = { { 0.0, 0.0, 1.0 },
{ 0.0, 0.0, 1.0 },
{ 0.0, 0.0, 1.0 },
{ 0.0, 0.0, 1.0 }
};

GLfloat textpoints[4][2] = {
	{ 0.0, 0.0 },
	{ 0.0, 1.0 },
	{ 1.0, 0.0 },
	{ 1.0, 1.0 }
};

Plane::Plane(int parts)
{
	this->parts = parts;
}

/*GLfloat ctrlpoints[10][3] = {
	 { -1.5, -1.5, 4.0 }, { -0.5, -1.5, 2.0 },
	 {0.5, -1.5, -1.0 }, { 1.5, -1.5, 2.0 } ,
	 { -1.5, -0.5, 1.0 }, { -0.5, -0.5, 3.0 },
	 {0.5, -0.5, 0.0 }, { 1.5, -0.5, -1.0 } ,
	 { -1.5, 0.5, 4.0 }, { -0.5, 0.5, 0.0 }
};*/



void Plane::draw()
{
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, // U varia entre 0 e 1
		3, 2, //cada coordenada tem 3 valores , 2 = de segunda ordem
		0.0, 1.0, // V varia entre 0 e 1
		6, 2,  // 6 = 2 coordenadas por coluna * 3 valores por coordenada, 2 = de segunda ordem
		&ctrlpoints[0][0]); //pontos de controlo	
	glMap2f(GL_MAP2_NORMAL, 0.0, 1.0, 3, 2, 0.0, 1.0, 6, 2, &nrmlcompon[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &textpoints[0][0]); //definir coordenadas de textura


	glMapGrid2f(parts, 0.0, 1.0, parts, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, parts, 0, parts);

	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_TEXTURE_COORD_2);

}

Patch::Patch(int order, int partsU, int partsV, string compute)
{
	this->order = order;
	this->partsU = partsU;
	this->partsV = partsV;

	if (compute == "point")
		mode = GL_POINT;
	else if (compute == "line")
		mode = GL_LINE;
	else if (compute == "fill")
		mode = GL_FILL;
}

void Patch::addControlPoint(float x, float y, float z)
{
	controlpoints.push_back(controlpoint(x, y, z));
}
GLfloat ctrlpoints_flyingVehicle[9][3] = {
	/*{ 0, 0.0, 2.0 }, { 0.5, 1.5, 2.0 },
	{ 0.5, 1.5, 2.0 }, { 1.5, 0, 1.5 }*/
	/*{ 0, 0, 0 }, { 0.5, 0, 0 }, { 1, 0, 0 },
	{ 0, 0, 0.5 }, { 0.5, 1, 0.5 }, { 1, 0, 0.5 }, 
	{ 0, 0, 1 }, { 0.5, 0, 1 }, { 1, 0, 1 }*/

	{ 0.25, 0, 0.25 }, { 0.5, 0, 0 }, { 0.75, 0, 0.25 },
	{ 0, 0, 0.5 }, { 0.5, 1, 0.5 }, { 1, 0, 0.5 },
	{ 0.25, 0, 0.75 }, { 0.5, 0, 1 }, { 0.75, 0,0.75 }

	/*{ 0.25, 0.25, 0.25 }, { 0.5, 0, 0 }, {0.75, 0.25, 0.25},
	{ 0, 0, 0.5 }, { 0.5, 0.5, 0.5 }, {1, 0, 0.5},
	{ 0, 0.25, 0.75}, { 0.5, 0, 1 }, {0.75, 0.25, 0.75},*/
};

void Patch::draw()
{
	int size = controlpoints.size();

	GLfloat *ctrl_points = new GLfloat[size*3];

	for (int i = 0, j = 0; i < size*3; j++) //popular o array com os controlpoints
	{
		ctrl_points[i++] = controlpoints[j].x;
		ctrl_points[i++] = controlpoints[j].y;
		ctrl_points[i++] = controlpoints[j].z;
	}

	glPushMatrix();
	glEnable(GL_MAP2_VERTEX_3);

	GLint currentFrontFace;
	glGetIntegerv(GL_FRONT_FACE, &currentFrontFace);

	if ( currentFrontFace == GL_CW)
	{
		glEnable(GL_AUTO_NORMAL);
	}
	else if (currentFrontFace == GL_CCW)
	{
		glFrontFace(GL_CW);
		glEnable(GL_AUTO_NORMAL);
		glFrontFace(GL_CCW);
	}

	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order + 1, 0.0, 1.0, 3 * (order + 1), order + 1, &ctrl_points[0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, &textpoints[0][0]); //definir coordenadas de textura

	glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);
	glEvalMesh2(mode, 0, partsU, 0, partsV);

	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_AUTO_NORMAL);
	glPopMatrix();
}

void Vehicle::draw()
{
	Patch p1(2, 10, 10, "fill");
	p1.addControlPoint(0.25, 0, 0.25);
	p1.addControlPoint(0.5, 0, 0);
	p1.addControlPoint(0.75, 0, 0.25);
	p1.addControlPoint(0, 0, 0.5);
	p1.addControlPoint(0.5, 1, 0.5);
	p1.addControlPoint(1, 0, 0.5);
	p1.addControlPoint(0.25, 0, 0.75);
	p1.addControlPoint(0.5, 0, 1);
	p1.addControlPoint(0.75, 0, 0.75);

	Rectangle asa(-0.25, 0.25, -2, 2);
	Triangle miniAsa(2.25, 1.75, 2, 0, 0, -0.5, 0, 0, 0);
	Rectangle asaLigacao(0, 0.15, 0, 0.5);
	//glTranslatef(3, 3, 3); //so para nao come�ar na origem
	
	//glTranslatef(-5, 5, -5);
	glRotatef(90, 0, 1, 0);
	glScalef(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslatef(-0.5, -0.25, 1);
	asaLigacao.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -0.25, -1);
	asaLigacao.draw();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	miniAsa.draw();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	miniAsa.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.25, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5, 0, -0.5);
	asa.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(-0.5, 0, -0.5);
	asa.draw();
	glPopMatrix();

	glScalef(7, 1, 1);
	glPushMatrix();
	glTranslatef(-0.5, 0, -0.5);
	p1.draw();
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	glTranslatef(-0.5, 0, -0.5);
	p1.draw();
	glPopMatrix();
}

Waterline::Waterline()
{
	init("data//vertexShaderWaterline.vert", "data//fragmentShaderWaterline.frag");
	CGFshader::bind();
	// Initialize parameter in memory
	normScale = 0.6;

	// Store Id for the uniform "normScale", new value will be stored on bind()
	scaleLoc = glGetUniformLocation(id(), "normScale");
	timeLoc = glGetUniformLocation(id(), "time");

	heightMapTexture = new CGFtexture("data//watermap.jpg");
	texture = new CGFtexture("data//water.jpg");

	// get the uniform location for the sampler
	heightMapLoc = glGetUniformLocation(id(), "hImage");
	textureLoc = glGetUniformLocation(id(), "texture");

	// set the texture id for that sampler to match the GL_TEXTUREn that you 
	// will use later e.g. if using GL_TEXTURE0, set the uniform to 0
	glUniform1i(heightMapLoc, 0);
	glUniform1i(textureLoc, 1);
	glUniform1f(scaleLoc, normScale);
	//secImageLoc = glGetUniformLocation(id(), "water");
	//glUniform1i(secImageLoc, 1);

	lastTimestamp = clock();
	CGFshader::unbind();
}

Waterline::Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader)
{
	init(vertexshader.c_str(), fragmentshader.c_str());
	CGFshader::bind();

	// Initialize parameter in memory
	normScale = 0.6;

	// Store Id for the uniform "normScale", new value will be stored on bind()
	scaleLoc = glGetUniformLocation(id(), "normScale");
	timeLoc = glGetUniformLocation(id(), "time");

	heightMapTexture = new CGFtexture(heightmap.c_str());
	texture = new CGFtexture(texturemap.c_str());

	// get the uniform location for the sampler
	heightMapLoc = glGetUniformLocation(id(), "hImage");
	textureLoc = glGetUniformLocation(id(), "texture");

	// set the texture id for that sampler to match the GL_TEXTUREn that you 
	// will use later e.g. if using GL_TEXTURE0, set the uniform to 0
	glUniform1i(heightMapLoc, 0);
	glUniform1i(textureLoc, 1);
	glUniform1f(scaleLoc, normScale);

	lastTimestamp = clock();
	CGFshader::unbind();
}

void Waterline::bind(float timestamp)
{
	CGFshader::bind();

	// update uniforms
	float timeSinceLastUpdate = ((timestamp - lastTimestamp) / CLOCKS_PER_SEC);

	if (timeSinceLastUpdate > 0.2)
	{
		totalTime += timeSinceLastUpdate;
		lastTimestamp = timestamp;
	}

	glUniform1f(timeLoc, totalTime);

	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE0);

	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	heightMapTexture->apply();

	// do the same for other textures DESCOMENTAR PARA FRAGMENTSHADER
	glActiveTexture(GL_TEXTURE1);

	texture->apply();

	glActiveTexture(GL_TEXTURE0);
}

void Waterline::draw()
{
	this->bind();

	glScalef(3, 1, 3);
	plane.draw();
	glTranslatef(0, 0, 1);
	plane.draw();
	glTranslatef(0, 0, 1);
	plane.draw();

	this->unbind();
}