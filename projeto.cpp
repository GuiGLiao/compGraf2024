#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Variáveis de posição e escala do boneco
float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
float scale = 0.5f;
float runningSpeed = 1.0f;

// Variáveis de rotação para as articulações
float headAngle = 0.0f;
float leftArmAngleX = 0.0f;
float rightArmAngleX = 0.0f;
float leftArmAngleY = 0.0f;
float rightArmAngleY = 0.0f;
float leftElbowAngle = 0.0f;
float rightElbowAngle = 0.0f;
float leftLegAngle = 0.0f;
float rightLegAngle = 0.0f;
float leftKneeAngle = 0.0f;
float rightKneeAngle = 0.0f;
float anguloQuadril = 0.0f;
float torsoLeanAngle = 0.0f; 
float leftArmAngleZ = 0.0f;
float rightArmAngleZ = 0.0f;

// Variáveis globais para controle do sentido do movimento
bool legsMovingForward = true; // Controla o movimento alternado das pernas
bool armsMovingForward = true; // Controla o movimento alternado dos braços
bool body = true; // Controla o movimento do torso
bool leftArmAngle = true;
bool rightArmAngle = true;
bool isWalking = false;
bool isRunning = false;
bool isCameraAnimating = true;
bool leftArmZ = true;
bool rightArmZ = true;
float walkSpeed = 0.5f;

// Variáveis de controle da câmera
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 5.0f;

// Variáveis de controle do mouse
int mouseButton = -1;
int mouseX = 0, mouseY = 0;

// Variáveis de iluminação
//GLfloat lightIntensity = 1.0f;
//GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float ambientIntensity = 0.2f;
float diffuseIntensity = 1.0f;
float specularIntensity = 1.0f;
int sombraTipo = 0;
float globalLightIntensity = 1.0f;

// Variáveis de texturização
GLuint texture[4];

// Função para carregar a textura usando stb_image
void carregarTextura(const char *filename, int indice)
{
    int largura, altura, canais;

    unsigned char *dados = stbi_load(filename, &largura, &altura, &canais, 0);

    if (!dados)
        exit(1);

    // gerar textura
    glGenTextures(1, &texture[indice]);
    glBindTexture(GL_TEXTURE_2D, texture[indice]);

    // configurar parametros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, (canais == 4) ? GL_RGBA : GL_RGB, largura, altura, 0,
    (canais == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dados);


    // liberar a memoria da imagem
    stbi_image_free(dados);              
}

