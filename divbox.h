#ifndef __DIVBOX_H__
#define __DUVBOX_H__

#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "lasercutter.h"


#define MAXDIVS 100
class BoxWithDivs;

class Span
{
public:
  Span(int page, double ox, double oy, double angle, double len)
  {
    m_next = NULL;
    m_oy = oy;
    m_ox = ox;
    m_angle = angle;
    m_start = 0;
    m_end = len;
    m_page = page;
  }
  ~Span()
  {
    delete m_next;
  }

  double m_ox, m_oy;
  double m_angle;
  double m_start;
  double m_end;
  int m_page;
  Span *m_next;
};

class SpanObject
{
public:
  SpanObject(double size, int id)
  {
    m_size = size;
    m_id = id;
  }

  double m_size;
  int m_id;
};

class SpanManager
{
public:
  SpanManager()
  {
    m_nrObjects = 0;
    m_spans = NULL;
  }

  ~SpanManager()
  {
    for (int i = 0; i < m_nrObjects ; i++)
      {
        delete m_objs[i];
      }
    delete m_spans;
  }

  void AppendSpan(int page, double ox, double oy, double angle, double len)
  {
    Span *s = new Span(page, ox, oy, angle, len);
    if (!m_spans)
      {
        m_spans = s;
      }
    else
      {
        Span *c = m_spans;
        while (c->m_next) c = c->m_next;
        c->m_next = s;
      }
  }
  void PrependSpan(int page, double ox, double oy, double angle, double len)
  {
    Span *s = new Span(page, ox, oy, angle, len);
    s->m_next = m_spans;
    m_spans = s;
  }

  void AddObject(double len, int id)
  {
    SpanObject *so = new SpanObject(len, id);
    m_objs[m_nrObjects++] = so;
  }

  void RenderLayout(BoxWithDivs *box);// pass NULL to only test

  void BuildLayout(SpanObject **o, int startWith, int size)
  {
    if (1 == size)
      {
        //				printf("testlayout");
        RenderLayout(NULL);
        return;
      }
    SpanObject *tmp = o[startWith];
    o[startWith] = o[0];
    o[0] = tmp;
    for (int i = 0; i < size -1; i++)
      {
        BuildLayout(o+1, i, size -1);
      }
  }

  void CalcLayout()
  {
    m_bestLayoutNrSpans = INT_MAX;
    //			printf("calclayout\n");
    for (int i = 0; i < m_nrObjects; i++)
      {
        //				printf("test l %d\n", i);
        BuildLayout(m_objs, i, m_nrObjects);
      }
  }

private:
  Span *m_spans;
  SpanObject *m_objs[MAXDIVS*MAXDIVS];
  SpanObject *m_objsBestLayout[MAXDIVS*MAXDIVS];
  int m_bestLayoutNrSpans;
  double m_bestLayoutRemaining;
  int m_nrObjects;
};


class VDiv
{
public:
  VDiv(VDiv *next, double fromLeft)
  {
    m_next = next;
    m_fromLeft = fromLeft;
  }
  ~VDiv()
  {
    delete m_next;
  }

  double m_fromLeft;
  VDiv *m_next;
};

class Div
{
public:
  Div(Div *next, double fromBottom)
  {
    m_fromBottom = fromBottom;
    m_next = next;
    m_vdivs = NULL;
  }
  ~Div()
  {
    delete m_next;
    delete m_vdivs;
  }

  void AddVDiv(double fromLeft)
  {
    m_vdivs = new VDiv(m_vdivs, fromLeft);
  }

  double m_fromBottom;
  Div *m_next;
  VDiv *m_vdivs;
};

