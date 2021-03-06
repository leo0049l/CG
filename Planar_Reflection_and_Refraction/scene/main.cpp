#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "iostream"
#include "fstream"
#include "mesh.h"
#include "FreeImage.h"
#include "glew.h"
#include "glut.h"
#define PI 3.14159265
using namespace std;
int pic = -1;
unsigned int texObject[100];
float tr = 0.5f;
int c1 = 5,c2=5 ;
float r = 0.5f;
class light_f
{
private:
	float light[8][12];
	float ambient[4];
	int counter;
public:
	light_f::light_f()
	{
		FILE* f_light;
		char line[512];
		counter = 0;
		f_light = fopen("CornellBox.light", "r");
		while (fgets(line, 512, f_light) != NULL)
		{
			int len = strlen(line) - 1;
			if (line[len] < 48)
				line[len] = '\0';
			else
				line[len + 1] = '\0';
			char* buf = strtok(line, " ");
			if (buf[0] == 'l')
			{
				for (int i = 0; i < 12; i++)
				{
					buf = strtok(NULL, " ");
					light[counter][i] = atof(buf);
				}   counter++;
			}
			else
			{
				for (int j = 0; j < 3; j++)
				{
					buf = strtok(NULL, " ");
					ambient[j] = atof(buf);
				}   ambient[3] = 1;
			}
		}
		fclose(f_light);

	}
	float light_r(int x, int y){ return light[x][y]; }
	float* ambient_r(){ return ambient; }
	int counter_r(){ return counter; }
	void ambient_w(float i){ ambient[3] = i; }
};
light_f l;
class view_f
{
private:
	float eye[3];
	float vat[3];
	float vup[3];
	float fovy;
	float dnear;
	float dfar;
	float viewport[4];
public:
	view_f::view_f()
	{
		FILE* f_view;
		char line[512];
		f_view = fopen("CornellBox.view", "r");
		while (fgets(line, 512, f_view) != NULL)
		{

			int len = strlen(line) - 1;
			if (line[len]<48)
				line[len] = '\0';
			else line[len + 1] = '\0';
			char* buf = strtok(line, " ");
			if (!strcmp(buf, "eye"))
			{
				for (int i = 0; i < 3; i++)
				{
					buf = strtok(NULL, " ");
					eye[i] = atof(buf);
				}
			}
			else if (!strcmp(buf, "vat"))
			{
				for (int i = 0; i < 3; i++)
				{
					buf = strtok(NULL, " ");
					vat[i] = atof(buf);
				}
			}
			else if (!strcmp(buf, "vup"))
			{
				for (int i = 0; i < 3; i++)
				{
					buf = strtok(NULL, " ");
					vup[i] = atof(buf);
				}
			}
			else if (!strcmp(buf, "fovy"))
			{
				buf = strtok(NULL, " ");
				fovy = atof(buf);
			}
			else if (!strcmp(buf, "dnear"))
			{
				buf = strtok(NULL, " ");
				dnear = atof(buf);
			}
			else if (!strcmp(buf, "dfar"))
			{
				buf = strtok(NULL, " ");
				dfar = atof(buf);
			}
			else if (!strcmp(buf, "viewport"))
			{
				for (int i = 0; i < 4; i++)
				{
					buf = strtok(NULL, " ");
					viewport[i] = atof(buf);
				}
			}
		}
		fclose(f_view);
	}
	float eye_r(int i){ return eye[i]; }
	float vat_r(int i){ return vat[i]; }
	float vup_r(int i){ return vup[i]; }
	float fovy_r(){ return fovy; }
	float dnear_r(){ return dnear; }
	float dfar_r(){ return dfar; }
	float viewport_r(int i){ return viewport[i]; }
	void zoomin()
	{
		float vec;
		for (int i = 0; i < 3; i++)
		{
			vec = (vat[i] - eye[i]) / 3;
			eye[i] += vec;
		}
	}
	void zoomout()
	{
		float vec;
		for (int i = 0; i < 3; i++)
		{
			vec = (vat[i] - eye[i]) / 3;
			eye[i] -= vec;
		}
	}
	void moveleft()
	{
		float vec[3][3] = { { cos(15.0*PI / 180), 0, sin(15.0*PI / 180) }, { 0, 1, 0 }, { -sin(15.0*PI / 180), 0, cos(15.0*PI / 180) } };
		float x, y, z;

		x = vec[0][0] * (eye[0] - vat[0]) + vec[0][1] * (eye[1] - vat[1]) + vec[0][2] * (eye[2] - vat[2]);
		y = vec[1][0] * (eye[0] - vat[0]) + vec[1][1] * (eye[1] - vat[1]) + vec[1][2] * (eye[2] - vat[2]);
		z = vec[2][0] * (eye[0] - vat[0]) + vec[2][1] * (eye[1] - vat[1]) + vec[2][2] * (eye[2] - vat[2]);
		eye[0] = vat[0] + x; eye[1] = vat[1] + y; eye[2] = vat[2] + z;
	}
	void moveright()
	{
		float vec[3][3] = { { cos(-15.0*PI / 180), 0, sin(-15.0*PI / 180) }, { 0, 1, 0 }, { -sin(-15.0*PI / 180), 0, cos(-15.0*PI / 180) } };
		float x, y, z;

		x = vec[0][0] * (eye[0] - vat[0]) + vec[0][1] * (eye[1] - vat[1]) + vec[0][2] * (eye[2] - vat[2]);
		y = vec[1][0] * (eye[0] - vat[0]) + vec[1][1] * (eye[1] - vat[1]) + vec[1][2] * (eye[2] - vat[2]);
		z = vec[2][0] * (eye[0] - vat[0]) + vec[2][1] * (eye[1] - vat[1]) + vec[2][2] * (eye[2] - vat[2]);
		eye[0] = vat[0] + x; eye[1] = vat[1] + y; eye[2] = vat[2] + z;
	}
	void viewport_w(GLsizei x, GLsizei y){ viewport[2] = x; viewport[3] = y; }
};
view_f v;
mesh *object[100];
int object_counter = 0;
class scene_f
{
private:
	char obj[100][30];
	float parameter[100][10];
	char name[50][30];
	int counter;//object number
	int count;//pic number
	int num[100];
	int type[100];// object  //0 for no-texture // 1 for single // 2 for multi // 3 for cube
	int type1[50];// picture //
public:
	scene_f::scene_f()
	{
		count = 0;
		counter = 0;
	}
	void setobj(int i, char* a)
	{
		strcpy(obj[i], a);
	}
	void setparameter(int i, int j, float value)
	{
		parameter[i][j] = value;
	}
	void p1()
	{
		counter++;
	}
	void p11()
	{
		count++;
	}
	void settype(int i,int j)
	{
		type[i]= j;
	}
	void settype1(int i, int j)
	{
		type1[i] = j;
	}
	void setnum(int i,int j){ num[i] = j; };
	int num_r(int i){ return num[i]; }
	char* obj_r(int i){ return obj[i]; }
	float parameter_r(int x, int y){ return parameter[x][y]; }
	int counter_r(){ return counter; }
	int count_r(){ return count; }
	int type_r(int i){ return type[i]; }
	int type1_r(int i){ return type1[i]; }
	void setname(char* buf, int i){ strcpy(name[i], buf); }
	char* getname(int i){ return name[i]; }
};
scene_f s;
class texture_f
{
private:
public:
	texture_f::texture_f()
	{
		FILE* f_texture;
		char line[512];
		int count1 = 0;//object number
		int count = 0; // pic num
		int type;
		f_texture = fopen("CornellBox.scene", "r");
		while (fgets(line, 512, f_texture) != NULL)
		{

			int len = strlen(line) - 1;
			if (line[len]<48)
				line[len] = '\0';
			else line[len + 1] = '\0';
			char* buf = strtok(line, " ");
			if (!strcmp(buf, "model"))
			{
				buf = strtok(NULL, " ");
				s.setobj(count1, buf);
				for (int i = 0; i < 10; i++)
				{
					buf = strtok(NULL, " ");
					s.setparameter(count1, i, atof(buf));
				} 
				s.settype(count1, type);
				object[object_counter] = new mesh(s.obj_r(count1));
				object_counter++;
				s.setnum(count1,pic);
				s.p1();
				count1++;
			}
			else
			{
				if (buf[0] == 'n')
				{
					type = 0;
					s.settype1(count, 0);
				}
				else if (buf[0] == 's')
				{
					type = 1;
					buf = strtok(NULL, " ");
					s.setname(buf, count);
					s.settype1(count, 1);
					pic += 1; s.p11(); count++;
				}
				else if (buf[0] == 'm')
				{
					type = 2;
					buf = strtok(NULL, " ");
					s.setname(buf, count);
					s.settype1(count, 2);
					buf = strtok(NULL, " ");
					s.setname(buf, count+1);
					s.settype1(count+1, 2);
					pic += 2; s.p11(); s.p11(); count += 2;
				}
				else if (buf[0] == 'c')
				{
					type = 3;
					for (int i = 0; i < 6; i++)
					{
						buf = strtok(NULL, " ");
						s.setname(buf, count+i);
						s.settype1(count+i, 3);
						s.p11();
					} pic += 1; count += 6;

				}

			}
		}
		fclose(f_texture);
		cout << object_counter << endl;

	}
};
texture_f t;
void reshape(GLsizei x, GLsizei y){ v.viewport_w(x, y); }
void lighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	for (int i = 0; i <l.counter_r(); i++)
	{

		GLfloat light_specular[] = { l.light_r(i, 9), l.light_r(i, 10), l.light_r(i, 11), 1 };
		GLfloat light_diffse[] = { l.light_r(i, 6), l.light_r(i, 7), l.light_r(i, 8), 1 };
		GLfloat light_ambient[] = { l.light_r(i, 3), l.light_r(i, 4), l.light_r(i, 5), 1 };
		GLfloat light_position[] = { l.light_r(i, 0), l.light_r(i, 1), l.light_r(i, 2), 1 };

		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light_diffse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light_ambient);
	} l.ambient_w(1.0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, l.ambient_r());


}
void display()
{
	//CLEAR THE BUFFER
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	//glAlphaFunc(GL_GREATER, 0.5f);
	//glDepthFunc(GL_LEQUAL);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

	glViewport(v.viewport_r(0), v.viewport_r(1), (GLsizei)v.viewport_r(2), (GLsizei)v.viewport_r(3));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(v.fovy_r(), (float)v.viewport_r(2) / v.viewport_r(3), v.dnear_r(), v.dfar_r());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(v.eye_r(0), v.eye_r(1), v.eye_r(2),   //eye
		v.vat_r(0), v.vat_r(1), v.vat_r(2),   //center
		v.vup_r(0), v.vup_r(1), v.vup_r(2)); //up

	lighting();
	//Step 1: Set stencil buffer
	int  i;
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);
		for (i = 0; i < s.counter_r(); i++)
		{
			if (i == s.counter_r() - 1){
				glStencilFunc(GL_ALWAYS, 1, 1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			}
			glPushMatrix();
			glTranslated(s.parameter_r(i, 7), s.parameter_r(i, 8), s.parameter_r(i, 9));
			glRotated(s.parameter_r(i, 3), s.parameter_r(i, 4), s.parameter_r(i, 5), s.parameter_r(i, 6));
			glScaled(s.parameter_r(i, 0), s.parameter_r(i, 1), s.parameter_r(i, 2));


			int lastMaterial = -1;
			for (size_t j = 0; j < object[i]->fTotal; ++j)
			{

				// set material property if this face used different material
				if (lastMaterial != object[i]->faceList[j].m)
				{

					lastMaterial = (int)object[i]->faceList[j].m;
					glMaterialfv(GL_FRONT, GL_AMBIENT, object[i]->mList[lastMaterial].Ka);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, object[i]->mList[lastMaterial].Kd);
					glMaterialfv(GL_FRONT, GL_SPECULAR, object[i]->mList[lastMaterial].Ks);
					glMaterialfv(GL_FRONT, GL_SHININESS, &object[i]->mList[lastMaterial].Ns);

					//you can obtain the texture name by object->mList[lastMaterial].map_Kd
					//load them once in the main function before mainloop
					//bind them in display function here

				}

				glBegin(GL_TRIANGLES);
				for (size_t k = 0; k < 3; ++k)
				{
					//textex corrd. object->tList[object->faceList[i][j].t].ptr
					glNormal3fv(object[i]->nList[object[i]->faceList[j][k].n].ptr);
					glVertex3fv(object[i]->vList[object[i]->faceList[j][k].v].ptr);

				}
				glEnd();
			}

			glPopMatrix();
	}
		//Step 2: Refraction
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glFrontFace(GL_CCW);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (i = 0; i < s.counter_r(); i++)
		{
			glPushMatrix();
			glTranslated(s.parameter_r(i, 7), s.parameter_r(i, 8), s.parameter_r(i, 9));
			glRotated(s.parameter_r(i, 3), s.parameter_r(i, 4), s.parameter_r(i, 5), s.parameter_r(i, 6));
			glScaled(s.parameter_r(i, 0), s.parameter_r(i, 1), s.parameter_r(i, 2));


			int lastMaterial = -1;
			for (size_t j = 0; j < object[i]->fTotal; ++j)
			{

				// set material property if this face used different material
				if (lastMaterial != object[i]->faceList[j].m)
				{

					lastMaterial = (int)object[i]->faceList[j].m;
					if (i == s.counter_r() - 1){
						object[i]->mList[lastMaterial].Ka[3] = 0.5f;  //alpha
						object[i]->mList[lastMaterial].Kd[3] = 0.5f;
						object[i]->mList[lastMaterial].Ks[3] = 0.5f;
					}
					glMaterialfv(GL_FRONT, GL_AMBIENT, object[i]->mList[lastMaterial].Ka);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, object[i]->mList[lastMaterial].Kd);
					glMaterialfv(GL_FRONT, GL_SPECULAR, object[i]->mList[lastMaterial].Ks);
					glMaterialfv(GL_FRONT, GL_SHININESS, &object[i]->mList[lastMaterial].Ns);

					//you can obtain the texture name by object->mList[lastMaterial].map_Kd
					//load them once in the main function before mainloop
					//bind them in display function here

				}

				glBegin(GL_TRIANGLES);
				for (size_t k = 0; k < 3; ++k)
				{
					//textex corrd. object->tList[object->faceList[i][j].t].ptr
					glNormal3fv(object[i]->nList[object[i]->faceList[j][k].n].ptr);
					glVertex3fv(object[i]->vList[object[i]->faceList[j][k].v].ptr);

				}
				glEnd();
			}

			glPopMatrix();
		}
		glDisable(GL_BLEND);
		glAccum(GL_ACCUM,tr);
	//Step 3: Reflection
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glFrontFace(GL_CW);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		for (i = 0; i < s.counter_r(); i++)
		{
			if (s.parameter_r(i, 7)>-20)
			{
				glPushMatrix();
				glTranslated(-20-(s.parameter_r(i, 7)+20), s.parameter_r(i, 8), s.parameter_r(i, 9));
				glRotated(s.parameter_r(i, 3), s.parameter_r(i, 4), s.parameter_r(i, 5), s.parameter_r(i, 6));
				glScaled(-s.parameter_r(i, 0), s.parameter_r(i, 1), s.parameter_r(i, 2));


				int lastMaterial = -1;
				for (size_t j = 0; j < object[i]->fTotal; ++j)
				{

					// set material property if this face used different material
					if (lastMaterial != object[i]->faceList[j].m)
					{

						lastMaterial = (int)object[i]->faceList[j].m;
						glMaterialfv(GL_FRONT, GL_AMBIENT, object[i]->mList[lastMaterial].Ka);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, object[i]->mList[lastMaterial].Kd);
						glMaterialfv(GL_FRONT, GL_SPECULAR, object[i]->mList[lastMaterial].Ks);
						glMaterialfv(GL_FRONT, GL_SHININESS, &object[i]->mList[lastMaterial].Ns);

						//you can obtain the texture name by object->mList[lastMaterial].map_Kd
						//load them once in the main function before mainloop
						//bind them in display function here

					}

					glBegin(GL_TRIANGLES);
					for (size_t k = 0; k < 3; ++k)
					{
						//textex corrd. object->tList[object->faceList[i][j].t].ptr
						glNormal3fv(object[i]->nList[object[i]->faceList[j][k].n].ptr);
						glVertex3fv(object[i]->vList[object[i]->faceList[j][k].v].ptr);

					}
					glEnd();
				}

				glPopMatrix();
			}
		}
		glAccum(GL_ACCUM, r);
		glAccum(GL_RETURN, 1.0);
        //Step 4: Combination
		glFrontFace(GL_CCW);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, 0, 255);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		for (i = 0; i < s.counter_r(); i++)
		{
			glPushMatrix();
			glTranslated(s.parameter_r(i, 7), s.parameter_r(i, 8), s.parameter_r(i, 9));
			glRotated(s.parameter_r(i, 3), s.parameter_r(i, 4), s.parameter_r(i, 5), s.parameter_r(i, 6));
			glScaled(s.parameter_r(i, 0), s.parameter_r(i, 1), s.parameter_r(i, 2));


			int lastMaterial = -1;
			for (size_t j = 0; j < object[i]->fTotal; ++j)
			{

				// set material property if this face used different material
				if (lastMaterial != object[i]->faceList[j].m)
				{

					lastMaterial = (int)object[i]->faceList[j].m;
					glMaterialfv(GL_FRONT, GL_AMBIENT, object[i]->mList[lastMaterial].Ka);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, object[i]->mList[lastMaterial].Kd);
					glMaterialfv(GL_FRONT, GL_SPECULAR, object[i]->mList[lastMaterial].Ks);
					glMaterialfv(GL_FRONT, GL_SHININESS, &object[i]->mList[lastMaterial].Ns);

					//you can obtain the texture name by object->mList[lastMaterial].map_Kd
					//load them once in the main function before mainloop
					//bind them in display function here

				}

				glBegin(GL_TRIANGLES);
				for (size_t k = 0; k < 3; ++k)
				{
					//textex corrd. object->tList[object->faceList[i][j].t].ptr
					glNormal3fv(object[i]->nList[object[i]->faceList[j][k].n].ptr);
					glVertex3fv(object[i]->vList[object[i]->faceList[j][k].v].ptr);

				}
				glEnd();
			}

			glPopMatrix();
		}
	/////////////////////////////////////////////////////////////	
	glutSwapBuffers();
	glutPostRedisplay();
}
void reflect(int i)
{
	if (i == 1)
	{
		if (c1 < 10)
		{
			r += 0.1;
			c1++;
		}
	}
	else if (i == 2)
	{
		if (c1 > 0)
		{
			r -= 0.1;
			c1--;
		}
	}
	cout<<r<<" "<<tr<<endl;
}
void trans(int i)
{
	if (i == 1){
		if (c2 < 10)
		{
			tr += 0.1;
			c2++;
		}
	}
	else if (i == 2)
	{
		if (c2>0)
		{
			tr -= 0.1;
			c2--;
		}
	}
	cout << r << " " << tr << endl;
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		v.zoomin();
		break;
	case 'a':
		v.moveleft();
		break;
	case 's':
		v.zoomout();
		break;
	case 'd':
		v.moveright();
		break;
	case 'r':
		reflect(1);
		break;
	case 'f':
		reflect(2);
		break;
	case 't':
		trans(1);
		break;
	case 'g':
		trans(2);
		break;

	}
}

