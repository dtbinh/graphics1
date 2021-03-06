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
#include <stdio.h>
#include <string.h>

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

	int numTri;
    // copy scene
	this->scene = *scene;
	
	int numVertex = this->scene.mesh.num_vertices;
	normalList = (Vector3*) malloc(sizeof(Vector3) * numVertex);
	memset(normalList, 0, sizeof(Vector3) * numVertex);

GLfloat mat_specular[] = { 0.5, 0.5, 1.0, 0.5 };
   GLfloat mat_shininess[] = { 20.0 };
   GLfloat light_position[] = { 10.0, 7.0, 15.0, 0.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);


	//glOrtho(-1.0,1.0,-1.0,1.0,-5.0,5.0);
	Vector3 camPos = (*camera).get_position();
	Vector3 camDir = (*camera).get_direction();
	Vector3 camUp = (*camera).get_up();

	std::cout << "Position: (" << camPos.x << "," << camPos.y << "," << camPos.z << ")\n";
	std::cout << "Direction: (" << camDir.x << "," << camDir.y << "," << camDir.z << ")\n";
	std::cout << "Up: (" << camUp.x << "," << camUp.y << "," << camUp.z << ")\n";

	Vector3 newPos, temp;
	newPos.x = 3;
	newPos.y = 0;
	newPos.z = 0;

	int indA, indB, indC;
	Vector3 vertA, vertB, vertC, vectorU, vectorV, normal;
	int i;

	
//for each triangle get surface normal
//index into list by vertex and reaverage normal
numTri = this->scene.mesh.num_triangles;

	for(i=0;i<numTri;i++){
		//get vertex index
		indA = this->scene.mesh.triangles[i].vertices[0]; 
		indB = this->scene.mesh.triangles[i].vertices[1]; 
		indC = this->scene.mesh.triangles[i].vertices[2]; 

		//get vertex
		vertA = this->scene.mesh.vertices[indA];
		vertB = this->scene.mesh.vertices[indB];
		vertC = this->scene.mesh.vertices[indC];

	//compute vectors
	vectorU = vertB - vertA;
	vectorV = vertC - vertA;

	//cross product
	normal.x = vectorU.y*vectorV.z - vectorU.z*vectorV.y;
	normal.y = vectorU.z*vectorV.x - vectorU.x*vectorV.z;
	normal.z = vectorU.x*vectorV.y - vectorU.y*vectorV.x;

	//set vertex a
	temp = normalList[indA];
	temp += normalize(normal);
	normalList[indA] = normalize(temp);	

	//set vertex b
	temp = normalList[indB];
	temp += normalize(normal);
	normalList[indB] = normalize(temp);

	//set vertex c
	temp = normalList[indC];
	temp += normalize(normal);
	normalList[indC] = normalize(temp);
}

	zWater = (float*) malloc (sizeof(float) * WATER_ARRAY_SIZE_X * WATER_ARRAY_SIZE_Y);
	int index = 0;
	for(int y = 0; y < WATER_ARRAY_SIZE_Y; y++){
		for(int x = 0; x < WATER_ARRAY_SIZE_X; x++){
			zWater[index++] = this->scene.heightmap->compute_height(Vector2(-1.0 + (2.0/WATER_ARRAY_SIZE_X)*x, -1.0 + (2.0/WATER_ARRAY_SIZE_Y)*y));
		}
	}

    return true;
}
/*
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
				memset( nextVert, 0, sizeof( NormalList ) );
					nextVert->vertex = vertex;
					nextVert->accessed++;
					nextVert->average = normal;
					nl->next = nextVert;
			}
			//recurse 
		}
	}
	
}
*/
/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
    // TODO any cleanup code, e.g., freeing memory
//	free(this->nList);
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
/*
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
		return vertex;
	}
}
*/

void OpenglProject::myWaterDraw(){

	glBegin(GL_LINE_STRIP);
	int index = 0;
	for(int y = 0; y < WATER_ARRAY_SIZE_Y; y++){
		for(int x = 0; x < WATER_ARRAY_SIZE_X; x++){
			glVertex3f(-1.0 + (2.0/WATER_ARRAY_SIZE_X)*x, zWater[index++], -1.0 + (2.0/WATER_ARRAY_SIZE_Y)*y);
		}
	}

	glEnd();
	glFlush();

}

void OpenglProject::myDraw(){
	int cinCrap;
	Vector3 normal, vertA, vertB, vertC;
	int i, hash, indA, indB, indC;
	NormalList temp;

	glBegin( GL_TRIANGLES); 
glColor3f( 0.0f, 0.0f, 1.0f ); // sets color to blue 
//memset( &nList, 0, sizeof( nList ) );
int numTri = this->scene.mesh.num_triangles;

	for(i=0;i<numTri;i++){
		//get vertex index
		indA = this->scene.mesh.triangles[i].vertices[0]; 
		indB = this->scene.mesh.triangles[i].vertices[1]; 
		indC = this->scene.mesh.triangles[i].vertices[2]; 

		//get vertex
		vertA = this->scene.mesh.vertices[indA];
		vertB = this->scene.mesh.vertices[indB];
		vertC = this->scene.mesh.vertices[indC];

	normal = normalList[indA];
	glNormal3f(normal.x, normal.z, normal.y);
	glVertex3f( vertA.x, vertA.y, vertA.z );
	
	normal = normalList[indB];
	glNormal3f(normal.x, normal.z, normal.y);
	glVertex3f( vertB.x, vertB.y, vertB.z );
	

	normal = normalList[indC];
	glNormal3f(normal.x, normal.z, normal.y);
	glVertex3f( vertC.x, vertC.y, vertC.z );
//glVertex3f( vertB.x, vertB.y, vertB.z );
	//glVertex3f( vertC.x, vertC.y, vertC.z ); 

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




    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	vPos = (*camera).get_position();
	vAt = (*camera).get_direction();
	vCen = vPos + vAt;
	vUp = (*camera).get_up();
	gluLookAt(vPos.x, vPos.y, vPos.z, vCen.x, vCen.y, vCen.z, vUp.x, vUp.y,  vUp.z);

	glPushMatrix();
	Vector3 meshTrans = this->scene.mesh_position.position;
	glTranslatef(meshTrans.x, meshTrans.y, meshTrans.z);
	Quaternion meshRot = this->scene.mesh_position.orientation;
	glRotatef(meshRot.w / (2 * PI) * 360.0, meshRot.x, meshRot.y, meshRot.z);
	Vector3 meshSc = this->scene.mesh_position.scale;
    	glScalef(meshSc.x, meshSc.y, meshSc.z);
	myDraw();
	glPopMatrix();

	glPushMatrix();	
	Vector3 heightTrans = this->scene.heightmap_position.position;
	glTranslatef(heightTrans.x, heightTrans.y, heightTrans.z);
	Quaternion heightRot = this->scene.heightmap_position.orientation;
	glRotatef(heightRot.w / (2 * PI) * 360.0, heightRot.x, heightRot.y, heightRot.z);
	Vector3 heightSc = this->scene.heightmap_position.scale;
    	glScalef(heightSc.x, heightSc.y, heightSc.z);
	myWaterDraw();
	glPopMatrix();

	glMatrixMode( GL_PROJECTION ); // set current matrix 
	glLoadIdentity(); // Clears the matrix 
	gluPerspective(60,1,2,30);

	

    // TODO render code
	




}



} /* _462 */

