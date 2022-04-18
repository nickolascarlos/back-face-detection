// Back Face Detection
// Detecção de Superfícies Escondidas

#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void init();
void display();
void RotacaoPontoEixoX(int px, int py, int pz, double theta, int* buffer);
void RotacionaCuboEixoX(double theta);
void RotacaoPontoEixoY(int px, int py, int pz, double theta, int* buffer);
void RotacionaCuboEixoY(double theta);
void desenhaFace1(); void desenhaFace2(); void desenhaFace3();
void desenhaFace4(); void desenhaFace5(); void desenhaFace6();
void desenhaFaces();
void calculaVetorFace(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, int* buffer);
void calculaVetorNormalFace1(int* vetor_normal);
void calculaVetorNormalFace2(int* vetor_normal);
void calculaVetorNormalFace3(int* vetor_normal);
void calculaVetorNormalFace4(int* vetor_normal);
void calculaVetorNormalFace5(int* vetor_normal);
void calculaVetorNormalFace6(int* vetor_normal);
int faceEstaVisivel(int vetor_x, int vetor_y, int vetor_z);
int face1Visivel(); int face3Visivel(); int face5Visivel();
int face2Visivel(); int face4Visivel(); int face6Visivel();

// Vértices do objeto tridimensional a ser renderizado
float vertices[8][3] = {
                        {-50, 50, 50},
                        {50, 50, 50},
                        {-75, -75, 50},
                        {75, -75, 50},

                        {-50, 50, -50},
                        {50, 50, -50},
                        {-75, -75, -50},
                        {75, -75, -50},
                    };

int vertices_rotacionados[8][3];

int eixoRotacao = 0;

int main(int argc, char** argv) {
    glutInit(&argc, argv);                              
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (512, 512);           
    glutInitWindowPosition (100, 100);                  
    glutCreateWindow ("Deteccao de Superficies Escondidas");  
    init();
    glutDisplayFunc(display);
    glutMainLoop();   
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  gluOrtho2D(-512/2.0, 512/2.0, -512/2.0, 512/2.0);

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 3; j++)
        vertices[i][j] *= 2;
}

void display() {
    glColor3ub(255, 255, 255);
    
    double theta = 0;
    while (1) {

       // Rotaciona o cubo, vértices resultantes da rotação
       // são armazenados no vetor vertices_rotacionados
        if (eixoRotacao == 0)
            RotacionaCuboEixoX(theta);
        else
            RotacionaCuboEixoY(theta);

       desenhaFaces();
                
       glFlush();
       usleep(100000);
       glClear(GL_COLOR_BUFFER_BIT);
       
       theta += 0.1;
       if (theta > 5) {
           theta = 0;
           eixoRotacao = !eixoRotacao; // Troca o eixo de rotação
       }
    }   
}

void RotacaoPontoEixoY(int px, int py, int pz, double theta, int* buffer) {
    double nx = sin(theta) * pz + cos(theta) * px;
    double nz = cos(theta) * pz - sin(theta) * px;
    
    buffer[0] = (int) nx;
    buffer[1] = py;
    buffer[2] = (int) nz;
}

void RotacionaCuboEixoY(double theta) {
    for (int i = 0; i < 8; i++) {
        int buffer_ponto[3];
        RotacaoPontoEixoY(vertices[i][0], vertices[i][1], vertices[i][2], theta, buffer_ponto);
        vertices_rotacionados[i][0] = buffer_ponto[0];
        vertices_rotacionados[i][1] = buffer_ponto[1];
        vertices_rotacionados[i][2] = buffer_ponto[2];
    }
}

void RotacaoPontoEixoX(int px, int py, int pz, double theta, int* buffer) {
    double ny = cos(theta) * py - sin(theta) * pz;
    double nz = sin(theta) * py + cos(theta) * pz;
    
    buffer[0] = px;
    buffer[1] = (int) ny;
    buffer[2] = (int) nz;
}

void RotacionaCuboEixoX(double theta) {
    for (int i = 0; i < 8; i++) {
        int buffer_ponto[3];
        RotacaoPontoEixoX(vertices[i][0], vertices[i][1], vertices[i][2], theta, buffer_ponto);
        vertices_rotacionados[i][0] = buffer_ponto[0];
        vertices_rotacionados[i][1] = buffer_ponto[1];
        vertices_rotacionados[i][2] = buffer_ponto[2];
    }
}

