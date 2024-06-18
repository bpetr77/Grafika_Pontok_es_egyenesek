//=============================================================================================
// Mintaprogram: Zöld háromszög. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!! 
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak 
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Bakó Péter	
// Neptun : OTQVE5
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"

const char * const vertexSource = R"(
	#version 330
	precision highp float;		// normal floats, makes no difference on desktop computers

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix
	layout(location = 0) in vec2 vp;	// Varying input: vp = vertex position is expected in attrib array 0

	void main() {
		gl_Position = vec4(vp.x, vp.y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
	}
)";

const char * const fragmentSource = R"(
	#version 330
	precision highp float;	// normal floats, makes no difference on desktop computers
	
	uniform vec3 color;		// uniform variable, the color of the primitive
	out vec4 outColor;		// computed color of the current pixel

	void main() {
		outColor = vec4(color, 1);	// computed color is the color of the primitive
	}
)";

GPUProgram gpuProgram; 


class Object {
public:
	std::vector<vec3> vertices;
	unsigned int vao = 0, vbo = 0;

	void create() {
		glGenVertexArrays(1, &vao); glBindVertexArray(vao);
		glGenBuffers(1, &vbo); glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	void addVertex(const vec3& vertex) {
		vertices.push_back(vertex);
	}

	void updateGPU() {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}

	void Draw(int type, vec3 color) {
		if (vertices.size() > 0) {
			glBindVertexArray(vao);
			gpuProgram.setUniform(color, "color");
			glDrawArrays(type, 0, vertices.size());
		}
	}

};

class PointCollection {
public:
	Object points;

	void addPoint(const vec3& point) {
		points.vertices.push_back(point);
		printf("Point added: \n%.1f, %.1f\n", point.x, point.y);
	}

	vec3* findNearbyPoint(const vec3& point) {
		for (vec3& p : points.vertices) {
			float length = (float)sqrt(pow(p.x - point.x, 2) + pow(p.y - point.y, 2));
			if (length < 0.01f) {
				return &p;
			}
		}
		return NULL;
	}

	void draw() {
		int location = glGetUniformLocation(gpuProgram.getId(), "color");
		glUniform3f(location, 1.0f, 0.0f, 0.0f);

		glBindVertexArray(points.vao);
		glDrawArrays(GL_POINTS, 0, points.vertices.size());
	}
};

class Line {
public:
	vec3 start;
	vec3 end;

	Line(const vec3& start, const vec3& end) {
		if (start.x <= end.x){
			this->start = start;
			this->end = end;
		}
		else {
			this->start = end;
			this->end = start;
		}

		if ((start.x != end.x && start.x != -1.1f && end.x != 1.1f) || (start.x == end.x && start.y != 1.1f && end.y != -1.1f && start.y != end.y)) {
			vec3 v(end.x - start.x, end.y - start.y, 0.0f);
			float A = v.y;
			float B = -v.x;
			float C = start.y * end.x - start.x * end.y;
			printf("Line added: \n");
			printf("Implicit: %.1f x + %.1f y + %.1f = 0\n", A, B, C);  //negyjegyu fuggvenytablazat 70. oldalrol neztem ide a kepletet
			printf("Parametric: r(t) = (%.1f, %.1f) + (%.1f, %.1f)\n", start.x, start.y, v.x, v.y);  //Bevezetes a szamitaselmeletbe sajat jegyzetbol
		}	
	}

	vec3 crossPoint(const Line& other) const {
		if (start.x == end.x) { 
			float m2 = (other.end.y - other.start.y) / (other.end.x - other.start.x);
			float c2 = other.start.y - m2 * other.start.x;
			float x = start.x;
			float y = m2 * x + c2;
			return vec3(x, y, 1.0f);
		}
		else if (other.start.x == other.end.x) { 
			float m1 = (end.y - start.y) / (end.x - start.x);
			float c1 = start.y - m1 * start.x;
			float x = other.start.x;
			float y = m1 * x + c1;
			return vec3(x, y, 1.0f);
		}
		else {
			float m1 = (end.y - start.y) / (end.x - start.x);
			float c1 = start.y - m1 * start.x;
			float m2 = (other.end.y - other.start.y) / (other.end.x - other.start.x);
			float c2 = other.start.y - m2 * other.start.x;
			if (m1 == m2) {
				return vec3();
			}
			float x = (c2 - c1) / (m1 - m2);
			float y = m1 * x + c1;
			return vec3(x, y, 1.0f);
		}
	}


