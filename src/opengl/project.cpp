/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "opengl/project.hpp"

// use this header to include the OpenGL headers
// DO NOT include gl.h or glu.h directly; it will not compile correctly.
#include "application/opengl.hpp"


// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

// definitions of functions for the OpenglProject class

// constructor, invoked when object is created
OpenglProject::OpenglProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!
	
}

// destructor, invoked when object is destroyed
OpenglProject::~OpenglProject()
{
    // TODO any final cleanup of members
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
	
}

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param scene The scene to render.
 * @return true on success, false on error.
 */
bool OpenglProject::initialize( Camera* camera, Scene* scene )
{

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
/*
int i;
Vector3 myVector;
for(i=0;i<12;i+=3){
	myVector = this->scene.mesh.vertices[i];
glVertex3f( myVector.x, myVector.y, myVector.z ); 
myVector = this->scene.mesh.vertices[i+1];
glVertex3f( myVector.x, myVector.y, myVector.z ); 
myVector = this->scene.mesh.vertices[i+2];
glVertex3f( myVector.x, myVector.y, myVector.z ); 
}
*/
    // copy scene
    this->scene = *scene;

	glEnable(GL_LIGHTING);
    GLfloat lightpos[] = { 0.0, 0.0, 15.0 };
    GLfloat lightcolor[] = { 1.0, 1.0, 0.0 };
    GLfloat ambcolor[] = { 0.0, 0.0, 1.0 };

    glEnable(GL_LIGHTING);                               // enable lighting
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambcolor);     // ambient light

    glEnable(GL_LIGHT0);                                 // enable light source
    glLightfv(GL_LIGHT0,GL_POSITION,lightpos);           // config light source
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightcolor);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightcolor);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightcolor);



	//glOrtho(-1.0,1.0,-1.0,1.0,-5.0,5.0);
	Vector3 camPos = (*camera).get_position();
	Vector3 camDir = (*camera).get_direction();
	Vector3 camUp = (*camera).get_up();

	std::cout << "Position: (" << camPos.x << "," << camPos.y << "," << camPos.z << ")\n";
	std::cout << "Direction: (" << camDir.x << "," << camDir.y << "," << camDir.z << ")\n";
	std::cout << "Up: (" << camUp.x << "," << camUp.y << "," << camUp.z << ")\n";

	Vector3 newPos;
	newPos.x = 3;
	newPos.y = 0;
	newPos.z = 0;

//	this->nList = (NormalList*)malloc(sizeof(NormalList) * GOOD_PRIME);

	//doing normal calculations
	ZeroMemory( &nList, sizeof( nList ) );
	Vector3 vertA, vertB, vertC, vectorU, vectorV, normal;
	int i;
	double x, y, z;
	long hashA, hashB, hashC;
	
//for each triangle get surface normal
//index into hashmap by vertex and reaverage normal
for(i=0;i<936;i+=3){
	//get vertices
	vertA = this->scene.mesh.vertices[i]; 
	vertB = this->scene.mesh.vertices[i+1];
	vertC = this->scene.mesh.vertices[i+2];

	//compute vectors
	vectorU = vertB - vertA;
	vectorV = vertC - vertA;

	//cross product
	normal.x = vectorU.y*vectorV.z - vectorU.z*vectorV.y;
	normal.y = vectorU.z*vectorV.x - vectorU.x*vectorV.z;
	normal.z = vectorU.x*vectorV.y - vectorU.y*vectorV.x;

	//calculate hashes per vertex
	x = ceil(abs(vertA.x));
	y = ceil(abs(vertA.y));
	z = ceil(abs(vertA.z));
	hashA = ((int)(pow(x , y) * pow(y , z)))% GOOD_PRIME;
/*	x = abs(vertB.x);
	y = abs(vertB.y);
	z = abs(vertB.z);
	hashB = ((long)(ceil(pow(x , y) * pow(y , z))))% GOOD_PRIME;
	x = abs(vertC.x);
	y = abs(vertC.y);
	z = abs(vertC.z);
	hashC = ((long)(ceil(pow(x , y) * pow(y , z))))% GOOD_PRIME;
*/
	//update normal value
	NormalList* temp =  &(this->nList[hashA]);
	updateNormalList(vertA, temp, normal);
//	updateNormalList(vertB, hashB, normal);
//	updateNormalList(vertC, hashC, normal);


	/*
	glVertex3f( vertA.x, vertA.y, vertA.z );
	glVertex3f( vertB.x, vertB.y, vertB.z );
	glVertex3f( vertC.x, vertC.y, vertC.z ); 
	*/

//	std::cout << "VECTOR: (" << myVector.x << "," << myVector.y << "," << myVector.z << ")\n";
}

	//(*camera).position = newPos;



    // TODO opengl initialization code and precomputation of mesh/heightmap
    return true;
}

