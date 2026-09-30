using System.Xml.Serialization;
using static Program;

namespace testySolvera
{
    public class UnitTest1
    {
        [Fact]
        public void zerowaFunkcjaLiniowa()
        {
            bool error = false;
            try
            {
                LinearFunction l1 = new LinearFunction(0, 0);
            }
            catch (Exception)
            {
                error = true;
            }

            Assert.True(error); 
            
        }

        [Fact]
        public void wynikNiewymierny()
        {
            LinearFunction l1 = new LinearFunction(MathF.E, -4);
            LinearFunction l2 = new LinearFunction(MathF.PI / 6, -MathF.Sqrt(2));

            float? result = l1.Intersection(l2);

            Assert.Equal(result, 1.1782049504854f);
        }

        [Fact]
        public void RozwiazaniaFnKwadratowej()
        {
            LinearFunction l1 = new LinearFunction(1 / 2, 1);
            QuadraticFunction q1 = new QuadraticFunction(2, 5, 1);

            (float?, float?) result = l1.Intersection(q1);

            Assert.Equal(result, (-1.6403882032022f, -0.6096117967978f));
        }

        [Fact]
        public void najwiekszyWspolRownyZero()
        {
            bool error = false;
            try
            {
                QuadraticFunction q1 = new QuadraticFunction(0, 2, 2);
            }
            catch (Exception)
            {
                error = true;
            }

            Assert.True(error);
            
        }

        [Fact]
        public void zwrocPoprawykat()
        {
            LinearFunction l1 = new LinearFunction(-1f/4f, 3);

            float result = l1.angle();

            Assert.Equal(result, 104.0362434679265f);
        }

        [Fact]
        public void nieProstopadle()
        {
            LinearFunction l1 = new LinearFunction(2, 3);
            LinearFunction l2 = new LinearFunction(1 / 17, 3);

            Assert.False(l1.isPerpendicularTo(l2));
        }

        [Fact]
        public void Prostopadle()
        {
            LinearFunction l1 = new LinearFunction(2f, 3f);
            LinearFunction l2 = new LinearFunction(-1f/2f, 3f);

            Assert.True(l1.isPerpendicularTo(l2));
        }

    }
}
