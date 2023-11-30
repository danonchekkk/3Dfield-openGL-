#include <iostream>
#include <fstream>
#include "glut.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

const int sizeField = 99;
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
float cameraDistance = 125.0f;  // Начальное расстояние камеры от начала координат

bool flagTypePrint = true;//флаг того как мы рисуем(линиями или сплошной заливкой)

bool isRotatingX = false;
bool isRotatingY = false;
bool isRotatingZ = false;
float rotationSpeed = 1.0;

int minValueInMatrix = 0;//минимальное значение по оси Z
int maxValueInMatrix = 0;//максимальное значение по оси Z

class Mesh 
{
public:

	float field[sizeField][sizeField];

	Mesh() 
	{
		std::ifstream file("test_01.txt");
		for (int i = 0; i < sizeField; i++)
		{
			for (int j = 0; j < sizeField; j++)
			{
				file >> field[i][j];
				if (field[i][j] < minValueInMatrix)
					minValueInMatrix = field[i][j];
				else if (field[i][j] > maxValueInMatrix)
					maxValueInMatrix = field[i][j];
			}
		}
		file.close();
		
	}
	~Mesh() {}

	

	void Draw() //метод который рисует поверхность и сразу раскрашивает ее
	{
		for (int i = 0; i < sizeField - 1; i++)
		{
			for (int j = 0; j < sizeField - 1; j++)
			{
				if(!flagTypePrint)
					glBegin(GL_QUADS);
				else
					glBegin(GL_LINE_LOOP);

				// Рассчитываем коэффициент, зависящий от координаты по Z
				float normal = (field[i][j] - minValueInMatrix) / (maxValueInMatrix - minValueInMatrix);

				// Вычисляем цвета в зависимости от коэффициента
				float red = 255;
				float green = 1.0 - normal;
				float blue = 0.0;

				// Устанавливаем цвета узлов
				glColor3f(red, green, blue);
				glVertex3f(i, j, field[i][j]);

				glColor3f(red, green, blue);
				glVertex3f(i + 1, j, field[i + 1][j]);

				glColor3f(red, green, blue);
				glVertex3f(i + 1, j + 1, field[i + 1][j + 1]);

				glColor3f(red, green, blue);
				glVertex3f(i, j + 1, field[i][j + 1]);

				glEnd();
			}
		}
	}

	void Osi()
	{
		glLoadIdentity();
		gluLookAt(0, 0, 18.0f, 13.0f, 7.5f, 0, 0, 1, 0);
		glRotatef(angleX - 60, 1, 0, 0);
		glRotatef(angleY, 0, 1, 0);
		glRotatef(angleZ, 0, 0, 1);
		glBegin(GL_LINES);
		// Ось X (красный цвет)
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		// Ось Y (зеленый цвет)
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);
		// Ось Z (синий цвет)
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glEnd();

		glFlush();
	}
};


void idle() 
{
	glutPostRedisplay();
}

void rotateObject(int value)
{
	if (isRotatingX)
	{
		angleX += rotationSpeed; 
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, rotateObject, 0);
	}
	if (isRotatingY) 
	{
		angleY += rotationSpeed; 
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, rotateObject, 0);
	}
	if (isRotatingZ) 
	{
		angleZ += rotationSpeed;
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, rotateObject, 0);
	}
}

void read_special_kb(int key, int x, int y) 
{
	if (key == GLUT_KEY_HOME)
	{
		angleX = 0.0f;
		angleY = 0.0f;
		angleZ = 0.0f;
		cameraDistance = 125.0f;
	}

	if (key == GLUT_KEY_PAGE_DOWN)
	{
		if (flagTypePrint == false)
			flagTypePrint = true;
		else
			flagTypePrint = false;
	}
	if (key == GLUT_KEY_F7)
	{
		if (!isRotatingX && !isRotatingY && !isRotatingZ)
		{
			// начать автоматическое вращение при нажатии клавиши f7
			isRotatingX = true;
			glutTimerFunc(1000 / 60, rotateObject, 0);
		}
		else if (isRotatingX && !isRotatingY && !isRotatingZ)
		{
			isRotatingY = true;
			isRotatingX = false;
			glutTimerFunc(1000 / 60, rotateObject, 0);
		}
		else if (!isRotatingX && isRotatingY && !isRotatingZ)
		{
			isRotatingZ = true;
			isRotatingY = false;
			glutTimerFunc(1000 / 60, rotateObject, 0);
		}
		else if (!isRotatingX && !isRotatingY && isRotatingZ)
		{
			// прекратить вращение при повторном нажатии клавиши f7
			isRotatingZ = false;
		}
	}
}

void read_kb(unsigned char key, int x, int y)
{
	if (key == 's')
		angleX += 2.0;
	else if (key == 'w')
		angleX -= 2.0;

	if (key == 'd')
		angleY += 2.0;
	else if (key == 'a')
		angleY -= 2.0;

	if (key == 'q')
		angleZ += 2.0;
	else if (key == 'e')
		angleZ -= 2.0;

	if (key == 27)
		exit(0);

	if (key == '+' || key == '=')
		cameraDistance -= 2.0f;  // Уменьшаем расстояние камеры
	else if (key == '-')
		cameraDistance += 2.0f;  // Увеличиваем расстояние камеры

}

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-50.0f, -50.0f, 0.0f);//задаем начальное положение фигуры
	Mesh mesh;
	gluLookAt(0.0f, 0.0f, cameraDistance, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//начало кода
	
	glTranslatef(0, sizeField / 2, 0);
	glRotatef(angleX - 60, 1, 0, 0);
	glTranslatef(0, -sizeField / 2, 0);
	glTranslatef(sizeField / 2, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glTranslatef(-sizeField / 2, 0, 0);
	glTranslatef(sizeField / 2, sizeField / 2, sizeField / 2);
	glRotatef(angleZ, 0, 0, 1);
	glTranslatef(-sizeField / 2, -sizeField / 2, -sizeField / 2);

	mesh.Draw();
	mesh.Osi();

	//конец кода
	glutSwapBuffers();
	system("cls");
	std::cout << "\nMin value (Z) = " << minValueInMatrix << std::endl;
	std::cout << "Max value (Z) = " << maxValueInMatrix << std::endl;

}

void ReshapeWindow(GLsizei width, GLsizei height) 
{
	if (height == 0)
		height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("3D поверхность");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.27, 0.42, 0.0, 1);//цвет фона

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(read_kb);
	glutSpecialFunc(read_special_kb);

	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