// Função de inicialização
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    carregarTextura("texturas/skin.jpg", 0);
    carregarTextura("texturas/camiseta.jpg", 1);
    carregarTextura("texturas/jeans.jpg", 2);
    carregarTextura("texturas/amelia-watson.jpg", 3);
    // Configurações de luz e material
    GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void configuraIluminacao()
{
    GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
    
    // Luz ambiente ajustada pelo fator global
    GLfloat light_ambient[] = { 
        0.2f * globalLightIntensity, 
        0.2f * globalLightIntensity, 
        0.2f * globalLightIntensity, 
        1.0 
    };
    
    // Luz difusa ajustada pelo fator global
    GLfloat light_diffuse[] = { 
        1.0f * globalLightIntensity, 
        1.0f * globalLightIntensity, 
        1.0f * globalLightIntensity, 
        1.0 
    };
    
    // Luz especular ajustada pelo fator global
    GLfloat light_specular[] = { 
        1.0f * globalLightIntensity, 
        1.0f * globalLightIntensity, 
        1.0f * globalLightIntensity, 
        1.0 
    };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void desenhaEsfera(float tamanho, int texturaIndice){
    glBindTexture(GL_TEXTURE_2D, texture[texturaIndice]);
    glPushMatrix();
    glScalef(tamanho, tamanho, tamanho);

    glBegin(GL_QUADS);
    for (float theta = 0; theta < 2 * M_PI; theta += M_PI / 20) {
        for (float phi = 0; phi < M_PI; phi += M_PI / 20) {
            float x1 = cos(theta) * sin(phi);
            float y1 = cos(phi);
            float z1 = sin(theta) * sin(phi);

            float x2 = cos(theta + M_PI / 20) * sin(phi);
            float y2 = cos(phi);
            float z2 = sin(theta + M_PI / 20) * sin(phi);

            float x3 = cos(theta + M_PI / 20) * sin(phi + M_PI / 20);
            float y3 = cos(phi + M_PI / 20);
            float z3 = sin(theta + M_PI / 20) * sin(phi + M_PI / 20);

            float x4 = cos(theta) * sin(phi + M_PI / 20);
            float y4 = cos(phi + M_PI / 20);
            float z4 = sin(theta) * sin(phi + M_PI / 20);

            glTexCoord2f(theta / (2 * M_PI), phi / M_PI);
            glVertex3f(x1, y1, z1);

            glTexCoord2f((theta + M_PI / 20) / (2 * M_PI), phi / M_PI);
            glVertex3f(x2, y2, z2);

            glTexCoord2f((theta + M_PI / 20) / (2 * M_PI), (phi + M_PI / 20) / M_PI);
            glVertex3f(x3, y3, z3);

            glTexCoord2f(theta / (2 * M_PI), (phi + M_PI / 20) / M_PI);
            glVertex3f(x4, y4, z4);
    }
}
    glEnd();
    glPopMatrix();

}

void desenhaCubo(float tamanho, int texturaIndice) {
    glBindTexture(GL_TEXTURE_2D, texture[texturaIndice]);
    glPushMatrix();
    glScalef(tamanho, tamanho, tamanho);
    
    glBegin(GL_QUADS);
    // Face frontal
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

    // Face traseira
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

    // Face superior
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

    // Face inferior
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

    // Face direita
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

    // Face esquerda
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

    glPopMatrix();
}

void desenhaCilindro(float tamanho, int texturaIndice) {
    glBindTexture(GL_TEXTURE_2D, texture[texturaIndice]);
    glPushMatrix();
    glScalef(tamanho, tamanho, tamanho);

    glBegin(GL_QUADS);
    for (int i = 0; i < 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        float nextTheta = (i + 10) * M_PI / 180.0f;

        glTexCoord2f(i / 360.0f, 0.0f);
        glVertex3f(cos(theta), -1.0f, sin(theta));

        glTexCoord2f((i + 10) / 360.0f, 0.0f);
        glVertex3f(cos(nextTheta), -1.0f, sin(nextTheta));

        glTexCoord2f((i + 10) / 360.0f, 1.0f);
        glVertex3f(cos(nextTheta), 1.0f, sin(nextTheta));

        glTexCoord2f(i / 360.0f, 1.0f);
        glVertex3f(cos(theta), 1.0f, sin(theta));
    }
    glEnd();

    glPopMatrix();
}

void configuraSombreamento(int sombraTipo)
{
 if (sombraTipo == 0)
  glShadeModel(GL_FLAT);  // sombreamento plano (flat)

 if (sombraTipo == 1)
   glShadeModel(GL_SMOOTH);  // sombreamento suave (Gouraud)

 glutPostRedisplay();  
}

// Função para desenhar o boneco
void drawBoneco() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(scale, scale, scale);

    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f); // Posicionar o corpo principal para o ponto do quadril
    glRotatef(anguloQuadril, 0.0f, -1.0f, 0.0f); // Rotação no eixo do quadril
    glRotatef(torsoLeanAngle, 1.0f, 0.0f, 0.0f); // Inclinação do torso

    // Torso
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f); // Subir o torso para cima do quadril
    glScalef(1.0f, 2.0f, 0.5f); // Escala do torso
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.5, 1);
    glPopMatrix();

    // Pescoço
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f); // Subir o pescoço para cima do torso
    configuraSombreamento(sombraTipo);
    desenhaCilindro(0.2, 0);

    // Cabeça
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f); // Conectar a cabeça ao topo do pescoço
    glRotatef(headAngle, 0.0f, 1.0f, 0.0f); // Rotação da cabeça
    configuraSombreamento(sombraTipo);
    desenhaEsfera(0.4, 0);
    glPopMatrix(); // Fim da cabeça

    glPopMatrix(); // Fim do pescoço

    // Ombro esquerdo - conectado ao torso
    glPushMatrix();
    glTranslatef(-0.6f, 1.8f, 0.0f);  // Ajuste da posição para alinhar ao lado superior esquerdo do torso
    desenhaEsfera(0.2, 1);  // Esfera representando o ombro esquerdo

    // Braço esquerdo
    glPushMatrix();
    glRotatef(leftArmAngleY, 0.0f, 1.0f, 0.0f); // Rotação no eixo Y
    glRotatef(leftArmAngleX, 1.0f, 0.0f, 0.0f); // Rotação no eixo X
    glRotatef(leftArmAngleZ, 0.0f, 0.0f, 1.0f); // Rotação no eixo Z

    // Braço superior
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f); // Posição do braço superior
    glScalef(0.3f, 1.0f, 0.3f); // Escala do braço superior
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.45, 1);
    glPopMatrix();

    // Cotovelo esquerdo
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f); // Posição do cotovelo
    desenhaEsfera(0.15, 2);
    glPopMatrix();

    // Antebraço esquerdo
    glTranslatef(0.0f, -1.0f, 0.0f); // Deslocamento para o antebraço
    glRotatef(leftElbowAngle, 1.0f, 0.0f, 0.0f); // Rotação no cotovelo
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f); // Posição do antebraço
    glScalef(0.3f, 1.0f, 0.3f); // Escala do antebraço
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.35, 0);
    glPopMatrix();

    glPopMatrix(); // Fim do braço esquerdo
    glPopMatrix(); // Fim do ombro esquerdo

    // Ombro direito - conectado ao torso
    glPushMatrix();
    glTranslatef(0.6f,1.8f, 0.0f);  // Ajuste da posição para alinhar ao lado superior direito do torso
    desenhaEsfera(0.2, 1);  // Esfera representando o ombro direito

    // Braço direito
    glPushMatrix();
    glRotatef(rightArmAngleY, 0.0f, 1.0f, 0.0f); // Rotação no eixo Y
    glRotatef(rightArmAngleX, 1.0f, 0.0f, 0.0f); // Rotação no eixo X
    glRotatef(rightArmAngleZ, 0.0f, 0.0f, 1.0f); // Rotação no eixo Z

    // Braço superior direito
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f); // Posição do braço superior
    glScalef(0.3f, 1.0f, 0.3f); // Escala do braço superior
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.45, 1);
    glPopMatrix();

    // Cotovelo direito
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f); // Posição do cotovelo
    desenhaEsfera(0.15, 2);
    glPopMatrix();

    // Antebraço direito
    glTranslatef(0.0f, -1.0f, 0.0f); // Deslocamento para o antebraço
    glRotatef(rightElbowAngle, 1.0f, 0.0f, 0.0f); // Rotação no cotovelo
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f); // Posição do antebraço
    glScalef(0.3f, 1.0f, 0.3f); // Escala do antebraço
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.35, 0);
    glPopMatrix();

    glPopMatrix(); // Fim do braço direito
    glPopMatrix(); // Fim do ombro direito

    glPopMatrix(); // Fim do corpo principal


    // Articulação quadril
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    desenhaEsfera(0.2, 1);
    glPopMatrix();



    // Perna esquerda
    glPushMatrix();
    glTranslatef(-0.25f, -1.0f, 0.0f); // Mover para a junção com o corpo
    glRotatef(leftLegAngle, 1.0f, 0.0f, 0.0f); // Rotacionar em torno da junção
    glTranslatef(0.0f, -0.75f, 0.0f); // Mover a perna para baixo após a rotação
    glPushMatrix();
    glScalef(0.5f, 1.5f, 0.5f);
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.5, 2);
    glPopMatrix();

    // Bola do joelho esquerdo
    glPushMatrix();
    glTranslatef(0.0f, -0.75f, 0.0f); // Mover para a junção com a perna inferior
    desenhaEsfera(0.2, 0);
    glPopMatrix();

    // Joelho esquerdo
    glTranslatef(0.0f, -0.7f, 0.0f); // Mover para a junção com a perna inferior
    glRotatef(leftKneeAngle, 1.0f, 0.0f, 0.0f); // Rotacionar em torno da junção
    glTranslatef(0.0f, -0.75f, 0.0f); // Mover a perna inferior para baixo após a rotação
    glPushMatrix();
    glScalef(0.5f, 1.5f, 0.5f);
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.4, 0);
    glPopMatrix();
    glPopMatrix();


    // Perna direita
    glPushMatrix();
    glTranslatef(0.25f, -1.0f, 0.0f); // Mover para a junção com o corpo
    glRotatef(rightLegAngle, 1.0f, 0.0f, 0.0f); // Rotacionar em torno da junção
    glTranslatef(0.0f, -0.75f, 0.0f); // Mover a perna para baixo após a rotação
    glPushMatrix();
    glScalef(0.5f, 1.5f, 0.5f);
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.5, 2);
    glPopMatrix();

    // Bola do joelho direito
    glPushMatrix();
    glTranslatef(0.0f, -0.75f, 0.0f); // Mover para a junção com a perna inferior
    desenhaEsfera(0.2, 0);
    glPopMatrix();


    // Joelho direito
    glTranslatef(0.0f, -0.70f, 0.0f); // Mover para a junção com a perna inferior
    glRotatef(rightKneeAngle, 1.0f, 0.0f, 0.0f); // Rotacionar em torno da junção
    glTranslatef(0.0f, -0.75f, 0.0f); // Mover a perna inferior para baixo após a rotação
    glPushMatrix();
    glScalef(0.5f, 1.5f, 0.5f);
    configuraSombreamento(sombraTipo);
    desenhaCubo(0.4, 0);
    glPopMatrix();
    glPopMatrix();

    

    glPopMatrix();
}

