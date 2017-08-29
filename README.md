# haskell-as-templates
Translation of a Haskell run-time fizzbuzz to a C++ compile-time fizzbuzz. I'm so sorry.

## What is this?

1. Take a normal FizzBuzz, written in Haskell.
```haskell
import Control.Monad

m `divides` n = (n `mod` m == 0)

toFizzBuzz n
    | 15 `divides` n = "FizzBuzz"
    |  5 `divides` n = "Buzz"
    |  3 `divides` n = "Fizz"
    |      otherwise = show n

main = mapM_ putStrLn $ take 100 $ map toFizzBuzz [1..]
```
2. Re-implement lazy evaluation in C++ templates.
3. Reimplement lazy infinite lists in C++ templates.
4. Reimplement Peano arithmetic in C++ templates, up through modular arithmetic.
5. Reimplement some Haskell library functions (in C++ templates, of course)
6. Generate the lazy, infinite list of FizzBuzz results at compile time.
7. Do not profit.