void desenhaFaces() {

    int facesEscondidas[6];
    for (int i = 0; i < 6; i++)
        facesEscondidas[i] = 0;

    glColor3ub(0, 255, 0);
    if (face1Visivel())
        desenhaFace1();
    else
        facesEscondidas[0] = 1;


    glColor3ub(255, 0, 0);
    if (face2Visivel())
        desenhaFace2();
    else
        facesEscondidas[1] = 1;

    glColor3ub(255, 0, 255);
    if (face3Visivel())
        desenhaFace3();
    else
        facesEscondidas[2] = 1;

    glColor3ub(255, 255, 255);
    if (face4Visivel())
        desenhaFace4();
    else
        facesEscondidas[3] = 1;

    glColor3ub(0, 255, 255);
    if (face5Visivel())
        desenhaFace5();
    else
        facesEscondidas[4] = 1;

    glColor3ub(255, 255, 0);
    if (face6Visivel())
        desenhaFace6();
    else
        facesEscondidas[5] = 1;

    printf("AS FACES { ");
    for (int i = 0; i < 6; i++)
        if (facesEscondidas[i])
            printf("%d, ", i + 1);
    printf(" } NÃO ESTÃO VISÍVEIS            \r");

}

void desenhaFace1() {
    glBegin(GL_POLYGON);
        glVertex2i(vertices_rotacionados[0][0], vertices_rotacionados[0][1]); // A
        glVertex2i(vertices_rotacionados[2][0], vertices_rotacionados[2][1]); // C
        glVertex2i(vertices_rotacionados[3][0], vertices_rotacionados[3][1]); // D
        glVertex2i(vertices_rotacionados[1][0], vertices_rotacionados[1][1]); // B
    glEnd();
}

void desenhaFace2() {
    glBegin(GL_POLYGON);
        glVertex2i(vertices_rotacionados[2][0], vertices_rotacionados[2][1]); // C
        glVertex2i(vertices_rotacionados[6][0], vertices_rotacionados[6][1]); // G
        glVertex2i(vertices_rotacionados[7][0], vertices_rotacionados[7][1]); // H
        glVertex2i(vertices_rotacionados[3][0], vertices_rotacionados[3][1]); // D
    glEnd();
}

void desenhaFace3() {
    glBegin(GL_POLYGON);
        glVertex2i(vertices_rotacionados[6][0], vertices_rotacionados[6][1]); // G
        glVertex2i(vertices_rotacionados[4][0], vertices_rotacionados[4][1]); // E
        glVertex2i(vertices_rotacionados[5][0], vertices_rotacionados[5][1]); // F
        glVertex2i(vertices_rotacionados[7][0], vertices_rotacionados[7][1]); // H
    glEnd();
}

void desenhaFace4() {
    glBegin(GL_POLYGON);
        glVertex2i(vertices_rotacionados[1][0], vertices_rotacionados[1][1]); // B
        glVertex2i(vertices_rotacionados[5][0], vertices_rotacionados[5][1]); // F
        glVertex2i(vertices_rotacionados[4][0], vertices_rotacionados[4][1]); // E
        glVertex2i(vertices_rotacionados[0][0], vertices_rotacionados[0][1]); // A
    glEnd();
}

void desenhaFace5() {
    glBegin(GL_POLYGON);
        glVertex2i(vertices_rotacionados[0][0], vertices_rotacionados[0][1]); // B
        glVertex2i(vertices_rotacionados[4][0], vertices_rotacionados[4][1]); // F
        glVertex2i(vertices_rotacionados[6][0], vertices_rotacionados[6][1]); // E
        glVertex2i(vertices_rotacionados[2][0], vertices_rotacionados[2][1]); // A
    glEnd();
}

void desenhaFace6() {
    glBegin(GL_POLYGON);
        glVertex2i(vertices_rotacionados[3][0], vertices_rotacionados[3][1]); // B
        glVertex2i(vertices_rotacionados[7][0], vertices_rotacionados[7][1]); // F
        glVertex2i(vertices_rotacionados[5][0], vertices_rotacionados[5][1]); // E
        glVertex2i(vertices_rotacionados[1][0], vertices_rotacionados[1][1]); // A
    glEnd();
}

void calculaVetorNormalFace1(int* vetor_normal) { // v
    calculaVetorFace(
        vertices_rotacionados[0][0], vertices_rotacionados[0][1], vertices_rotacionados[0][2],
        vertices_rotacionados[2][0], vertices_rotacionados[2][1], vertices_rotacionados[2][2],
        vertices_rotacionados[3][0], vertices_rotacionados[3][1], vertices_rotacionados[3][2],
        vetor_normal
    );
}

