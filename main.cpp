#include "divbox.h"
#include <ctype.h>
#include <string.h>

bool ParseNumber(FILE *in, double *r)
{
  char buffer[1024];
  int c;
  while (isspace(c = fgetc(in)) && !feof(in));

  if (feof(in))
    return false;

  if (!(isdigit(c) || c == '+' || c == '-' || c == '.' ))
    {
      ungetc(c, in);
      return false;
    }

  int count = 0;
  while (!isspace(c) && !feof(in))
    {
      assert(count < 1023);
      buffer[count++] = c;
      c = fgetc(in);
    }

  buffer[count] = 0;
  if (r)
    {
      *r = strtod(buffer, NULL);
    }

  return true;
}

double ParseNumber(FILE *in, char const *errorMsgIfNotFound)
{
  double r = 0;

  if (!ParseNumber(in, &r))
    {
      puts(errorMsgIfNotFound);
      exit(1);
    }

  return r;
}

char const *ParseString(FILE *in)
{
  static char buffer[1024];
  int c;
  while (isspace(c = fgetc(in)) && !feof(in));
  int count = 0;

  while (!isspace(c) && !feof(in))
    {
      assert(count < 1023);
      buffer[count++]=c;
      c = fgetc(in);
    }
  buffer[count]=0;

  return buffer;
}

BoxWithDivs *Parse(FILE *in)
{
  double w=200,h=50,l=300,dh=45, thickness=3, toothWidth=6, taper=.01;
  double laser_cutter_height = 700;
  double laser_cutter_width = 400;
  char const *name = "divbox.eps";
  BoxWithDivs *ret = NULL;
  while (true)
    {
      int c;
      while (isspace(c = fgetc(in)) && !feof(in));

      if (feof(in))
        return ret;

      switch(c)
        {
        case '#':
          while (true)
            {
              c = fgetc(in);
              if (feof(in))
                return ret;

              if (c == 10 || c == 13)
                {
                  break;
                }
            }
          break;
        case 'B': // box dimensions
          w = ParseNumber(in, "Expected width of box\n");
          l = ParseNumber(in, "Expected Length of box\n");
          h = ParseNumber(in, "Expected Height of box\n");
          if (!ParseNumber(in, &dh))
            {
              dh = h;
            }
          printf("Dimensions W=%f L=%f H=%f DivH=%f\n", w,l,h,dh);
          break;
        case 'T':
          if (ret)
            {
              printf("cannot specify thickness after Divs have been created\n");
              exit(1);
            }
          thickness = ParseNumber(in, "Expected material thickness\n");
          if (!ParseNumber(in, &toothWidth))
            {
              toothWidth = thickness;
            }
          if (!ParseNumber(in, &taper))
            {
              taper = .01;
            }
          printf("Thickness=%f ToothWidth=%f Taper=%f\n", thickness, toothWidth, taper);
          break;
        case 'D': // div
          if (!ret)
            {
              ret = new BoxWithDivs(name, w, l, h, dh, thickness, toothWidth, taper, laser_cutter_width, laser_cutter_height);
            }
          {
            double fb = ParseNumber(in, "Expected Div location\n");
            ret->AddDiv(fb);
          }
          printf("Added div\n");
          break;
        case 'V': // div
          if (!ret)
            {
              ret = new BoxWithDivs(name, w, l, h, dh, thickness, toothWidth, taper, laser_cutter_width, laser_cutter_height);
            }
          {
            double fr = ParseNumber(in, "Expected Div location\n");
            ret->AddVDiv(fr);
          }
          printf("Added vdiv\n");
          break;
        case 'N':
          name = ParseString(in);
          if (!ret)
            {
              printf("name set to %s\n", name);
            }
          else
            {
              printf("cannot set name after divs gave been created\n");
              exit(1);
            }
          break;
        case 'X':
          if (!ret)
            {
              ret = new BoxWithDivs(name, w, l, h, dh, thickness, toothWidth, taper, laser_cutter_width, laser_cutter_height);
            }
          return ret;
          break;
        default:
          printf("illegal character %c\n", c);
          exit(1);
        }
    }
  if (!ret)
    {
      ret = new BoxWithDivs(name, w, l, h, dh, thickness, toothWidth, taper, laser_cutter_width, laser_cutter_height);
    }
  return ret;
}

void help()
{
  printf(
        "Usage: divbox [file]\n"
        "  Reads a description of a box from a file or stdin and writes an eps file for the lasercutter\n\n"
        "Syntax of the input.\n"
        "  #                     Lines starting with # are ignored.\n"
        "  B <w> <l> <h> [dh]    Specify box dimensions, Width, Length, Height   and optionally the height of the divisions (if omitted dh will equal h).\n"
        "  N <name.eps>          Set the name of the outputfile.\n"
        "  T <t> [tw] [taper]    Set the thickness of the material, and optionally the size of the teeth and the amount of tapering  of the teeth.\n"
        "  D <pos>               Add a horizontal division at location <pos> along the <l> side of the box.\n"
        "  V <pos>               Add a vertical division in the current horizontal box at location <pos> along the <w> side of the box.\n"
        "  X (or EOF)            Exit the program and write the file\n.");

  exit(0);
}

int main(int argc, char **argv)
{
  FILE *in;
  if (argc == 1)
    {
      in = stdin;
    }
  else if (argc == 2)
    {
      if (!strcmp(argv[1] , "-h") || !strcmp(argv[1], "--help"))
        {
          help();
        }

      in = fopen(argv[1], "r");

      if (!in)
        {
          printf("cannot open file %s\n", argv[1]);
          exit(1);
        }
    }
  else
    {
      help();
    }
  BoxWithDivs *test = Parse(in);

  delete test;
  return 0;
}