class BoxWithDivs
{
public:
  BoxWithDivs(
      char const *name,
      double width,
      double length,
      double height,
      double divHeight,
      double woodThickness,
      double toothWidth,
      double taper,
      const double laser_cutter_width,
      const double laser_cutter_height
      )
    : m_d(name, laser_cutter_width, laser_cutter_height),
      m_laser_cutter_height{laser_cutter_height},
      m_laser_cutter_width{laser_cutter_width}
  {
    m_divs = 0;
    m_curDivForVDivs = m_firstVDivs = new Div(NULL, 0);
    m_width = width;
    m_height = height;
    m_length = length;
    m_divHeight = divHeight;
    m_woodThickness = woodThickness;
    m_toothWidth = toothWidth;
    m_taper = taper;
    BaseBox();
    m_divCount = 0;

    m_divsFitInGap = 0;
    while ( CompactedDivsVertSize(m_divsFitInGap+1) < m_width - 2 * m_woodThickness)
      {
        m_divsFitInGap++;
      }
    m_sizeUsedV = (m_width + 2 * m_height - 2 * m_woodThickness);
    m_sizeUsedH = (m_length + m_width + 1.5 * m_toothWidth );
    const double remainsOnSide = m_laser_cutter_width - m_sizeUsedH;
    const int rowsOnSide =  (int)floor(remainsOnSide / m_divHeight);

    int rowsOnTop = (int)floor((m_laser_cutter_height -  m_sizeUsedV )/ divHeight);
    double sideH = rowsOnSide > rowsOnTop ? m_laser_cutter_height : m_sizeUsedV;
    double topW = rowsOnSide > rowsOnTop ? m_sizeUsedH : m_laser_cutter_width;

    for (int i = 0; i < rowsOnSide; i++)
      {
        m_spans.AppendSpan(0, m_sizeUsedH + (i+1) * m_divHeight, 0, M_PI/2, sideH);
      }

    for (int i = 0; i < rowsOnTop; i++)
      {
        m_spans.AppendSpan(0, 0, m_sizeUsedV +i * m_divHeight, 0, topW);
      }


    int rows = (int)floor(m_laser_cutter_height / divHeight);
    for (int i = 0; i < rows; i++)
      {
        m_spans.AppendSpan(
              1,
              0,
              m_laser_cutter_height - (i+1)* m_divHeight,
              0,
              m_laser_cutter_width
              );
      }

  }
  ~BoxWithDivs()
  {
    RenderDivs();
    delete m_divs;
    delete m_firstVDivs;
  }

  double CompactedDivsVertSize(int nr)
  {
    return m_divHeight * (nr) - ((nr)/2)* m_woodThickness;
  }

  void AddDiv(double fromBottom);
  void AddVDiv(double fromLeft);
  void DrawSingleDiv(int page, int id, double len, double x, double y, double angle);

private:
  void RenderDivs();
  Div *m_divs;
  Div *m_firstVDivs;
  Div *m_curDivForVDivs;
  double m_width, m_height, m_length, m_divHeight, m_woodThickness, m_toothWidth, m_taper;
  LaserCutterDrawing m_d;
  const double m_laser_cutter_height;
  const double m_laser_cutter_width;
  void BaseBox();
  void AddDivHoles(double fromBottom);
  void AddVDivHoles(double fromLeft, int top, int bottom);
  void DrawVDivHoles(int page, double fromLeft, double ox, double oy,  double angle);
  void DrawDivsHor(int number, double xstart, double ystart, double w, double h);
  void DrawDivsInGap(int nr);
  double GetVDivSize(int top, int bottom);
  double GetDivPos(int div)
  {
    Div *s = m_divs;

    for (int i = 0; i < div; i++)
      {
        s = s->m_next;
        assert(s);
      }

    return s->m_fromBottom;
  }

  int m_divCount;


  void DivsHorSize(int number);

  int m_divsFitInGap;
  double m_sizeUsedH;
  double m_sizeUsedV;

  double m_divX[MAXDIVS];
  double m_divY[MAXDIVS];
  double m_divAngle[MAXDIVS];
  int m_divPage[MAXDIVS];

  SpanManager m_spans;
};
#endif
