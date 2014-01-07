#include "visualiser.h"

#define OUTPUTRATE          48000
#define SPECTRUM_SIZE        1024
#define DIVISION_CONSTANT 8
#define MULTIPLICATION_CONSTANT 15000
ColorVec3f color = {0, 1, 0};
FMOD::Channel         *channel = 0;
static float spectrum[SPECTRUM_SIZE];

int mainWindowInit(int argc, char **argv)
{
    A_InitAudio();
    V_ExecuteDisplay(argc, argv);
    return 0;
}
void V_Init()
{
    glClearColor(0, 0, 0, 0.5);
}
void V_DrawArray()
{
    int i;
    GLfloat x1, y1, x2, y2;

    for(i = 0; i < SPECTRUM_SIZE/DIVISION_CONSTANT; i++)
    {
        x1 = (i/(SPECTRUM_SIZE/DIVISION_CONSTANT*1.0)) * glutGet(GLUT_WINDOW_WIDTH);
        x2 = (i/(SPECTRUM_SIZE/DIVISION_CONSTANT*1.0)) * glutGet(GLUT_WINDOW_WIDTH);
        y1 = spectrum[i] * MULTIPLICATION_CONSTANT * -1;
        y2 = spectrum[i] * MULTIPLICATION_CONSTANT;
        glowLine(x1, y1, x2, y2, 1.5, color);
    }
}
void glowLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat thickness, ColorVec3f color)
{
    GLfloat theta = atan((y2 - y1) / (x2 - x1));
    GLfloat y = thickness * cos(theta);
    GLfloat x = thickness * sin(theta);

    glBegin(GL_POLYGON);

    glColor4f(color.r, color.g, color.b, 1);
    glVertex2d(x2, y2);

    glColor4f(color.r, color.g, color.b, 0);
    glVertex2d(x2 - x, y2 + y);

    glColor4f(color.r, color.g, color.b, 0);
    glVertex2d(x1 - x, y1 + y);

    glColor4f(color.r, color.g, color.b, 1);
    glVertex2d(x1, y1);

    glColor4f(color.r, color.g, color.b, 0);
    glVertex2d(x1 + x, y1 - y);

    glColor4f(color.r, color.g, color.b, 0);
    glVertex2d(x2 + x, y2 - y);

    glEnd();
}
void V_Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    A_UpdateArray();
    V_DrawArray();

    glFlush();
    glutSwapBuffers();
}
void V_ReshapeFunc(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, -h/2, h/2);
}
int V_ExecuteDisplay(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1200, 800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("Audio Visualiser");
    V_Init();
    glutReshapeFunc(V_ReshapeFunc);
    glutDisplayFunc(V_Display);
    glutIdleFunc(V_Display);
    glutMainLoop();
    return 0;
}
int A_InitAudio()
{
    FMOD::System          *system  = 0;
    FMOD::Sound           *sound   = 0;
    FMOD_RESULT            result;
    FMOD_CREATESOUNDEXINFO exinfo;
    int                    driver, recorddriver, outputfreq;
    unsigned int           version;
    /*
            Create a System object and initialize.
        */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    ERRCHECK(result);

    driver = 0;
    result = system->setDriver(driver);
    ERRCHECK(result);

    recorddriver = 0;
    result = system->setSoftwareFormat(OUTPUTRATE, FMOD_SOUND_FORMAT_PCM16, 1, 0, FMOD_DSP_RESAMPLER_LINEAR);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    system->getSoftwareFormat(&outputfreq, 0, 0, 0, 0, 0);
    ERRCHECK(result);

    /*
            Create a sound to record to.
        */
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = 1;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = OUTPUTRATE;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5;

    result = system->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound);
    ERRCHECK(result);

    result = system->recordStart(recorddriver, sound, true);
    ERRCHECK(result);

    Sleep(200);      /* Give it some time to record something */

    result = system->playSound(FMOD_CHANNEL_REUSE, sound, false, &channel);
    ERRCHECK(result);

    /* Dont hear what is being recorded otherwise it will feedback.  Spectrum analysis is done before volume scaling in the DSP chain */
    result = channel->setVolume(0);
    ERRCHECK(result);
    return 0;
}
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}
void A_UpdateArray()
{
    FMOD_RESULT result;

    result = channel->getSpectrum(spectrum, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
    ERRCHECK(result);
}