// Função de display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Configurar a câmera
    gluLookAt(cameraDistance * sin(cameraAngleX) * cos(cameraAngleY),
              cameraDistance * sin(cameraAngleY),
              cameraDistance * cos(cameraAngleX) * cos(cameraAngleY),
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);



    drawBoneco();
    glutSwapBuffers();
}

// Função de reshape
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            isWalking = true;
            isRunning = false;
            break;
        
        case 'r':
            isRunning = true;
            isWalking = false;
            break;
        
        case 's':
            isWalking = false;
            isRunning = false;
            // Resetar ângulos quando não estiver andando/correndo
            leftLegAngle = 0.0f;
            rightLegAngle = 0.0f;
            leftArmAngleX = 0.0f;
            rightArmAngleX = 0.0f;
            leftKneeAngle = 0.0f;
            rightKneeAngle = 0.0f;
            leftElbowAngle = 0.0f;
            rightElbowAngle = 0.0f;
            torsoLeanAngle = 0.0f;
            break;

        case 'd': // levantar o braço direito horizontalmente
            if(rightArmAngleZ < 90.0f)
                rightArmAngleZ += 5.0f;
            break;
        case 'f': // abaixar o braço direito horizontalmente
            if(rightArmAngleZ > 0.0f)
                rightArmAngleZ -= 5.0f;
            break;
        case 'a': //
            if(leftArmAngleZ > -90.0f)
                leftArmAngleZ -= 5.0f;
            break;
        case 'g':
            if(leftArmAngleZ < 0.0f)
                leftArmAngleZ += 5.0f;
            break;
        case 'q':  // Inclinar para frente
           if (torsoLeanAngle < 60.0f)
               torsoLeanAngle += 5.0f;
           break;
        case 'e':  // Retornar à posição inicial
           if(torsoLeanAngle > 0.0f)
               torsoLeanAngle -= 5.0f;
           break;
        case '1': headAngle += 5.0f; break;
        case '2': headAngle -= 5.0f; break;
        case '3': 
            leftArmAngleX += 5.0f; 
            break;
        case '4': leftArmAngleX -= 5.0f; break;
        case '5': rightArmAngleX += 5.0f; break;
        case '6': rightArmAngleX -= 5.0f; break;
        case '7': leftElbowAngle += 5.0f; break;
        case '8': leftElbowAngle -= 5.0f; break;
        case '9': rightElbowAngle += 5.0f; break;
        case '0': rightElbowAngle -= 5.0f; break;
        case 'z': leftLegAngle += 5.0f; break;
        case 'x': leftLegAngle -= 5.0f; break;
        case 'c': rightLegAngle += 5.0f; break;
        case 'v': rightLegAngle -= 5.0f; break;
        case 'b': leftKneeAngle += 5.0f; break;
        case 'n': leftKneeAngle -= 5.0f; break;
        case 'm': rightKneeAngle += 5.0f; break;
        case ',': rightKneeAngle -= 5.0f; break;
         // Aumentar luminosidade global
        case '+':
            globalLightIntensity = std::min(2.0f, globalLightIntensity + 0.1f);
            break;
        
        // Diminuir luminosidade global
        case '-':
            globalLightIntensity = std::max(0.0f, globalLightIntensity - 0.1f);
            break;

        case 't':
            sombraTipo = 1;
            break; // Flat shading
        case 'y':
            sombraTipo = 0;
            break; // Smooth shading
        case '*':
            runningSpeed = std::min(2.0f, runningSpeed + 0.1f);
            break;

        case '/':
            runningSpeed = std::max(0.5f, runningSpeed - 0.1f);
            break;
        case 'p': // Pausa a animação de câmera
            isCameraAnimating = !isCameraAnimating;
            break;
        case 27: exit(0); break; // ESC key
    }
    configuraIluminacao();
    glutPostRedisplay();
}