	bool isPointNear(const vec3& point) const {
		// Ezt a kepletet a negyjegyu fuggvenytablazat 71. oldalarol irtam.
		float distance = (float)((end.y - start.y) * point.x - (end.x - start.x) * point.y + end.x * start.y - end.y * start.x) / (float)sqrt(pow(end.y - start.y, 2) + pow(end.x - start.x, 2));
		if (distance < 0) {
			distance = -distance;
		}
		return distance < 0.01f;
	}


	Line segmentInsideSquare()const {
		if (start.x == end.x && start.y != end.y) {
			return Line(vec3(start.x, 1.1f, 1.0f), vec3(start.x, -1.1f, 1.0f));
		}
		float m = (end.y - start.y) / (end.x - start.x);
		vec3 newStart;
		vec3 newEnd;
		newStart = vec3(-1.1f, start.y - m * (1.1f + start.x), 1.0f);
		newEnd = vec3(1.1f, end.y + m * (1.1f - end.x), 1.0f);
		return Line(newStart, newEnd);
	}

	Line moveLine(const vec3& newPoint) const {
		if(start.x == end.x && start.y != end.y)
			return Line(vec3(newPoint.x, 1.1f, 1.0f), vec3(newPoint.x, -1.1f, 1.0f));

		float m = (end.y - start.y) / (end.x - start.x);

		vec3 newStart(-1.1f, newPoint.y - m * (newPoint.x + 1.1f), 1.0f);	
		vec3 newEnd(1.1f, newPoint.y + m * (1.1f - newPoint.x), 1.0f);		

		return Line(newStart, newEnd);
	}

};

class LineCollection{
public:
	Object lines; 
	int selectedLineIndex = -1;
	bool RightButtonDown = false;

	void addLine(const Line& line) {
		lines.vertices.push_back(line.start);
		lines.vertices.push_back(line.end);
		lines.updateGPU();
	}

	Line nearbyLine(const vec3& point) {
		for (size_t i = 0; i < lines.vertices.size(); i += 2) {
			vec3 start = lines.vertices[i];
			vec3 end = lines.vertices[i + 1];
			Line line(start, end);

			if (line.isPointNear(point)) {
				return line;
			}
		}
		return Line(vec3(), vec3());
	}


	void draw(vec3 color) {
		lines.Draw(GL_LINES, color);
	}
};

enum ProgramAllapot {
	PONT_RAJZOLAS,
	EGYENES_RAJZOLAS,
	EGYENESEK_METSZETE,
	EGYENESEK_MOZGATASA
};

PointCollection points;
LineCollection lineCollection;
ProgramAllapot programAllapot = EGYENES_RAJZOLAS;
std::vector<vec3*> selectedPoints;
std::vector<Line> selectedLines;



void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);

	glPointSize(10);
	glLineWidth(3);

	points.points.create();


	lineCollection.lines.create();


	points.points.updateGPU();


	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}


void onDisplay() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);     
	glClear(GL_COLOR_BUFFER_BIT); 

	float MVPtransf[4][4] = {1, 0, 0, 0,
							  0, 1, 0, 0,   
							  0, 0, 1, 0,
							  0, 0, 0, 1 };

	int location = glGetUniformLocation(gpuProgram.getId(), "MVP");
	glUniformMatrix4fv(location, 1, GL_TRUE, &MVPtransf[0][0]);	

	points.draw(); 
	lineCollection.draw(vec3(0, 1, 1));

	glutSwapBuffers(); 
}


