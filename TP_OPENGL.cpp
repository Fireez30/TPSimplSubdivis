///////////////////////////////////////////////////////////////////////////////
// Imagina
// ----------------------------------------------------------------------------
// IN - Synthèse d'images - Modélisation géométrique
// Auteur : Gilles Gesquière
// ----------------------------------------------------------------------------
// Base du TP 1
// programme permettant de créer des formes de bases.
// La forme représentée ici est un polygone blanc dessiné sur un fond rouge
///////////////////////////////////////////////////////////////////////////////  

#include <stdio.h>     
#include <stdlib.h>     
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
//#include "GL/freeglut.h"
#include "glad/glad.h"
#include <fstream>

/* Dans les salles de TP, vous avez généralement accès aux glut dans C:\Dev. Si ce n'est pas le cas, téléchargez les .h .lib ...
Vous pouvez ensuite y faire référence en spécifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>. 
Si vous mettez glut dans le répertoire courant, on aura alors #include "glut.h" 
*/


#include "CVector.h"
#include "CPoint.h"
#include "Voxel.h"

using namespace std;

// Définition de la taille de la fenêtre
#define WIDTH  480

#define HEIGHT 480

// Définition de la couleur de la fenêtre
#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1


// Touche echap (Esc) permet de sortir du programme
#define KEY_ESC 27
#define PRECISION 0.1

    int ortho4 = -5;
    float x_min=999;
    float x_max=0;
    float y_min=999;
    float y_max=0;
    float z_min=999;
    float z_max=0;
    bool firstPass = true;
// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 


struct Triangle {
    unsigned indice1;
    unsigned indice2;
    unsigned indice3;


    //GL_UNSIGNED_BYTE indice1;
    //GL_UNSIGNED_BYTE indice2;
    //GL_UNSIGNED_BYTE indice3;
};

int main(int argc, char **argv) 
{  
    // initialisation  des paramètres de GLUT en fonction
    // des arguments sur la ligne de commande

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);

    // définition et création de la fenêtre graphique, ainsi que son titre
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Premier exemple : carré");

    // initialisation de OpenGL et de la scène
    initGL();
    init_scene();

    // choix des procédures de callback pour
    // le tracé graphique
    glutDisplayFunc(&window_display);
    // le redimensionnement de la fenêtre
    glutReshapeFunc(&window_reshape);
    // la gestion des événements clavier
    glutKeyboardFunc(&window_key);

    // la boucle prinicipale de gestion des événements utilisateur
    glutMainLoop();

    return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque
GLvoid initGL() 
{
    gladLoadGL();
    glClearColor(RED, GREEN, BLUE, ALPHA);
  //  glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
}

