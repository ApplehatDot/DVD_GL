/* DVD_GL
 * Latest Updated on: 03.01.2025
 * By: ApplehatDoesStuff (Github)
 */

#include <GL/freeglut.h>
#include "stb_image.h" // Do wczytywania tekstur PNG
#include <windows.h>
#include <stdbool.h>

// Pozycja prostokąta
bool isFullscreen = false;
int wWidth, wHeight;
float rectX = 0.0f;
float rectY = 0.0f;
float rectWidth = 0.2f;
float rectHeight = 0.12f;
float speedX = 0.01f;
float speedY = 0.01f;
GLuint texture;

// odczytaj rozdzielczość używaną przez użytkownika
void getCurrentResolution(int *width, int *height){
    *width = glutGet(GLUT_SCREEN_WIDTH);
    *height = glutGet(GLUT_SCREEN_HEIGHT);
}

// Funkcja do przełączania się między trybem pełnoekranowym a okienkowym
void toggleFullscreen() {
    if (isFullscreen) {
        wWidth = 800;
        wHeight = 600;

        glutReshapeWindow(wWidth, wHeight);
        glutPositionWindow(100, 100);
    } else {
        int screenWidth, screenHeight;
        getCurrentResolution(&screenWidth, &screenHeight);
        wWidth = screenWidth;
        wHeight = screenHeight;
        glutFullScreen();
        glutReshapeWindow(screenWidth, screenHeight);
    }
    isFullscreen = !isFullscreen;
}

// Funkcja obsługi klawiszy
void handleKeypress(int key, int x, int y) {
    switch (key) {
        case VK_ESCAPE: // Klawisz Escape
            exit(0);
        case GLUT_KEY_F12: // Klawisz F12
            toggleFullscreen();
            break;
        case GLUT_KEY_F2:
            wWidth = 800;
            wHeight = 600;

            glutReshapeWindow(wWidth, wHeight);
            glutPositionWindow(100, 100);
            break;
        default:
            break;
    }
}

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
    wWidth = w;
    wHeight = h;
	
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
	MessageBoxW(NULL, L"Brak zdjęcia 'logo.png'.", L"Błąd", MB_OK);
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
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DVD screensaver thing.");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(handleKeypress); // Rejestracja funkcji obsługi klawiszy
    glutTimerFunc(25, update, 0); // Rozpoczęcie animacji

    glutMainLoop();
    return 0;
}