// Função de controle da câmera
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngleX -= 0.1f;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleX += 0.1f;
            break;
        case GLUT_KEY_UP:
            cameraAngleY += 0.1f;
            if (cameraAngleY > 1.5f) cameraAngleY = 1.5f; // Limitar o ângulo para evitar virar a câmera de cabeça para baixo
            break;
        case GLUT_KEY_DOWN:
            cameraAngleY -= 0.1f;
            if (cameraAngleY < -1.5f) cameraAngleY = -1.5f; // Limitar o ângulo para evitar virar a câmera de cabeça para baixo
            break;
        case GLUT_KEY_PAGE_UP:
            cameraDistance -= 0.3f;
            if (cameraDistance < 1.0f) cameraDistance = 1.0f; // Limitar a proximidade
            break;
        case GLUT_KEY_PAGE_DOWN:
            cameraDistance += 0.3f;
            break;
        case GLUT_KEY_HOME:
            cameraAngleX = 0.0f;
            cameraAngleY = 0.0f;
            cameraDistance = 5.0f;
            break;
    }
    glutPostRedisplay();
}

// Função de controle do mouse
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouseButton = button;
        mouseX = x;
        mouseY = y;
    } else {
        mouseButton = -1;
    }
}

void motion(int x, int y) {
    int dx = x - mouseX;
    int dy = y - mouseY;

    if (mouseButton == GLUT_LEFT_BUTTON) {
        cameraAngleX += dx * 0.01f;
        cameraAngleY += dy * 0.01f;
        if (cameraAngleY > 1.5f) cameraAngleY = 1.5f; // Limitar o ângulo para evitar virar a câmera de cabeça para baixo
        if (cameraAngleY < -1.5f) cameraAngleY = -1.5f; // Limitar o ângulo para evitar virar a câmera de cabeça para baixo
    }

    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}

