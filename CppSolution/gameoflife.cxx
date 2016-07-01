#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/thread/thread.hpp> //sleep

//system("clear")
#ifdef __cplusplus__
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#define log(msg) do{ std::cout<<msg<<std::endl;}while(0)

const std::string ALIVE = "*";
const std::string DEAD  = "-";

template<typename INPUT,typename OUTPUT>
inline bool lex_cast(const INPUT & in, OUTPUT & out){
  try {
    out = boost::lexical_cast<OUTPUT>( in );
  }
  catch( boost::bad_lexical_cast& e ) {
    return false;
  }
  return true;
}

void printGrid(std::vector< std::vector<bool> > & grids){
  for(size_t col=0; col<grids.size(); ++col){
    for(size_t row=0; row<grids[col].size(); ++row){
      if(grids[col][row]){
        if(row == grids[col].size()-1)
          std::cout<<"*";
        else
          std::cout<<"* ";
      } else {
        if(row == grids[col].size()-1)
          std::cout<<" ";
        else
          std::cout<<"  ";
      }
    }
    std::cout<<std::endl;
  }
}

int aliveNeighbour(const std::vector< std::vector<bool> > & grids, const size_t col,const size_t row){
  int alive_num = 0;
  size_t size_end = grids.size()-1;
  size_t up,low,left,right;
  if(row == 0)       {left = size_end;}
  else               {left = row-1;}

  if(row == size_end){right = 0;}
  else               {right = row+1;}

  if(col == 0)       {up = size_end;}
  else               {up = col-1;}

  if(col == size_end){low = 0;}
  else               {low = col+1;}

  if(grids[up][row]) alive_num++;
  if(grids[low][row]) alive_num++;

  if(grids[up][left]) alive_num++;
  if(grids[up][right]) alive_num++;

  if(grids[low][left]) alive_num++;
  if(grids[low][right]) alive_num++;

  if(grids[col][left]) alive_num++;
  if(grids[col][right]) alive_num++;

  return alive_num;
}

void evolve(std::vector< std::vector<bool> > & grids, int & iterations,bool print=false){
  if(print){
    log("Iterations:["<<iterations<<"]");
    printGrid(grids);
    std::cout << "Press enter to continue ...";
    std::cin.get();
  }

  while(iterations--){
    std::vector< std::vector<bool> > new_grids = grids;
    //for each cell
    for(size_t col=0; col<grids.size(); ++col){
      for(size_t row=0; row<grids[col].size(); ++row){
        //8 directions
        int alive_num = aliveNeighbour(grids,col,row);
        if(grids[col][row]){
          //For Alive cell
          if     (alive_num<2) new_grids[col][row] = false;
          else if(alive_num>3) new_grids[col][row] = false;
        }
        else {
          //For Dead cell
          if(alive_num == 3)  new_grids[col][row] = true;
        }
      }//for row
    }//for col
    grids = new_grids;

    if(print){
      log("Iterations:["<<iterations<<"]");
      printGrid(grids);
      boost::this_thread::sleep(boost::posix_time::milliseconds(500));
      system("clear");
    }
  }//while iterations

}

int main(int argc,  char* argv[]){
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " [filename] [true/false(print generations)]" << std::endl;
    return 1;
  }
  std::string filename = argv[1];
  std::ifstream infile(filename.c_str());


  std::string print = argv[2];
  bool print_generations = false;
  if(print == "true"){
    print_generations = true;
  }
  int size       = 0;
  int iterations = 0;
  std::vector< std::vector<bool> > grids;

  std::string line;
  int count = 1;
  while (std::getline(infile, line)){

    //Read size and iterations
    if(size == 0 && lex_cast(line,size)){
      std::vector<bool> grids_row(size,false);
      grids.clear();
      grids.resize(size,grids_row);
      continue;
    }
    if(iterations <= 0 && lex_cast(line,iterations)){ continue; }

    //Read Grids
    std::vector<std::string> cells_row;
    boost::split(cells_row,line,boost::is_any_of(" "));
    for(size_t it=0; it<cells_row.size();++it){
      //std::cout<<cells_row[it]<<" ";
      std::vector<bool> & grids_row_ref = grids[grids.size()-size];


      if(cells_row[it]==ALIVE){
        grids_row_ref[it] = true;
        //std::cout<<"* ";
      } else {
        //std::cout<<"- ";
      }
    }
    //std::cout<<std::endl;

    size--;
    //Finish reading 1 set of grids, game starts
    if(size == 0){
      std::cout<<count<<std::endl;
      evolve(grids,iterations,print_generations);
      printGrid(grids);
      count++;
    }
  }

}
