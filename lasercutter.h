#ifndef __LASERCUTTER_H__
#define __LASERCUTTER_H__

#include <cairo/cairo.h>
#include <math.h>
#include <stdlib.h>

#define LASERCUTTERW 700
#define LASERCUTTERH 400

class LaserCutterDrawing
{
    public:
        LaserCutterDrawing(char const *name);
        ~LaserCutterDrawing();

        cairo_t *m_context;

        static double mm2points(double mm);
		void MoveAbs(double x, double y);
		void MoveRel(double angle, double len);
		void LineRel(double angle, double length);
		void HolesLine(double angle, double len, double toothWidth, double thickness);
		void ToothedLine(double angle, double len, double toothWidth, double depth, double taper);
		int GetNumTeeth(double len, double toothWidth, double *realTW = NULL);
    private:
        cairo_surface_t *m_surface;
        
};


#endif


