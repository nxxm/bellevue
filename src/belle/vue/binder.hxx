class UndoString : public std::string
{
  struct proxy
  {
    UndoString * str;
    size_t pos;

    proxy(UndoString * us, size_t position)
      : str(us), pos(position)
    {}

    // Invoked when proxy is used to modify the value.
    void operator = (const char & rhs) 
    {
      str->old = str->at(pos);
      str->old_pos = pos;
      str->at(pos) = rhs;
    }

    // Invoked when proxy is used to read the value.
    operator const char & () const
    {
      return str->at(pos);
    }
  };

  char old;
  int old_pos;

public:

  UndoString(const std::string & s)
    : std::string(s), old(0), old_pos(-1)
  {}

  // This operator replaces std::string's non-const operator [].
  proxy operator [] (size_t index)
  {
    return proxy(this, index);
  }

  using std::string::operator [];

  void undo()
  {
    if(old_pos == -1)
      throw std::runtime_error("Nothing to undo!");

    std::string::at(old_pos) = old;
    old = 0;
    old_pos = -1;
  }
};



using namespace std;
class Visualizer {
    std::vector <int> & vect;
  public:
    Visualizer (vector<int> &v) : vect(v) {}
    void data_changed () {
       std::cout << "Now size is: " << vect.size();
    }
};
int main () // A data visualization application.
{
  std::vector <int> vector;
  Visualizer visu (vector);
  //...
  vector.push_back (10);
  visu.data_changed ();
  vector.push_back (20);
  visu.data_changed ();
  // Many more insert/remove calls here
  // and corresponding calls to visualizer.
}









class VisualizableVector {
  public:
    class proxy {
      public:
        proxy (vector<int> *v) : vect (v) {
          std::cout << "Before size is: " << vect->size ();
        }
        vector<int> * operator -> () {
          return vect;
        }
        ~proxy () {
          std::cout << "After size is: " << vect->size ();
        }
      private:
        vector <int> * vect;
    };
    VisualizableVector (vector<int> *v) : vect(v) {}    
    proxy operator -> () {
       return proxy (vect);
    }
  private:
    vector <int> * vect;
};
int main()
{
  VisualizableVector vecc (new vector<int>);
  //...
  vecc->push_back (10); // Note use of -> operator instead of . operator
  vecc->push_back (20);
}










template <class NextAspect, class Para>
class Aspect
{
  protected:
    Aspect (Para p): para_(p) {}
    Para  para_;
  public:
    NextAspect operator -> () 
    {
      return NextAspect (para_);
    }
};

template <class NextAspect, class Para>
struct Visualizing : Aspect <NextAspect, Para>
{
  public:
    Visualizing (Para p) 
       : Aspect <NextAspect, Para> (p) 
    {
	std::cout << "Before Visualization aspect" << std::endl;
    }
    ~Visualizing () 
    {
	std::cout << "After Visualization aspect" << std::endl;
    }
};
template <class NextAspect, class Para>
struct Locking : Aspect <NextAspect, Para>
{
  public:
    Locking (Para p) 
       : Aspect <NextAspect, Para> (p) 
    {
		std::cout << "Before Lock aspect" << std::endl;
    }
    ~Locking () 
    {
	std::cout << "After Lock aspect" << std::endl;
    }
};
template <class NextAspect, class Para>
struct Logging : Aspect <NextAspect, Para>
{
  public:
    Logging (Para p) 
        : Aspect <NextAspect, Para> (p) 
    {
		std::cout << "Before Log aspect" << std::endl;
    }
    ~Logging () 
    {
	std::cout << "After Log aspect" << std::endl;
    }
};
template <class Aspect, class Para>
class AspectWeaver 
{
public:
    AspectWeaver (Para p) : para_(p) {}    
    Aspect operator -> () 
    {
       return Aspect (para_);
    }
private:
	Para para_;
};

#define AW1(T,U) AspectWeaver <T <U, U>, U >
#define AW2(T,U,V) AspectWeaver <T < U <V, V> , V>, V >
#define AW3(T,U,V,X) AspectWeaver <T < U <V <X, X>, X> , X>, X >

int main()
{ 
  AW3(Visualizing, Locking, Logging, vector <int> *) 
	  X (new vector<int>);
  //...
  X->push_back (10); // Note use of -> operator instead of . operator
  X->push_back (20);
  return 0;
}