void atualiza(int value) {
    if (isCameraAnimating) {
        // Rotação da câmera
        cameraAngleX += 0.01f;
        if (cameraAngleX > 2 * M_PI) {
            cameraAngleX -= 2 * M_PI;
        }
    }

    // Animação de andar/correr
    if (isWalking) {
        // Movimento mais suave e lento
        leftLegAngle = 15.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        rightLegAngle = -15.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        
        // Movimento dos braços complementar
        leftArmAngleX = -10.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        rightArmAngleX = 10.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        
        // Movimentos sutis de joelhos e cotovelos
        leftKneeAngle = 10.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        rightKneeAngle = -10.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        leftElbowAngle = 5.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
        rightElbowAngle = -5.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.005);
    }
    else if (isRunning) {
        // Movimento de corrida (como no exemplo anterior)
        static bool legsMovingForward = true;
        static bool armsMovingForward = true;

        if (legsMovingForward) {
            leftLegAngle += 15.0f * runningSpeed;
            rightLegAngle -= 15.0f * runningSpeed;
            
            leftKneeAngle = 30.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01); 
            rightKneeAngle = -30.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            
            if (leftLegAngle >= 60.0f || rightLegAngle <= -60.0f)
                legsMovingForward = false;
        } else {
            leftLegAngle -= 15.0f * runningSpeed;
            rightLegAngle += 15.0f * runningSpeed;
            
            leftKneeAngle = -30.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            rightKneeAngle = 30.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            
            if (leftLegAngle <= -60.0f || rightLegAngle >= 60.0f)
                legsMovingForward = true;
        }

        // Movimento dos braços
        if (armsMovingForward) {
            leftArmAngleX -= 20.0f * runningSpeed;
            rightArmAngleX += 20.0f * runningSpeed;
            
            leftElbowAngle = 45.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            rightElbowAngle = -45.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            
            if (leftArmAngleX <= -60.0f || rightArmAngleX >= 60.0f)
                armsMovingForward = false;
        } else {
            leftArmAngleX += 20.0f * runningSpeed;
            rightArmAngleX -= 20.0f * runningSpeed;
            
            leftElbowAngle = -45.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            rightElbowAngle = 45.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.01);
            
            if (leftArmAngleX >= 60.0f || rightArmAngleX <= -60.0f)
                armsMovingForward = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(25, atualiza, 0);
}

