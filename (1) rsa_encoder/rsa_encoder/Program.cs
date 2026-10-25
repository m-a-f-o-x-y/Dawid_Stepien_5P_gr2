/*
 *  Program - klasa bazowa
    LinearFunction <- Program
    QuadraticFunction <- Program

    Program służy do obliczania punktów wspólnych funkcji kwadratowej i liniowej
    Tworzę 2 klasy potomne, które przez konstruktory przyjmują 
    współczynniki do samych siebie.

    Działają na liczbach zmiennoprzecinkowych pojedynczej precyzji (float'y) 
    w celu szybszego obliczania

    Szczegóły przy linijkach kodu...
 */
public class Program
{
    // Klasa bazowa tych funkcji

    const float deg_to_radian = 180.0f / MathF.PI;
    public abstract class Function
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

        public bool isPerpendicularTo(LinearFunction l2) {
            return (a * l2.a) == -1;
        }

        public float angle(bool returnInRadians = false)
        {
            float rad = MathF.Atan(this.a);
            if (returnInRadians)
            {
                return rad;
            }

            float res = rad * deg_to_radian;
            if (res < 0){
                res = -res;
                res += 90.0f;
               
            }
            return res;
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
        public float a, b, c, p, q;

        public QuadraticFunction(float _a, float _b, float _c)
        {
            if (_a == 0)
            {
                throw new ArgumentException("[!!!] a = 0"); // Inaczej powstałby funkcja liniowa
            }

            a = _a;
            b = _b;
            c = _c;

            (p, q) = vertex();
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

                float _solutionB = _solutionA - MathF.Sqrt(delta) / a; // redukujemy obliczenia poprzez wyliczenie
                                                                       // różnicy rozwiązań
                                                                       // tj. x2 = x1 - (sqrt(delta) / a)
                                                                       // można łatwo wyprowadzić, obliczając ogólnioną
                                                                       // różnicę rozwiązań funkcji kwadratowej

                return (_solutionA, _solutionB); // 2 miejsca zerowe w krotce
            }
        }

        public (float?, float?) Intersection(QuadraticFunction _funcB)
        {
            float _localA = (a - _funcB.a), _localB = (b - _funcB.b), _localC = (c - _funcB.c);

            float delta = (_localB * _localB) - 4 * _localA * _localC;

            if (delta < 0)
            {
                return (null, null); // nie ma rozwiązań (przynajmniej w zb. liczb rzeczywistych)
            }
            else if (delta == 0)
            {
                float calcValue = -_localB / (2 * _localA); // x0 = -b / 2a
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
                    (2 * _localA);

                float _solutionB = _solutionA - MathF.Sqrt(delta) / _localA; // redukujemy obliczenia poprzez wyliczenie
                                                                             // różnicy rozwiązań
                                                                             // tj. x2 = x1 - (sqrt(delta) / a)
                                                                             // można łatwo wyprowadzić, obliczając ogólnioną
                                                                             // różnicę rozwiązań funkcji kwadratowej

                return (_solutionA, _solutionB); // 2 miejsca zerowe w krotce

            }
        }

        public (float, float) vertex()
        {
            float p, q;
            p = -b / 2 * a;
            q = Calc(p);

            return (p, q);
        }
    }
    public static void Main()
    {
            QuadraticFunction q1 = new QuadraticFunction(1, 7, 3);
            QuadraticFunction q2 = new QuadraticFunction(-1, -11, 9);

        Console.WriteLine(q1.Intersection(q2));
    }
}