//
// Created by viviwu on 2023/5/12.
//

// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream, std::ofstream

int ifstream_read () {
  std::ifstream ifs ("test.txt", std::ifstream::binary);
  if (ifs) {
    // get length of file:
    ifs.seekg (0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg (0, ifs.beg);

    // allocate memory:
    char * buffer = new char [length];

    // read data as a block:
    ifs.read (buffer,length);

    ifs.close();

    // print content:
    std::cout.write (buffer,length);

    delete[] buffer;
  }

  return 0;
}

struct Hu{
  char name [32];
  int age;
};

int ofstream_write(){
  std::ofstream ofs ("new.bin", std::ifstream::binary);
  if (ofs){
    Hu u; 
    printf ("Enter your family name: ");
    scanf ("%79s", u.name);
    printf ("Enter your age: ");
    scanf ("%d",&u.age);

    ofs.write((char *)&u, sizeof(u));
    ofs.close();
  }

  getchar();
  std::ifstream ifs ("new.bin", std::ifstream::binary);
  if (ifs){
    Hu h;
    ifs.read((char *)&h, sizeof(h));
    printf("name:%s, age=%d", h.name, h.age);
    ifs.close();
  }
  return 0;
}

// Copy a file
int copy_file () {
  std::ifstream infile ("test.txt",std::ifstream::binary);
  std::ofstream outfile ("new.txt",std::ofstream::binary);

  // get size of file
  infile.seekg (0,infile.end);
  long size = infile.tellg();
  infile.seekg (0);

  // allocate memory for file content
  char* buffer = new char[size];

  // read content of infile
  infile.read (buffer,size);

  // write to outfile
  outfile.write (buffer,size);

  // release dynamically-allocated memory
  delete[] buffer;

  outfile.close();
  infile.close();
  return 0;
}

int main(){
//  ifstream_read ();

  ofstream_write();

//  copy_file ();

  return 0;
}