void LoadTexture(char* pFilename,int n,int type,int i)
{
FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pFilename, 0), pFilename);
FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
int iWidth = FreeImage_GetWidth(p32BitsImage);
int iHeight = FreeImage_GetHeight(p32BitsImage);
if (type == 3)
{
if (i == 0)
{
glBindTexture(GL_TEXTURE_CUBE_MAP, texObject[n]);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE,
(void*)FreeImage_GetBits(p32BitsImage));
if (i==5)
glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
else
{
glBindTexture(GL_TEXTURE_2D, texObject[n]);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE,(void*)FreeImage_GetBits(p32BitsImage));
glGenerateMipmap(GL_TEXTURE_2D);
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
FreeImage_Unload(p32BitsImage);
FreeImage_Unload(pImage);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ACCUM | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("Scene");
	glewInit();
	FreeImage_Initialise();
	glGenTextures(100, texObject);
	int a = 0;
	for (int i = 0; i < s.count_r(); i++)
	{
		if (s.type1_r(i) == 1)
		{

			LoadTexture(s.getname(i), a,1,0);
			a++;
		}
		else if (s.type1_r(i) == 2)
		{
			LoadTexture(s.getname(i), a,2,0);
			a++;
			LoadTexture(s.getname(i + 1), a, 2, 1); i++;
			a++;
		}
		else if (s.type1_r(i) == 3)
		{
			for (int j = 0; j < 6; j++)
			{
				LoadTexture(s.getname(i+j), a, 3, j); 
			}a++; i += 5;
		}
	}
	FreeImage_DeInitialise();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}