void calculaVetorNormalFace2(int* vetor_normal) { // v
    calculaVetorFace(
        vertices_rotacionados[2][0], vertices_rotacionados[2][1], vertices_rotacionados[2][2],
        vertices_rotacionados[6][0], vertices_rotacionados[6][1], vertices_rotacionados[6][2],
        vertices_rotacionados[7][0], vertices_rotacionados[7][1], vertices_rotacionados[7][2],
        vetor_normal
    );
}

void calculaVetorNormalFace3(int* vetor_normal) { // v
    calculaVetorFace(
        vertices_rotacionados[6][0], vertices_rotacionados[6][1], vertices_rotacionados[6][2],
        vertices_rotacionados[4][0], vertices_rotacionados[4][1], vertices_rotacionados[4][2],
        vertices_rotacionados[5][0], vertices_rotacionados[5][1], vertices_rotacionados[5][2],
        vetor_normal
    );
}

void calculaVetorNormalFace4(int* vetor_normal) { // v
    calculaVetorFace(
        vertices_rotacionados[1][0], vertices_rotacionados[1][1], vertices_rotacionados[1][2],
        vertices_rotacionados[5][0], vertices_rotacionados[5][1], vertices_rotacionados[5][2],
        vertices_rotacionados[4][0], vertices_rotacionados[4][1], vertices_rotacionados[4][2],
        vetor_normal
    );
}

void calculaVetorNormalFace5(int* vetor_normal) { // v
    calculaVetorFace(
        vertices_rotacionados[0][0], vertices_rotacionados[0][1], vertices_rotacionados[0][2],
        vertices_rotacionados[4][0], vertices_rotacionados[4][1], vertices_rotacionados[4][2],
        vertices_rotacionados[6][0], vertices_rotacionados[6][1], vertices_rotacionados[6][2],
        vetor_normal
    );
}

void calculaVetorNormalFace6(int* vetor_normal) { // v
    calculaVetorFace(
        vertices_rotacionados[3][0], vertices_rotacionados[3][1], vertices_rotacionados[3][2],
        vertices_rotacionados[7][0], vertices_rotacionados[7][1], vertices_rotacionados[7][2],
        vertices_rotacionados[5][0], vertices_rotacionados[5][1], vertices_rotacionados[5][2],
        vetor_normal
    );
}

void calculaVetorFace(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, int* buffer) {
    int ab_x = bx - ax;
    int ab_y = by - ay;
    int ab_z = bz - az;

    int bc_x = cx - bx;
    int bc_y = cy - by;
    int bc_z = cz - bz;

    // Calcula o produto vetorial dos vetores ab e bc
    buffer[0] = ab_y * bc_z - ab_z * bc_y;
    buffer[1] = ab_z * bc_x - ab_x * bc_z;
    buffer[2] = ab_x * bc_y - ab_y * bc_x;
}

int faceEstaVisivel(int vetor_x, int vetor_y, int vetor_z) {
    // Vetor do observador
    int observador_x = 0;
    int observador_y = 0;
    int observador_z = -1;

    // Calcula o produto escalar
    float PE = observador_x * vetor_x + observador_y * vetor_y + observador_z * vetor_z;

    return PE < 0;
}

int face1Visivel() {
    int vetor_normal[3];
    calculaVetorNormalFace1(vetor_normal);
    return faceEstaVisivel(vetor_normal[0], vetor_normal[1], vetor_normal[2]);
}

int face2Visivel() {
    int vetor_normal[3];
    calculaVetorNormalFace2(vetor_normal);
    return faceEstaVisivel(vetor_normal[0], vetor_normal[1], vetor_normal[2]);
}

int face3Visivel() {
    int vetor_normal[3];
    calculaVetorNormalFace3(vetor_normal);
    return faceEstaVisivel(vetor_normal[0], vetor_normal[1], vetor_normal[2]);
}

int face4Visivel() {
    int vetor_normal[3];
    calculaVetorNormalFace4(vetor_normal);
    return faceEstaVisivel(vetor_normal[0], vetor_normal[1], vetor_normal[2]);
}

int face5Visivel() {
    int vetor_normal[3];
    calculaVetorNormalFace5(vetor_normal);
    return faceEstaVisivel(vetor_normal[0], vetor_normal[1], vetor_normal[2]);
}

int face6Visivel() {
    int vetor_normal[3];
    calculaVetorNormalFace6(vetor_normal);
    return faceEstaVisivel(vetor_normal[0], vetor_normal[1], vetor_normal[2]);
}