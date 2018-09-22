#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "divbox.h"

void BoxWithDivs::BaseBox()
{
	m_d.LineRel(0, m_length + m_width - 2 * m_woodThickness);
	m_d.MoveRel(M_PI, m_width - 2 * m_woodThickness);

	m_d.LineRel(M_PI/2, m_toothWidth);
	m_d.ToothedLine(M_PI/2, m_height- m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(M_PI/2, m_woodThickness);

	m_d.LineRel(M_PI, m_woodThickness + m_toothWidth);
	m_d.ToothedLine(M_PI, m_length - 2 * (m_toothWidth+m_woodThickness), m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(M_PI, m_toothWidth + m_woodThickness);
	
	m_d.LineRel(-M_PI/2, m_woodThickness);
	m_d.ToothedLine(-M_PI/2, m_height - m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(-M_PI/2, m_toothWidth);

	m_d.MoveAbs(m_length, m_height);

	m_d.LineRel(M_PI/2, m_toothWidth);
	m_d.ToothedLine(M_PI/2, m_width - 2 * m_woodThickness - 2 * m_toothWidth, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(M_PI/2, m_toothWidth);

	m_d.LineRel(M_PI, m_woodThickness+m_toothWidth);
	m_d.ToothedLine(M_PI, m_length - 2 * (m_toothWidth+m_woodThickness), m_toothWidth, -m_woodThickness, m_taper);
	m_d.LineRel(M_PI, m_woodThickness+m_toothWidth);

	m_d.LineRel(-M_PI/2, m_toothWidth);
	m_d.ToothedLine(-M_PI/2, m_width - 2 * m_woodThickness - 2 * m_toothWidth, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(-M_PI/2, m_toothWidth);

	m_d.MoveAbs(m_length, m_height + m_width - 2 * m_woodThickness);

	m_d.LineRel(M_PI/2, m_woodThickness);
	m_d.ToothedLine(M_PI/2, m_height - m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(M_PI/2, m_toothWidth);

	m_d.MoveAbs(0, m_height + m_width - 2 * m_woodThickness);

	m_d.LineRel(M_PI/2, m_woodThickness);
	m_d.ToothedLine(M_PI/2, m_height - m_toothWidth - m_woodThickness, m_toothWidth, -m_woodThickness, m_taper);
	m_d.LineRel(M_PI/2, m_toothWidth);

	m_d.LineRel(0, m_length + m_width - 2 * m_woodThickness);

	m_d.LineRel(-M_PI/2, m_toothWidth);
	m_d.ToothedLine(-M_PI/2, m_height- m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness, m_taper);

	m_d.LineRel(M_PI, m_toothWidth);
	m_d.ToothedLine(M_PI, m_width - 2 *m_woodThickness - 2 * m_toothWidth, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(M_PI, m_toothWidth);

	m_d.MoveAbs(m_length+m_width - 2*m_woodThickness, 0);

	m_d.LineRel(M_PI/2, m_toothWidth);
	m_d.ToothedLine(M_PI/2, m_height- m_toothWidth - m_woodThickness, m_toothWidth, -m_woodThickness, m_taper);

	m_d.LineRel(M_PI, m_toothWidth);
	m_d.ToothedLine(M_PI, m_width - 2 *m_woodThickness - 2 * m_toothWidth, m_toothWidth, -m_woodThickness, m_taper);
	m_d.LineRel(M_PI, m_toothWidth);

}

void BoxWithDivs::AddDivHoles(double distFromLeftInside)
{
	m_d.MoveAbs(m_woodThickness * .5 + distFromLeftInside, m_toothWidth + m_height - m_divHeight);
	m_d.HolesLine(M_PI/2, m_divHeight - 2 * m_toothWidth -m_woodThickness, m_toothWidth, m_woodThickness);

	m_d.MoveAbs(m_woodThickness * -.5 + distFromLeftInside, m_height + m_divHeight + m_width - 2 * m_woodThickness - m_toothWidth);
	m_d.HolesLine(-M_PI/2, m_divHeight - 2 * m_toothWidth -m_woodThickness, m_toothWidth, m_woodThickness);

	m_d.MoveAbs(m_woodThickness * .5 + distFromLeftInside, m_height + m_toothWidth);
	m_d.HolesLine(M_PI/2, m_width - 2 * m_toothWidth - 2 * m_woodThickness, m_toothWidth, m_woodThickness);

}

void BoxWithDivs::DrawDivsHor(int number, double xstart, double ystart, double w, double h)
{
	if (m_divHeight * number - (number/2)* m_woodThickness > h)
	{
		printf("error! no space for %d divs in alotted space", number);
		exit(1);
	}

	double DB = m_width - 2 * m_woodThickness;
	double displacement;
	m_d.GetNumTeeth(DB - 2 * m_toothWidth, m_toothWidth , &displacement);


	if (m_width + m_toothWidth > w)
	{
		printf("error! divs too wide for alotted space");
		exit(1);
	}



	for (int i = 0; i <= number; i +=2)
	{
		m_d.MoveAbs(xstart+m_woodThickness, ystart + i * m_divHeight - (i/2) * m_woodThickness);
		m_d.LineRel(0, DB + displacement);
		if (i < number)
		{
			m_divX[i] = xstart + m_width;
			m_divY[i] = ystart + i * m_divHeight - (i/2) * m_woodThickness + m_divHeight;
			m_divAngle[i] = M_PI;
			m_divPage[i] = 0;

			m_d.MoveAbs(xstart+m_woodThickness, ystart + i * m_divHeight - (i/2) * m_woodThickness);
//			m_d.LineRel(M_PI/2, m_divHeight - m_woodThickness);
			m_d.LineRel(M_PI/2, m_toothWidth);
			m_d.ToothedLine(M_PI/2, m_divHeight-m_woodThickness - 2*m_toothWidth, m_toothWidth, m_woodThickness, m_taper);
			m_d.LineRel(M_PI/2, m_toothWidth);
			if (i+1 < number)
			{
				m_divX[i+1] = xstart + displacement;
				m_divY[i+1] = ystart + i * m_divHeight - (i/2) * m_woodThickness + m_divHeight - m_woodThickness;
				m_divAngle[i+1] = 0;
				m_divPage[i+1] = 0;
				m_d.MoveRel(0, displacement);
				m_d.MoveRel(M_PI/2, m_woodThickness);
//				m_d.LineRel(M_PI/2, m_divHeight - m_woodThickness);
				m_d.LineRel(M_PI/2, m_toothWidth);
				m_d.ToothedLine(M_PI/2, m_divHeight-m_woodThickness - 2*m_toothWidth, m_toothWidth, m_woodThickness, m_taper);
				m_d.LineRel(M_PI/2, m_toothWidth);
			}
			m_d.MoveAbs(xstart+DB+m_woodThickness, ystart +i * m_divHeight - (i/2) * m_woodThickness);
//			m_d.LineRel(M_PI/2, m_divHeight - m_woodThickness);
			m_d.LineRel(M_PI/2, m_toothWidth);
			m_d.ToothedLine(M_PI/2, m_divHeight-m_woodThickness - 2*m_toothWidth, m_toothWidth, -m_woodThickness, m_taper);
			m_d.LineRel(M_PI/2, m_toothWidth);

			if (i+1 < number)
			{
				m_d.MoveRel(0, displacement);
				m_d.MoveRel(M_PI/2, m_woodThickness);
//				m_d.LineRel(M_PI/2, m_divHeight - m_woodThickness);
				m_d.LineRel(M_PI/2, m_toothWidth);
				m_d.ToothedLine(M_PI/2, m_divHeight-m_woodThickness - 2*m_toothWidth, m_toothWidth, -m_woodThickness, m_taper);
				m_d.LineRel(M_PI/2, m_toothWidth);
			}

			if (displacement < m_toothWidth)
			{
				m_d.MoveAbs(xstart+DB+m_woodThickness, ystart +i * m_divHeight - (i/2) * m_woodThickness + m_divHeight - m_woodThickness);
				m_d.LineRel(M_PI, m_toothWidth -displacement);
				m_d.MoveAbs(xstart+displacement+m_woodThickness, ystart +i * m_divHeight - (i/2) * m_woodThickness + m_divHeight);
				m_d.LineRel(0, m_toothWidth -displacement);
				m_d.MoveAbs(xstart+DB+ displacement+m_woodThickness, ystart +i * m_divHeight - (i/2) * m_woodThickness + m_divHeight);
				m_d.LineRel(M_PI, m_toothWidth -displacement);
				
			}
			
			m_d.MoveAbs(xstart+m_woodThickness, ystart +i * m_divHeight - (i/2) * m_woodThickness + m_divHeight - m_woodThickness);
			m_d.LineRel(0, m_toothWidth);
			m_d.ToothedLine(0, DB - 2 * m_toothWidth + 2 * displacement, displacement, m_woodThickness , m_taper);

		}
	}


}


double BoxWithDivs::GetVDivSize(int top, int bottom)
{
	double start = 0;
	if (top < 0)
	{
		start = m_woodThickness;
	}
	else
	{
		Div *t = m_divs;

		for (int i = 0; i < top; i++)
		{
			assert(t);
			t = t->m_next;
		}
		assert(t);

		start = t->m_fromBottom + m_woodThickness /2;
	}

	double end = 0;
	if (bottom < 0)
	{
		end = m_length - m_woodThickness;
	}
	else
	{
		Div *t = m_divs;

		for (int i = 0; i < bottom; i++)
		{
			assert(t);
			t = t->m_next;
		}
		assert(t);

		end = t->m_fromBottom - m_woodThickness /2;
	}

//	printf("getvdivsize %d %d  %f -  %f = %f\n", top, bottom, end , start, end - start);
	return end - start;
}

void BoxWithDivs::DrawDivsInGap(int nr)
{
    DrawDivsHor(nr, m_length, m_height, m_laser_cutter_width - m_length, m_width - 2 * m_woodThickness);
}

void dumpdivs(Div *s)
{
	printf("dump: ");
	for (; s ; s = s->m_next)
	{
        printf("[%p %f %p] ", (void*)s, s->m_fromBottom, (void*)s->m_next);
	}
	printf("\n");
}

void BoxWithDivs::AddDiv(double fromBottom)
{
	if (m_divCount >= MAXDIVS)
	{
		printf("max divs is %d, extraneous divs ignored\n", MAXDIVS);
		return;
	}
	m_divCount++;
	m_curDivForVDivs = m_divs = new Div(m_divs, fromBottom);
	Div *cur = m_divs;
//	dumpdivs(m_divs);
	Div *prev = NULL;
	while (cur->m_next && cur->m_fromBottom > cur->m_next->m_fromBottom)
	{
		Div *tmp = cur->m_next;
		if (!prev)
		{
			m_divs = tmp;
		}
		else
		{
			prev->m_next = tmp;
		}
		cur->m_next = tmp->m_next;
		tmp->m_next = cur;
//		printf("sorted\n");
//		dumpdivs(m_divs);
		prev = tmp;
	}
//	dumpdivs(m_divs);

}

void BoxWithDivs::AddVDiv(double fromLeft)
{
	m_curDivForVDivs->AddVDiv(fromLeft);
}

void BoxWithDivs::RenderDivs()
{

	int count = m_divCount;

	if (count > m_divsFitInGap)
	{
		count = m_divsFitInGap;
	}

	DrawDivsInGap(count);

	count = m_divCount - count;

	if (count)
	{
		for (int i = 0; i < count ; i++)
		{
			m_spans.AddObject(m_width, m_divsFitInGap+i);
		}
	}
	else
	{
		//see how much space is left and use the space in the spanmanager
		double size = CompactedDivsVertSize(m_divCount);
		double remains = m_width - 2 * m_woodThickness - size;
		int rows = (int)floor(remains/ m_divHeight);
		for (int i = rows-1; i >=0; i--)
		{
			m_spans.PrependSpan(0, m_length, m_height + size + m_divHeight * i, 0, m_sizeUsedH - m_length);
		}
	}

	// add vdivs to spanmanager
	int id = m_divCount;
	for (VDiv *vd = m_firstVDivs->m_vdivs; vd; vd = vd->m_next)
	{
		m_spans.AddObject(GetVDivSize(-1, 0) + 2*m_woodThickness, id++);
	}

	int curDiv = 0;
	for (Div *cur = m_divs; cur; cur = cur->m_next)
	{
		for (VDiv *vd = cur->m_vdivs; vd; vd = vd->m_next)
		{
			int nextDiv = curDiv+1;
			if (nextDiv >= m_divCount) nextDiv = -1;
			double size = GetVDivSize(curDiv, nextDiv) + 2*m_woodThickness;
			m_spans.AddObject(size , id++);
		}
		curDiv++;
	}

	m_spans.CalcLayout();

	m_spans.RenderLayout(this);

	for (VDiv *vd = m_firstVDivs->m_vdivs; vd; vd = vd->m_next)
	{
		AddVDivHoles(vd->m_fromLeft, -1, 0);
	}

	curDiv = 0;
	for (Div *cur = m_divs; cur; cur = cur->m_next)
	{
		AddDivHoles(cur->m_fromBottom);
		for (VDiv *vd = cur->m_vdivs; vd; vd = vd->m_next)
		{
			int nextDiv = curDiv+1;
			if (nextDiv >= m_divCount) nextDiv = -1;
			AddVDivHoles(vd->m_fromLeft, curDiv, nextDiv);
		}
		curDiv++;
	}

}

void BoxWithDivs::DrawSingleDiv(int page, int id, double len, double x, double y, double a)
{
	if (page > 0)
	{
		printf("multipage not yet supported\n");
		return;
	}

	m_divX[id] = x;
	m_divY[id] = y;
	m_divAngle[id] = a;
	m_divPage[id] = page;


//	m_d.MoveAbs(x, y);
//	m_d.LineRel(a,len);
//	m_d.MoveAbs(x, y);
//	m_d.LineRel(a+M_PI/2, m_divHeight);
//	return;

	m_d.MoveAbs(x, y);
	m_d.MoveRel(a, m_woodThickness);
	m_d.MoveRel(a+M_PI/2, m_woodThickness);
	len -= 2 * m_woodThickness;

	m_d.LineRel(a+M_PI/2, m_toothWidth);
	m_d.ToothedLine(a+M_PI/2, m_divHeight -2 * m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(a+M_PI/2, m_toothWidth);

	m_d.LineRel(a, len);
	
	m_d.LineRel(a-M_PI/2, m_toothWidth);
	m_d.ToothedLine(a-M_PI/2, m_divHeight -2 * m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(a-M_PI/2, m_toothWidth);

	m_d.LineRel(a + M_PI, m_toothWidth);
	m_d.ToothedLine(a+M_PI, len - 2 * m_toothWidth, m_toothWidth, m_woodThickness, m_taper);
	m_d.LineRel(a + M_PI, m_toothWidth);

	return;
	
	m_d.LineRel(a, m_toothWidth);
	m_d.ToothedLine(a, len - 2 * m_toothWidth, m_toothWidth, -m_woodThickness, m_taper);
	m_d.LineRel(a, m_toothWidth);

	m_d.LineRel(a+M_PI/2, m_toothWidth);
	m_d.ToothedLine(a+M_PI/2, m_divHeight - 2 * m_toothWidth - m_woodThickness, m_toothWidth, -m_woodThickness, m_taper);
	m_d.LineRel(a+M_PI/2, m_toothWidth);

	m_d.LineRel(a + M_PI, len);

	m_d.LineRel(a-M_PI/2, m_toothWidth);
	m_d.ToothedLine(a-M_PI/2, m_divHeight - 2 * m_toothWidth - m_woodThickness, m_toothWidth, -m_woodThickness, m_taper);
	m_d.LineRel(a-M_PI/2, m_toothWidth);

}

void BoxWithDivs::DrawVDivHoles(int page, double fromLeft, double ox, double oy, double angle)
{
	if (page >0)
	{
		printf("mulitpage not yet supported\n");
		return;
	}
	double xstart = ox + cos(angle) * (fromLeft + m_woodThickness/2);
	double ystart = oy + sin(angle) * (fromLeft + m_woodThickness/2);
	m_d.MoveAbs(xstart, ystart);
	m_d.MoveRel(angle, m_woodThickness);
	m_d.MoveRel(angle+M_PI/2, m_woodThickness);
	m_d.MoveRel(angle + M_PI/2, m_toothWidth);
	m_d.HolesLine(angle + M_PI/2, m_divHeight - 2 * m_toothWidth - m_woodThickness, m_toothWidth, m_woodThickness);
}

void BoxWithDivs::AddVDivHoles(double fromLeft, int top, int bottom)
{
	double ox,oy;
	double angle;
	int page = 0;
    double topFromBottom = 0; //, bottomFromBottom = 0;
	if (top < 0)
	{
		ox = m_length - m_woodThickness;
		oy = m_height + m_width - 2*m_woodThickness;
		angle = 0;
		topFromBottom = m_woodThickness/2;
	}
	else
	{
		ox = m_divX[top];
		oy = m_divY[top];
		angle = m_divAngle[top];
		page = m_divPage[top];
		topFromBottom = GetDivPos(top);
	}

	DrawVDivHoles(page, fromLeft, ox, oy, angle);
	if (bottom < 0)
	{
		ox = m_length + m_width - m_woodThickness;
		oy = m_height;
		angle = M_PI;
        //bottomFromBottom = m_length - m_woodThickness/2;
	}
	else
	{
		ox = m_divX[bottom];
		oy = m_divY[bottom];
		angle = m_divAngle[bottom];
		page = m_divPage[bottom];
        //bottomFromBottom = GetDivPos(bottom);
	}

	DrawVDivHoles(page, fromLeft, ox, oy, angle);

	m_d.MoveAbs(topFromBottom+m_woodThickness/2, m_height + fromLeft - m_woodThickness/2);
	m_d.MoveRel(0, m_toothWidth);
	m_d.HolesLine(0, GetVDivSize(top, bottom) - 2 * m_toothWidth , m_toothWidth, m_woodThickness);

}

void SpanManager::RenderLayout(BoxWithDivs *box)
{
	Span *cur = m_spans;
//	printf("renderlayout: <span %f> ", cur->m_end);
	int nrSpans = 0;
	double curPos = 0;
    //bool fits = true;
	for (int i = 0; i < m_nrObjects; i++)
	{
		while (cur && cur->m_end - curPos < m_objs[i]->m_size)
		{
			cur = cur->m_next;
			curPos = 0;
			nrSpans++;
//			printf("<span %f> ", cur ? cur->m_end : 0);
		}
		if (!cur)
		{
			nrSpans +=1000;
            //fits = false;
			continue;
		}

		if (box)
		{
			box->DrawSingleDiv(cur->m_page, m_objs[i]->m_id, m_objs[i]->m_size, cur->m_ox + cos(cur->m_angle)* curPos, cur->m_oy + sin(cur->m_angle) * curPos, cur->m_angle);
		}
//		printf("<obj %f %d> ", m_objs[i]->m_size, m_objs[i]->m_id);
		curPos += m_objs[i]->m_size;
	}
//	printf("\n");
	double remaining = 0;

	if (cur)
	{
		remaining = cur->m_end - curPos;
	}

	if (nrSpans < m_bestLayoutNrSpans || (nrSpans == m_bestLayoutNrSpans && remaining < m_bestLayoutRemaining))
	{
		m_bestLayoutNrSpans = nrSpans;
		m_bestLayoutRemaining = remaining;
		memcpy(m_objsBestLayout, m_objs, sizeof(SpanObject *) * m_nrObjects);
	}

}








