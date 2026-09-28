/*
 *  Program - klasa bazowa
    LinearFunction <- Program
    QuadraticFunction <- Program

    Program służy do obliczania punktów wspólnych funkcji kwadratowej i liniowej
    Tworzę 2 klasy potomne, które przez konstruktory przyjmują 
    współczynniki do samych siebie.

    Szczegóły przy linijkach kodu...
 */
public class Program
{ 
    // Klasa bazowa tych funkcji
    public class Function
    {
        virtual public float Calc(float x) { return 0.0f; }
    }

    public class LinearFunction : Function
    {
        public float a, b;
        public LinearFunction(float _a, float _b)
        {
            if (_a == 0 && _b == 0)
            {
                throw new ArgumentException("'a' oraz 'b' nie mogą być równocześnie zerowe!");
            }
            a = _a;
            b = _b;
        }

        public float Calc(float x)
        {
            return a * x + b;
        }

        public float? Intersection(LinearFunction _funcB)
        {
            if (_funcB.a == a) { return null; }             // jeśli funkcje mają tan
                                                            // sam współczynnik kierunkowy, nie mają punktów wspólnych. 
                                                            // zwraca najmniejszą możliwą wartość, która może przyjąć
                                                            // float, która również tutaj będzie oznaczała brak punktów
                                                            // wspólnych
                                                            //
            return (_funcB.b - b) / (a - _funcB.a);         // WYPROWADZENIE:
                                                            // f1 = f2
                                                            // ax + b = a'x + b'
                                                            // ax - a'x = b' - b
                                                            // x(a - a') = b' - b
                                                            //     b' - b
                                                            // x = ------
                                                            //     a - a'
                                                            // ,gdzie x to argument, gdzie funkcje się przecinają

                                                            
        }

        public (float?, float?) Intersection(QuadraticFunction _funcB)
        {
            return _funcB.Intersection(this); // dla D.R.Y.
         }
    }

    public class QuadraticFunction : Function
    {
        public float a, b, c;

        public QuadraticFunction(float _a, float _b, float _c)
        {
            if (_a == 0)
            {
                throw new ArgumentException("[!!!] a = 0"); // Inaczej powstałby funkcja liniowa
            }

            a = _a;
            b = _b;
            c = _c;
        }
        public float Calc(float x)
        {
            return a * x * x + b * x + c;
        }

        public (float?, float?) Intersection(LinearFunction _funcB)
        {
            float _localB = (b - _funcB.a), _localC = (c - _funcB.b); // definiuję 'monikery'
                                                                      // współczynników przy określonych stopniach
                                                                      // dla:
                                                                      // 1. uproszczenia kodu
                                                                      // 2. oszczędzenie obliczeń
            float delta = (_localB * _localB) - 4 * a * _localC;        // delta = b^2 - 4 * a * c

            if (delta < 0)
            {
                return (null, null); // nie ma rozwiązań (przynajmniej w zb. liczb rzeczywistych)
            }
            else if (delta == 0)
            {
                float calcValue = -_localB / (2 * a); // x0 = -b / 2a
                return (calcValue, null);
            }
            else
            {
                //   -b ± √delta
                // ----------------
                //       2a
                //        ⬎
                float _solutionA = (-_localB + 
                    (float)Math.Sqrt(delta)
                    )
                    /
                    (2 * a);
   
                float _solutionB = _solutionA - (float)Math.Sqrt(delta) / a; // redukujemy obliczenia poprzez wyliczenie
                                                           // różnicy rozwiązań
                                                           // tj. x2 = x1 - (sqrt(delta) / a)
                                                           // można łatwo wyprowadzić, obliczając ogólnioną
                                                           // różnicę rozwiązań funkcji kwadratowej

                return (_solutionA, _solutionB); // 2 miejsca zerowe w krotce
            }
        }
    }

    public static void Main()
    {
        LinearFunction l1 = new LinearFunction(3, 2);
        QuadraticFunction q1 = new QuadraticFunction(-0.1f, 6, 1);

        Console.WriteLine(l1.Intersection(q1));
    }
}