void OpenglProject::updateNormalList(Vector3 vertex, NormalList* nl, Vector3 normal){
	if(nl->accessed == 0){
		//empty bin, new vertex by default
		std::cout << "NEW VERTEX (" << vertex.x << "," << vertex.y << "," << vertex.z << ")\n"; 
	nl->vertex = vertex;
	nl->accessed++;
	nl->average = normal;
	}
	else {
		//something in the bin
		if((nl->vertex.x == vertex.x) && (nl->vertex.y == vertex.y) && (nl->vertex.z == vertex.z)){
			//found our old vertex, lets update
			nl->average = ((nl->average * nl->accessed) + normal)/(nl->accessed + 1);
			nl->accessed++;
		std::cout << "FOUND OLD\n";
		}
		else{
			//keep checking along the bin
			if(nl->next != 0){
				//there is a next
				std::cout << "HAS NEXT";
				updateNormalList(vertex, nl->next, normal);
			}
			else {
				//no next, make new vertex and add next pointer
				std::cout << "NEW VERTEX (in bin)(" << vertex.x << "," << vertex.y << "," << vertex.z << ")\n"; 
				NormalList* nextVert = (NormalList*) malloc(sizeof(NormalList));
				ZeroMemory( nextVert, sizeof( NormalList ) );
					nextVert->vertex = vertex;
					nextVert->accessed++;
					nextVert->average = normal;
					nl->next = nextVert;
			}
			//recurse 
		}
	}
	
}

/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
    // TODO any cleanup code, e.g., freeing memory
	free(this->nList);
}

/**
 * Perform an update step. This happens on a regular interval.
 * @param dt The time difference from the previous frame to the current.
 */
void OpenglProject::update( real_t dt )
{
	
    // update our heightmap
    scene.heightmap->update( dt );
	

    // TODO any update code, e.g. commputing heightmap mesh positions and normals
}

Vector3 OpenglProject::getNormal(Vector3 vertex){
	
	int x,y,z, hash;

	x = ceil(abs(vertex.x));
	y = ceil(abs(vertex.y));
	z = ceil(abs(vertex.z));
	hash = ((int)(pow(x , y) * pow(y , z)))% GOOD_PRIME;
	NormalList* nl =  &(this->nList[hash]);

	if((nl->vertex.x == vertex.x) && (nl->vertex.y == vertex.y) && (nl->vertex.z == vertex.z)){
			//found vertex
			return nl->average;
		}
	else{
		while(nl->next !=0){
			nl=nl->next;
			if((nl->vertex.x == vertex.x) && (nl->vertex.y == vertex.y) && (nl->vertex.z == vertex.z)){
				//found vertex
				return nl->average;
			}
		}
		std::cout << "UH OH COULDN'T FIND NORMAL!\n";
		return 0;
	}
}

void OpenglProject::myDraw(){
	Vector3 normal, vertA, vertB, vertC;
	int i, hash;
	NormalList temp;

	glBegin( GL_LINE_STRIP); 
glColor3f( 0.0f, 0.0f, 1.0f ); // sets color to blue 
ZeroMemory( &nList, sizeof( nList ) );
for(i=0;i<936;i+=3){
	//get vertices
	vertA = this->scene.mesh.vertices[i]; 
	vertB = this->scene.mesh.vertices[i+1];
	vertC = this->scene.mesh.vertices[i+2];

	//normal = getNormal(vertA);

	glVertex3f( vertA.x, vertA.y, vertA.z );
	glVertex3f( vertB.x, vertB.y, vertB.z );
	glVertex3f( vertC.x, vertC.y, vertC.z ); 

//	std::cout << "VECTOR: (" << myVector.x << "," << myVector.y << "," << myVector.z << ")\n";
}
glEnd(); 
glFlush();
}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see math/camera.hpp
 */
void OpenglProject::render( const Camera* camera )
{
	Vector3 vPos, vAt, vCen, vUp;
	
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode( GL_PROJECTION ); // set current matrix 
	glLoadIdentity(); // Clears the matrix 
	gluPerspective(60,1,2,30);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	vPos = (*camera).get_position();
	vAt = (*camera).get_direction();
	vCen = vPos + vAt;
	vUp = (*camera).get_up();
	gluLookAt(vPos.x, vPos.y, vPos.z, vCen.x, vCen.y, vCen.z, vUp.x, vUp.y,  vUp.z);

    
	myDraw();

	

    // TODO render code
	




}



} /* _462 */