void menu(int option) {
    switch (option) {
        case 1: // Iniciar animação
            isWalking = true;
            isRunning = false;
            isCameraAnimating = true;
            break;
        case 2: // Parar animação
            // Código para parar animação
            isWalking = false;
            isRunning = false;
            isCameraAnimating = false;
            break;
        case 3: // Reiniciar articulações
            headAngle = leftArmAngleX = rightArmAngleX = leftArmAngleY = rightArmAngleY = 0.0f;
            leftElbowAngle = rightElbowAngle = leftLegAngle = rightLegAngle = 0.0f;
            leftKneeAngle = rightKneeAngle = 0.0f;
            anguloQuadril = 0.0f;
            leftArmAngleZ = 0.0f;
            rightArmAngleZ = 0.0f;
            torsoLeanAngle = 0.0f;

            break;
        case 4: // Aumentar iluminação
            globalLightIntensity = std::min(2.0f, globalLightIntensity + 1.0f);
            break;
        case 5: // Diminuir iluminação
            globalLightIntensity = std::max(0.0f, globalLightIntensity - 1.0f);
            break;
        case 0: // Sair
            exit(0);
            break;
    }
    configuraIluminacao();
    glutPostRedisplay();
}


// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Boneco 3D Articulado");

    init();
    glEnable(GL_DEPTH_TEST); // Habilita teste de profundidade
    glutCreateMenu(menu);
    glutAddMenuEntry("Iniciar Animacao", 1);
    glutAddMenuEntry("Parar Animacao", 2);
    glutAddMenuEntry("Reiniciar Articulacoes", 3);
    glutAddMenuEntry("Aumentar Iluminacao", 4);
    glutAddMenuEntry("Diminuir Iluminacao", 5);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(25, atualiza, 0);

    glutMainLoop();
    return 0;
}