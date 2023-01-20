#include "Simple_problem.h"

using namespace Gecode;
using namespace std;

class Simple_problem : public Space {
  protected:
    IntVarArray vars;
  public:   
    Simple_problem(): vars(*this,3, 1, 4){
      Rnd r1(12U);
      distinct(*this, vars);
      branch(*this, vars, INT_VAR_DEGREE_MAX(), INT_VAL_RND(r1));
    }

    virtual void constrain(const Space& _b) {
      // right now, do nothing
      const Simple_problem& b = static_cast<const Simple_problem&>(_b);// cast the space
      for(int i = 0; i < vars.size(); ++i){
          rel(*this, vars[i], IRT_NQ, b.vars[i].val());
      }
    }

    // search support
    Simple_problem(Simple_problem& s) : Space(s) {
      vars.update(*this, s.vars);
    }
  
    virtual Space* copy(void) {
      return new Simple_problem(*this);
    }

    // print solution
    void print(void) const {
        std::cout << vars << std::endl;
    }
};

// main function
int main(int argc, char* argv[]) {
  // create model and search engine
  Simple_problem* m = new Simple_problem();
  Gecode::Search::Options opts;
  opts.threads = 0;// use as many threads as possible
  Gecode::Search::TimeStop maxTime(1000);
  opts.stop = &maxTime; 
  DFS<Simple_problem> e(m);
  delete m;
  int nbSol = 0;

  // search and print all solutions
  while (Simple_problem* s = e.next()) {
    s->print(); delete s;
    ++nbSol;
    //maxTime.reset();
  }
  std::cout << "number of solutions : " << nbSol << "\n";
  return 0;
}