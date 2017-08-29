//  
//  Lazy compile-time fizzbuzz computed by C++ templates,
//  without conditionals or the use of machine arithmetic.
//
//         -- Matt Noonan (matt.noonan@gmail.com)

#include <iostream>

using namespace std;

//
//  The natural numbers: Nat = Zero | Succ Nat
//

template <typename n>
struct Succ
{
  typedef Succ eval;
  static const unsigned int toInt = 1 + n::toInt;
  static void print(ostream & o) { o << toInt; }
};

struct Zero
{
  typedef Zero eval;
  static const unsigned int toInt = 0;
  static void print(ostream & o) { o << toInt; }
};

//
//  Arithmetic operators
//    Plus Zero n = n
//    Plus Succ(n) m = Plus n Succ(m)
//    Times Zero n = Zero
//    Times Succ(n) m = Plus m (Times n m)
//

template <typename a, typename b>
struct Plus
{
  typedef typename Plus<typename a::eval,
                        typename b::eval>::eval eval;
};

template <typename M>
struct Plus <Zero, M>
{ typedef typename M::eval eval; };

template <typename N, typename M>
struct Plus <Succ<N>, M>
{ typedef typename Plus<N, Succ<M> >::eval eval; };

template <typename a, typename b>
struct Times
{
  typedef typename Times<typename a::eval,
                         typename b::eval>::eval eval;
};

template <typename M>
struct Times <Zero, M>
{ typedef Zero::eval eval; };

template <typename N, typename M>
struct Times <Succ<N>, M>
{ typedef typename Plus<M,
                        typename Times<N,M>::eval
                        >::eval eval; };

//
//  Lists
//

struct Nil
{
  typedef Nil eval;
  static void print(ostream & o) { }
};

template <typename x, typename xs>
struct Cons
{
  typedef Cons eval;
  static void print(ostream & o) {
    x::eval::print(o); o << endl; xs::eval::print(o);
  }
};

//
//  Take the first n elements of a list
//

template <typename, typename> struct Take;

template <typename _> struct Take<Zero,_>
{ typedef Nil eval; };

template <typename n, typename x, typename xs>
struct Take<Succ<n>, Cons<x,xs> >
{
  typedef Cons<x, Take<n, xs> > eval;
};

template <typename a, typename b>
struct Take
{
  typedef typename Take<typename a::eval,
                        typename b::eval>::eval eval;
};

//
//  Iterate f x0 makes the infinite list
//  x0, f(x0), f(f(x0)), ...
//

template <template<typename> class f, typename x0> struct Iterate
{
  typedef Cons<x0, Iterate<f, f<x0> > > eval;
};

//
//  Map a function over a list
//

template <template<typename> class a, typename b> struct Map
{ typedef typename Map<a,
                       typename b::eval>::eval eval;
};

template <template<typename> class f>
struct Map<f, Nil>
{ typedef Nil eval; };

template <template<typename> class f, typename x, typename xs>
struct Map<f, Cons<x,xs> >
{
  typedef Cons<f<x>, Map<f,xs> > eval;
};

//
//  Some useful things for making fizzes and buzzes
//

struct Fizz
{ static void print(ostream & o) { o << "Fizz"; } };

struct Buzz
{ static void print(ostream & o) { o << "Buzz"; } };

struct FizzBuzz
{ static void print(ostream & o) { o << "FizzBuzz"; } };

//
//  Some useful numbers
//

typedef Succ<Zero> One;
typedef Succ<One> Two;
typedef Succ<Two> Three;
typedef Plus<Two, Three> Five;
typedef Times<Two, Five> Ten;
typedef Times<Three, Five> Fifteen;
typedef Times<Ten, Ten> OneHundred;

//
//  Booleans
//

struct True {};
struct False {};

//
//  If/then/else
//

template <typename p, typename t, typename f>
struct If
{
  typedef typename If<typename p::eval, t, f>::eval eval;
  static void print(ostream & o) { eval::print(o); }
};

template <typename t, typename _>
struct If<True, t, _>
{
  typedef t eval;
};

template <typename _, typename f>
struct If<False, _, f>
{ typedef f eval; };

//
//  Testing if x divides y
//

template <typename a, typename b, typename c>
struct _Divides
{
  typedef typename _Divides<typename a::eval,
                            typename b::eval,
                            typename c::eval>::eval eval;
};

template <typename _, typename __>
struct _Divides<_, __, Zero> { typedef False eval; };

template <typename a>
struct _Divides<a, Zero, Zero> { typedef True eval; };

template <typename a, typename b>
struct _Divides<a, Zero, b>
{
  typedef typename _Divides<a, a, b>::eval eval;
};

template <typename _, typename n, typename m>
struct _Divides<_, Succ<n>, Succ<m> >
{
  typedef typename _Divides<_, n, m>::eval eval;
};

template <typename a, typename b>
struct Divides
{
  typedef typename _Divides<a, a, b>::eval eval;
};

//
//  "Otherwise" sugar
//

template <typename a>
struct Otherwise
{
  typedef typename a::eval eval;
  static void print(ostream & o) { a::eval::print(o); }
};

//
//  Convert a number to fizzes, buzzes as appropriate
//

template <typename n>
struct toFizzBuzz
{
  typedef typename
  If< Divides<Fifteen, n>, FizzBuzz,
  If< Divides<   Five, n>,     Buzz,
  If< Divides<  Three, n>,     Fizz,
  Otherwise<                      n
  > > > >::eval eval;
};

int main(void)
{
  // Make all of the natural numbers
  typedef Iterate<Succ, One> Naturals;

  // Apply fizzbuzz rules to every natural number
  typedef Map<toFizzBuzz, Naturals> FizzBuzzedNaturals;

  // Print out the first hundred fizzbuzzed numbers
  Take<OneHundred, FizzBuzzedNaturals>::eval::print(cout);

  return 0;
}
