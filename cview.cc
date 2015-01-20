#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <cstdlib>
#include <cmath>


const char * rd_preface[] = {
  "Display \"Marching Cubes\" \"Screen\" \"rgbdouble\"",
};


void Usage(char * argv[]);

int main(int argc, char * argv[])
{
  int nplanes, nrows, ncols;

  if(argc < 5)
    {
      Usage(argv);
      return -1;
    }

  ncols = strtol(argv[1], 0, 10);
  nrows = strtol(argv[2], 0, 10);
  nplanes = strtol(argv[3], 0, 10);

  // Read in the polyset
  string data;

  ifstream in_file;
  
  const char * in_filename = argv[4];

  in_file.open(in_filename);
  if(!in_file)
    {
      cerr << "Can't open " << in_filename << " for input." << endl;
      return -1;
    }

  string line;
  getline(in_file, line);
  while(!in_file.eof())
    {
      data += line + '\n';
      getline(in_file, line);
    }
  in_file.close();


  // Output preface
  int n = sizeof(rd_preface) / sizeof(const char *);
  for(int i = 0; i < n; i++)
    {
      cout << rd_preface[i] << endl;
    }

  cout << "ObjectBegin \"Data\"\n";
  cout << data;
  cout << "ObjectEnd\n";
  
  const float SPIN_PERIOD = 100; // Number of frames
  const float WOBBLE_PERIOD = 200;  // Number of frames

  int frame_count = 0;
  while(1)
    {
      cout << "FrameBegin " << frame_count++ << "\n";
      cout << "WorldBegin\n";
      // Translate
      int max_dim = ncols;
      max_dim = (max_dim > nrows)? max_dim : nrows;
      max_dim = (max_dim > nplanes)? max_dim : nplanes;

      cout << "Translate 0 0 " << -max_dim << "\n";

      // Rotate X (wobble)
      cout << "Rotate \"X\" " << 45.0 * sin(frame_count / WOBBLE_PERIOD * 2.0 * M_PI) << "\n";
      // Rotate Y (spin)
      cout << "Rotate \"Y\" " << frame_count / SPIN_PERIOD * 360 << "\n";
      // Rotate (orient)
      cout << "Rotate \"X\" -90\n";

      // Translate (center)
      cout << "Translate " << -(float)(ncols-1) / 2.0
	   << " " << -(float)(nrows-1) / 2.0
	   << " " <<-(float)(nplanes-1) / 2.0 << "\n";

      // Frame
      cout << "Color 1.0 0.0 0.0\n";
      cout << "Line 0 0 0  " 
	   << (ncols-1) << " 0 0\n";
      cout << "Line 0 " << (nrows-1) << " 0  " 
	   << (ncols-1) << " " << (nrows-1) << " 0\n";
      cout << "Line 0 0 " << (nplanes-1) << "  "
	   << (ncols-1) << " 0 " << (nplanes-1) << "\n";
      cout << "Line 0 " << (nrows-1) << " " << (nplanes-1) << "  "
	   << (ncols-1) << " " << (nrows-1) << " " << (nplanes-1) << "\n";

      cout << "Color 0.0 1.0 0.0\n";
      cout << "Line 0 0 0  "
	   << "0 " << (nrows-1) << " 0\n";
      cout << "Line " << (ncols-1) << " 0 0  "
	   << (ncols-1) << " " << (nrows-1) << " 0\n";
      cout << "Line 0 0 " << (nplanes-1) << "  "
	   << "0 " << (nrows-1) << " " << (nplanes-1) << "\n";
      cout << "Line " << (ncols-1) << " 0 " << (nplanes-1) << "  "
	   << (ncols-1) << " " << (nrows-1) << " " << (nplanes-1) << "\n";

      cout << "Color 0.0 0.0 1.0\n";
      cout << "Line 0 0 0  "
	   << "0 0 " << (nplanes-1) << "\n";
      cout << "Line " << (ncols-1) << " 0 0  "
	   << (ncols-1) << " 0 " << (nplanes-1) << "\n";
      cout << "Line 0 " << (nrows-1) << " 0  "
	   << "0 " << (nrows-1) << " " << (nplanes-1) << "\n";
      cout << "Line " << (ncols-1) << " " << (nrows-1) << " 0  "
	   << (ncols-1) << " " << (nrows-1) << " " << (nplanes-1) << "\n";

      // Lighting
      cout << "OptionBool \"DoubleSide\" on\n";
      //      cout << "FarLight 0 0 -1 1 1 1 1\n";
      cout << "FarLight 1 -1 -1  0.2 1 0.2 0.8\n";
      cout << "FarLight 0  1 -1  1 0.2 0.2 0.8\n";
      cout << "FarLight -1 -1 -1 0.2 0.2 1 0.8\n";
	
      cout << "Ka 0.1\n";
      cout << "Kd 0.9\n";

      // Data
      cout << "Color 1.0 1.0 1.0\n";
      cout << "ObjectInstance \"Data\"\n";

      // Restore
      cout << "Ka 1.0\n";
      cout << "Kd 0.0\n";

      cout << "WorldEnd\n";
      cout << "FrameEnd" << endl;
    }
  
  return 0;
}

void Usage(char * argv[])
{
  cerr << "Usage: " << argv[0] << " n_cols  n_rows  n_planes polyset_file" << endl;
  cerr << "   Output goes to standard out and should be piped to rd_view" << endl;
}