void onKeyboard(unsigned char key, int pX, int pY) {
	switch (key) {
	case 'p':
		programAllapot = PONT_RAJZOLAS;
		break;
	case 'l':
		programAllapot = EGYENES_RAJZOLAS;
		break;
	case 'i':
		programAllapot = EGYENESEK_METSZETE;
		break;
	case 'm':
		programAllapot = EGYENESEK_MOZGATASA;
		break;
	}
}

void onKeyboardUp(unsigned char key, int pX, int pY) {
}


void onMouseMotion(int pX, int pY) {
	float cX = 2.0f * pX / windowWidth - 1;	
	float cY = 1.0f - 2.0f * pY / windowHeight;

	if (lineCollection.RightButtonDown && lineCollection.selectedLineIndex != -1) {
		Line line(lineCollection.lines.vertices[lineCollection.selectedLineIndex], lineCollection.lines.vertices[lineCollection.selectedLineIndex + 1]);
		Line movedLine = line.moveLine(vec3(cX, cY, 1.0f));
		lineCollection.lines.vertices[lineCollection.selectedLineIndex] = movedLine.start;
		lineCollection.lines.vertices[lineCollection.selectedLineIndex + 1] = movedLine.end;
		lineCollection.lines.updateGPU();
		glutPostRedisplay();
	}

}


void onMouse(int button, int state, int pX, int pY) { 
	float cX = 2.0f * pX / windowWidth - 1;	
	float cY = 1.0f - 2.0f * pY / windowHeight;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && programAllapot == PONT_RAJZOLAS) {
			points.addPoint(vec3(cX, cY, 1.0f));
			points.points.updateGPU();
			glutPostRedisplay(); 
		}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && programAllapot == EGYENES_RAJZOLAS) {
			vec3* point = points.findNearbyPoint(vec3(cX, cY, 0.0f));
			if (point != NULL) { 
				selectedPoints.push_back(point);
				if (selectedPoints.size() == 2) { 
					Line line(*selectedPoints[0], *selectedPoints[1]);
					Line uj = line.segmentInsideSquare(); 
					lineCollection.addLine(uj);
					glutPostRedisplay();
					selectedPoints.clear();
				}
			}
		}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && programAllapot == EGYENESEK_METSZETE) {
		Line line = lineCollection.nearbyLine(vec3(cX, cY, 1.0f));
		if (line.start.z != 0.0f && line.end.z != 0.0f) { 
			selectedLines.push_back(line);
			if (selectedLines.size() == 2) { 
				vec3 metszet = selectedLines[0].crossPoint(selectedLines[1]);
				if (metszet.z != 0.0f) {
					points.addPoint(metszet);
					points.points.updateGPU();
					glutPostRedisplay();
					selectedLines.clear();
				}
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && programAllapot == EGYENESEK_MOZGATASA) {
		Line line = lineCollection.nearbyLine(vec3(cX, cY, 1.0f));
		if (line.start.z != 0.0f && line.end.z != 0.0f) {
			for (size_t i = 0; i < lineCollection.lines.vertices.size(); i += 2) {
				if (lineCollection.lines.vertices[i].x == line.start.x && lineCollection.lines.vertices[i].y == line.start.y && lineCollection.lines.vertices[i + 1].x == line.end.x && lineCollection.lines.vertices[i + 1].y == line.end.y) {
					lineCollection.selectedLineIndex = i;
					break;
				}
			}
			lineCollection.RightButtonDown = true;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		lineCollection.RightButtonDown = false;
		lineCollection.selectedLineIndex = -1;
	}
	

	char * buttonStat;
	switch (state) {
	case GLUT_DOWN: buttonStat = "pressed"; break;
	case GLUT_UP:   buttonStat = "released"; break;
	}

	switch (button) {
	case GLUT_LEFT_BUTTON:      break;
	case GLUT_MIDDLE_BUTTON:  break;
	case GLUT_RIGHT_BUTTON:    break;
	}
}


void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME);
}
