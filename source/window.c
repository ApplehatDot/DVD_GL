#include <GL/freeglut.h>
#include "stb_image.h" // Do wczytywania tekstur PNG
#include <stdbool.h>

// Pozycja prostokąta
float rectX = 0.0f;
float rectY = 0.0f;
float rectWidth = 0.2f;
float rectHeight = 0.12f;
float speedX = 0.01f;
float speedY = 0.01f;

// Tekstura
GLuint texture;

// Funkcja rysująca prostokąt
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Ustawienie tekstury
    glBindTexture(GL_TEXTURE_2D, texture);

    // Rysowanie prostokąta
    glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(rectX - rectWidth / 2.0f, rectY - rectHeight / 2.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(rectX + rectWidth / 2.0f, rectY - rectHeight / 2.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(rectX + rectWidth / 2.0f, rectY + rectHeight / 2.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(rectX - rectWidth / 2.0f, rectY + rectHeight / 2.0f);
	glEnd();


    glutSwapBuffers();
}

// Funkcja wywoływana przy zmianie rozmiaru okna
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

// Funkcja aktualizująca pozycję prostokąta
void update(int value) {
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // Sprawdź, czy prostokąt odbija się od krawędzi
    if (rectX + rectWidth / 2.0f >= 1.0f || rectX - rectWidth / 2.0f <= -1.0f) {
        speedX = -speedX;
    }
    if (rectY + rectHeight / 2.0f >= 1.0f || rectY - rectHeight / 2.0f <= -1.0f) {
        speedY = -speedY;
    }

    // Aktualizowanie pozycji prostokąta
    rectX += speedX;
    rectY += speedY;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

// Funkcja inicjalizująca
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Kolor tła
    glEnable(GL_TEXTURE_2D); // Włącz teksturowanie
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Wczytaj teksturę
    int width, height, channels;
    unsigned char* image = stbi_load("logo.png", &width, &height, &channels, 0);
    if (image == NULL) {
        fprintf(stderr, "Nie można wczytać tekstury\n");
        exit(EXIT_FAILURE);
    }

    // Ustawienia tekstury
    glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4 ? GL_RGBA : GL_RGB), width, height, 0, (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("DVD screensaver thing.");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0); // Rozpoczęcie animacji

    glutMainLoop();
    return 0;
}
