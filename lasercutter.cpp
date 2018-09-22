#include <cairo/cairo.h>
#include <cairo/cairo-ps.h>
#include <math.h>

#include "lasercutter.h"

double LaserCutterDrawing::mm2points(double mm)
{
  double inch = mm * 0.039370079;

  return inch * 72.0;
}

void LaserCutterDrawing::MoveAbs(double x, double y)
{
  cairo_move_to(m_context, x, y);
}

void LaserCutterDrawing::MoveRel(double angle, double len)
{
  cairo_rotate(m_context, angle);
  cairo_rel_move_to(m_context, len, 0);
  cairo_rotate(m_context, -angle);
}

void LaserCutterDrawing::LineRel(double angle, double len)
{
  cairo_rotate(m_context, angle);
  cairo_rel_line_to(m_context, len, 0);
  cairo_rotate(m_context, -angle);
}


int LaserCutterDrawing::GetNumTeeth(double len, double toothWidth, double *realTW)
{
  int num = (int)(len / toothWidth);

  if (!(num %2))
    num++;

  double width = len / num;

  num--;
  num /=2;

  if (realTW)
    {
      *realTW = width;
    }

  return num+1;
}


// for angle=0 draw a line with teeth sticking up from left to right, use negative depth to get the teeth to the other side
void LaserCutterDrawing::ToothedLine(double angle, double len, double toothWidth, double depth, double taper)
{
  cairo_rotate(m_context, angle);

  if (len < toothWidth)
    {
      cairo_rel_line_to(m_context, len, 0);
      cairo_rotate(m_context, -angle);
      return;
    }

  double width;
  int num = GetNumTeeth(len, toothWidth, &width) - 1;

  cairo_rel_line_to(m_context,taper, 0);
  for (int i = 0; i< num; i++)
    {
      cairo_rel_line_to(m_context,- 2* taper, depth);
      cairo_rel_line_to(m_context, width + 2 * taper, 0);
      cairo_rel_line_to(m_context,- 2* taper, -depth);
      cairo_rel_line_to(m_context, width + 2 * taper, 0);
    }
  cairo_rel_line_to(m_context,- 2* taper, depth);
  cairo_rel_line_to(m_context, width + 2 * taper, 0);
  cairo_rel_line_to(m_context,- 2* taper, -depth);
  cairo_rotate(m_context, -angle);
}


//for angle = 0 draw a line of holes to the right, bottom of the holes aligned to starting point
void LaserCutterDrawing::HolesLine(double angle, double len, double toothWidth, double thickness)
{
  cairo_rotate(m_context, angle);
  int num = (int)(len / toothWidth);

  if (!(num %2))
    num++;

  double width = len / num;

  num--;
  num /=2;

  for (int i = 0; i<= num; i++)
    {
      cairo_rel_line_to(m_context, width, 0);
      cairo_rel_line_to(m_context, 0, thickness);
      cairo_rel_line_to(m_context, -width, 0);
      cairo_rel_line_to(m_context, 0, -thickness);
      if (i < num)
        cairo_rel_move_to(m_context, 2* width , 0);
      else
        cairo_rel_move_to(m_context, width , 0);

    }
  cairo_rotate(m_context, -angle);


}


LaserCutterDrawing::LaserCutterDrawing(
    char const *name,
    const double laser_cutter_width,
    const double laser_cutter_height
    )
{
  m_surface = cairo_ps_surface_create(
        name,
        mm2points(laser_cutter_width),
        mm2points(laser_cutter_height)
        );
  cairo_ps_surface_set_eps(m_surface, true);

  m_context = cairo_create(m_surface);

  cairo_scale(m_context, mm2points(1), -mm2points(1));
  cairo_translate(m_context, 0, -400);
  cairo_set_line_width(m_context, mm2points(.1));
  cairo_set_source_rgb(m_context, 1.0,0,0);
  cairo_move_to(m_context, 0, 0);
}


LaserCutterDrawing::~LaserCutterDrawing()
{
  cairo_stroke(m_context);
  cairo_show_page(m_context);
  cairo_destroy(m_context);
  cairo_surface_destroy(m_surface);
}