// Initialisation de la scene. Peut servir à stocker des variables de votre programme
// à initialiser
void init_scene()
{
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // C'est l'endroit où l'on peut dessiner. On peut aussi faire appel
    // à une fonction (render_scene() ici) qui contient les informations
    // que l'on veut dessiner

    render_scene();

    // trace la scène grapnique qui vient juste d'être définie
    glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{  
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
    // possible de changer la taille de l'objet dans la fenêtre. Augmentez ces valeurs si l'objet est
    // de trop grosse taille par rapport à la fenêtre.

    float x_min_f = x_min - (abs(x_max - x_min));
    float x_max_f = x_max + (abs(x_max - x_min));
    float y_min_f = y_min - (abs(y_max - y_min));
    float y_max_f = y_max + (abs(y_max - y_min));
    float z_min_f = z_min - (abs(z_max - z_min));
    float z_max_f = z_max + (abs(z_max - z_min));

    glOrtho(x_min_f, x_max_f, y_min_f, y_max_f, z_min_f, z_max_f);
    

    // toutes les transformations suivantes s´appliquent au modèle de vue
    glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y) 
{  
    switch (key) {
    case KEY_ESC:
        exit(1);
        break;

    case 'z':;
        ortho4++;
        glutPostRedisplay();
        break;

    case 's':
        ortho4--;
        glutPostRedisplay();
        break;

    default:
        printf ("La touche %d n´est pas active.\n", key);
        break;
    }
}


double rand_float(double a, double b) {
    return ((double)rand() / RAND_MAX) * (b - a) + a;
}



//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene()
{

    // Exercice 1
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //string fic;
    //cout << "Fichier ? " << endl;
   // cin >> fic;
   ifstream File ("MeshSegmentation.off");
    string Line;
    getline(File, Line);

    cout << Line << endl;

    unsigned NbPoints;
    unsigned NbTriangles;
    unsigned Zero;

    File >> NbPoints;
    File >> NbTriangles;
    File >> Zero;

    float Points[NbPoints*3];
    int Indices[NbTriangles*3];
    int Voisins[NbTriangles*3];
    
    cout << NbPoints << " " << NbTriangles << " " << Zero << endl;
    float X;
    
    for (int i =0 ; i < 3 * NbPoints; i++) {
        File >> X;
        Points[i] = X;
    }
    


    for (int z = 0; z < NbPoints * 3; z++){
        if (z % 3 == 0){
            if (Points[z] < x_min)
                x_min = Points[z];
            else if (Points[z] > x_max)
                x_max = Points[z];
        }
        else if (z % 3 == 1){
             if (Points[z] < y_min)
                y_min = Points[z];
            else if (Points[z] > y_max)
                y_max = Points[z];
        }
        else if (z % 3 == 2){
             if (Points[z] < z_min)
                z_min = Points[z];
            else if (Points[z] > z_max)
                z_max = Points[z];
        }
    } 

    int I1;
    unsigned nbL = 0;
    float tmp;
    for (unsigned i (0); i < 3 * NbTriangles; i++) {
        if (nbL == 0) 
            File >> tmp;
        nbL++;
        File >> I1;
        cout << I1 << " "  << endl;

        Indices[i] = I1; 
        if (nbL == 3)
            nbL = 0;
    }
   

    File.close();

    cout << Indices[3 * NbTriangles-3];

    cout << Indices[3 * NbTriangles-2];

    cout << Indices[3 * NbTriangles-1];

    cout << "Fin parser lancement scence" << endl; 

  /* vector<vector<int>> indicesVoisins (NbTriangles);

for (int i = 0; i < NbTriangles; i++){
    indicesVoisins[i].push_back(-1);
    indicesVoisins[i].push_back(-1);
    indicesVoisins[i].push_back(-1);
    }

    for (int i = 0; i < NbTriangles; i++){

    if (indicesVoisins[3 * i][0] != -1)
        if(indicesVoisins[3 * i][1] != -1)
            indicesVoisins[3 * i][2] = i;
        else if (indicesVoisins[3 * i][0] == -1)
                indicesVoisins[3 * i][0] = i;
            else 
                indicesVoisins[3 * i][1] = i;   


    if (indicesVoisins[3 * i+1][0] != -1)
        if(indicesVoisins[3 * i+1][1] != -1)
            indicesVoisins[3 * i+1][2] = i;
        else if (indicesVoisins[3 * i+1][0] == -1)
                indicesVoisins[3 * i+1][0] = i;
            else 
                indicesVoisins[3 * i+1][1] = i; 


    if (indicesVoisins[3 * i+2][0] != -1)
        if(indicesVoisins[3 * i+2][1] != -1)
            indicesVoisins[3 * i+2][2] = i;
        else if (indicesVoisins[3 * i+2][0] == -1)
                indicesVoisins[3 * i+2][0] = i;
            else 
                indicesVoisins[3 * i+2][1] = i; 
    }*/


    vector<vector<int>> indicesVoisins (NbTriangles);

  for (int i = 0; i < NbTriangles; i++){
    indicesVoisins[i].push_back(-1);
    indicesVoisins[i].push_back(-1);
    indicesVoisins[i].push_back(-1);
    }

for (int i = 0; i < NbTriangles; i++){
    int cpt = 0;
    for (int j = 0; j < NbTriangles; j++){
        if (i != j){
            if ((Indices[3 * i] ==  Indices[3 *j]) || (Indices[3 * i+1] ==  Indices[3 *j+1]) || (Indices[3 * i+2] ==  Indices[3 *j+2]))
                cpt++;

            if ((Indices[3 * i+1] ==  Indices[3 *j]) || (Indices[3 * i+2] ==  Indices[3 * j]))
                cpt++;

            if ((Indices[3 * i] ==  Indices[3 *j+1]) || (Indices[3 * i] ==  Indices[3 * j+2]))
                cpt++;

            if ((Indices[3 * i+1] == Indices[3 * j+2]) || (Indices[3 * i+2] == Indices[3 * j+1]))
                cpt++;
        
            if (cpt >= 2){
                if (indicesVoisins[i][0] == -1)
                    indicesVoisins[i][0] = j;
                
                else if (indicesVoisins[i][1] == -1)
                    indicesVoisins[i][1] = j;

                    else indicesVoisins[i][2] = j;

            }
        }
    }
}

for (int i = 0; i < indicesVoisins.size(); i++){
    cout << "Voisins de " << i << " : " << endl;
    cout << indicesVoisins[i][0] << " " << indicesVoisins[i][1] << " " << indicesVoisins[i][2] << endl;
}

    GLfloat all_normal[NbTriangles * 3];

    for (int i = 0; i < NbTriangles; i++){
        CPoint p1 = CPoint(Points[Indices[3 * i] *3],Points[Indices[3 * i] *3 +1],Points[Indices[3 * i] *3 +2]);
        CPoint p2 = CPoint(Points[Indices[3 * i + 1] *3],Points[Indices[3 * i + 1] *3 +1],Points[Indices[3 * i + 1] *3 +2]);
        CPoint p3 = CPoint(Points[Indices[3 * i + 2] *3],Points[Indices[3 * i + 2] *3 +1],Points[Indices[3 * i + 2] *3 +2]);

        CVector v1 = CVector(p2.getX(),p2.getY(),p2.getZ(),p1.getX(),p1.getY(),p1.getZ());
        CVector v2 = CVector(p3.getX(),p3.getY(),p3.getZ(),p1.getX(),p1.getY(),p1.getZ());

       // v1.Normalize();
        //v2.Normalize();

    CVector normal = v1.Vectoriel(v2);
        normal.Normalize();

       all_normal[i * 3] = normal.getX();
        all_normal[i * 3 + 1] = normal.getY();
        all_normal[i * 3 + 2] = normal.getZ();

    }
/* */
    cout << "test" << endl;


int rayon = 10, hauteur =20;
    gluLookAt(rayon + 5, hauteur / 2, rayon + 5, 0, 0, 0, 0, 1, 0);
    glOrtho(-ortho4, ortho4, -ortho4, ortho4, -ortho4, ortho4);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glOrtho(-5,5,-5,5,-5,5);
   // gluLookAt(0,0.0,0,0,0,0, 0, 1, 0);   // toutes les transformations suivantes s´appliquent au modèle de vue 
//  gluLookAt(x_max_f , y_max_f, z_max_f, (x_min + x_max) / 2.0, (y_min + y_max) / 2.0, (z_min+ z_max) / 2.0, 0, 1, 0);   // toutes les transformations suivantes s´appliquent au modèle de vue 

   /* GLfloat lightColor0[] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat lightPos0[] = {20.0f,50.0f,20.0f,10.0f};
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor0);

    int MatSpec[4] = { 0,0,0,0 };
    glMaterialiv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
    glMaterialiv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);*/

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, Points);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, all_normal);
    //glDrawElements(GL_TRIANGLES, 3 * NbTriangles, GL_UNSIGNED_INT, Indices);
    for (unsigned i (0) ; i < NbTriangles; i++) {
        GLfloat x = all_normal[3*i];
        GLfloat y = all_normal[3*i+1];
        GLfloat z = all_normal[3*i+2];
        float sin = y / sqrt(x*x+y*y);
        float factor  = asin(sin);
        if (factor == 0)
            factor = 0.1;
        if  (factor < 1)
            factor * 10;
        glColor3f(1/factor, 0.0, 0.0);
        glDrawElements(GL_TRIANGLES, 3 * i, GL_UNSIGNED_INT, Indices);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
     //glutSwapBuffers();

    /* */


}
